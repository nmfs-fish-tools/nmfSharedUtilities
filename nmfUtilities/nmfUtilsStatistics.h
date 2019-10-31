/**
 @file nmfStatUtils.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This header file defines MSVPA_X2 statistical utility functions.
 @date Jan 24, 2017
*/

#pragma once



#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <iomanip>
#include <chrono>
#include <math.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multi_array.hpp>

#include "nmfUtils.h"
#include "nmfUtilsComplex.h"


// Holds outputs from linear regression
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

    void   ShepSRR(boost::numeric::ublas::matrix<double> &Data,
                   int &NYears,
                   NLRegOut &NLOut);

    void   LinReg(boost::numeric::ublas::matrix<double> &Data,
                  int &N,
                  LinRegOut &OutVar);

    void   USERFUNC(boost::numeric::ublas::vector<double> &Parms,
                    int &NYears,
                    boost::numeric::ublas::matrix<double> &SRData,
                    double &Value);

    double BetaI(double A, double B, double X);

    double BetaCF(double A, double B, double X);

    double GAMMLN(double XX);
    double FACTRL(int N);
    double FACTLN(int N);
    double BETAFUNC(double A, double B);
    double ProbF(double F, int DF1, int DF2);
    double ProbT(double T, int DF);

    void calculateSSResiduals(const int& NumSpeciesOrGuilds,
                              const int& RunLength,
                              const std::vector<double>& Observed,
                              const std::vector<double>& Estimated,
                              std::vector<double>& SSResiduals);

    bool calculateSSDeviations(const int& NumSpeciesOrGuilds,
                               const int& RunLength,
                               const std::vector<double>& Estimated,
                               const std::vector<double>& Means,
                               std::vector<double>& SSDeviations);

    void calculateSSTotals(const int& NumSpeciesOrGuilds,
                           const std::vector<double>& SSDeviations,
                           const std::vector<double>& SSResiduals,
                           std::vector<double>& SSTotal);

    // Coefficient of Determination
    void calculateRSquared(const int& NumSpeciesOrGuilds,
                           const std::vector<double>& SSDeviations,
                           const std::vector<double>& SSTotals,
                           std::vector<double>& RSquared);

    void calculateAIC(const int& NumSpeciesOrGuilds,
                      const int& NumParameters,
                      const int& RunLength,
                      const std::vector<double>& SSResiduals,
                      std::vector<double>& AIC);

    // Coefficient of Correlation
    bool calculateR(const int& NumSpeciesOrGuilds,
                    const int& RunLength,
                    const std::vector<double>& MeanObserved,
                    const std::vector<double>& MeanEstimated,
                    const std::vector<double>& Observed,
                    const std::vector<double>& Estimated,
                    std::vector<double>& R);

    bool calculateRMSE(const int& NumSpeciesOrGuilds,
                       const int& RunLength,
                       const std::vector<double>& Observed,
                       const std::vector<double>& Estimated,
                       std::vector<double>& RMSE);

    bool calculateRI(const int& NumSpeciesOrGuilds,
                       const int& RunLength,
                       const std::vector<double>& Observed,
                       const std::vector<double>& Estimated,
                       std::vector<double>& RI);

    void calculateAE(const int& NumSpeciesOrGuilds,
                     const std::vector<double>& MeanObserved,
                     const std::vector<double>& MeanEstimated,
                     std::vector<double>& AE);

    bool calculateAAE(const int& NumSpeciesOrGuilds,
                      const int& RunLength,
                      const std::vector<double>& Observed,
                      const std::vector<double>& Estimated,
                      std::vector<double>& AAE);

    bool calculateMEF(const int& NumSpeciesOrGuilds,
                      const int& RunLength,
                      const std::vector<double>& MeanObserved,
                      const std::vector<double>& Observed,
                      const std::vector<double>& Estimated,
                      std::vector<double>& MEF);

    bool calculateMohnsRhoForParameter(const int& NumPeels,
                                       const int& NumSpeciesOrGuilds,
                                       const int& RunLength,
                                       const std::vector<double>& EstParameter,
                                       std::vector<double>& mohnsRhoValue);

    bool calculateMohnsRhoForTimeSeries(
            const int& NumPeels,
            const int& NumSpecies,
            const std::vector<std::vector<double> >& TimeSeries,
            std::vector<double>& mohnsRhoValue);
}


