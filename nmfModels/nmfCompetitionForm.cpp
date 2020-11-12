
#include "nmfCompetitionForm.h"

nmfCompetitionForm::nmfCompetitionForm(std::string competitionType)
{
    m_type             = competitionType;
    m_numSpecies       = 0;
    m_numberParameters = 0;
    m_parameterRanges.clear();

    m_FunctionMap["NO_K"]     = NOKCompetition;
    m_FunctionMap["MS-PROD"]  = MSPRODCompetition;
    m_FunctionMap["AGG-PROD"] = AGGPRODCompetition;
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
        boost::numeric::ublas::matrix<double>& competitionBetaGuilds)
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
                competitionBetaGuilds(i,j) = parameters[startPos++];
//std::cout << "Extracting AGG: " <<   competitionBetaGuilds(i,j) << std::endl;
            }
        }
    }

}


void
nmfCompetitionForm::loadParameterRanges(
        std::vector<std::pair<double,double> >& parameterRanges,
        Data_Struct& beeStruct)
{
    std::pair<double,double> aPair;

    m_numSpecies = beeStruct.NumSpecies;
    m_numGuilds  = beeStruct.NumGuilds;

    if (m_type == "NO_K") {
        for (unsigned i=0; i<beeStruct.CompetitionMin.size(); ++i) {
            for (unsigned j=0; j<beeStruct.CompetitionMin[0].size(); ++j) {
                aPair = std::make_pair(beeStruct.CompetitionMin[i][j],
                                       beeStruct.CompetitionMax[i][j]);
                parameterRanges.emplace_back(aPair);
                m_parameterRanges.emplace_back(aPair);
            }
        }
        m_numberParameters += beeStruct.CompetitionMin.size() *
                              beeStruct.CompetitionMin[0].size();
    }

    if (m_type == "MS-PROD") {
        for (unsigned i=0; i<beeStruct.CompetitionBetaSpeciesMin.size(); ++i) {
            for (unsigned j=0; j<beeStruct.CompetitionBetaSpeciesMin[0].size(); ++j) {
                aPair = std::make_pair(beeStruct.CompetitionBetaSpeciesMin[i][j],
                                       beeStruct.CompetitionBetaSpeciesMax[i][j]);
                parameterRanges.emplace_back(aPair);
                m_parameterRanges.emplace_back(aPair);
            }
        }
        m_numberParameters += beeStruct.CompetitionBetaSpeciesMin.size() *
                              beeStruct.CompetitionBetaSpeciesMin[0].size();
    }

    if ((m_type == "MS-PROD") || (m_type == "AGG-PROD")) {
        for (unsigned i=0; i<beeStruct.CompetitionBetaGuildsMin.size(); ++i) {
            for (unsigned j=0; j<beeStruct.CompetitionBetaGuildsMin[0].size(); ++j) {
                aPair = std::make_pair(beeStruct.CompetitionBetaGuildsMin[i][j],
                                       beeStruct.CompetitionBetaGuildsMax[i][j]);
                parameterRanges.emplace_back(aPair);
                m_parameterRanges.emplace_back(aPair);
            }
        }
        m_numberParameters += beeStruct.CompetitionBetaGuildsMin.size() *
                              beeStruct.CompetitionBetaGuildsMin[0].size();
    }
}

double
nmfCompetitionForm::evaluate(int& TimeMinus1,
                             int& SpeciesNum,
                             double& BiomassAtTime,
                             double& SystemCarryingCapacity,
                             std::vector<double>& GrowthRate,
                             double& GuildCarryingCapacity,
                             boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
                             boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
                             boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuild,
                             boost::numeric::ublas::matrix<double>& EstBiomassSpecies,
                             boost::numeric::ublas::matrix<double>& EstBiomassGuild)
{
    if (m_FunctionMap.find(m_type) == m_FunctionMap.end()) {
        return 0;
    } else {
        return m_FunctionMap[m_type](TimeMinus1,SpeciesNum,BiomassAtTime,
                                     SystemCarryingCapacity,GrowthRate,
                                     GuildCarryingCapacity,
                                     EstCompetitionAlpha,
                                     EstCompetitionBetaSpecies,
                                     EstCompetitionBetaGuild,
                                     EstBiomassSpecies,
                                     EstBiomassGuild);
    }
}


double
nmfCompetitionForm::NOKCompetition(int& timeMinus1,
                                   int& speciesNum,
                                   double& biomassAtTime,
                                   double& systemCarryingCapacity,
                                   std::vector<double>& growthRate,
                                   double& guildCarryingCapacity,
                                   boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
                                   boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
                                   boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
                                   boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
                                   boost::numeric::ublas::matrix<double> &EstBiomassGild)
{
    double competitionSum = 0;

    for (unsigned col=0; col<EstCompetitionAlpha.size1(); ++col) {
        competitionSum += EstCompetitionAlpha(speciesNum,col) * EstBiomassSpecies(timeMinus1,col);
    }

    return biomassAtTime*competitionSum;
}


/*
 *  r(i)B(i,t)[(∑β(i,j)B(j,t))/KG - (∑β(i,G)B(G,t))/(K(σ) - K(G))]
 */
double
nmfCompetitionForm::MSPRODCompetition(
        int& timeMinus1,
        int& speciesNum,
        double& biomassAtTime,
        double& systemCarryingCapacity,
        std::vector<double>& growthRate,
        double& guildCarryingCapacity,
        boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
        boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
        boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
        boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
        boost::numeric::ublas::matrix<double> &EstBiomassGuild)
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
        sumOverSpecies += EstCompetitionBetaSpecies(speciesNum,j)*
                          EstBiomassSpecies(timeMinus1,j);
    }
    for (unsigned j=0; j<numGuilds; ++j) {
        sumOverGuilds += EstCompetitionBetaGuild(speciesNum,j)*
                         EstBiomassGuild(timeMinus1,j);
    }

    term1 = sumOverSpecies/guildCarryingCapacity;
    term2 = sumOverGuilds/(systemCarryingCapacity - guildCarryingCapacity);

    return growthRate[speciesNum]*biomassAtTime*(term1-term2);
}


/*
 *  riBi,t[(∑βi,GBG,t)/(Kσ - KG)]
 */
double
nmfCompetitionForm::AGGPRODCompetition(
        int& timeMinus1,
        int& speciesNum,
        double& biomassAtTime,
        double& systemCarryingCapacity,
        std::vector<double>& growthRate,
        double& guildCarryingCapacity,
        boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
        boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
        boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
        boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
        boost::numeric::ublas::matrix<double> &EstBiomassGuild)
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
        sumOverGuilds += EstCompetitionBetaGuild(speciesNum,j)*
                         EstBiomassGuild(timeMinus1,j);
    }

    term2 = sumOverGuilds/(systemCarryingCapacity - guildCarryingCapacity);

    return growthRate[speciesNum]*biomassAtTime*(term2);
}
