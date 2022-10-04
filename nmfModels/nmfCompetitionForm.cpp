
#include "nmfCompetitionForm.h"

nmfCompetitionForm::nmfCompetitionForm(std::string competitionType)
{
    m_Type             = competitionType;
    m_NumSpecies       = 0;
    m_NumParameters = 0;
    m_ParameterRanges.clear();
    m_isAGGPROD = false;
    m_CompetitionMap.clear();
    m_CompetitionKey.clear();

    m_FunctionMap["Null"]     = &nmfCompetitionForm::FunctionMap_Null;
    m_FunctionMap["NO_K"]     = &nmfCompetitionForm::FunctionMap_NOK;
    m_FunctionMap["MS-PROD"]  = &nmfCompetitionForm::FunctionMap_MSPROD;
    m_FunctionMap["AGG-PROD"] = &nmfCompetitionForm::FunctionMap_AGGPROD;

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
    return m_CompetitionMap[m_Type];
}

std::string
nmfCompetitionForm::getKey()
{
    return m_CompetitionKey[m_Type];
}

void
nmfCompetitionForm::setAggProd(bool isAggProd)
{
    m_isAGGPROD = isAggProd;
}

void
nmfCompetitionForm::setType(std::string newType)
{
    m_Type = newType;
}

int
nmfCompetitionForm::getNumParameters()
{
    return m_NumParameters;
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
    if (m_Type == "NO_K") {
        for (int i=0; i<m_NumSpecies; ++i) {
            for (int j=0; j<m_NumSpecies; ++j) {
                competitionAlpha(i,j) = parameters[startPos++];
            }
        }
    } else if (m_Type == "MS-PROD") {
        for (int i=0; i<m_NumSpecies; ++i) {
            for (int j=0; j<m_NumSpecies; ++j) {
                competitionBetaSpecies(i,j) = parameters[startPos++];
//std::cout << "Extracting MS: " <<   competitionBetaSpecies(i,j) << std::endl;
            }
        }
    }
    if (m_Type == "MS-PROD") {
        for (int i=0; i<m_NumSpecies; ++i) {
            for (int j=0; j<m_NumGuilds; ++j) {
                competitionBetaGuilds(i,j) = parameters[startPos++];
//std::cout << "Extracting AGG: " <<   competitionBetaGuilds(i,j) << std::endl;
            }
        }
    } else if (m_Type == "AGG-PROD") {
        for (int i=0; i<m_NumGuilds; ++i) {
            for (int j=0; j<m_NumGuilds; ++j) {
                competitionBetaGuildsGuilds(i,j) = parameters[startPos++];
//std::cout << "Extracting AGG: " <<   competitionBetaGuildsGuilds(i,j) << std::endl;
            }
        }
    }
}


