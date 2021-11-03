
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
    std::map<std::string,std::string> m_CompetitionMap;
    std::map<std::string,std::string> m_CompetitionKey;
    bool m_isAGGPROD;

    std::map<std::string, long double(nmfCompetitionForm::*)(
            const int& timeMinus1,
            const int& speciesOrGuildNum,
            const double& biomassAtTime,
            const std::vector<double>& growthRate,
            const boost::numeric::ublas::matrix<double> &growthRateCovariate,
            const double& guildCarryingCapacity,
            const double& systemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate
    )> m_FunctionMap;



public:
    nmfCompetitionForm(std::string type);
   ~nmfCompetitionForm() {};

    int getNumParameters();
    void setType(std::string newType);
    void extractParameters(
            const std::vector<double>& parameters,
            int& startPos,
            boost::numeric::ublas::matrix<double>& competition,
            boost::numeric::ublas::matrix<double>& competitionBetaSpecies,
            boost::numeric::ublas::matrix<double>& competitionBetaGuilds,
            boost::numeric::ublas::matrix<double>& competitionBetaGuildsGuilds);
    void loadParameterRanges(
            std::vector<std::pair<double,double> >& parameterRanges,
            nmfStructsQt::ModelDataStruct& beeStruct);
    long double evaluate(
            const int& timeMinus1,
            const int& speciesOrGuildNum,
            const double& biomassAtTime,
            const std::vector<double>& growthRate,
            const boost::numeric::ublas::matrix<double> &growthRateCovariate,
            const double& guildCarryingCapacity,
            const double& systemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate);
    long double NoCompetition(
            const int& timeMinus1,
            const int& speciesOrGuildNum,
            const double& biomassAtTime,
            const std::vector<double>& growthRate,
            const boost::numeric::ublas::matrix<double> &growthRateCovariate,
            const double& guildCarryingCapacity,
            const double& systemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate);
    long double NOKCompetition(
            const int& timeMinus1,
            const int& speciesOrGuildNum,
            const double& biomassAtTime,
            const std::vector<double>& growthRate,
            const boost::numeric::ublas::matrix<double> &growthRateCovariate,
            const double& guildCarryingCapacity,
            const double& systemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate);
    long double MSPRODCompetition(
            const int& timeMinus1,
            const int& speciesOrGuildNum,
            const double& biomassAtTime,
            const std::vector<double>& growthRate,
            const boost::numeric::ublas::matrix<double> &growthRateCovariate,
            const double& guildCarryingCapacity,
            const double& systemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate);
    long double AGGPRODCompetition(
            const int& timeMinus1,
            const int& speciesOrGuildNum,
            const double& biomassAtTime,
            const std::vector<double>& growthRate,
            const boost::numeric::ublas::matrix<double> &growthRateCovariate,
            const double& guildCarryingCapacity,
            const double& systemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate);
    void setupFormMaps();
    void setAggProd(bool isAggProd);
    std::string getExpression();
    std::string getKey();
};


