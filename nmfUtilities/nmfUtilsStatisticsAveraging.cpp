
#include "nmfConstants.h"
#include "nmfUtilsStatisticsAveraging.h"

nmfUtilsStatisticsAveraging::nmfUtilsStatisticsAveraging()
{
    clearEstData();
    clearTrimmedData();
    clearAveragedData();

    // Set up function map
    m_FunctionMap["Unweighted"]   = &nmfUtilsStatisticsAveraging::calculateUnweighted;
    m_FunctionMap["AIC Weighted"] = &nmfUtilsStatisticsAveraging::calculateAICWeighted;
}

void
nmfUtilsStatisticsAveraging::clearAveragedData()
{
    m_AveInitBiomass.clear();
    m_AveGrowthRates.clear();
    m_AveGrowthRateShapes.clear();
    m_AveGrowthRateCovariateCoeffs.clear();
    m_AveCarryingCapacities.clear();
    m_AveCarryingCapacityCovariateCoeffs.clear();
    m_AveCompetitionAlpha.clear();
    m_AvePredationExponent.clear();
    m_AveCatchability.clear();
    m_AveCatchabilityCovariateCoeffs.clear();
    m_AveCompetitionBetaSpecies.clear();
    m_AveCompetitionBetaGuilds.clear();
    m_AveCompetitionBetaGuildsGuilds.clear();
    m_AvePredationRho.clear();
    m_AvePredationHandling.clear();
    m_AveSurveyQ.clear();
    m_AveSurveyQCovariateCoeffs.clear();
    m_AveBMSY.clear();
    m_AveMSY.clear();
    m_AveFMSY.clear();
    m_AveBiomass.clear();
}

void
nmfUtilsStatisticsAveraging::clearEstData()
{
    m_Fitness.clear();
    m_AIC.clear();
    m_EstInitBiomass.clear();
    m_EstGrowthRates.clear();
    m_EstGrowthRateShapes.clear();
    m_EstGrowthRateCovariateCoeffs.clear();
    m_EstCarryingCapacities.clear();
    m_EstCarryingCapacityCovariateCoeffs.clear();
    m_EstPredationExponent.clear();
    m_EstCatchability.clear();
    m_EstCatchabilityCovariateCoeffs.clear();
    m_EstSurveyQ.clear();
    m_EstSurveyQCovariateCoeffs.clear();
    m_EstCompetitionAlpha.clear();
    m_EstCompetitionBetaSpecies.clear();
    m_EstCompetitionBetaGuilds.clear();
    m_EstCompetitionBetaGuildsGuilds.clear();
    m_EstPredationRho.clear();
    m_EstPredationHandling.clear();
    m_EstBiomass.clear();
}

void
nmfUtilsStatisticsAveraging::clearTrimmedData()
{
    m_AIC_trimmed.clear();
    m_EstInitBiomass_trimmed.clear();
    m_EstGrowthRates_trimmed.clear();
    m_EstGrowthRateShapes_trimmed.clear();
    m_EstGrowthRateCovariateCoeffs_trimmed.clear();
    m_EstCarryingCapacities_trimmed.clear();
    m_EstCarryingCapacityCovariateCoeffs_trimmed.clear();
    m_EstCatchability_trimmed.clear();
    m_EstCatchabilityCovariateCoeffs_trimmed.clear();
    m_EstPredationHandling_trimmed.clear();
    m_EstPredationRho_trimmed.clear();
    m_EstPredationExponent_trimmed.clear();
    m_EstCompetitionAlpha_trimmed.clear();
    m_EstCompetitionBetaSpecies_trimmed.clear();
    m_EstCompetitionBetaGuilds_trimmed.clear();
    m_EstCompetitionBetaGuildsGuilds_trimmed.clear();
    m_EstSurveyQ_trimmed.clear();
    m_EstSurveyQCovariateCoeffs_trimmed.clear();
    m_EstBMSY_trimmed.clear();
    m_EstMSY_trimmed.clear();
    m_EstFMSY_trimmed.clear();
}

