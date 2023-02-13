/**
 * @file nmfUtilsStatisticsAveraging.h
 * @brief Definition for the nmfUtilsStatisticsAveraging class
 * @date Mar 04, 2021
 *
 * This class describes the nmfUtilsStatisticsAveraging class for the MultiSpecies tools. It
 * allows the user to save find the timeseries averages of the passed timeseries.
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

#ifndef NMF_UTILS_STATISTICS_AVERAGING
#define NMF_UTILS_STATISTICS_AVERAGING

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <fstream>

#include <QList>
#include <QString>

#include "nmfUtils.h"

#include <boost/numeric/ublas/matrix.hpp>

/**
 * @brief This class is used to contain all data that comprise an average estimation
 */
class nmfUtilsStatisticsAveraging {

private:
    std::vector<double> m_Fitness;
    std::vector<double> m_AIC; // Not a vector of vectors because we're just using the model AIC value and not the AIC values from each of the species
    std::vector< std::vector<double> > m_EstInitBiomass;
    std::vector< std::vector<double> > m_EstGrowthRates;
    std::vector< std::vector<double> > m_EstGrowthRateShapes;
    std::vector< std::vector<double> > m_EstGrowthRateCovariateCoeffs;
    std::vector< std::vector<double> > m_EstCarryingCapacities;
    std::vector< std::vector<double> > m_EstCarryingCapacityCovariateCoeffs;
    std::vector< std::vector<double> > m_EstPredationExponent;
    std::vector< std::vector<double> > m_EstCatchability;
    std::vector< std::vector<double> > m_EstCatchabilityCovariateCoeffs;
    std::vector< std::vector<double> > m_EstSurveyQ;
    std::vector< std::vector<double> > m_EstSurveyQCovariateCoeffs;
    std::vector< std::vector<double> > m_EstBMSY;
    std::vector< std::vector<double> > m_EstMSY;
    std::vector< std::vector<double> > m_EstFMSY;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionAlpha;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaSpecies;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaGuilds;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaGuildsGuilds;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstPredationRho;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstPredationHandling;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstBiomass;
    std::vector<double> m_AveInitBiomass;
    std::vector<double> m_AveGrowthRates;
    std::vector<double> m_AveGrowthRateShapes;
    std::vector<double> m_AveGrowthRateCovariateCoeffs;
    std::vector<double> m_AveCarryingCapacities;
    std::vector<double> m_AveCarryingCapacityCovariateCoeffs;
    std::vector<double> m_AvePredationExponent;
    std::vector<double> m_AveCatchability;
    std::vector<double> m_AveCatchabilityCovariateCoeffs;
    std::vector<double> m_AveSurveyQ;
    std::vector<double> m_AveSurveyQCovariateCoeffs;
    std::vector<double> m_AveBMSY;
    std::vector<double> m_AveMSY;
    std::vector<double> m_AveFMSY;
    boost::numeric::ublas::matrix<double> m_AveCompetitionAlpha;
    boost::numeric::ublas::matrix<double> m_AveCompetitionBetaSpecies;
    boost::numeric::ublas::matrix<double> m_AveCompetitionBetaGuilds;
    boost::numeric::ublas::matrix<double> m_AveCompetitionBetaGuildsGuilds;
    boost::numeric::ublas::matrix<double> m_AvePredationRho;
    boost::numeric::ublas::matrix<double> m_AvePredationHandling;
    boost::numeric::ublas::matrix<double> m_AveBiomass;
    std::vector<double> m_AIC_trimmed;
    std::vector< std::vector<double> > m_EstInitBiomass_trimmed;
    std::vector< std::vector<double> > m_EstGrowthRates_trimmed;
    std::vector< std::vector<double> > m_EstGrowthRateShapes_trimmed;
    std::vector< std::vector<double> > m_EstGrowthRateCovariateCoeffs_trimmed;
    std::vector< std::vector<double> > m_EstCarryingCapacities_trimmed;
    std::vector< std::vector<double> > m_EstCarryingCapacityCovariateCoeffs_trimmed;
    std::vector< std::vector<double> > m_EstCatchability_trimmed;
    std::vector< std::vector<double> > m_EstCatchabilityCovariateCoeffs_trimmed;
    std::vector< std::vector<double> > m_EstPredationExponent_trimmed;
    std::vector< std::vector<double> > m_EstSurveyQ_trimmed;
    std::vector< std::vector<double> > m_EstSurveyQCovariateCoeffs_trimmed;
    std::vector< std::vector<double> > m_EstBMSY_trimmed;
    std::vector< std::vector<double> > m_EstMSY_trimmed;
    std::vector< std::vector<double> > m_EstFMSY_trimmed;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionAlpha_trimmed;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaSpecies_trimmed;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaGuilds_trimmed;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaGuildsGuilds_trimmed;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstPredationRho_trimmed;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstPredationHandling_trimmed;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstBiomass_trimmed;

