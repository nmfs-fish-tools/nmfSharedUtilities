/**
 * @file nmfUtilsStatistics.h
 * @brief This header file defines multi-species statistical utility functions.
 * @date Jan 24, 2017
 *
 * @copyright
 * Public Domain Notice\n
 *
 * National Oceanic And Atmospheric Administration\n\n
 *
 * This software is a "United States Government Work" under the terms of the
 * United States Copyright Act.  It was written as part of the author's official
 * duties as a United States Government employee/contractor and thus cannot be copyrighted.
 * This software is freely available to the public for use. The National Oceanic
 * And Atmospheric Administration and the U.S. Government have not placed any
 * restriction on its use or reproduction.  Although all reasonable efforts have
 * been taken to ensure the accuracy and reliability of the software and data,
 * the National Oceanic And Atmospheric Administration and the U.S. Government
 * do not and cannot warrant the performance or results that may be obtained
 * by using this software or data. The National Oceanic And Atmospheric
 * Administration and the U.S. Government disclaim all warranties, express
 * or implied, including warranties of performance, merchantability or fitness
 * for any particular purpose.\n\n
 *
 * Please cite the author(s) in any work or product based on this material.
 */

#ifndef NMF_UTILS_STATISTICS
#define NMF_UTILS_STATISTICS

#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <string>
#include <utility>
#include <vector>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multi_array.hpp>

#include "nmfConstants.h"
#include "nmfUtils.h"
#include "nmfUtilsComplex.h"

/**
 * @brief Holds outputs from non linear regression
 */
struct NLRegOut {
    double A;
    double B;
    double K;
    double RegMS;   // regression mean square
    int    RegDf;   // Regression degrees of freedom = Nparms
    double ResSE;   // SQRT of errms
    double ErrorMS; // Error mean squares
    int    ErrorDF; // Error DF = N- (NParms+1)
    double F;       // F-ratio = regMS/ErrorMS
    double ProbF;
    double Rsquare; // R-square = regSS/TotalSS
    int    ItFlag;  // 1 if maxits reached
};

/**
 * @brief Holds outputs from linear regression
 */
struct LinRegOut {
    double Alpha;   // intercept
    double Beta;    // Slope
    double RegMS;   // regression mean square
    int    RegDf;   // Regression degrees of freedom = 1
    double ErrorMS; // Error mean squares
    int    ErrorDF; // Error DF = N-2
    double F;       // F-ratio = regMS/ErrorMS
    double K;       // Growth
    double Rsquare; // R-square = regSS/TotalSS
    double ResSE;   // Residual standard error of regression
    double BetaSE;  // Std Error of beta estimate
    double AlphaSE; // Std Error of Alpha estimate
    double BetaT;   // T-score for beta estimate
    double AlphaT;  // T-score for alpha estimate
    double MeanX;   // Average X..needed for predicted values
    double SSX;     // Needed for SE of predicted values
    int    N;       // Holds sample size
};

namespace nmfUtilsStatistics {


    double BetaCF(double A, double B, double X);

    double BetaI(double A, double B, double X);

    double BETAFUNC(double A, double B);

    /**
     * @brief Calculates the average absolute error: Σ|Eₜ-Oₜ| / n
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param runLength : time period in years
     * @param observed : observed biomass
     * @param estimated : estimated biomass
     * @param aae : average error per species or guild
     * @return True if no error, else False
     */
    bool calculateAAE(const int& numSpeciesOrGuilds,
                      const int& runLength,
                      const std::vector<double>& observed,
                      const std::vector<double>& estimated,
                      std::vector<double>& aae);
    /**
     * @brief Calculates the average error (or bias): Σ(Eₜ-Oₜ) / n
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param meanObserved : mean values of observed biomass per species
     * @param meanEstimated : mean values of estimated biomass per species
     * @param ae : average error per species or guild
     */
    void calculateAE(const int& numSpeciesOrGuilds,
                     const std::vector<double>& meanObserved,
                     const std::vector<double>& meanEstimated,
                     std::vector<double>& ae);

