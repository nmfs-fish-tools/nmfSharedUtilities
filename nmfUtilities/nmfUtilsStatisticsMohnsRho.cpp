
#include "nmfConstants.h"
#include "nmfUtilsStatisticsMohnsRho.h"

nmfUtilsStatisticsMohnsRho::nmfUtilsStatisticsMohnsRho()
{
    clearEstData();
}



void
nmfUtilsStatisticsMohnsRho::clearEstData()
{
    m_Peel.clear();
    m_EstInitBiomass.clear();
    m_EstGrowthRates.clear();
    m_EstCarryingCapacities.clear();
    m_EstPredationExponent.clear();
    m_EstCatchability.clear();
    m_EstSurveyQ.clear();
    m_EstCompetitionAlpha.clear();
    m_EstCompetitionBetaSpecies.clear();
    m_EstCompetitionBetaGuilds.clear();
    m_EstCompetitionBetaGuildsGuilds.clear();
    m_EstPredationRho.clear();
    m_EstPredationHandling.clear();
    m_EstBiomass.clear();
}


void
nmfUtilsStatisticsMohnsRho::loadEstData(
        int& Peel,
        std::vector<double>& EstInitBiomass,        // estimated values for each species
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
        boost::numeric::ublas::matrix<double>& EstBiomass)
{
    m_Peel.push_back(Peel);
    m_EstInitBiomass.push_back(nmfUtils::convertVectorToMatrix(EstInitBiomass));
    m_EstGrowthRates.push_back(nmfUtils::convertVectorToMatrix(EstGrowthRates));
    m_EstCarryingCapacities.push_back(nmfUtils::convertVectorToMatrix(EstCarryingCapacities));
    m_EstCatchability.push_back(nmfUtils::convertVectorToMatrix(EstCatchability));
    m_EstSurveyQ.push_back(nmfUtils::convertVectorToMatrix(EstSurveyQ));
    m_EstCompetitionAlpha.push_back(EstCompetitionAlpha);
    m_EstCompetitionBetaSpecies.push_back(EstCompetitionBetaSpecies);
    m_EstCompetitionBetaGuilds.push_back(EstCompetitionBetaGuilds);
    m_EstCompetitionBetaGuildsGuilds.push_back(EstCompetitionBetaGuildsGuilds);
    m_EstPredationRho.push_back(EstPredationRho);
    m_EstPredationHandling.push_back(EstPredationHandling);
    m_EstPredationExponent.push_back(nmfUtils::convertVectorToMatrix(EstPredationExponent));
    m_EstBiomass.push_back(EstBiomass);

    m_ParameterMap[nmfConstantsMSSPM::ParameterNameInitialBiomass]            = m_EstInitBiomass;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNameGrowthRate]                = m_EstGrowthRates;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNameCarryingCapacity]          = m_EstCarryingCapacities;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNameCatchability]              = m_EstCatchability;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNameSurveyQ]                   = m_EstSurveyQ;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNameCompetitionAlpha]          = m_EstCompetitionAlpha;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNameCompetitionBetaSpecies]    = m_EstCompetitionBetaSpecies;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNameCompetitionBetaGuild]      = m_EstCompetitionBetaGuilds;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNameCompetitionBetaGuildGuild] = m_EstCompetitionBetaGuildsGuilds;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNamePredationRho]              = m_EstPredationRho;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNamePredationHandling]         = m_EstPredationHandling;
    m_ParameterMap[nmfConstantsMSSPM::ParameterNamePredationExponent]         = m_EstPredationExponent;
}


int
nmfUtilsStatisticsMohnsRho::getNumPeels()
{
    return (int)m_Peel.size();
}


bool
nmfUtilsStatisticsMohnsRho::calculateMohnsRhoVectors(
        const QStringList& EstParamNames,
        std::vector<std::vector<double> >& MohnsRhoVectors)
{
    MohnsRhoVectors.clear();
    std::vector<double> MohnsRhoVector;
    std::vector< boost::numeric::ublas::matrix<double> > ParameterMatrices;

    for (QString Parameter : EstParamNames) {
        MohnsRhoVector.clear();
        ParameterMatrices = m_ParameterMap[Parameter];
        if (ParameterMatrices[0].size2() == 1) {
            if (! nmfUtilsStatistics::calculateMohnsRhoFor1dParameter(
                        ParameterMatrices,MohnsRhoVector)) {
                return false;
            }
        } else if (ParameterMatrices[0].size2() > 1) {
            if (! nmfUtilsStatistics::calculateMohnsRhoFor2dParameter(
                        ParameterMatrices,MohnsRhoVector)) {
                return false;
            }
        }
        MohnsRhoVectors.push_back(MohnsRhoVector);
    }

    return true;
}

