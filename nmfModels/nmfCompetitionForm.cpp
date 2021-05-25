
#include "nmfCompetitionForm.h"

nmfCompetitionForm::nmfCompetitionForm(std::string competitionType)
{
    m_type             = competitionType;
    m_numSpecies       = 0;
    m_numberParameters = 0;
    m_parameterRanges.clear();
    m_isAGGPROD = false;
    m_CompetitionMap.clear();
    m_CompetitionKey.clear();

    m_FunctionMap["Null"]     = &nmfCompetitionForm::NoCompetition;
    m_FunctionMap["NO_K"]     = &nmfCompetitionForm::NOKCompetition;
    m_FunctionMap["MS-PROD"]  = &nmfCompetitionForm::MSPRODCompetition;
    m_FunctionMap["AGG-PROD"] = &nmfCompetitionForm::AGGPRODCompetition;

    setupFormMaps();
}

void
nmfCompetitionForm::setupFormMaps()
{
    std::string index1    = (m_isAGGPROD) ? "I" : "i";
    std::string index2    = (m_isAGGPROD) ? "J" : "j";
    std::string Bit       = "B<sub>"+index1+",t</sub>";
    std::string Ki        = "K<sub>"+index1+"</sub>";
    std::string riBit     = "r<sub>"+index1+"</sub>" + Bit;
    std::string Bjt       = "B<sub>"+index2+",t</sub>";
    std::string Betaij    = "&beta;<sub>"+index1+",j</sub>";
    std::string BetaiG    = "&beta;<sub>"+index1+",G</sub>";
    std::string phraseB   = (m_isAGGPROD) ? "Guild I" : "Species i";

    m_CompetitionMap["Null"]      = "";
    m_CompetitionMap["NO_K"]      = " - " + Bit + "&sum;&alpha;<sub>i,j</sub>" + Bjt;
    m_CompetitionMap["MS-PROD"]   = " - " + riBit + \
"[(&sum;"+Betaij+Bjt+")/K<sub>G</sub> - \
(&sum;" + BetaiG + "B<sub>G,t</sub>)/(K<sub>&sigma;</sub> - K<sub>G</sub>)]";
    m_CompetitionMap["AGG-PROD"]  = " - " + riBit + "[(&sum;" + BetaiG + "B<sub>G,t</sub>)/(K<sub>&sigma;</sub> - K<sub>G</sub>)]";

    m_CompetitionKey["Null"]      = "";
    m_CompetitionKey["NO_K"]      = "&alpha;<sub>i,j</sub> = Competition Coefficient (effect of Species j on " + phraseB + ")<br/>";
    m_CompetitionKey["MS-PROD"]   = Betaij + " = Competition Coefficient (effect of Species j in same Guild as " + phraseB + ", on " + phraseB + ")<br/>";
    m_CompetitionKey["MS-PROD"]  += BetaiG + " = Competition Coefficient (effect of Guild G on " + phraseB + ")<br/>";
    m_CompetitionKey["MS-PROD"]  += "K<sub>G</sub> = Guild Carrying Capacity (fixed at initial value)<br/>";
    m_CompetitionKey["MS-PROD"]  += "K<sub>&sigma;</sub> = System Carrying Capacity (fixed at initial value)<br/>";
    m_CompetitionKey["AGG-PROD"]  = BetaiG + " = Competition Coefficient (effect of Guild G on " + phraseB + ")<br/>";
    m_CompetitionKey["AGG-PROD"] += "K<sub>G</sub> = Guild Carrying Capacity (fixed at initial value)<br/>";
    m_CompetitionKey["AGG-PROD"] += "K<sub>&sigma;</sub> = System Carrying Capacity (fixed at initial value)<br/>";
}

std::string
nmfCompetitionForm::getExpression()
{
    return m_CompetitionMap[m_type];
}

std::string
nmfCompetitionForm::getKey()
{
    return m_CompetitionKey[m_type];
}

void
nmfCompetitionForm::setAggProd(bool isAggProd)
{
    m_isAGGPROD = isAggProd;
}

void
nmfCompetitionForm::setType(std::string newType)
{
    m_type = newType;
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
        nmfStructsQt::ModelDataStruct& dataStruct)
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
