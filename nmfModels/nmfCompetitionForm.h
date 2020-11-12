
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

    std::map<std::string, double(*)(
            int& timeMinus1,
            int& speciesNum,
            double& biomassAtTime,
            double& systemCarryingCapacity,
            std::vector<double>& growthRate,
            double& guildCarryingCapacity,
            boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
            boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
            boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
            boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
            boost::numeric::ublas::matrix<double>& EstBiomassGuild
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
            boost::numeric::ublas::matrix<double>& competitionBetaGuilds);
    void loadParameterRanges(
            std::vector<std::pair<double,double> >& parameterRanges,
            Data_Struct& beeStruct);
    double evaluate(int& timeMinus1,
                    int& speciesNum,
                    double& biomassAtTime,
                    double& systemCarryingCapacity,
                    std::vector<double>& growthRate,
                    double& guildCarryingCapacity,
                    boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                    boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                    boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                    boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                    boost::numeric::ublas::matrix<double>& EstBiomassGuild);
    static double NOKCompetition(int& timeMinus1,
                                 int& speciesNum,
                                 double& biomassAtTime,
                                 double& systemCarryingCapacity,
                                 std::vector<double>& growthRate,
                                 double& guildCarryingCapacity,
                                 boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                                 boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                                 boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                                 boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                                 boost::numeric::ublas::matrix<double>& EstBiomassGuild);
    static double MSPRODCompetition(int& timeMinus1,
                                    int& speciesNum,
                                    double& biomassAtTime,
                                    double& systemCarryingCapacity,
                                    std::vector<double>& growthRate,
                                    double& guildCarryingCapacity,
                                    boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                                    boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                                    boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                                    boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                                    boost::numeric::ublas::matrix<double>& EstBiomassGuild);
    static double AGGPRODCompetition(int& timeMinus1,
                                     int& speciesNum,
                                     double& biomassAtTime,
                                     double& systemCarryingCapacity,
                                     std::vector<double>& growthRate,
                                     double& guildCarryingCapacity,
                                     boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                                     boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                                     boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                                     boost::numeric::ublas::matrix<double>& EstBiomassGuild);

};


