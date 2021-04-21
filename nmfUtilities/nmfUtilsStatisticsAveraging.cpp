
#include "nmfConstants.h"
#include "nmfUtilsStatisticsAveraging.h"

nmfUtilsStatisticsAveraging::nmfUtilsStatisticsAveraging()
{
    m_EstInitBiomass.clear();
    m_EstGrowthRates.clear();
    m_EstCarryingCapacities.clear();
    m_EstPredationHandling.clear();
    m_EstPredationRho.clear();
    m_EstPredationExponent.clear();
    m_EstCompetitionAlpha.clear();
    m_EstCompetitionBetaSpecies.clear();
    m_EstCompetitionBetaGuilds.clear();
    m_EstCompetitionBetaGuildsGuilds.clear();
    m_AveInitBiomass.clear();
    m_AveGrowthRates.clear();
    m_AveCarryingCapacities.clear();
    m_AveCompetitionAlpha.clear();

    // Set up function map
    m_FunctionMap["Unweighted"]   = &nmfUtilsStatisticsAveraging::calculateUnweighted;
    m_FunctionMap["AIC Weighted"] = &nmfUtilsStatisticsAveraging::calculateAICWeighted;
}

void
nmfUtilsStatisticsAveraging::loadEstData(
        std::vector<double>& AIC,
        std::vector<double>& EstInitBiomass,
        std::vector<double>& EstGrowthRates,
        std::vector<double>& EstCarryingCapacities,
        std::vector<double>& EstPredationExponent,
        std::vector<double>& EstCatchability,
        boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
        boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
        boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuilds,
        boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildsGuilds,
        boost::numeric::ublas::matrix<double>& EstPredationRho,
        boost::numeric::ublas::matrix<double>& EstPredationHandling,
        boost::numeric::ublas::matrix<double>& EstBiomass)
{
    m_AIC.push_back(AIC);
    m_EstInitBiomass.push_back(EstInitBiomass);
    m_EstGrowthRates.push_back(EstGrowthRates);
    m_EstCarryingCapacities.push_back(EstCarryingCapacities);
    m_EstPredationExponent.push_back(EstPredationExponent);
    m_EstCatchability.push_back(EstCatchability);
    m_EstCompetitionAlpha.push_back(EstCompetitionAlpha);
    m_EstCompetitionBetaSpecies.push_back(EstCompetitionBetaSpecies);
    m_EstCompetitionBetaGuilds.push_back(EstCompetitionBetaGuilds);
    m_EstCompetitionBetaGuildsGuilds.push_back(EstCompetitionBetaGuildsGuilds);
    m_EstPredationRho.push_back(EstPredationRho);
    m_EstPredationHandling.push_back(EstPredationHandling);
    m_EstBiomass.push_back(EstBiomass);
}


void
nmfUtilsStatisticsAveraging::getAveData(std::vector<double>& AveInitBiomass,
                                        std::vector<double>& AveGrowthRates,
                                        std::vector<double>& AveCarryingCapacities,
                                        std::vector<double>& AvePredationExponent,
                                        std::vector<double>& AveCatchability,
                                        boost::numeric::ublas::matrix<double>& AveCompetitionAlpha,
                                        boost::numeric::ublas::matrix<double>& AveCompetitionBetaSpecies,
                                        boost::numeric::ublas::matrix<double>& AveCompetitionBetaGuilds,
                                        boost::numeric::ublas::matrix<double>& AveCompetitionBetaGuildsGuilds,
                                        boost::numeric::ublas::matrix<double>& AvePredationRho,
                                        boost::numeric::ublas::matrix<double>& AvePredationHandling,
                                        boost::numeric::ublas::matrix<double>& AveBiomass)
{
    AveInitBiomass            = m_AveInitBiomass;
    AveGrowthRates            = m_AveGrowthRates;
    AveCarryingCapacities     = m_AveCarryingCapacities;
    AvePredationExponent      = m_AvePredationExponent;
    AveCatchability           = m_AveCatchability;
    AveCompetitionAlpha       = m_AveCompetitionAlpha;
    AveCompetitionBetaSpecies = m_AveCompetitionBetaSpecies;
    AveCompetitionBetaGuilds  = m_AveCompetitionBetaGuilds;
    AveCompetitionBetaGuildsGuilds = m_AveCompetitionBetaGuildsGuilds;
    AvePredationRho           = m_AvePredationRho;
    AvePredationHandling      = m_AvePredationHandling;
    AveBiomass                = m_AveBiomass;
}

