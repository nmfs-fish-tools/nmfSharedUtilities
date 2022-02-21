/**
 * @file nmfUtilsStatisticsMohnsRho.h
 * @brief Definition for the nmfUtilsStatisticsMohnsRho class
 * @date Mar 04, 2021
 *
 * This class describes the nmfUtilsStatisticsMohnsRho class for the MultiSpecies tools. It
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

#ifndef NMF_UTILS_STATISTICS_MOHNS_RHO
#define NMF_UTILS_STATISTICS_MOHNS_RHO

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <fstream>

#include <QList>
#include <QString>

#include "nmfUtils.h"
#include "nmfUtilsStatistics.h"

#include <boost/numeric/ublas/matrix.hpp>

/**
 * @brief Class containing data and methods for Mohn's Rho Summary Statistics calculations
 */
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
    /**
     * @brief Class constructor
     */
    nmfUtilsStatisticsMohnsRho();
   ~nmfUtilsStatisticsMohnsRho() {}

    /**
     * @brief Calculates the Mohn's Rho values for the passed in parameters
     * @param EstParamNames : Estimated parameter names on which to calculate the Mohn's Rho values
     * @param MohnsRhoVectors : Mohn's Rhos values bundled as a vector
     * @return True if calculation was successful, otherwise false
     */
    bool calculateMohnsRhoVectors(
            const QStringList& EstParamNames,
            std::vector<std::vector<double> >& MohnsRhoVectors);
    /**
     * @brief Gets the number of peels for the Mohn's Rho run
     * @return Number of peels of yearly ranges
     */
    int getNumPeels();
    /**
     * @brief Loads the passed in estimated values into their respective class structures
     * @param Peel : current peel number
     * @param EstInitBiomass : estimated initial biomass vector
     * @param EstGrowthRates : estimated growth rate vector
     * @param EstCarryingCapacities : estimated carrying capacities vector
     * @param EstPredationExponent : estimated predation exponent vector
     * @param EstCatchability : estimated catchability vector
     * @param EstSurveyQ : estimated survey q vector
     * @param EstCompetitionAlpha : estimated competition alpha matrix
     * @param EstCompetitionBetaSpecies : estimated competition beta species matrix
     * @param EstCompetitionBetaGuilds : estimated competition beta guilds matrix
     * @param EstCompetitionBetaGuildsGuilds : estimated competition beta guilds guilds matrix
     * @param EstPredationRho : estimated predation rho matrix
     * @param EstPredationHandling : estimated predation handling matrix
     * @param EstBiomass : estimated biomass matrix
     */
    void loadEstData(int& Peel,
                     std::vector<double>& EstInitBiomass,
                     std::vector<double>& EstGrowthRates,
                     std::vector<double>& EstCarryingCapacities,
                     std::vector<double>& EstPredationExponent,
                     std::vector<double>& EstCatchability,
                     std::vector<double>& EstSurveyQ,
                     boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuilds,
                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildsGuilds,
                     boost::numeric::ublas::matrix<double>& EstPredationRho,
                     boost::numeric::ublas::matrix<double>& EstPredationHandling,
                     boost::numeric::ublas::matrix<double>& EstBiomass);


};

#endif