    /**
     * @brief Calculates Akaike Info Criterion: n*ln(σ²) + 2*K, where n = number of observations,
     * σ² = ssResiduals/n, and K = number of parameters
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param numParameters : number of parameters
     * @param runLength : time period in years
     * @param ssResiduals : sum of square residuals per species or guild
     * @param aic : Akaika Info Criterion per species or guild
     */
    void calculateAIC(const int& numSpeciesOrGuilds,
                      const int& numParameters,
                      const int& runLength,
                      const std::vector<double>& ssResiduals,
                      std::vector<double>& aic);
    /**
     * @brief Calculates the negative log maximum likelihood value
     * @param Species : the particular species whose mle value you're finding
     * @param ObsBiomass : Observed Biomass matrix
     * @param EstBiomass : Estimated Biomass matrix
     * @return Returns the per species -log maximum likelihood value
     */
    double calculateMaximumLikelihoodNoRescale(
            const int& Species,
            const boost::numeric::ublas::matrix<double>& ObsBiomass,
            const boost::numeric::ublas::matrix<double>& EstBiomass);
    /**
      * @brief calculateMaximumLikelihoodNoRescale
      * @param speciesWeights : vector of species fitness (biomass) weights
      * @param isEffortFitToCatch : true if the harvest type is EffortFitToCatch; false otherwise
      * @param ObsCatch : Observed Catch matrix
      * @param ObsBiomass : Observed Biomass matrix
      * @param EstCatch : Estimated Catch matrix (= qEB, catchability*Effort*EstBiomass)
      * @param EstBiomass : Estimated Biomass matrix
      * @param FitWeights : Biomass and Catch fit weights (from 0 to 1)
      * @return Returns the total -log maximum likelihood value
      */
    double calculateMaximumLikelihoodNoRescale(
            const boost::numeric::ublas::vector<double>& speciesWeights,
            const bool& isEffortFitToCatch,
            const boost::numeric::ublas::matrix<double>& ObsCatch,
            const boost::numeric::ublas::matrix<double>& ObsBiomass,
            const boost::numeric::ublas::matrix<double>& EstCatch,
            const boost::numeric::ublas::matrix<double>& EstBiomass,
            const boost::numeric::ublas::matrix<double>& FitWeights);
    /**
     * @brief Calculates the mean of the passed matrix for the particular species
     * @param useLogData : boolean signifying if log of data should be taken prior to calculations
     * @param obsBiomass : 2-dimensional (observed biomass) matrix
     * @param speciesNum : Species number to use to find the mean
     * @return Returns the mean of the matrix for the particular Species number
     */
    double calculateMean(
            const bool useLogData,
            const boost::numeric::ublas::matrix<double>& obsBiomass,
            const int speciesNum);
    /**
     * @brief Calculates the modeling efficiency statistic: [Σ(Oₜ-Ō)²-Σ(Eₜ-Oₜ)²] / Σ(Oₜ-Ō)²
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param runLength : time period in years
     * @param meanObserved : mean values of observed biomass per species
     * @param observed : observed biomass
     * @param estimated : estimated biomass
     * @param mef : model efficiency values by species or guilds
     * @return True if no error found, else False
     */
    bool calculateMEF(const int& numSpeciesOrGuilds,
                      const int& runLength,
                      const std::vector<double>& meanObserved,
                      const std::vector<double>& observed,
                      const std::vector<double>& estimated,
                      std::vector<double>& mef);
    /**
     * @brief Calculates the model efficiency fitness of 1 - ⵉ(Be - Bo)² / ⵉ(Bo - Bm)²
     * where (e)stimated, (o)bserved, and (m)ean (B)iomass or Catch
     * @param speciesWeights : vector of species fitness (biomass) weights
     * @param isEffortFitToCatch : true if the harvest type is EffortFitToCatch; false otherwise
     * @param ObsCatch : Observed Catch matrix
     * @param ObsBiomass : Observed Biomass matrix
     * @param EstCatch : Estimated Catch matrix (= qEB, catchability*Effort*EstBiomass)
     * @param EstBiomass : Estimated Biomass matrix
     * @param FitWeights : Biomass and Catch fit weights (from 0 to 1)
     * @return Returns the model efficiency value
     */
    double calculateModelEfficiency(
            const boost::numeric::ublas::vector<double>& speciesWeights,
            const bool& isEffortFitToCatch,
            const boost::numeric::ublas::matrix<double>& ObsCatch,
            const boost::numeric::ublas::matrix<double>& ObsBiomass,
            const boost::numeric::ublas::matrix<double>& EstCatch,
            const boost::numeric::ublas::matrix<double>& EstBiomass,
            const boost::numeric::ublas::matrix<double>& FitWeights);
    /**
     * @brief Calculates the Mohns Rho values for the given 1-dimensional parameter
     * @param VectorOfParameterMatrices
     * @param MohnsRhoVector
     * @return True if no error found, else False
     */
    bool calculateMohnsRhoFor1dParameter(
            std::vector<boost::numeric::ublas::matrix<double> >& VectorOfParameterMatrices,
            std::vector<double>& MohnsRhoVector);
    /**
     * @brief Calculates the Mohns Rho values for the given 2-dimensional parameter
     * @param VectorOfParameterMatrices ; vector containing 2d parameters (i.e., competition alpha)
     * @param MohnsRhoVector : vector of doubles containing Mohn's Rho value per species
     * @return True if no error found, else False
     */
    bool calculateMohnsRhoFor2dParameter(
            std::vector<boost::numeric::ublas::matrix<double> >& VectorOfParameterMatrices,
            std::vector<double>& MohnsRhoVector);
    /**
     * @brief Calculates the Mohns Rho values for the given time series
     * @param numPeels : number of peels, where a peel is defined as a
     * year range which is 1 year less than the previous
     * @param numSpecies : number of species
     * @param timeSeries : time series to calculate Mohns rho values for
     * @param mohnsRhoVector : Mohns Rho value per species
     * @return True if no error found, else False
     */
    bool calculateMohnsRhoForTimeSeries(
            const int& numPeels,
            const int& numSpecies,
            const std::vector<std::vector<double> >& timeSeries,
            std::vector<double>& mohnsRhoVector);
    /**
     * @brief Calculates the sum of squares fitness of ⵉ(Be - Bo)²
     * @param speciesWeights : vector of species fitness (biomass) weights
     * @param isEffortFitToCatch : true if the harvest type is EffortFitToCatch; false otherwise
     * @param ObsCatch : Observed Catch matrix
     * @param ObsBiomass : Observed Biomass matrix
     * @param EstCatch : Estimated Catch matrix (= qEB, catchability*Effort*EstBiomass)
     * @param EstBiomass : Estimated Biomass matrix
     * @param FitWeights : Biomass and Catch fit weights (from 0 to 1)
     * @return Returns the fitness value for SSE
     */
    double calculateLeastSquares(
            const boost::numeric::ublas::vector<double>& speciesWeights,
            const bool& isEffortFitToCatch,
            const boost::numeric::ublas::matrix<double>& ObsCatch,
            const boost::numeric::ublas::matrix<double>& ObsBiomass,
            const boost::numeric::ublas::matrix<double>& EstCatch,
            const boost::numeric::ublas::matrix<double>& EstBiomass,
            const boost::numeric::ublas::matrix<double>& FitWeights);
    /**
     * @brief Calculate the correlation coefficient: Σ[(Oₜ-Ō)(Eₜ-Ē)] / sqrt{Σ(Oₜ-Ō)²Σ(Eₜ-Ē)²}
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param runLength : time period in years
     * @param meanObserved : mean values of observed biomass per species
     * @param meanEstimated : mean values of estimated biomass per species
     * @param observed : observed biomass
     * @param estimated : estimated biomass
     * @param correlationCoefficient : the coefficient of correlation (aka. R)
     * @return True if no error found, else False
     */
    bool calculateR(const int& numSpeciesOrGuilds,
                    const int& runLength,
                    const std::vector<double>& meanObserved,
                    const std::vector<double>& meanEstimated,
                    const std::vector<double>& observed,
                    const std::vector<double>& estimated,
                    std::vector<double>& correlationCoefficient);
    /**
     * @brief Calculates the Reliability Index: exp[sqrt{(1/n)Σ([log(Oₜ/Eₜ)]²)}]
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param runLength : time period in years
     * @param observed : observed biomass
     * @param estimated : estimated biomass
     * @param reliabilityIndex : the reliability index of the passed time series
     * @return True if no error found, else False
     */
    bool calculateRI(const int& numSpeciesOrGuilds,
                     const int& runLength,
                     const std::vector<double>& observed,
                     const std::vector<double>& estimated,
                     std::vector<double>& reliabilityIndex);
    /**
     * @brief Calculates the root mean square error: sqrt{(Σ(Eₜ-Oₜ)²)/n}
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param runLength : time period in years
     * @param observed : observed biomass
     * @param estimated : estimated biomass
     * @param rmse : root mean square error
     * @return True if no error found, else False
     */
    bool calculateRMSE(const int& numSpeciesOrGuilds,
                       const int& runLength,
                       const std::vector<double>& observed,
                       const std::vector<double>& estimated,
                       std::vector<double>& rmse);
    /**
     * @brief Calculate coefficient of determination: SSdeviations/SStotals
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param ssDeviations : sum of the square deviations
     * @param ssTotals : sum of the square totals
     * @param rSquared : determination coefficients per species or guild
     */
    void calculateRSquared(const int& numSpeciesOrGuilds,
                           const std::vector<double>& ssDeviations,
                           const std::vector<double>& ssTotals,
                           std::vector<double>& rSquared);
    /**
     * @brief Calculates a sigma per species from the passed observed biomass matrix
     * @param ObsBiomass : observed biomass with arguments (time,species)
     * @return Vector of doubles where each double is the standard deviation of the particular species
     */
    std::vector<double> calculateSigmasSquared(const boost::numeric::ublas::matrix<double>& ObsBiomass);
    /**
     * @brief Calculates the sum of the square deviations: Σ(Eₜ-Ō)² over all t years
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param runLength : time period in years
     * @param estimated : estimated biomass
     * @param meanObserved : mean values of observed biomass per species
     * @param ssDeviations : the sum of the square deviation values
     * @return True if no error found, else False
     */
    bool calculateSSDeviations(const int& numSpeciesOrGuilds,
                               const int& runLength,
                               const std::vector<double>& estimated,
                               const std::vector<double>& meanObserved,
                               std::vector<double>& ssDeviations);
    /**
     * @brief Calculates the sum of the square residual values: Σ(Oₜ-Eₜ)² over all t years
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param runLength : time period in years
     * @param observed : observed biomass
     * @param estimated : estimated biomass
     * @param ssResiduals : sum of square residual values per species or guild
     */
    void calculateSSResiduals(const int& numSpeciesOrGuilds,
                              const int& runLength,
                              const std::vector<double>& observed,
                              const std::vector<double>& estimated,
                              std::vector<double>& ssResiduals);

