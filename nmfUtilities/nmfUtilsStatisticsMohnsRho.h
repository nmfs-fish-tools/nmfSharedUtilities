/**
 * @file nmfUtilsStatisticsMohnsRho.h
 * @brief Definition for the nmfUtilsStatisticsMohnsRho class
 * @date Mar 04, 2021
 *
 * This class describes the nmfUtilsStatisticsMohnsRho class for the MultiSpecies tools. It
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
#include "nmfUtilsStatistics.h"

#include <boost/numeric/ublas/matrix.hpp>


class nmfUtilsStatisticsMohnsRho {

private:
    std::vector<int> m_Peel;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstInitBiomass;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstGrowthRates;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCarryingCapacities;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCatchability;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstSurveyQ;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionAlpha;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaSpecies;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaGuilds;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstCompetitionBetaGuildsGuilds;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstPredationRho;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstPredationHandling;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstPredationExponent;
    std::vector< boost::numeric::ublas::matrix<double> > m_EstBiomass;

    std::map<QString,std::vector<boost::numeric::ublas::matrix<double> > > m_ParameterMap;

    void clearAveragedData();
    void clearEstData();

public:
    nmfUtilsStatisticsMohnsRho();
   ~nmfUtilsStatisticsMohnsRho() {}

    int getNumPeels();
    void loadEstData(int& Peel,
                     std::vector<double>& EstInitBiomass,
                     std::vector<double>& EstGrowthRates,
                     std::vector<double>& EstCarryingCapacities,
                     std::vector<double>& EstExponent,
                     std::vector<double>& EstCatchability,
                     std::vector<double>& EstSurveyQ,
                     boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuilds,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildsGuilds,
                     boost::numeric::ublas::matrix<double>& EstPredationRho,
                     boost::numeric::ublas::matrix<double>& EstPredationHandling,
                     boost::numeric::ublas::matrix<double>& EstBiomass);
    bool calculateMohnsRhoVectors(
            const QStringList& EstParamNames,
            std::vector<std::vector<double> >& MohnsRhoVectors);


};



