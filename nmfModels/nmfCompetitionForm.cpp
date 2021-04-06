
#include "nmfCompetitionForm.h"

nmfCompetitionForm::nmfCompetitionForm(std::string competitionType)
{
    m_type             = competitionType;
    m_numSpecies       = 0;
    m_numberParameters = 0;
    m_parameterRanges.clear();

    m_FunctionMap["Null"]     = &nmfCompetitionForm::NoCompetition;
    m_FunctionMap["NO_K"]     = &nmfCompetitionForm::NOKCompetition;
    m_FunctionMap["MS-PROD"]  = &nmfCompetitionForm::MSPRODCompetition;
    m_FunctionMap["AGG-PROD"] = &nmfCompetitionForm::AGGPRODCompetition;
}

int
nmfCompetitionForm::getNumParameters()
{
    return m_numberParameters;
}

void
nmfCompetitionForm::extractParameters(
        const std::vector<double>& parameters,
        int& startPos,
        boost::numeric::ublas::matrix<double>& competitionAlpha,
        boost::numeric::ublas::matrix<double>& competitionBetaSpecies,
        boost::numeric::ublas::matrix<double>& competitionBetaGuilds,
        boost::numeric::ublas::matrix<double>& competitionBetaGuildsGuilds)

{
    if (m_type == "NO_K") {
        for (int i=0; i<m_numSpecies; ++i) {
            for (int j=0; j<m_numSpecies; ++j) {
                competitionAlpha(i,j) = parameters[startPos++];
            }
        }
    } else if (m_type == "MS-PROD") {
        for (int i=0; i<m_numSpecies; ++i) {
            for (int j=0; j<m_numSpecies; ++j) {                
                competitionBetaSpecies(i,j) = parameters[startPos++];
//std::cout << "Extracting MS: " <<   competitionBetaSpecies(i,j) << std::endl;
            }
        }
    }
    if (m_type == "MS-PROD") {
        for (int i=0; i<m_numSpecies; ++i) {
            for (int j=0; j<m_numGuilds; ++j) {
                competitionBetaGuilds(i,j) = parameters[startPos++];
//std::cout << "Extracting AGG: " <<   competitionBetaGuilds(i,j) << std::endl;
            }
        }
    } else if (m_type == "AGG-PROD") {
        for (int i=0; i<m_numGuilds; ++i) {
            for (int j=0; j<m_numGuilds; ++j) {
                competitionBetaGuildsGuilds(i,j) = parameters[startPos++];
//std::cout << "Extracting AGG: " <<   competitionBetaGuildsGuilds(i,j) << std::endl;
            }
        }
    }

}