    /**
     * @brief Calculate sum of the square totals: ssresiduals + ssdeviations
     * @param numSpeciesOrGuilds : the number of either species or guilds
     * @param ssDeviations : the sum of the square deviation values
     * @param ssResiduals : sum of square residual values per species or guild
     * @param ssTotal : sum of square total values per species or guild
     */
    void calculateSSTotals(const int& numSpeciesOrGuilds,
                           const std::vector<double>& ssDeviations,
                           const std::vector<double>& ssResiduals,
                           std::vector<double>& ssTotal);

    /**
     * @brief Returns the log factorial so that you can do really big numbers, for
     * example in the binomial or log-likelihood calculations
     * @param N : value to find log factorial of
     * @return : Returns the log factorial
     */
    double FACTLN(int N);

    double FACTRL(int N);

    double GAMMLN(double XX);

    /**
     * @brief This is a public procedure to calculate linear regression coefficients
     * and significance tests.  Results are output to a publicly declared variable
     * type LinRegOut that holds the outputs. This should be declared both as a type
     * and public variable in the declarations section of the users code. This
     * procedure accepts an Nx2 matrix of double data containing X and Y values
     * and the Number of data points, N. Any transformations for linearity
     * should be done prior to submitting the data to the procedure.
     * @param dataIn : matrix of data
     * @param numColumns : number of columns in matrix (aka. N); should be > 2
     * @param dataOut : output linear regression coefficients
     */
    void LinReg(boost::numeric::ublas::matrix<double> &dataIn,
                int &numColumns,
                LinRegOut &dataOut);

    double ProbF(double F,
                 int DF1,
                 int DF2);

    double ProbT(double T,
                 int DF);

    void ShepSRR(boost::numeric::ublas::matrix<double> &Data,
                 int &NYears,
                 NLRegOut &NLOut);

    /**
     * @brief Calculates the sums of squares against the actual data and
     * returns -SS to find a minimum.
     * @param parameters : parameters A, K, and B
     * @param numYears : number of years in data set
     * @param SRData : data matrix
     * @param negSumSquares : negative sum of squares value since we're minimizing
     */
    void USERFUNC(boost::numeric::ublas::vector<double> &parameters,
                  int &numYears,
                  boost::numeric::ublas::matrix<double> &SRData,
                  double &negSumSquares);

}

#endif
