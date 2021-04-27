/**
 * @file nmfUtilsStatisticsAveraging.h
 * @brief Definition for the nmfUtilsStatisticsAveraging class
 * @date Mar 04, 2021
 *
 * This class describes the nmfUtilsStatisticsAveraging class for the MultiSpecies tools. It
 * allows the user to save find the timeseries averages of the passed timeseries.
 *
 */
#pragma once

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <fstream>

#include <QList>
#include <QString>

#include "nmfUtils.h"

#include <boost/numeric/ublas/matrix.hpp>


class nmfUtilsStatisticsAveraging {

private:
    std::vector<double> m_Fitness;
    std::vector<double> m_AIC; // Not a vector of vectors because we're just using the model AIC value and not the AIC values from each of the species
    std::vector< std::vector<double> > m_EstInitBiomass;
    std::vector< std::vector<double> > m_EstGrowthRates;
    std::vector< std::vector<double> > m_EstCarryingCapacities;
    std::vector< std::vector<double> > m_EstPredationExponent;
    std::vector< std::vector<double> > m_EstCatchability;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionAlpha;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaSpecies;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaGuilds;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaGuildsGuilds;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstPredationRho;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstPredationHandling;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstBiomass;
    std::vector<double> m_AveInitBiomass;
    std::vector<double> m_AveGrowthRates;
    std::vector<double> m_AveCarryingCapacities;
    std::vector<double> m_AvePredationExponent;
    std::vector<double> m_AveCatchability;
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
    std::vector< std::vector<double> > m_EstCarryingCapacities_trimmed;
    std::vector< std::vector<double> > m_EstPredationExponent_trimmed;
    std::vector< std::vector<double> > m_EstCatchability_trimmed;
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
                                 const bool& isPercent);
    void clearAveragedData();
    void clearEstData();
    void clearTrimmedData();

public:
    nmfUtilsStatisticsAveraging();
   ~nmfUtilsStatisticsAveraging() {}

    void loadEstData(double& Fitness,
                     std::vector<double>& AIC,
                     std::vector<double>& EstInitBiomass,
                     std::vector<double>& EstGrowthRates,
                     std::vector<double>& EstCarryingCapacities,
                     std::vector<double>& EstExponent,
                     std::vector<double>& EstCatchability,
                     boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuilds,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildsGuilds,
                     boost::numeric::ublas::matrix<double>& EstPredationRho,
                     boost::numeric::ublas::matrix<double>& EstPredationHandling,
                     boost::numeric::ublas::matrix<double>& EstBiomass);
    void calculateAverage(const int& numberOfTopRunsToUse,
                          const bool& isPercent,
                          const QString& averagingAlgorithm);
    void calculateUnweighted();
    void calculateAICWeighted();
    void getAveData(std::vector<double>& Fitness,
                    std::vector<double>& AveInitBiomass,
                    std::vector<double>& AveGrowthRates,
                    std::vector<double>& AveCarryingCapacities,
                    std::vector<double>& AveExponent,
                    std::vector<double>& AveCatchability,
                    boost::numeric::ublas::matrix<double>& AveCompetitionAlpha,
                    boost::numeric::ublas::matrix<double>& AveCompetitionBetaSpecies,
                    boost::numeric::ublas::matrix<double>& AveCompetitionBetaGuilds,
                    boost::numeric::ublas::matrix<double>& AveCompetitionBetaGuildsGuilds,
                    boost::numeric::ublas::matrix<double>& AvePredationRho,
                    boost::numeric::ublas::matrix<double>& AvePredationHandling,
                    boost::numeric::ublas::matrix<double>& AveBiomass);

};