    std::map<QString, void(nmfUtilsStatisticsAveraging::*)()> m_FunctionMap;
    void calculateWeighted(const std::vector<double>& weights);
    bool createTrimmedStructures(const int& numberOfTopRunsToUse,
                                 const bool& isUsingAll,
                                 const bool& isPercent);
    void clearAveragedData();
    void clearEstData();
    void clearTrimmedData();

public:
    /**
     * @brief Class constructor
     */
    nmfUtilsStatisticsAveraging();
   ~nmfUtilsStatisticsAveraging() {}

    /**
     * @brief This method calculates the average biomass/parameters of the sub runs
     * @param numberOfTopRunsToUse : determines how many and which sub runs to use
     * @param isUsingAll : defines if the the "using All" item has been selected for the averaging
     * @param isPercent  : defines if the numberOfTopRunsToUse is an absolute number or a percentage
     * @param averagingAlgorithm : algorithm by which to average the sub runs
     */
    void calculateAverage(const int& numberOfTopRunsToUse,
                          const bool& isUsingAll,
                          const bool& isPercent,
                          const QString& averagingAlgorithm);
    /**
     * @brief Calculates an unweighted average
     */
    void calculateUnweighted();
    /**
     * @brief Calculates an AIC (Akaike Information Criterion) average
     */
    void calculateAICWeighted();
    /**
     * @brief Returns (by reference) the averaged parameters and biomass
     * @param Fitness : average fitness values vector
     * @param AveInitBiomass : average initial biomass vector
     * @param AveGrowthRates : average growth rate vector
     * @param AveGrowthRateShapes : average growth rate shapes parameter vector
     * @param AveGrowthRateCovariateCoeffs : average growth rate covariate coefficients vector
     * @param AveCarryingCapacities : average carrying capacities vector
     * @param AveCarryingCapacityCovariateCoeffs : average carrying capacity covariate coefficients vector
     * @param AvePredationExponent : average predation exponent vector
     * @param AveCatchability : average catchability vector
     * @param AveCatchabilityCovariateCoeffs : average catchability covariate coefficients vector
     * @param AveSurveyQ : average survey q vector
     * @param AveSurveyQCovariateCoeffs : average survey q covariate coefficients vector
     * @param AveBMSY : average Biomass MSY values for all species
     * @param AveMSY  : average MSY values for all species
     * @param AveFMSY : average Fishing MSY values for all species
     * @param AveCompetitionAlpha : average competition alpha matrix
     * @param AveCompetitionBetaSpecies : average competition beta for species matrix
     * @param AveCompetitionBetaGuilds : average competition beta for guilds matrix
     * @param AveCompetitionBetaGuildsGuilds : average competition beta for guilds guilds matrix
     * @param AvePredationRho : average predation rho matrix
     * @param AvePredationHandling : average predation handling matrix
     * @param AveBiomass : average biomass matrix
     */
    void getAveData(std::vector<double>& Fitness,
                    std::vector<double>& AveInitBiomass,
                    std::vector<double>& AveGrowthRates,
                    std::vector<double>& AveGrowthRateShapes,
                    std::vector<double>& AveGrowthRateCovariateCoeffs,
                    std::vector<double>& AveCarryingCapacities,
                    std::vector<double>& AveCarryingCapacityCovariateCoeffs,
                    std::vector<double>& AvePredationExponent,
                    std::vector<double>& AveCatchability,
                    std::vector<double>& AveCatchabilityCovariateCoeffs,
                    std::vector<double>& AveSurveyQ,
                    std::vector<double>& AveSurveyQCovariateCoeffs,
                    std::vector<double>& AveBMSY,
                    std::vector<double>& AveMSY,
                    std::vector<double>& AveFMSY,
                    boost::numeric::ublas::matrix<double>& AveCompetitionAlpha,
                    boost::numeric::ublas::matrix<double>& AveCompetitionBetaSpecies,
                    boost::numeric::ublas::matrix<double>& AveCompetitionBetaGuilds,
                    boost::numeric::ublas::matrix<double>& AveCompetitionBetaGuildsGuilds,
                    boost::numeric::ublas::matrix<double>& AvePredationRho,
                    boost::numeric::ublas::matrix<double>& AvePredationHandling,
                    boost::numeric::ublas::matrix<double>& AveBiomass);
    /**
     * @brief Loads the passed in estimated values into their respective class structures
     * @param Fitness : fitness value of estimated values
     * @param AIC : Akaike Information Criterion of estimated values vector
     * @param EstInitBiomass : estimated initial biomass vector
     * @param EstGrowthRates : estimated growth rate vector
     * @param EstGrowthRateShapes : estimated growth rate shapes parameter vector
     * @param EstGrowthRateCovariateCoeffs : estimated growth rate covariate coefficient vector
     * @param EstCarryingCapacities : estimated carrying capacities vector
     * @param EstCarryingCapacityCovariateCoeffs : estimated carrying capacity covariate coefficients vector
     * @param EstPredationExponent : estimated predation exponent vector
     * @param EstCatchability : estimated catchability vector
     * @param EstCatchabilityCovariateCoeffs : estimated catchability covariate coefficients vector
     * @param EstSurveyQ : estimated survey q vector
     * @param EstSurveyQCovariateCoeffs : estimated survey q covariate coefficient vector
     * @param EstBMSY : estimated Biomass MSY values
     * @param EstMSY  : estimated MSY values
     * @param EstFMSY : estimated Fishing MSY values
     * @param EstCompetitionAlpha : estimated competition alpha matrix
     * @param EstCompetitionBetaSpecies : estimated competition beta species matrix
     * @param EstCompetitionBetaGuilds : estimated competition beta guilds matrix
     * @param EstCompetitionBetaGuildsGuilds : estimated competition beta guilds guilds matrix
     * @param EstPredationRho : estimated predation rho matrix
     * @param EstPredationHandling : estimated predation handling matrix
     * @param EstBiomass : estimated biomass matrix
     */
    void loadEstData(double& Fitness,
                     std::vector<double>& AIC,
                     std::vector<double>& EstInitBiomass,
                     std::vector<double>& EstGrowthRates,
                     std::vector<double>& EstGrowthRateShapes,
                     std::vector<double>& EstGrowthRateCovariateCoeffs,
                     std::vector<double>& EstCarryingCapacities,
                     std::vector<double>& EstCarryingCapacityCovariateCoeffs,
                     std::vector<double>& EstPredationExponent,
                     std::vector<double>& EstCatchability,
                     std::vector<double>& EstCatchabilityCovariateCoeffs,
                     std::vector<double>& EstSurveyQ,
                     std::vector<double>& EstSurveyQCovariateCoeffs,
                     std::vector<double>& EstBMSY,
                     std::vector<double>& EstMSY,
                     std::vector<double>& EstFMSY,
                     boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuilds,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildsGuilds,
                     boost::numeric::ublas::matrix<double>& EstPredationRho,
                     boost::numeric::ublas::matrix<double>& EstPredationHandling,
                     boost::numeric::ublas::matrix<double>& EstBiomass);
};

#endif