void
nmfCompetitionForm::loadParameterRanges(
        std::vector<double>& parameterInitialValues,
        std::vector<std::pair<double,double> >& parameterRanges,
        nmfStructsQt::ModelDataStruct& dataStruct)
{
    bool isCheckedAlpha              = nmfUtils::isEstimateParameterChecked(dataStruct,"CompetitionAlpha");
    bool isCheckedBetaSpeciesSpecies = nmfUtils::isEstimateParameterChecked(dataStruct,"CompetitionBetaSpeciesSpecies");
    bool isCheckedBetaGuildSpecies   = nmfUtils::isEstimateParameterChecked(dataStruct,"CompetitionBetaGuildSpecies");
    bool isCheckedBetaGuildGuild     = nmfUtils::isEstimateParameterChecked(dataStruct,"CompetitionBetaGuildGuild");
    double initialValue;
    std::pair<double,double> aPair;

    if (m_Type == "Null")
        return;

    m_NumSpecies = dataStruct.NumSpecies;
    m_NumGuilds  = dataStruct.NumGuilds;

    if (m_Type == "NO_K") {
        for (unsigned i=0; i<dataStruct.CompetitionMin.size(); ++i) {
            for (unsigned j=0; j<dataStruct.CompetitionMin[0].size(); ++j) {
                if (isCheckedAlpha) {
                    aPair = std::make_pair(dataStruct.CompetitionMin[i][j],
                                           dataStruct.CompetitionMax[i][j]);
                } else {
                    initialValue = dataStruct.Competition[i][j];
                    aPair = std::make_pair(initialValue,initialValue);
                }
                parameterRanges.emplace_back(aPair);
                m_ParameterRanges.emplace_back(aPair);
                parameterInitialValues.emplace_back(dataStruct.Competition[i][j]);
            }
        }
        m_NumParameters += dataStruct.CompetitionMin.size() *
                           dataStruct.CompetitionMin[0].size();
    }

    else if (m_Type == "MS-PROD") {
        for (unsigned i=0; i<dataStruct.CompetitionBetaSpeciesMin.size(); ++i) {
            for (unsigned j=0; j<dataStruct.CompetitionBetaSpeciesMin[0].size(); ++j) {
                if (isCheckedBetaSpeciesSpecies) {
                    aPair = std::make_pair(dataStruct.CompetitionBetaSpeciesMin[i][j],
                                           dataStruct.CompetitionBetaSpeciesMax[i][j]);
                } else {
                    initialValue = dataStruct.CompetitionBetaSpecies[i][j];
                    aPair = std::make_pair(initialValue,initialValue);
                }
                parameterRanges.emplace_back(aPair);
                m_ParameterRanges.emplace_back(aPair);
                parameterInitialValues.emplace_back(dataStruct.CompetitionBetaSpecies[i][j]);
            }
        }
        m_NumParameters += dataStruct.CompetitionBetaSpeciesMin.size() *
                              dataStruct.CompetitionBetaSpeciesMin[0].size();

        for (unsigned i=0; i<dataStruct.CompetitionBetaGuildsMin.size(); ++i) {
            for (unsigned j=0; j<dataStruct.CompetitionBetaGuildsMin[0].size(); ++j) {
                if (isCheckedBetaGuildSpecies) {
                    aPair = std::make_pair(dataStruct.CompetitionBetaGuildsMin[i][j],
                                           dataStruct.CompetitionBetaGuildsMax[i][j]);
                } else {
                    initialValue = dataStruct.CompetitionBetaGuilds[i][j];
                    aPair = std::make_pair(initialValue,initialValue);
                }
                parameterRanges.emplace_back(aPair);
                m_ParameterRanges.emplace_back(aPair);
                parameterInitialValues.emplace_back(dataStruct.CompetitionBetaGuilds[i][j]);
            }
        }
        m_NumParameters += dataStruct.CompetitionBetaGuildsMin.size() *
                           dataStruct.CompetitionBetaGuildsMin[0].size();
    }

    else if (m_Type == "AGG-PROD") {
        for (unsigned i=0; i<dataStruct.CompetitionBetaGuildsGuildsMin.size(); ++i) {
            for (unsigned j=0; j<dataStruct.CompetitionBetaGuildsGuildsMin[0].size(); ++j) {
                if (isCheckedBetaGuildGuild) {
                    aPair = std::make_pair(dataStruct.CompetitionBetaGuildsGuildsMin[i][j],
                                           dataStruct.CompetitionBetaGuildsGuildsMax[i][j]);
                } else {
                    initialValue = dataStruct.CompetitionBetaGuildsGuilds[i][j];
                    aPair = std::make_pair(initialValue,initialValue);
                }
                parameterRanges.emplace_back(aPair);
                m_ParameterRanges.emplace_back(aPair);
                parameterInitialValues.emplace_back(dataStruct.CompetitionBetaGuildsGuilds[i][j]);
            }
        }
        m_NumParameters += dataStruct.CompetitionBetaGuildsGuildsMin.size() *
                           dataStruct.CompetitionBetaGuildsGuildsMin[0].size();
    }
}

long double
nmfCompetitionForm::evaluate(
        const std::string& covariateAlgorithmType,
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
        const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate)
{
    if (m_FunctionMap.find(m_Type) == m_FunctionMap.end()) {
        return 0;
    } else {
        return (this->*m_FunctionMap[m_Type])(covariateAlgorithmType,
                                              timeMinus1,
                                              speciesOrGuildNum,
                                              biomassAtTime,
                                              growthRate,
                                              growthRateCovariate,
                                              guildCarryingCapacity,
                                              systemCarryingCapacity,
                                              EstBiomassSpecies,
                                              EstBiomassGuild,
                                              EstCompetitionAlpha,
                                              CompetitionAlphaCovariate,
                                              EstCompetitionBetaSpecies,
                                              CompetitionBetaSpeciesCovariate,
                                              EstCompetitionBetaGuild,
                                              CompetitionBetaGuildCovariate,
                                              EstCompetitionBetaGuildGuild,
                                              CompetitionBetaGuildGuildCovariate);
    }
}

long double
nmfCompetitionForm::FunctionMap_Null(
        const std::string& covariateAlgorithmType,
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
        const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate)
{
    return 0.0;
}


long double
nmfCompetitionForm::FunctionMap_NOK(
        const std::string& covariateAlgorithmType,
        const int& timeMinus1,
        const int& species,
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
        const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate)
{
    long double competitionSum = 0;
    double CompetitionAlphaCovariateCoeff = 0.0; // RSK estimate this later
    double EstCompetitionAlphaTerm;

    for (unsigned row=0; row<EstCompetitionAlpha.size2(); ++row) {
        EstCompetitionAlphaTerm = nmfUtils::applyCovariate(nullptr,
                    covariateAlgorithmType,EstCompetitionAlpha(row,species),
                    CompetitionAlphaCovariateCoeff,CompetitionAlphaCovariate(timeMinus1,species));
        competitionSum += (long double)EstCompetitionAlphaTerm * (long double)EstBiomassSpecies(timeMinus1,row);
    }

    return double(biomassAtTime)*double(competitionSum);
}