void
nmfCompetitionForm::loadParameterRanges(
        std::vector<std::pair<double,double> >& parameterRanges,
        Data_Struct& dataStruct)
{
    bool isCheckedAlpha              = nmfUtils::isEstimateParameterChecked(dataStruct,"CompetitionAlpha");
    bool isCheckedBetaSpeciesSpecies = nmfUtils::isEstimateParameterChecked(dataStruct,"CompetitionBetaSpeciesSpecies");
    bool isCheckedBetaGuildSpecies   = nmfUtils::isEstimateParameterChecked(dataStruct,"CompetitionBetaGuildSpecies");
    bool isCheckedBetaGuildGuild     = nmfUtils::isEstimateParameterChecked(dataStruct,"CompetitionBetaGuildGuild");
    double min;
    std::pair<double,double> aPair;

    if (m_type == "Null")
        return;

    m_numSpecies = dataStruct.NumSpecies;
    m_numGuilds  = dataStruct.NumGuilds;

    if (m_type == "NO_K") {
        for (unsigned i=0; i<dataStruct.CompetitionMin.size(); ++i) {
            for (unsigned j=0; j<dataStruct.CompetitionMin[0].size(); ++j) {
                if (isCheckedAlpha) {
                    aPair = std::make_pair(dataStruct.CompetitionMin[i][j],
                                           dataStruct.CompetitionMax[i][j]);
                } else {
                    min   = dataStruct.CompetitionMin[i][j];
                    aPair = std::make_pair(min,min);
                }
                parameterRanges.emplace_back(aPair);
                m_parameterRanges.emplace_back(aPair);
            }
        }
        m_numberParameters += dataStruct.CompetitionMin.size() *
                              dataStruct.CompetitionMin[0].size();
    }

    else if (m_type == "MS-PROD") {
        for (unsigned i=0; i<dataStruct.CompetitionBetaSpeciesMin.size(); ++i) {
            for (unsigned j=0; j<dataStruct.CompetitionBetaSpeciesMin[0].size(); ++j) {
                if (isCheckedBetaSpeciesSpecies) {
                    aPair = std::make_pair(dataStruct.CompetitionBetaSpeciesMin[i][j],
                                           dataStruct.CompetitionBetaSpeciesMax[i][j]);
                } else {
                    min   = dataStruct.CompetitionBetaSpeciesMin[i][j];
                    aPair = std::make_pair(min,min);
                }
                parameterRanges.emplace_back(aPair);
                m_parameterRanges.emplace_back(aPair);
            }
        }
        m_numberParameters += dataStruct.CompetitionBetaSpeciesMin.size() *
                              dataStruct.CompetitionBetaSpeciesMin[0].size();

        for (unsigned i=0; i<dataStruct.CompetitionBetaGuildsMin.size(); ++i) {
            for (unsigned j=0; j<dataStruct.CompetitionBetaGuildsMin[0].size(); ++j) {
                if (isCheckedBetaGuildSpecies) {
                    aPair = std::make_pair(dataStruct.CompetitionBetaGuildsMin[i][j],
                                           dataStruct.CompetitionBetaGuildsMax[i][j]);
                } else {
                    min   = dataStruct.CompetitionBetaGuildsMin[i][j];
                    aPair = std::make_pair(min,min);
                }
                parameterRanges.emplace_back(aPair);
                m_parameterRanges.emplace_back(aPair);
            }
        }
        m_numberParameters += dataStruct.CompetitionBetaGuildsMin.size() *
                              dataStruct.CompetitionBetaGuildsMin[0].size();
    }

    else if (m_type == "AGG-PROD") {
        for (unsigned i=0; i<dataStruct.CompetitionBetaGuildsGuildsMin.size(); ++i) {
            for (unsigned j=0; j<dataStruct.CompetitionBetaGuildsGuildsMin[0].size(); ++j) {
                if (isCheckedBetaGuildGuild) {
                    aPair = std::make_pair(dataStruct.CompetitionBetaGuildsGuildsMin[i][j],
                                           dataStruct.CompetitionBetaGuildsGuildsMax[i][j]);
                } else {
                    min   = dataStruct.CompetitionBetaGuildsGuildsMin[i][j];
                    aPair = std::make_pair(min,min);
                }
                parameterRanges.emplace_back(aPair);
                m_parameterRanges.emplace_back(aPair);
            }
        }
        m_numberParameters += dataStruct.CompetitionBetaGuildsGuildsMin.size() *
                              dataStruct.CompetitionBetaGuildsGuildsMin[0].size();
    }
}

long double
nmfCompetitionForm::evaluate(const int& TimeMinus1,
                             const int& SpeciesNum,
                             const double& BiomassAtTime,
                             const double& SystemCarryingCapacity,
                             const std::vector<double>& GrowthRate,
                             const double& GuildCarryingCapacity,
                             const boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                             const boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                             const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                             const boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildGuild,
                             const boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                             const boost::numeric::ublas::matrix<double>& EstBiomassGuild)
{
    if (m_FunctionMap.find(m_type) == m_FunctionMap.end()) {
        return 0;
    } else {
        return (this->*m_FunctionMap[m_type])(TimeMinus1,SpeciesNum,BiomassAtTime,
                                     SystemCarryingCapacity,GrowthRate,
                                     GuildCarryingCapacity,
                                     EstCompetitionAlpha,
                                     EstCompetitionBetaSpecies,
                                     EstCompetitionBetaGuild,
                                     EstCompetitionBetaGuildGuild,
                                     EstBiomassSpecies,
                                     EstBiomassGuild);
    }
}

long double
nmfCompetitionForm::NoCompetition(const int& timeMinus1,
                                  const int& speciesNum,
                                  const double& biomassAtTime,
                                  const double& systemCarryingCapacity,
                                  const std::vector<double>& growthRate,
                                  const double& guildCarryingCapacity,
                                  const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
                                  const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
                                  const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
                                  const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
                                  const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
                                  const boost::numeric::ublas::matrix<double> &EstBiomassGuild)
{
    return 0.0;
}