void
nmfUtilsStatisticsAveraging::calculateUnweighted()
{
    int numRows;
    int numCols;
    int NumRuns    = m_EstInitBiomass.size();
    int NumSpecies = m_EstInitBiomass[0].size();
    int index=0;
    double sum;
    std::vector<std::vector<double> > aveVector = {m_AveInitBiomass,
                                                   m_AveGrowthRates,
                                                   m_AveCarryingCapacities,
                                                   m_AvePredationExponent,
                                                   m_AveCatchability};

    // Find averages for all vector estimated parameters
    for (std::vector<std::vector<double> > estVector : {m_EstInitBiomass,
                                                        m_EstGrowthRates,
                                                        m_EstCarryingCapacities,
                                                        m_EstPredationExponent,
                                                        m_EstCatchability})
    {
        for (int species=0; species<NumSpecies; ++species) {
            sum = 0;
            for (int run=0; run<NumRuns; ++run) {
                if (int(estVector[run].size()) == NumSpecies) {
                    sum += estVector[run][species];
                } else {
                    sum = 0;
                }
            }
            aveVector[index].push_back(sum/NumRuns);
        }
        ++index;
    }
    m_AveInitBiomass        = aveVector[0];
    m_AveGrowthRates        = aveVector[1];
    m_AveCarryingCapacities = aveVector[2];
    m_AvePredationExponent  = aveVector[3];
    m_AveCatchability       = aveVector[4];

    // Find averages for all matrix estimated parameters
    index = 0;
    std::vector< boost::numeric::ublas::matrix<double> > aveMatrix =
        {m_AveCompetitionAlpha,
         m_AveCompetitionBetaSpecies,
         m_AveCompetitionBetaGuilds,
         m_AveCompetitionBetaGuildsGuilds,
         m_AvePredationRho,
         m_AvePredationHandling,
         m_AveBiomass};
    for (std::vector<boost::numeric::ublas::matrix<double> > estMatrix : {
         m_EstCompetitionAlpha,
         m_EstCompetitionBetaGuildsGuilds,
         m_EstCompetitionBetaSpecies,
         m_EstCompetitionBetaGuilds,
         m_EstPredationRho,
         m_EstPredationHandling,
         m_EstBiomass})
    {
        numRows = estMatrix[0].size1(); // OK to use 0 since all matrices of same type
        numCols = estMatrix[0].size2(); // have the same dimensions
        nmfUtils::initialize(aveMatrix[index],numRows,numCols);
        for (int row=0; row<numRows; ++row) {
            for (int col=0; col<numCols; ++col) {
                sum = 0;
                for (int run=0; run<NumRuns; ++run) {
                    sum += estMatrix[run](row,col);
                }
                aveMatrix[index](row,col) = sum/NumRuns;
            }
        }
        ++index;
    }
    m_AveCompetitionAlpha            = aveMatrix[0];
    m_AveCompetitionBetaSpecies      = aveMatrix[1];
    m_AveCompetitionBetaGuilds       = aveMatrix[2];
    m_AveCompetitionBetaGuildsGuilds = aveMatrix[3];
    m_AvePredationRho                = aveMatrix[4];
    m_AvePredationHandling           = aveMatrix[5];
    m_AveBiomass                     = aveMatrix[6];
}

void
nmfUtilsStatisticsAveraging::calculateAICWeighted()
{

}

void
nmfUtilsStatisticsAveraging::calculateAverage(const QString& averagingAlgorithm)
{
    (this->*m_FunctionMap[averagingAlgorithm])();
}


//void
//nmfUtilsStatisticsAveraging::checkData(boost::numeric::ublas::matrix<double>& data)
//{
//    std::cout << "Data Check:" << std::endl;
//    for (int i=0; i<int(data.size1()); ++i) {
//        for (int j=0; j<int(data.size2()); ++j) {
//            std::cout << data(i,j) << " ";
//        }
//        std::cout << std::endl;
//    }
//}