long double
nmfCompetitionForm::FunctionMap_MSPROD(
        const std::string& covariateAlgorithmType,
        const int& timeMinus1,
        const int& species,
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
        const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate)
{
    unsigned numSpecies = growthRate.size();
    unsigned numGuilds  = EstCompetitionBetaGuild.size2();
    double sumOverSpecies = 0;
    double sumOverGuilds  = 0;
    double term1;
    double term2;
    double growthRateCovariateCoeff = 0.0;             // RSK estimate this later
    double CompetitionBetaSpeciesCovariateCoeff = 0.0; // RSK estimate this later
    double CompetitionBetaGuildCovariateCoeff   = 0.0; // RSK estimate this later

    if (guildCarryingCapacity == 0) {
        std::cout << "[Error 1] nmfCompetitionForm::FunctionMap_MSPROD: guildCarryingCapacity is 0" << std::endl;
        return 0;
    }

    if (systemCarryingCapacity == guildCarryingCapacity) {
        std::cout << "[Error 2] nmfCompetitionForm::FunctionMap_MSPROD: systemCarryingCapacity same as guildCarryingCapacity" << std::endl;
        return 0;
    }
    double CompetitionBetaSpeciesTerm;
    for (unsigned j=0; j<numSpecies; ++j) {
        CompetitionBetaSpeciesTerm = nmfUtils::applyCovariate(nullptr,
                    covariateAlgorithmType,EstCompetitionBetaSpecies(species,j),
                    CompetitionBetaSpeciesCovariateCoeff,CompetitionBetaSpeciesCovariate(timeMinus1,j));
        sumOverSpecies += CompetitionBetaSpeciesTerm*EstBiomassSpecies(timeMinus1,j);
    }
    double CompetitionBetaGuildTerm;
    for (unsigned j=0; j<numGuilds; ++j) {
        CompetitionBetaGuildTerm = nmfUtils::applyCovariate(nullptr,
                    covariateAlgorithmType,EstCompetitionBetaGuild(species,j),
                    CompetitionBetaGuildCovariateCoeff,CompetitionBetaGuildGuildCovariate(timeMinus1,j));
        sumOverGuilds += CompetitionBetaGuildTerm*EstBiomassGuild(timeMinus1,j);
//if (sumOverGuilds > 1e8) {
//qDebug() << sumOverGuilds << CompetitionBetaGuildTerm << EstBiomassGuild(timeMinus1,j);
//}
    }
//if (sumOverGuilds > 1e8) {
//qDebug() << "gbts: " << sumOverSpecies << guildCarryingCapacity << sumOverGuilds << systemCarryingCapacity;
//}

    term1 = sumOverSpecies/guildCarryingCapacity;
    term2 = sumOverGuilds/(systemCarryingCapacity - guildCarryingCapacity);

    double growthTerm = nmfUtils::applyCovariate(nullptr,
                covariateAlgorithmType,growthRate[species],
                growthRateCovariateCoeff,growthRateCovariate(timeMinus1,species));
//qDebug() << "gbts: " << growthTerm << biomassAtTime << term1 << term2;
    return growthTerm*biomassAtTime*(term1-term2);
}

long double
nmfCompetitionForm::FunctionMap_AGGPROD(
        const std::string& covariateAlgorithmType,
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
        const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate)
{
    unsigned numGuilds   = EstCompetitionBetaGuild.size2();
    double sumOverGuilds = 0;
    double term2;
    double growthRateCovariateCoeff = 0.0;                // RSK estimate this later
    double competitionBetaGuildGuildCovariateCoeff = 0.0; // RSK estimate this later

    if (systemCarryingCapacity == guildCarryingCapacity) {
        std::cout << "[Error 1] nmfCompetitionForm::FunctionMap_AGGPROD: systemCarryingCapacity" <<
                     " same as guildCarryingCapacity" << std::endl;
        return 0;
    }

    double CompetitionBetaGuildGuildTerm;
    for (unsigned j=0; j<numGuilds; ++j) {
//      CompetitionBetaGuildGuildTerm = EstCompetitionBetaGuildGuild(speciesOrGuildNum,j) *
//              (1.0+competitionBetaGuildGuildCovariateCoeff*CompetitionBetaGuildGuildCovariate(timeMinus1,speciesOrGuildNum));
        CompetitionBetaGuildGuildTerm = nmfUtils::applyCovariate(nullptr,
                        covariateAlgorithmType,EstCompetitionBetaGuildGuild(speciesOrGuildNum,j),
                        competitionBetaGuildGuildCovariateCoeff,CompetitionBetaGuildGuildCovariate(timeMinus1,speciesOrGuildNum));
        sumOverGuilds += CompetitionBetaGuildGuildTerm * EstBiomassGuild(timeMinus1,j);
    }

    term2 = sumOverGuilds/(systemCarryingCapacity - guildCarryingCapacity);

//  double growthRateTerm = growthRate[speciesOrGuildNum]*(1.0+growthRateCovariateCoeff*growthRateCovariate(timeMinus1,speciesOrGuildNum));
    double growthRateTerm = nmfUtils::applyCovariate(nullptr,
                covariateAlgorithmType,growthRate[speciesOrGuildNum],
                growthRateCovariateCoeff,growthRateCovariate(timeMinus1,speciesOrGuildNum));

    return growthRateTerm*biomassAtTime*term2;
}