/*
 *
 *  B(i,t)[(∑{α(i,j)B(j,t)}]
 *
 */
long double
nmfCompetitionForm::NOKCompetition(const int& timeMinus1,
                                   const int& speciesNum,
                                   const double& biomassAtTime,
                                   const double& systemCarryingCapacity,
                                   const std::vector<double>& growthRate,
                                   const double& guildCarryingCapacity,
                                   const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
                                   const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
                                   const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
                                   const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
                                   const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
                                   const boost::numeric::ublas::matrix<double> &EstBiomassGuild)
{
    long double competitionSum = 0;

    for (unsigned row=0; row<EstCompetitionAlpha.size2(); ++row) {
        competitionSum += (long double)(EstCompetitionAlpha(row,speciesNum)) * (long double)(EstBiomassSpecies(timeMinus1,row));
    }

    return double(biomassAtTime)*double(competitionSum);
}


/*
 *  r(i)B(i,t)[(∑β(i,j)B(j,t))/KG - (∑β(i,G)B(G,t))/(K(σ) - K(G))]
 */
long double
nmfCompetitionForm::MSPRODCompetition(
        const int& timeMinus1,
        const int& speciesNum,
        const double& biomassAtTime,
        const double& systemCarryingCapacity,
        const std::vector<double>& growthRate,
        const double& guildCarryingCapacity,
        const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
        const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
        const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
        const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
        const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
        const boost::numeric::ublas::matrix<double> &EstBiomassGuild)
{
    unsigned numSpecies = growthRate.size();
    unsigned numGuilds  = EstCompetitionBetaGuild.size2();
    double sumOverSpecies = 0;
    double sumOverGuilds  = 0;
    double term1;
    double term2;

    if (guildCarryingCapacity == 0) {
        std::cout << "[Error 1] nmfCompetitionForm::MSPRODCompetition: guildCarryingCapacity is 0" << std::endl;
        return 0;
    }

    if (systemCarryingCapacity == guildCarryingCapacity) {
        std::cout << "[Error 2] nmfCompetitionForm::MSPRODCompetition: systemCarryingCapacity same as guildCarryingCapacity" << std::endl;
        return 0;
    }
    for (unsigned j=0; j<numSpecies; ++j) {
        sumOverSpecies += EstCompetitionBetaSpecies(speciesNum,j)*EstBiomassSpecies(timeMinus1,j);
    }
    for (unsigned j=0; j<numGuilds; ++j) {
        sumOverGuilds += EstCompetitionBetaGuild(speciesNum,j)*EstBiomassGuild(timeMinus1,j);
    }

    term1 = sumOverSpecies/guildCarryingCapacity;
    term2 = sumOverGuilds/(systemCarryingCapacity - guildCarryingCapacity);

    return growthRate[speciesNum]*biomassAtTime*(term1-term2);
}


/*
 *  r(i)B(i,t)[(∑β(i,G)B(G,t))/(Kσ - KG)]
 */
long double
nmfCompetitionForm::AGGPRODCompetition(
        const int& timeMinus1,
        const int& speciesOrGuildNum,
        const double& biomassAtTime,
        const double& systemCarryingCapacity,
        const std::vector<double>& growthRate,
        const double& guildCarryingCapacity,
        const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
        const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
        const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
        const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
        const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
        const boost::numeric::ublas::matrix<double> &EstBiomassGuild)
{
    unsigned numGuilds  = EstCompetitionBetaGuild.size2();
    double sumOverGuilds  = 0;
    double term2;

    if (systemCarryingCapacity == guildCarryingCapacity) {
        std::cout << "[Error 1] nmfCompetitionForm::AGGPRODCompetition: systemCarryingCapacity" <<
                     " same as guildCarryingCapacity" << std::endl;
        return 0;
    }

    for (unsigned j=0; j<numGuilds; ++j) {
        sumOverGuilds += EstCompetitionBetaGuildGuild(speciesOrGuildNum,j)*
                         EstBiomassGuild(timeMinus1,j);
    }

    term2 = sumOverGuilds/(systemCarryingCapacity - guildCarryingCapacity);

    return growthRate[speciesOrGuildNum]*biomassAtTime*term2;
}