void
nmfUtilsStatisticsAveraging::calculateWeighted(const std::vector<double>& weights)
{
    int numRows;
    int numCols;
    int NumRuns    = m_EstInitBiomass_trimmed.size();
    int NumSpecies = m_EstInitBiomass_trimmed[0].size();
    int index=0;
    double sum;

    std::vector<std::vector<double> > aveVector = {m_AveInitBiomass,
                                                   m_AveGrowthRates,
                                                   m_AveGrowthRateShapes,
                                                   m_AveGrowthRateCovariateCoeffs,
                                                   m_AveCarryingCapacities,
                                                   m_AveCarryingCapacityCovariateCoeffs,
                                                   m_AvePredationExponent,
                                                   m_AveCatchability,
                                                   m_AveCatchabilityCovariateCoeffs,
                                                   m_AveSurveyQ,
                                                   m_AveSurveyQCovariateCoeffs,
                                                   m_AveBMSY,
                                                   m_AveMSY,
                                                   m_AveFMSY};
    clearAveragedData();

    // Find averages for all vector estimated parameters
    for (std::vector<std::vector<double> > estVector : {m_EstInitBiomass_trimmed,
                                                        m_EstGrowthRates_trimmed,
                                                        m_EstGrowthRateShapes_trimmed,
                                                        m_EstGrowthRateCovariateCoeffs_trimmed,
                                                        m_EstCarryingCapacities_trimmed,
                                                        m_EstCarryingCapacityCovariateCoeffs_trimmed,
                                                        m_EstPredationExponent_trimmed,
                                                        m_EstCatchability_trimmed,
                                                        m_EstCatchabilityCovariateCoeffs_trimmed,
                                                        m_EstSurveyQ_trimmed,
                                                        m_EstSurveyQCovariateCoeffs_trimmed,
                                                        m_EstBMSY_trimmed,
                                                        m_EstMSY_trimmed,
                                                        m_EstFMSY_trimmed})
    {
        for (int species=0; species<NumSpecies; ++species) {
            sum = 0;
            for (int run=0; run<NumRuns; ++run) {
                if (int(estVector[run].size()) == NumSpecies) {
                    sum += estVector[run][species] * weights[run];
                } else {
                    sum = 0;
                }
            }
            aveVector[index].push_back(sum);
        }
        ++index;
    }
    m_AveInitBiomass                     = aveVector[0];
    m_AveGrowthRates                     = aveVector[1];
    m_AveGrowthRateShapes                = aveVector[2];
    m_AveGrowthRateCovariateCoeffs       = aveVector[3];
    m_AveCarryingCapacities              = aveVector[4];
    m_AveCarryingCapacityCovariateCoeffs = aveVector[5];
    m_AvePredationExponent               = aveVector[6];
    m_AveCatchability                    = aveVector[7];
    m_AveCatchabilityCovariateCoeffs     = aveVector[8];
    m_AveSurveyQ                         = aveVector[9];
    m_AveSurveyQCovariateCoeffs          = aveVector[10];
    m_AveBMSY                            = aveVector[11];
    m_AveMSY                             = aveVector[12];
    m_AveFMSY                            = aveVector[13];

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
         m_EstCompetitionAlpha_trimmed,
         m_EstCompetitionBetaGuildsGuilds_trimmed,
         m_EstCompetitionBetaSpecies_trimmed,
         m_EstCompetitionBetaGuilds_trimmed,
         m_EstPredationRho_trimmed,
         m_EstPredationHandling_trimmed,
         m_EstBiomass_trimmed})
    {
        numRows = estMatrix[0].size1(); // OK to use 0 since all matrices of same type
        numCols = estMatrix[0].size2(); // have the same dimensions
        nmfUtils::initialize(aveMatrix[index],numRows,numCols);
        for (int time=0; time<numRows; ++time) {
            for (int species=0; species<numCols; ++species) {
                sum = 0;
                for (int run=0; run<NumRuns; ++run) {
                    sum += estMatrix[run](time,species) * weights[run];
                }
                aveMatrix[index](time,species) = sum;
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
nmfUtilsStatisticsAveraging::calculateAverage(const int& numberOfTopRunsToUse,
                                              const bool& isUsingAll,
                                              const bool& isPercent,
                                              const QString& averagingAlgorithm)
{
    // Need to trim vectors and matrices to include only the top n as specified by the following arguments
    bool ok = createTrimmedStructures(numberOfTopRunsToUse,isUsingAll,isPercent);

    if (ok) {
        (this->*m_FunctionMap[averagingAlgorithm])();
    }
}

bool
nmfUtilsStatisticsAveraging::createTrimmedStructures(const int& numberOfTopRunsToUse,
                                                     const bool& isUsingAll,
                                                     const bool& isPercent)
{
    int NumRuns_trimmed;
    int numRuns = m_AIC.size();

    clearTrimmedData();

//  bool useAllRuns = ((numberOfTopRunsToUse ==     100) &&   isPercent) ||
//                    ((numberOfTopRunsToUse == numRuns) && ! isPercent);

    if (isUsingAll) {
        m_AIC_trimmed                                = m_AIC;
        m_EstInitBiomass_trimmed                     = m_EstInitBiomass;
        m_EstGrowthRates_trimmed                     = m_EstGrowthRates;
        m_EstGrowthRateShapes_trimmed                = m_EstGrowthRateShapes;
        m_EstGrowthRateCovariateCoeffs_trimmed       = m_EstGrowthRateCovariateCoeffs;
        m_EstCarryingCapacities_trimmed              = m_EstCarryingCapacities;
        m_EstCarryingCapacityCovariateCoeffs_trimmed = m_EstCarryingCapacityCovariateCoeffs;
        m_EstPredationExponent_trimmed               = m_EstPredationExponent;
        m_EstCatchability_trimmed                    = m_EstCatchability;
        m_EstCatchabilityCovariateCoeffs_trimmed     = m_EstCatchabilityCovariateCoeffs;
        m_EstSurveyQ_trimmed                         = m_EstSurveyQ;
        m_EstSurveyQCovariateCoeffs_trimmed          = m_EstSurveyQCovariateCoeffs;
        m_EstCompetitionAlpha_trimmed                = m_EstCompetitionAlpha;
        m_EstCompetitionBetaSpecies_trimmed          = m_EstCompetitionBetaSpecies;
        m_EstCompetitionBetaGuilds_trimmed           = m_EstCompetitionBetaGuilds;
        m_EstCompetitionBetaGuildsGuilds_trimmed     = m_EstCompetitionBetaGuildsGuilds;
        m_EstPredationRho_trimmed                    = m_EstPredationRho;
        m_EstPredationHandling_trimmed               = m_EstPredationHandling;
        m_EstBMSY_trimmed                            = m_EstBMSY;
        m_EstMSY_trimmed                             = m_EstMSY;
        m_EstFMSY_trimmed                            = m_EstFMSY;
        m_EstBiomass_trimmed                         = m_EstBiomass;
    } else {

        if (isPercent) {
            NumRuns_trimmed = numRuns*(numberOfTopRunsToUse/100.0);
        } else {
            NumRuns_trimmed = (numberOfTopRunsToUse > numRuns) ? numRuns : numberOfTopRunsToUse;
        }

        // Go through the fitness vector and keep the top NumRuns_trimmed runs
        // Copy the list and then sort it
        std::vector<double> m_AIC_sorted = m_AIC;
        std::sort(m_AIC_sorted.begin(),m_AIC_sorted.end());
        // Next, find the position of the top NumRuns_trimmed fitness values
        std::vector<int> positionOfTopNRuns;

        int run=0;
        double fitnessValueToFind;
        while (run < NumRuns_trimmed) {
            // Find index of m_AIC_sorted in m_AIC
            fitnessValueToFind = m_AIC_sorted[run];
            std::vector<double>::iterator iter = std::find(m_AIC.begin(), m_AIC.end(), fitnessValueToFind);
            if (iter != m_AIC.end())
                // Get index of element from iterator
                positionOfTopNRuns.push_back(std::distance(m_AIC.begin(),iter));
            else {
                std::cout << "Error createTrimmedStructures: Item not found" << std::endl;
                return false;
            }
            ++run;
        }

        // Now copy the appropriate elements
        for (int index : positionOfTopNRuns) {
            m_AIC_trimmed.push_back(m_AIC[index]);
            m_EstInitBiomass_trimmed.push_back(m_EstInitBiomass[index]);
            m_EstGrowthRates_trimmed.push_back(m_EstGrowthRates[index]);
            m_EstGrowthRateShapes_trimmed.push_back(m_EstGrowthRateShapes[index]);
            m_EstGrowthRateCovariateCoeffs_trimmed.push_back(m_EstGrowthRateCovariateCoeffs[index]);
            m_EstCarryingCapacities_trimmed.push_back(m_EstCarryingCapacities[index]);
            m_EstCarryingCapacityCovariateCoeffs_trimmed.push_back(m_EstCarryingCapacityCovariateCoeffs[index]);
            m_EstPredationExponent_trimmed.push_back(m_EstPredationExponent[index]);
            m_EstCatchability_trimmed.push_back(m_EstCatchability[index]);
            m_EstCatchabilityCovariateCoeffs_trimmed.push_back(m_EstCatchabilityCovariateCoeffs[index]);
            m_EstCompetitionAlpha_trimmed.push_back(m_EstCompetitionAlpha[index]);
            m_EstCompetitionBetaSpecies_trimmed.push_back(m_EstCompetitionBetaSpecies[index]);
            m_EstCompetitionBetaGuilds_trimmed.push_back(m_EstCompetitionBetaGuilds[index]);
            m_EstCompetitionBetaGuildsGuilds_trimmed.push_back(m_EstCompetitionBetaGuildsGuilds[index]);
            m_EstPredationRho_trimmed.push_back(m_EstPredationRho[index]);
            m_EstPredationHandling_trimmed.push_back(m_EstPredationHandling[index]);
            m_EstSurveyQ_trimmed.push_back(m_EstSurveyQ[index]);
            m_EstSurveyQCovariateCoeffs_trimmed.push_back(m_EstSurveyQCovariateCoeffs[index]);
            m_EstBMSY_trimmed.push_back(m_EstBMSY[index]);
            m_EstMSY_trimmed.push_back(m_EstMSY[index]);
            m_EstFMSY_trimmed.push_back(m_EstFMSY[index]);
            m_EstBiomass_trimmed.push_back(m_EstBiomass[index]);
        }
    }
    return true;
}

void
nmfUtilsStatisticsAveraging::calculateUnweighted()
{
    int NumRuns;

    NumRuns = m_AIC_trimmed.size();
    std::vector<double> equalWeights;

    // print out weights:
    for (int i=0; i<NumRuns; ++i) {
        equalWeights.push_back(1.0/NumRuns);
    }

    // Now apply the weights
    calculateWeighted(equalWeights);

}

void
nmfUtilsStatisticsAveraging::calculateAICWeighted()
{
    int NumRuns = m_AIC_trimmed.size();

    // 1.  Create ΔAIC values
    // Step 1. Find smallest ΔAIC
    double minAIC = *std::min_element(m_AIC_trimmed.begin(),m_AIC_trimmed.end());

    // Setp 2. Find difference between smallest ΔAIC and all AICs
    std::vector<double> deltaAIC;
    for (int i=0; i<NumRuns; ++i) {
        deltaAIC.push_back(m_AIC_trimmed[i]-minAIC);
    }

    // 2. Find relative likelihood of each model where relLik = exp(-0.5 * ΔAIC)
    std::vector<double> relLik;
    double sumRelLikAllModels=0;
    double value;
    for (int i=0; i<NumRuns; ++i) {
        value = std::exp(-0.5 * deltaAIC[i]);
        sumRelLikAllModels += value;
        relLik.push_back(value);
    }

    // 3. AIC weight or model is relLik / sum(relLik for all models)
    std::vector<double> aicWeight;
    if (sumRelLikAllModels == 0) {
        for (int i=0; i<NumRuns; ++i) {
            aicWeight.push_back(0);
        }
    } else {
        for (int i=0; i<NumRuns; ++i) {
            aicWeight.push_back(relLik[i]/sumRelLikAllModels);
        }
    }

    // print out weights:
    for (int i=0; i<NumRuns; ++i) {
std::cout << "AIC Weight for Run (" << i+1 << " of " << NumRuns << "): " << aicWeight[i] << std::endl;
    }

    // Now apply the weights
    calculateWeighted(aicWeight);
}

void
nmfUtilsStatisticsAveraging::getAveData(std::vector<double>& Fitness,
                                        std::vector<double>& AveInitBiomass,
                                        std::vector<double>& AveGrowthRates,
                                        std::vector<double>& AveGrowthRateShapes,
                                        std::vector<double>& AveGrowthRateCovariateCoeffs,
                                        std::vector<double>& AveCarryingCapacities,
                                        std::vector<double>& AveCarryingCapacityCovariateCoeffs,
                                        std::vector<double>& AvePredationExponent,
                                        std::vector<double>& AveCatchability,
                                        std::vector<double>& AveCatchabilityCovariateCoeffs,
                                        std::vector<double>& AveSurveyQ,
                                        std::vector<double>& AveSurveyQCovariateCoeffs,
                                        std::vector<double>& AveBMSY,
                                        std::vector<double>& AveMSY,
                                        std::vector<double>& AveFMSY,
                                        boost::numeric::ublas::matrix<double>& AveCompetitionAlpha,
                                        boost::numeric::ublas::matrix<double>& AveCompetitionBetaSpecies,
                                        boost::numeric::ublas::matrix<double>& AveCompetitionBetaGuilds,
                                        boost::numeric::ublas::matrix<double>& AveCompetitionBetaGuildsGuilds,
                                        boost::numeric::ublas::matrix<double>& AvePredationRho,
                                        boost::numeric::ublas::matrix<double>& AvePredationHandling,
                                        boost::numeric::ublas::matrix<double>& AveBiomass)
{
    Fitness                            = m_Fitness;
    AveInitBiomass                     = m_AveInitBiomass;
    AveGrowthRates                     = m_AveGrowthRates;
    AveGrowthRateShapes                = m_AveGrowthRateShapes;
    AveGrowthRateCovariateCoeffs       = m_AveGrowthRateCovariateCoeffs;
    AveCarryingCapacities              = m_AveCarryingCapacities;
    AveCarryingCapacityCovariateCoeffs = m_AveCarryingCapacityCovariateCoeffs;
    AvePredationExponent               = m_AvePredationExponent;
    AveCatchability                    = m_AveCatchability;
    AveCatchabilityCovariateCoeffs     = m_AveCatchabilityCovariateCoeffs;
    AveCompetitionAlpha                = m_AveCompetitionAlpha;
    AveCompetitionBetaSpecies          = m_AveCompetitionBetaSpecies;
    AveCompetitionBetaGuilds           = m_AveCompetitionBetaGuilds;
    AveCompetitionBetaGuildsGuilds     = m_AveCompetitionBetaGuildsGuilds;
    AvePredationRho                    = m_AvePredationRho;
    AvePredationHandling               = m_AvePredationHandling;
    AveSurveyQ                         = m_AveSurveyQ;
    AveSurveyQCovariateCoeffs          = m_AveSurveyQCovariateCoeffs;
    AveBiomass                         = m_AveBiomass;
    AveBMSY                            = m_AveBMSY;
    AveMSY                             = m_AveMSY;
    AveFMSY                            = m_AveFMSY;
}

void
nmfUtilsStatisticsAveraging::loadEstData(
        double& Fitness,
        std::vector<double>& AIC,                   // per run, AIC values for all species and for model
        std::vector<double>& EstInitBiomass,        // estimated values for each species
        std::vector<double>& EstGrowthRates,
        std::vector<double>& EstGrowthRateShapes,
        std::vector<double>& EstGrowthRateCovariateCoeffs,
        std::vector<double>& EstCarryingCapacities,
        std::vector<double>& EstCarryingCapacityCovariateCoeffs,
        std::vector<double>& EstPredationExponent,
        std::vector<double>& EstCatchability,
        std::vector<double>& EstCatchabilityCovariateCoeffs,
        std::vector<double>& EstSurveyQ,
        std::vector<double>& EstSurveyQCovariateCoeffs,
        std::vector<double>& EstBMSY,
        std::vector<double>& EstMSY,
        std::vector<double>& EstFMSY,
        boost::numeric::ublas::matrix<double>& EstCompetitionAlpha,
        boost::numeric::ublas::matrix<double>& EstCompetitionBetaSpecies,
        boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuilds,
        boost::numeric::ublas::matrix<double>& EstCompetitionBetaGuildsGuilds,
        boost::numeric::ublas::matrix<double>& EstPredationRho,
        boost::numeric::ublas::matrix<double>& EstPredationHandling,
        boost::numeric::ublas::matrix<double>& EstBiomass)
{
    m_Fitness.push_back(Fitness);
    m_AIC.push_back(AIC.back()); // Store only the last element of AIC (it's the model average over all species for that particular run)
    m_EstInitBiomass.push_back(EstInitBiomass);
    m_EstGrowthRates.push_back(EstGrowthRates);
    m_EstGrowthRateShapes.push_back(EstGrowthRateShapes);
    m_EstGrowthRateCovariateCoeffs.push_back(EstGrowthRateCovariateCoeffs);
    m_EstCarryingCapacities.push_back(EstCarryingCapacities);
    m_EstCarryingCapacityCovariateCoeffs.push_back(EstCarryingCapacityCovariateCoeffs);
    m_EstPredationExponent.push_back(EstPredationExponent);
    m_EstCatchability.push_back(EstCatchability);
    m_EstCatchabilityCovariateCoeffs.push_back(EstCatchabilityCovariateCoeffs);
    m_EstSurveyQ.push_back(EstSurveyQ);
    m_EstSurveyQCovariateCoeffs.push_back(EstSurveyQCovariateCoeffs);
    m_EstCompetitionAlpha.push_back(EstCompetitionAlpha);
    m_EstCompetitionBetaSpecies.push_back(EstCompetitionBetaSpecies);
    m_EstCompetitionBetaGuilds.push_back(EstCompetitionBetaGuilds);
    m_EstCompetitionBetaGuildsGuilds.push_back(EstCompetitionBetaGuildsGuilds);
    m_EstPredationRho.push_back(EstPredationRho);
    m_EstPredationHandling.push_back(EstPredationHandling);
    m_EstBMSY.push_back(EstBMSY);
    m_EstMSY.push_back(EstMSY);
    m_EstFMSY.push_back(EstFMSY);
    m_EstBiomass.push_back(EstBiomass);
}
