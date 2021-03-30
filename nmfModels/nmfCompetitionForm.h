
#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <functional>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multi_array.hpp>

#include "nmfUtils.h"

class nmfCompetitionForm {

private:

    int m_numberParameters;
    int m_numSpecies;
    int m_numGuilds;
    std::string m_type; // "Null","NO_K"
    std::vector<std::pair<double,double> > m_parameterRanges;

    std::map<std::string, long double(nmfCompetitionForm::*)(
            const int& timeMinus1,
            const int& speciesNum,
            const double& biomassAtTime,
            const double& systemCarryingCapacity,
            const std::vector<double>& growthRate,
            const double& guildCarryingCapacity,
            const boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double>& EstBiomassGuild
    )> m_FunctionMap;



public:
    nmfCompetitionForm(std::string type);
   ~nmfCompetitionForm() {};

    int getNumParameters();
    void extractParameters(
            const std::vector<double>& parameters,
            int& startPos,
            boost::numeric::ublas::matrix<double>& competition,
            boost::numeric::ublas::matrix<double>& competitionBetaSpecies,
            boost::numeric::ublas::matrix<double>& competitionBetaGuilds,
            boost::numeric::ublas::matrix<double>& competitionBetaGuildsGuilds);
    void loadParameterRanges(
            std::vector<std::pair<double,double> >& parameterRanges,
            Data_Struct& beeStruct);
    long double evaluate(const int& timeMinus1,
                    const int& speciesNum,
                    const double& biomassAtTime,
                    const double& systemCarryingCapacity,
                    const std::vector<double>& growthRate,
                    const double& guildCarryingCapacity,
                    const boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                    const boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                    const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                    const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildGuild,
                    const boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                    const boost::numeric::ublas::matrix<double>& EstBiomassGuild);
    long double NoCompetition(const int& timeMinus1,
                         const int& speciesNum,
                         const double& biomassAtTime,
                         const double& systemCarryingCapacity,
                         const std::vector<double>& growthRate,
                         const double& guildCarryingCapacity,
                         const boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                         const boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                         const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                         const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildGuild,
                         const boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                         const boost::numeric::ublas::matrix<double>& EstBiomassGuild);
    long double NOKCompetition(const int& timeMinus1,
                          const int& speciesNum,
                          const double& biomassAtTime,
                          const double& systemCarryingCapacity,
                          const std::vector<double>& growthRate,
                          const double& guildCarryingCapacity,
                          const boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                          const boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                          const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                          const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildGuild,
                          const boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                          const boost::numeric::ublas::matrix<double>& EstBiomassGuild);
    long double MSPRODCompetition(const int& timeMinus1,
                             const int& speciesNum,
                             const double& biomassAtTime,
                             const double& systemCarryingCapacity,
                             const std::vector<double>& growthRate,
                             const double& guildCarryingCapacity,
                             const boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                             const boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                             const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                             const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildGuild,
                             const boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                             const boost::numeric::ublas::matrix<double>& EstBiomassGuild);
    long double AGGPRODCompetition(const int& timeMinus1,
                              const int& speciesNum,
                              const double& biomassAtTime,
                              const double& systemCarryingCapacity,
                              const std::vector<double>& growthRate,
                              const double& guildCarryingCapacity,
                              const boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                              const boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                              const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                              const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildGuild,
                              const boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                              const boost::numeric::ublas::matrix<double>& EstBiomassGuild);

};


