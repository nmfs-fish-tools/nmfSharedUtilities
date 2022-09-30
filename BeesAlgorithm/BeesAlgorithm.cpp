#include "BeesAlgorithm.h"



BeesAlgorithm::BeesAlgorithm(nmfStructsQt::ModelDataStruct theBeeStruct,
                             const bool &verbose)
{
    bool isAggProd  = (theBeeStruct.CompetitionForm == "AGG-PROD");
    int  numSpecies =  theBeeStruct.NumSpecies;
    int  numGuilds  =  theBeeStruct.NumGuilds;
//  QString tableHarvestCatch    = QString::fromStdString(nmfConstantsMSSPM::TableHarvestCatch);
//  QString tableBiomassAbsolute = QString::fromStdString(nmfConstantsMSSPM::TableBiomassAbsolute);
//  QString tableBiomassRelative = QString::fromStdString(nmfConstantsMSSPM::TableBiomassRelative);

    m_MaxFitness     = 999999999;
    m_BeeStruct      = theBeeStruct;
    m_Seed           = -1;
    m_DefaultFitness =  99999;
    m_NullFitness    = -999;
    m_PreviousUnits  = theBeeStruct.PreviousUnits;

    std::string growthForm      = theBeeStruct.GrowthForm;
    std::string harvestForm     = theBeeStruct.HarvestForm;
    std::string competitionForm = theBeeStruct.CompetitionForm;
    std::string predationForm   = theBeeStruct.PredationForm;

    m_GuildSpecies = theBeeStruct.GuildSpecies;
    m_GuildNum     = theBeeStruct.GuildNum;

    // Get number of independent runs
    m_Scaling = theBeeStruct.ScalingAlgorithm;
    if (verbose) {
        std::cout << "BeesAlgorithm: Parameters to Estimate: " << m_BeeStruct.TotalNumberParameters << std::endl;
    }
    // Make patch size a percentage of parameter space
    m_PatchSizePct = m_BeeStruct.BeesNeighborhoodSize/100.0;
    if (verbose) {
        std::cout << "BeesAlgorithm: Patch Size Pct: " << m_PatchSizePct << std::endl;
        std::cout << "BeesAlgorithm: Scaling Algorithm: " << m_Scaling << std::endl;
    }

    // Define growth, harvest, competition, and predation forms
    m_GrowthForm      = std::make_unique<nmfGrowthForm>(growthForm);
    m_HarvestForm     = std::make_unique<nmfHarvestForm>(harvestForm);
    m_CompetitionForm = std::make_unique<nmfCompetitionForm>(competitionForm);
    m_PredationForm   = std::make_unique<nmfPredationForm>(predationForm);

    // Set up default parameters ranges and neighborhood patch sizes
    initializeParameterRangesAndPatchSizes(theBeeStruct);

    if (verbose) {
        std::cout << "BeesAlgorithm: Initialized " << m_ParameterRanges.size() << " parameter ranges" << std::endl;
    }

    // Echo parameter ranges to shell
    if (verbose) {
        printParameterRanges(numSpecies,numGuilds,growthForm,harvestForm,
                             competitionForm,predationForm);
    }

//    if (theBeeStruct.isRelativeBiomass) {
//        nmfUtilsQt::convertMatrix(theBeeStruct.ObservedBiomassBySpecies,m_PreviousUnits[tableBiomassRelative],"mt");
//        nmfUtilsQt::convertMatrix(theBeeStruct.ObservedBiomassByGuilds, m_PreviousUnits[tableBiomassRelative],"mt");
//    } else {
//        nmfUtilsQt::convertMatrix(theBeeStruct.ObservedBiomassBySpecies,m_PreviousUnits[tableBiomassAbsolute],"mt");
//        nmfUtilsQt::convertMatrix(theBeeStruct.ObservedBiomassByGuilds, m_PreviousUnits[tableBiomassAbsolute],"mt");
//    }

    // Get the observed biomass
//    if (isAggProd) {
//        nmfUtils::initialize(m_ObsBiomassBySpeciesOrGuilds,
//                             theBeeStruct.ObservedBiomassByGuilds.size1(),
//                             theBeeStruct.ObservedBiomassByGuilds.size2());
//        m_ObsBiomassBySpeciesOrGuilds = theBeeStruct.ObservedBiomassByGuilds;
//    } else {
//        nmfUtils::initialize(m_ObsBiomassBySpeciesOrGuilds,
//                             theBeeStruct.ObservedBiomassBySpecies.size1(),
//                             theBeeStruct.ObservedBiomassBySpecies.size2());
//        m_ObsBiomassBySpeciesOrGuilds = theBeeStruct.ObservedBiomassBySpecies;
//    }

    nmfUtils::initialize(m_ObsBiomassByGuilds,
                         theBeeStruct.ObservedBiomassByGuilds.size1(),
                         theBeeStruct.ObservedBiomassByGuilds.size2());
    m_ObsBiomassByGuilds = theBeeStruct.ObservedBiomassByGuilds;
//nmfUtils::printMatrix("BiomassByGuild",m_ObsBiomassByGuilds,10,6);
    if (verbose) {
        std::cout << "BeesAlgorithm: Read Observed Biomass" << std::endl;
    }

    // Get the Catch
    nmfUtils::initialize(m_Catch,
                         theBeeStruct.Catch.size1(),
                         theBeeStruct.Catch.size2());
    m_Catch = theBeeStruct.Catch;
//std::cout << "m_Catch(0,0) before: " << m_Catch(0,0) << std::endl;
//    nmfUtilsQt::convertMatrix(m_Catch, m_PreviousUnits[tableHarvestCatch], "mt");
//std::cout << "prev units: " << m_PreviousUnits[tableHarvestCatch].toStdString() << std::endl;
//std::cout << "m_Catch(0,0) after: " << m_Catch(0,0) << std::endl;
    if (verbose) {
        std::cout << "BeesAlgorithm: Read Catch" << std::endl;
    }

    // Get the Effort
    nmfUtils::initialize(m_Effort,
                         theBeeStruct.Effort.size1(),
                         theBeeStruct.Effort.size2());
    m_Effort = theBeeStruct.Effort;
    if (verbose) {
        std::cout << "BeesAlgorithm: Read Effort" << std::endl;
    }

    // Get the Exploitation
    nmfUtils::initialize(m_Exploitation,
                         theBeeStruct.Exploitation.size1(),
                         theBeeStruct.Exploitation.size2());
    m_Exploitation = theBeeStruct.Exploitation;
    if (verbose) {
        std::cout << "BeesAlgorithm: Read Exploitation" << std::endl;
    }
//std::cout << "BeesAlgorithm::BeesAlgorithm end" << std::endl;
}

BeesAlgorithm::~BeesAlgorithm()
{
}

void
BeesAlgorithm::setSeed(const long& seed)
{
    m_Seed = seed;
}

void
BeesAlgorithm::checkAndIncrementSeed()
{
    if (m_Seed >= 0) {
        ++m_Seed;
    }
}

void
BeesAlgorithm::printParameterRanges(const int& NumSpecies,
                                    const int& NumGuilds,
                                    const std::string& GrowthForm,
                                    const std::string& HarvestForm,
                                    const std::string& CompetitionForm,
                                    const std::string& PredationForm)
{
    int m = 0;
    bool isLogistic    = (GrowthForm      == "Logistic");
    bool isHarvest     = (HarvestForm     == "Effort (qE)");
    bool isAlpha       = (CompetitionForm == "NO_K");
    bool isBetaGuilds  = (CompetitionForm == "AGG-PROD");
    bool isBetaSpecies = (CompetitionForm == "MS-PROD");
    bool isRho         = (PredationForm   != "Null");
    bool isHandling    = (PredationForm   == "Type II") || (PredationForm == "Type III");
    bool isExponent    = (PredationForm   == "Type III");
    int NumSpeciesOrGuilds = (isBetaGuilds) ? NumGuilds : NumSpecies;

    std::cout << "  Initial Absolute Biomass (B₀): ";
    for (int i=0; i<NumSpeciesOrGuilds; ++i) {
        std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
        ++m;
    }
    std::cout << "\n  Growth (r): ";
    for (int i=0; i<NumSpeciesOrGuilds; ++i) {
        std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
        ++m;
    }
    if (isLogistic) {
        std::cout << "\n  Carrying Capacity (K): ";
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
            ++m;
        }
    }
    if (isHarvest) {
        std::cout << "\n  Harvest: ";
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
            ++m;
        }
    }
    if (isAlpha) {
        std::cout << "\n  Competition (alpha):";
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumSpeciesOrGuilds; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
    }
    if (isBetaSpecies) {
        std::cout << "\n  Competition (beta(i)):";
        for (int i=0; i<NumSpecies; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumSpecies; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
        std::cout << "\n  Competition (beta(G)):";
        for (int i=0; i<NumSpecies; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumGuilds; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
    } else if (isBetaGuilds) {
        std::cout << "\n  Competition (beta(G)):";
        for (int i=0; i<NumGuilds; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumGuilds; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
    }

    if (isRho) {
        std::cout << "\n  Predation (rho):";
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumSpeciesOrGuilds; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
    }
    if (isHandling) {
        std::cout << "\n  Handling Time (h):";
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumSpeciesOrGuilds; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
    }
    if (isExponent) {
        std::cout << "\n  Predation Exponent (b):";
        std::cout << "\n    ";
        for (int j=0; j<NumSpeciesOrGuilds; ++j) {
            std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
            ++m;
        }
    }
    std::cout << std::endl;

}

void
BeesAlgorithm::loadInitBiomassParameterRanges(
        std::vector<double>& parameterInitialValues,
        std::vector<std::pair<double,double> >& parameterRanges,
        nmfStructsQt::ModelDataStruct& theBeeStruct)
{
    std::pair<double,double> aPair;
    bool isCheckedInitBiomass = nmfUtils::isEstimateParameterChecked(theBeeStruct,"InitBiomass");
    QString tableSpecies      = QString::fromStdString(nmfConstantsMSSPM::TableSpecies);

//    nmfUtilsQt::convertVector(theBeeStruct.InitBiomass,    m_PreviousUnits[tableSpecies], "mt");
//    nmfUtilsQt::convertVector(theBeeStruct.InitBiomassMin, m_PreviousUnits[tableSpecies], "mt");
//    nmfUtilsQt::convertVector(theBeeStruct.InitBiomassMax, m_PreviousUnits[tableSpecies], "mt");

    // Always load initial biomass values
    for (unsigned species=0; species<theBeeStruct.InitBiomassMin.size(); ++species) {
        if (isCheckedInitBiomass) {
            aPair = std::make_pair(theBeeStruct.InitBiomassMin[species],
                                   theBeeStruct.InitBiomassMax[species]);
        } else {
            aPair = std::make_pair(theBeeStruct.InitBiomass[species], //-nmfConstantsMSSPM::epsilon,
                                   theBeeStruct.InitBiomass[species]); //+nmfConstantsMSSPM::epsilon);
        }
        parameterRanges.emplace_back(aPair);
        parameterInitialValues.emplace_back(theBeeStruct.InitBiomass[species]);
    }
}

void
BeesAlgorithm::loadSurveyQParameterRanges(
        std::vector<double>& parameterInitialValues,
        std::vector<std::pair<double,double> >& parameterRanges,
        nmfStructsQt::ModelDataStruct& theBeeStruct)
{
    std::pair<double,double> aPair;
    bool isCheckedSurveyQ = nmfUtils::isEstimateParameterChecked(theBeeStruct,"SurveyQ");
    std::string speciesName;
    std::map<std::string,nmfStructsQt::CovariateStruct> covariateCoeffMap;
    nmfStructsQt::CovariateStruct covariateStruct;
    double initialValue;

    // If Survey Q is not estimated, hardcode 1.0's as min and max
    for (unsigned species=0; species<theBeeStruct.SurveyQMin.size(); ++species) {
        if (isCheckedSurveyQ) {
            aPair = std::make_pair(theBeeStruct.SurveyQMin[species],
                                   theBeeStruct.SurveyQMax[species]);
        } else {
            initialValue = theBeeStruct.SurveyQ[species];
            aPair = std::make_pair(initialValue,initialValue);
        }
        parameterRanges.emplace_back(aPair);
        parameterInitialValues.emplace_back(theBeeStruct.SurveyQ[species]);
    }

    // Always load SurveyQ Covariate Coefficient values
    for (unsigned species=0; species<theBeeStruct.SurveyQMin.size(); ++species) {
        speciesName       = theBeeStruct.SpeciesNames[species];
        covariateCoeffMap = theBeeStruct.SurveyQCovariateCoeff;
        covariateStruct   = covariateCoeffMap[speciesName];
        if (isCheckedSurveyQ) {
            aPair = std::make_pair(covariateStruct.CoeffMinValue,
                                   covariateStruct.CoeffMaxValue);
        } else {
            initialValue = covariateStruct.CoeffValue;
            aPair = std::make_pair(initialValue,initialValue);
        }
        parameterRanges.emplace_back(aPair);
        parameterInitialValues.emplace_back(covariateStruct.CoeffValue);
    }
}


/*
 *
 * Create the parameters based upon the initial biomass and the 4 different
 * forms: growth, catch, competition, predation.
 */
void
BeesAlgorithm::initializeParameterRangesAndPatchSizes(nmfStructsQt::ModelDataStruct& theBeeStruct)
{
//std::cout << "BeesAlgorithm: initializeParameterRangesAndPatchSizes" << std::endl;
    std::vector<std::pair<double,double> > parameterRanges;
    std::vector<double> parameterInitialValues;

    m_ParameterRanges.clear();
    m_PatchSizes.clear();
    loadInitBiomassParameterRanges(        parameterInitialValues, parameterRanges, theBeeStruct);
    m_GrowthForm->loadParameterRanges(     parameterInitialValues, parameterRanges, theBeeStruct);
    m_HarvestForm->loadParameterRanges(    parameterInitialValues, parameterRanges, theBeeStruct);
    m_CompetitionForm->loadParameterRanges(parameterInitialValues, parameterRanges, theBeeStruct);
    m_PredationForm->loadParameterRanges(  parameterInitialValues, parameterRanges, theBeeStruct);
    loadSurveyQParameterRanges(            parameterInitialValues, parameterRanges, theBeeStruct);
    m_ParameterRanges = parameterRanges;

    // Calculate the patch sizes for each parameter range
    for (unsigned int i=0;i<m_ParameterRanges.size();++i) {
        if (m_ParameterRanges[i].first == m_ParameterRanges[i].second) {
            m_PatchSizes.emplace_back(0);
        } else {
// RSK not sure about this logic
//          m_PatchSizes.emplace_back(m_PatchSizePct*(m_ParameterRanges[i].second-m_ParameterRanges[i].first));
            m_PatchSizes.emplace_back((m_ParameterRanges[i].second+m_ParameterRanges[i].first)/2.0);
        }
    }
}

void
BeesAlgorithm::extractInitBiomass(const std::vector<double>& parameters,
                                  int&                       startPos,
                                  std::vector<double>&       initBiomass)
{
    int numInitBiomassParameters = m_BeeStruct.InitBiomassMin.size();

    for (int i=startPos; i<numInitBiomassParameters; ++i) {
        initBiomass.emplace_back(parameters[i]);
    }
    startPos = numInitBiomassParameters;
}

void
BeesAlgorithm::extractGrowthParameters(const std::vector<double>& parameters,
                                       int&                       startPos,
                                       std::vector<double>&       growthRate,
                                       std::vector<double>&       growthRateCovariateCoeffs,
                                       std::vector<double>&       carryingCapacity,
                                       std::vector<double>&       carryingCapacityCovariateCoeffs,
                                       double&                    systemCarryingCapacity)
{
    if ((m_GrowthForm != nullptr)) {
        m_GrowthForm->extractParameters(parameters,startPos,
                                        growthRate,growthRateCovariateCoeffs,
                                        carryingCapacity,carryingCapacityCovariateCoeffs,
                                        systemCarryingCapacity);
    }
}

void
BeesAlgorithm::extractHarvestParameters(const std::vector<double>& parameters,
                                        int& startPos,
                                        std::vector<double>& catchability,
                                        std::vector<double>& catchabilityCovariateCoeffs)
{
    if ((m_HarvestForm != nullptr)) {
        m_HarvestForm->extractParameters(parameters,startPos,
                                         catchability,catchabilityCovariateCoeffs);
    }
}

void
BeesAlgorithm::extractCompetitionParameters(const std::vector<double>& parameters,
                                            int& startPos,
                                            boost::numeric::ublas::matrix<double>& alpha,
                                            boost::numeric::ublas::matrix<double>& betaSpecies,
                                            boost::numeric::ublas::matrix<double>& betaGuilds,
                                            boost::numeric::ublas::matrix<double>& betaGuildsGuilds)
{
    if ((m_CompetitionForm != nullptr)) {
        m_CompetitionForm->extractParameters(parameters,startPos,alpha,betaSpecies,betaGuilds,betaGuildsGuilds);
    }
}

void
BeesAlgorithm::extractPredationParameters(const std::vector<double>& parameters,
                                          int& startPos,
                                          boost::numeric::ublas::matrix<double>& predation)
{
    if ((m_PredationForm != nullptr)) {
        m_PredationForm->extractPredationParameters(parameters,startPos,predation);
    }
}

void
BeesAlgorithm::extractHandlingParameters(const std::vector<double>& parameters,
                                         int& startPos,
                                         boost::numeric::ublas::matrix<double>& handling)
{
    if ((m_PredationForm != nullptr)) {
        m_PredationForm->extractHandlingParameters(parameters,startPos,handling);
    }
}

void
BeesAlgorithm::extractExponentParameters(const std::vector<double>& parameters,
                                         int& startPos,
                                         std::vector<double>& exponents)
{
    if ((m_PredationForm != nullptr)) {
        m_PredationForm->extractExponentParameters(parameters,startPos,exponents);
    }
}


void
BeesAlgorithm::extractSurveyQParameters(const std::vector<double>& parameters,
                                        int&                       startPos,
                                        std::vector<double>&       surveyQ,
                                        std::vector<double>&       surveyQCovariateCoeffs)
{
    int numSurveyQParameters = m_BeeStruct.SurveyQMin.size();

    // Extract Survey Q parameters
    for (int i=startPos; i<startPos+numSurveyQParameters; ++i) {
        surveyQ.emplace_back(parameters[i]);
    }
    startPos += numSurveyQParameters;

    // Extract Survey Q Covariate Coefficient parameters
    for (int i=startPos; i<startPos+numSurveyQParameters; ++i) {
        surveyQCovariateCoeffs.emplace_back(parameters[i]);
    }
    startPos += numSurveyQParameters;
}

double
BeesAlgorithm::evaluateObjectiveFunction(const std::vector<double> &parameters)
{
    bool   isAggProd = (m_BeeStruct.CompetitionForm == "AGG-PROD");
    double EstBiomassTMinus1;
    double GrowthTerm;
    double HarvestTerm;
    double CompetitionTerm;
    double PredationTerm;
    double systemCarryingCapacity;
    double guildK;
    double fitness=0;
    double surveyQVal;
    double surveyQTerm;
    double eps = 0;
    double logEstBiomassVal = 0;
    int startPos=0;
    int timeMinus1;
    int NumYears   = m_BeeStruct.RunLength+1;
    int NumSpecies = m_BeeStruct.NumSpecies;
    int NumGuilds  = m_BeeStruct.NumGuilds;
    int guildNum;
    int NumSpeciesOrGuilds = (isAggProd) ? NumGuilds : NumSpecies;
    bool isEffortFitToCatch = (m_BeeStruct.HarvestForm == "Effort Fit to Catch");
    std::string covariateAlgorithmType = m_BeeStruct.CovariateAlgorithmType;
    std::vector<double> sigmasSquared;
    std::vector<double> initBiomass;
    std::vector<double> growthRate;
    std::vector<double> growthRateCovariateCoeffs;
    std::vector<double> carryingCapacity;
    std::vector<double> carryingCapacityCovariateCoeffs;
    std::vector<double> catchabilityCovariateCoeffs;
    std::vector<double> guildCarryingCapacity;
    std::vector<double> predationExponent;
    std::vector<double> catchability;
    std::vector<double> surveyQ;
    std::vector<double> surveyQCovariateCoeffs;
    boost::numeric::ublas::matrix<double> initBiomassCovariate;
    boost::numeric::ublas::matrix<double> growthRateCovariate;
    boost::numeric::ublas::matrix<double> carryingCapacityCovariate;
    boost::numeric::ublas::matrix<double> surveyQCovariate;
    boost::numeric::ublas::matrix<double> catchabilityCovariate;
    boost::numeric::ublas::matrix<double> predationRhoCovariate;
    boost::numeric::ublas::matrix<double> predationHandlingCovariate;
    boost::numeric::ublas::matrix<double> predationExponentCovariate;
    boost::numeric::ublas::matrix<double> competitionAlphaCovariate;
    boost::numeric::ublas::matrix<double> competitionBetaSpeciesCovariate;
    boost::numeric::ublas::matrix<double> competitionBetaGuildSpeciesCovariate;
    boost::numeric::ublas::matrix<double> competitionBetaGuildGuildCovariate;
    boost::numeric::ublas::matrix<double> EstBiomassSpecies;
    boost::numeric::ublas::matrix<double> EstBiomassGuilds;
    boost::numeric::ublas::matrix<double> EstBiomassRescaled;
    boost::numeric::ublas::matrix<double> ObsBiomassBySpeciesOrGuildsRescaled;
    boost::numeric::ublas::matrix<double> obsBiomassBySpeciesOrGuilds;
    boost::numeric::ublas::matrix<double> competitionAlpha;
    boost::numeric::ublas::matrix<double> competitionBetaSpecies;
    boost::numeric::ublas::matrix<double> competitionBetaGuilds;
    boost::numeric::ublas::matrix<double> competitionBetaGuildsGuilds;
    boost::numeric::ublas::matrix<double> predationRho;
    boost::numeric::ublas::matrix<double> predationHandling;
    boost::numeric::ublas::matrix<double> Effort   = m_BeeStruct.Effort;
    boost::numeric::ublas::matrix<double> ObsCatch = m_BeeStruct.Catch;
    boost::numeric::ublas::matrix<double> ObsCatchRescaled;
    boost::numeric::ublas::matrix<double> EstCatch;
    boost::numeric::ublas::matrix<double> EstCatchRescaled;

    nmfUtils::initialize(ObsCatchRescaled,                    NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(EstCatch,                            NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(EstCatchRescaled,                    NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(EstBiomassSpecies,                   NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(EstBiomassGuilds,                    NumYears,           NumGuilds);
    nmfUtils::initialize(EstBiomassRescaled,                  NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(ObsBiomassBySpeciesOrGuildsRescaled, NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(competitionAlpha,                    NumSpeciesOrGuilds, NumSpeciesOrGuilds);
    nmfUtils::initialize(competitionBetaSpecies,              NumSpecies,         NumSpecies);
    nmfUtils::initialize(competitionBetaGuilds,               NumSpeciesOrGuilds, NumGuilds);
    nmfUtils::initialize(competitionBetaGuildsGuilds,         NumGuilds,          NumGuilds);
    nmfUtils::initialize(predationRho,                        NumSpeciesOrGuilds, NumSpeciesOrGuilds);
    nmfUtils::initialize(predationHandling,                   NumSpeciesOrGuilds, NumSpeciesOrGuilds);

    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"InitBiomass",                   initBiomassCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"GrowthRate",                    growthRateCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"CarryingCapacity",              carryingCapacityCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"Catchability",                  catchabilityCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"SurveyQ",                       surveyQCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"PredationRho",                  predationRhoCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"PredationHandling",             predationHandlingCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"PredationExponent",             predationExponentCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"CompetitionAlpha",              competitionAlphaCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"CompetitionBetaSpeciesSpecies", competitionBetaSpeciesCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"CompetitionBetaGuildSpecies",   competitionBetaGuildSpeciesCovariate);
    nmfUtilsQt::getCovariates(m_BeeStruct,NumYears,"CompetitionBetaGuildGuild",     competitionBetaGuildGuildCovariate);

    if (isAggProd) {
        NumSpeciesOrGuilds = NumGuilds;
        obsBiomassBySpeciesOrGuilds = m_BeeStruct.ObservedBiomassByGuilds;
    } else {
        NumSpeciesOrGuilds = NumSpecies;
        obsBiomassBySpeciesOrGuilds = m_BeeStruct.ObservedBiomassBySpecies;
    }

//std::cout << "num params: " << parameters.size() << std::endl;
    // Load the parameters into their respective data structures for use in the objective function.
    extractInitBiomass(parameters,startPos,initBiomass);
    m_GrowthForm->extractParameters(parameters,startPos,
                                    growthRate,growthRateCovariateCoeffs,
                                    carryingCapacity,carryingCapacityCovariateCoeffs,
                                    systemCarryingCapacity);
    m_HarvestForm->extractParameters(parameters,startPos,
                                     catchability,catchabilityCovariateCoeffs);
    m_CompetitionForm->extractParameters(parameters,startPos,competitionAlpha,
                                         competitionBetaSpecies,competitionBetaGuilds,
                                         competitionBetaGuildsGuilds);
    m_PredationForm->extractPredationParameters(parameters,startPos,predationRho);
    m_PredationForm->extractHandlingParameters(parameters,startPos,predationHandling);
    m_PredationForm->extractExponentParameters(parameters,startPos,predationExponent);
    extractSurveyQParameters(parameters,startPos,
                             surveyQ,surveyQCovariateCoeffs);

    // Since we may be estimating SurveyQ, need to divide the Observed Biomass by the SurveyQ
    for (int species=0; species<int(obsBiomassBySpeciesOrGuilds.size2()); ++species) {
        surveyQVal = surveyQ[species];
        for (int time=0; time<int(obsBiomassBySpeciesOrGuilds.size1()); ++time) {
            surveyQTerm = nmfUtils::applyCovariate(nullptr,
                        covariateAlgorithmType,surveyQVal,
                        surveyQCovariateCoeffs[species],surveyQCovariate(time,species));
            obsBiomassBySpeciesOrGuilds(time,species) /= surveyQTerm;
        }
    }

//std::cout << "last start pos: " << startPos << std::endl;

    // guildCarryingCapacity is carrying capacity for the guild the species is a member of
    // Calculate carrying capacity for all guilds
    systemCarryingCapacity = 0;
    for (int i=0; i<NumGuilds; ++i) {
        guildK = 0;
        for (unsigned j=0; j<m_GuildSpecies[i].size(); ++j) {
            guildK += carryingCapacity[m_GuildSpecies[i][j]];
            systemCarryingCapacity += guildK;
        }
        guildCarryingCapacity.push_back(guildK);
    }

    // Evaluate the objective function for all years and species or guilds and put
    // result in matrix
    for (int i=0; i<NumSpeciesOrGuilds; ++i) {
//      EstBiomassSpecies(0,i) = m_ObsBiomassBySpeciesOrGuilds(0,i);
        EstBiomassSpecies(0,i) = initBiomass[i];
//if (i == 0) {
// std::cout << "EST BM S: " << EstBiomassSpecies(0,i) << std::endl;
//}
    }

    for (int i=0; i<NumGuilds; ++i) {
        EstBiomassGuilds(0,i)  = m_ObsBiomassByGuilds(0,i); // Remember there's only initial guild biomass data.
    }

//    bool isCheckedInitBiomass = nmfUtils::isEstimateParameterChecked(m_BeeStruct,"InitBiomass");


    double initBiomassCoeff = 0.0; // RSK will be estimated eventually
    sigmasSquared = nmfUtilsStatistics::calculateSigmasSquared(obsBiomassBySpeciesOrGuilds); // m_ObsBiomassBySpeciesOrGuilds);
    if (sigmasSquared.size() == 0) {
        return m_DefaultFitness;
    }
    for (int time=1; time<NumYears; ++time) {
        timeMinus1 = time - 1;
        for (int species=0; species<NumSpeciesOrGuilds; ++species) {
            // Find guild that speciesNum is in
            guildNum = m_GuildNum[species];

            EstBiomassTMinus1 = EstBiomassSpecies(timeMinus1,species);

            if (timeMinus1 == 0) {
                EstBiomassTMinus1 = nmfUtils::applyCovariate(nullptr,
                            covariateAlgorithmType,EstBiomassTMinus1,
                            initBiomassCoeff,initBiomassCovariate(timeMinus1,species));
            }
//if (time == 1 && species == 0) {
//    std::cout << "initBM,EstBM,ESTBM: " << initBiomass[species] << ", "
//              << EstBiomassSpecies(timeMinus1,species) << ", "
//              << EstBiomassTMinus1 << std::endl;
//}
            GrowthTerm      = m_GrowthForm->evaluate(covariateAlgorithmType,
                                                     EstBiomassTMinus1,
                                                     growthRate[species],
                                                     growthRateCovariateCoeffs[species],
                                                     growthRateCovariate(timeMinus1,species),
                                                     carryingCapacity[species],
                                                     carryingCapacityCovariateCoeffs[species],
                                                     carryingCapacityCovariate(timeMinus1,species));
            HarvestTerm     = m_HarvestForm->evaluate(covariateAlgorithmType,
                                                      timeMinus1,species,EstBiomassTMinus1,
                                                      m_Catch,m_Effort,m_Exploitation,
                                                      catchability[species],
                                                      catchabilityCovariateCoeffs[species],
                                                      catchabilityCovariate(timeMinus1,species));
//std::cout << "guild cc: " << guildCarryingCapacity[guildNum] << std::endl;
//std::cout << "system cc: " << systemCarryingCapacity << std::endl;
            CompetitionTerm = m_CompetitionForm->evaluate(covariateAlgorithmType,
                                                          timeMinus1,species,EstBiomassTMinus1,
                                                          growthRate,
                                                          growthRateCovariate,
                                                          guildCarryingCapacity[guildNum],
                                                          systemCarryingCapacity,
                                                          EstBiomassSpecies,
                                                          EstBiomassGuilds,
                                                          competitionAlpha,
                                                          competitionAlphaCovariate,
                                                          competitionBetaSpecies,
                                                          competitionBetaSpeciesCovariate,
                                                          competitionBetaGuilds,
                                                          competitionBetaGuildSpeciesCovariate,
                                                          competitionBetaGuildsGuilds,
                                                          competitionBetaGuildGuildCovariate);
            PredationTerm   = m_PredationForm->evaluate(covariateAlgorithmType,
                                                        timeMinus1,species,
                                                        EstBiomassSpecies,EstBiomassTMinus1,
                                                        predationRho,
                                                        predationRhoCovariate,
                                                        predationHandling,
                                                        predationHandlingCovariate,
                                                        predationExponent,
                                                        predationExponentCovariate);

            // Assume log normal error (eps = error term)
//          eps = std::log(m_ObsBiomassBySpeciesOrGuilds(timeMinus1,species)) - std::log(EstBiomassTMinus1);
//          logEstBiomassVal = std::log(EstBiomassTMinus1 + GrowthTerm - HarvestTerm - CompetitionTerm - PredationTerm) + eps;
            // This logic is needed for the correct calculation of eps on subsequent iterations
//          EstBiomassTMinus1 = std::exp(logEstBiomassVal + 0.5*sigmasSquared[species]);
//          EstBiomassSpecies(time,species) = EstBiomassTMinus1;
            // the ½σ² term is needed due to the difference between log normal and normal distributions

            EstBiomassTMinus1  += GrowthTerm - HarvestTerm - CompetitionTerm - PredationTerm;
            if (EstBiomassTMinus1 < 0) {
                EstBiomassTMinus1 = 0;
            }
            if ((EstBiomassTMinus1 < 0) || (std::isnan(std::fabs(EstBiomassTMinus1)))) {
                return m_DefaultFitness;
            }
            EstBiomassSpecies(time,species) = EstBiomassTMinus1;

            // update estBiomassGuilds for next time step
            for (int i=0; i<NumGuilds; ++i) {
                for (unsigned j=0; j<m_GuildSpecies[i].size(); ++j) {
                    EstBiomassGuilds(time,i) += EstBiomassSpecies(time,m_GuildSpecies[i][j]);
                }
            }
        }
    }

    // Scale the data
    if (m_Scaling == "Min Max") {
        nmfUtils::rescaleMatrixMinMax(ObsCatch, ObsCatchRescaled);
        nmfUtils::rescaleMatrixMinMax(EstCatch, EstCatchRescaled);
        nmfUtils::rescaleMatrixMinMax(EstBiomassSpecies, EstBiomassRescaled);
        nmfUtils::rescaleMatrixMinMax(obsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
//      nmfUtils::rescaleMatrixMinMax(m_ObsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
    } else if (m_Scaling == "Mean") {
        nmfUtils::rescaleMatrixMean(ObsCatch, ObsCatchRescaled);
        nmfUtils::rescaleMatrixMean(EstCatch, EstCatchRescaled);
        nmfUtils::rescaleMatrixMean(EstBiomassSpecies, EstBiomassRescaled);
        nmfUtils::rescaleMatrixMean(obsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
//      nmfUtils::rescaleMatrixMean(m_ObsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
    } else if (m_Scaling == "Z-Score") {
        nmfUtils::rescaleMatrixZScore(ObsCatch, ObsCatchRescaled);
        nmfUtils::rescaleMatrixZScore(EstCatch, EstCatchRescaled);
        nmfUtils::rescaleMatrixZScore(EstBiomassSpecies, EstBiomassRescaled);
        nmfUtils::rescaleMatrixZScore(obsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
//      nmfUtils::rescaleMatrixZScore(m_ObsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
    } else {
//      std::cout << "Error: No Scaling Algorithm detected. Defaulting to Min Max." << std::endl;
        nmfUtils::rescaleMatrixMinMax(ObsCatch, ObsCatchRescaled);
        nmfUtils::rescaleMatrixMinMax(EstCatch, EstCatchRescaled);
        nmfUtils::rescaleMatrixMinMax(EstBiomassSpecies, EstBiomassRescaled);
        nmfUtils::rescaleMatrixMinMax(obsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
//      nmfUtils::rescaleMatrixMinMax(m_ObsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
    }

    // Calculate fitness using the appropriate objective criterion
    if (m_BeeStruct.ObjectiveCriterion == "Least Squares") {
        fitness =  nmfUtilsStatistics::calculateLeastSquares(
                    isEffortFitToCatch,
                    ObsCatchRescaled, ObsBiomassBySpeciesOrGuildsRescaled,
                    EstCatchRescaled, EstBiomassRescaled,
                    m_BeeStruct.FitWeights);
    } else if (m_BeeStruct.ObjectiveCriterion == "Model Efficiency") {
        // Negate the MEF here since the ranges is from -inf to 1, where 1 is best.  So we negate it,
        // then minimize that, and then negate and plot the resulting value.
        fitness = -nmfUtilsStatistics::calculateModelEfficiency(
                    isEffortFitToCatch,
                    ObsCatchRescaled, ObsBiomassBySpeciesOrGuildsRescaled,
                    EstCatchRescaled, EstBiomassRescaled,
                    m_BeeStruct.FitWeights);
    } else if (m_BeeStruct.ObjectiveCriterion == "Maximum Likelihood") {
        // The maximum likelihood calculations must use the unscaled data or else the
        // results will be incorrect.
        fitness =  nmfUtilsStatistics::calculateMaximumLikelihoodNoRescale(
                    isEffortFitToCatch,
                    ObsCatchRescaled, ObsBiomassBySpeciesOrGuildsRescaled,
                    EstCatchRescaled, EstBiomassRescaled,
                    m_BeeStruct.FitWeights);
std::cout << "Warning....check this for using rescaled data with Max Likelihood" << std::endl;
    }

// Debug code to print out the CarryingCapacity covariates
//if (fitness < m_MaxFitness) {
//    for (int i=0; i<(int)parameters.size(); ++i) {
//        if (i == 0) {std::cout << std::endl;}
//        if ((i>=40) and (i<= 49)) {
//            std::cout << "parameter:  " << parameters[i] <<
//                         ", fitness: " << fitness << std::endl;
//        }
//    }
//    m_MaxFitness = fitness;
//}

    return fitness;
}


std::unique_ptr<Bee>
BeesAlgorithm::createRandomBee(bool doWhileLoop, std::string& errorMsg)
{
    bool foundAPotentialBee = false;
    bool timesUp = false;
    int timeDiff;
    double minVal;
    double maxVal;
    double fitness;
    QDateTime startTime = nmfUtilsQt::getCurrentTime();
    QDateTime endTime;
    std::vector<double> NullParameters = {};
    std::vector<double> parameters(m_BeeStruct.TotalNumberParameters,0.0);
    double randomValue;

//std::cout << "--> Num Parameters: " << m_BeeStruct.TotalNumberParameters << std::endl;
    while (! foundAPotentialBee) {
        for (int i=0; i<m_BeeStruct.TotalNumberParameters; ++i) {
            minVal = m_ParameterRanges[i].first;
            maxVal = m_ParameterRanges[i].second;
            randomValue = nmfUtils::getRandomNumber(m_Seed,0.0,1.0);
            parameters[i] = (minVal == maxVal) ? minVal :
                             minVal+(maxVal-minVal)*randomValue;
            checkAndIncrementSeed();
//std::cout << "--> range: " << i << "  [" << minVal << "," << maxVal <<
//             "], parameter:  " << parameters[i] << std::endl;
        }
        fitness = evaluateObjectiveFunction(parameters);
//std::cout << "--> fitness: " << fitness << std::endl;
        endTime   = nmfUtilsQt::getCurrentTime();
        timeDiff = int(startTime.msecsTo(endTime))*1000.0; // microseconds
//std::cout << "--> timeDiff: " << timeDiff/1000000.0 << "\n" << std::endl;
        timesUp = (timeDiff/1000000.0 > kTimeToSpendSearching);

        foundAPotentialBee = (fitness != m_DefaultFitness) || doWhileLoop || timesUp;
    }
    if (timesUp) {
        std::cout << "Fitness found: " << fitness << std::endl;
        errorMsg = "\nError: Parameter space too large. Please refine one or more parameter ranges or decrease a parameter.";
        return std::make_unique<Bee>(m_NullFitness,NullParameters);
    }

    return std::make_unique<Bee>(fitness,parameters);
}


std::unique_ptr<Bee>
BeesAlgorithm::createNeighborhoodBee(std::vector<double> &bestSiteParameters)
{
    double val;
    double fitness;
    double patchSize;
    //double rval = rand()/double(RAND_MAX);
    double rval;
    std::vector<double> parameters = {};
//std::cout << "rval: " << rval << std::endl;

    for (unsigned int i=0; i<bestSiteParameters.size(); ++i) {
        patchSize = m_PatchSizes[i];
//std::cout << "-> " << i << ", " << patchSize << std::endl;
        val = bestSiteParameters[i];
        rval = nmfUtils::getRandomNumber(m_Seed,0.0,1.0);
        checkAndIncrementSeed();
        if (patchSize > 0) {
            val = (rval < 0.5) ? (val+rval*patchSize) : (val-rval*patchSize);
            // In c++17, use...
            //std::clamp(val,m_parameterRanges[i].first,m_parameterRanges[i].second);
            val = (val < m_ParameterRanges[i].first)  ? m_ParameterRanges[i].first  : val;
            val = (val > m_ParameterRanges[i].second) ? m_ParameterRanges[i].second : val;
        }
        parameters.emplace_back(val);
    }

    fitness = evaluateObjectiveFunction(parameters);

    return std::move(std::make_unique<Bee>(fitness,parameters));
}


std::unique_ptr<Bee>
BeesAlgorithm::searchNeighborhoodForBestBee(std::unique_ptr<Bee> bestSite,
                                            int &neighborhoodSize)
{
    std::unique_ptr<Bee> bee;
    std::vector<std::unique_ptr<Bee> > neighborhoodBees;
    std::vector<double> bestSiteParameters = bestSite->getParameters();

    for (int i=0; i<neighborhoodSize; ++i) {
        bee = createNeighborhoodBee(bestSiteParameters);
        neighborhoodBees.emplace_back(std::move(bee));
    }

    std::sort(neighborhoodBees.begin(),neighborhoodBees.end(),beesCompareLT());

    return std::move(neighborhoodBees.front());
}


std::unique_ptr<Bee>
BeesAlgorithm::searchParameterSpaceForBestBee(int &runNum,
                                              int& subRunNum,
                                              std::string& errorMsg)
{
    bool done = false;
    int currentGeneration=0;
    int neighborhoodSize;
    int genNum;
    int numScoutBees;
    int numParameters  = m_BeeStruct.TotalNumberParameters;
    int numTotalBees   = m_BeeStruct.BeesNumTotal;
    int numEliteBees   = m_BeeStruct.BeesNumElite;
    int numOtherBees   = m_BeeStruct.BeesNumOther;
    int numBestSites   = m_BeeStruct.BeesNumBestSites;
    int numEliteSites  = m_BeeStruct.BeesNumEliteSites;
    int maxGenerations = m_BeeStruct.BeesMaxGenerations;
    std::unique_ptr<Bee> bestBee;
    std::vector<std::unique_ptr<Bee> > totalBeePopulation;
    std::vector<std::unique_ptr<Bee> > nextGenerationBees;
    std::vector<std::unique_ptr<Bee> > scoutBees;
    std::vector<std::unique_ptr<Bee> > bestSites;
    std::unique_ptr<Bee> theBestBee;
    std::string MSSPMName;
    double bestFitness;
    double bestFitnessInPopulation;
    double bestBeesFitness;
    int unused;

std::cout << "Searching parameter space for initial bees..." << std::endl;

    theBestBee = createRandomBee(false,errorMsg);
std::cout << "Found a bee" << std::endl;

    if (theBestBee->getFitness() == m_NullFitness) {
        return theBestBee;
    }
    for (int i=0; i<numTotalBees; ++i) {
//std::cout << "Creating bee: " << i << std::endl;
        totalBeePopulation.emplace_back(createRandomBee(false,errorMsg));
    }
std::cout << "Created " << numTotalBees << " initial bees." << std::endl;

    while (! done) {
        std::sort(totalBeePopulation.begin(),
                  totalBeePopulation.end(),
                  beesCompareLT());

        bestFitnessInPopulation = totalBeePopulation[0]->getFitness();
        bestBeesFitness         = theBestBee->getFitness();

        if ((bestBeesFitness == m_DefaultFitness) ||
                isABetterFitness(bestFitnessInPopulation,bestBeesFitness))
        {
            theBestBee->setFitness(bestFitnessInPopulation);
            theBestBee->setParameters(totalBeePopulation[0]->getParameters());
        }

        // Find the best sites
        bestSites.clear();
        for (int i=0; i<numBestSites; ++i) {
            bestSites.emplace_back(std::move(totalBeePopulation[i]));
        }

        // For each best bee, calculate the neighborhood size in terms of bees,
        // find those bees in each neighborhood and add only the best one to next_gen
        nextGenerationBees.clear();
        for (int i=0; i<numBestSites; ++i) {
            neighborhoodSize = (i < numEliteSites) ? numEliteBees : numOtherBees;
            nextGenerationBees.emplace_back(searchNeighborhoodForBestBee(std::move(bestSites[i]),
                                                                         neighborhoodSize));
        }

        // Now find the rest of the bees that make up the total number of bees as we'll
        // use them for new scouts.
        numScoutBees = numTotalBees - numBestSites;
        scoutBees.clear();
        for (int i=0; i<numScoutBees; ++i) {
            scoutBees.emplace_back(createRandomBee(false,errorMsg));
//std::cout << "scout bee: " << i << ": " << scoutBees[i]->getFitness() << std::endl;
        }

        totalBeePopulation.clear();
        for (int i=0; i<numBestSites; ++i) {
            totalBeePopulation.emplace_back(std::move(nextGenerationBees[i]));
        }
        for (int i=0; i<numScoutBees; ++i) {
            totalBeePopulation.emplace_back(std::move(scoutBees[i]));
        }

        // Decrease the patch size, so new neighborhoods get smaller and smaller
        for (int i=0; i<numParameters; ++i) {
           m_PatchSizes[i] *= 0.95;
        }

        ++currentGeneration;

        // Done when loop has gone through maxGenerations 
        done = (currentGeneration >= maxGenerations);
//std::cout << "Generation: " <<  currentGeneration << ", Fitness: " << theBestBee->getFitness() << std::endl;

        MSSPMName = "Run " + std::to_string(runNum) +
                    "-"    + std::to_string(subRunNum);
        bestFitness = theBestBee->getFitness();

        unused = 0;
        genNum = currentGeneration - 1;
        writeCurrentLoopFile(MSSPMName,
                     genNum,
                     bestFitness,
                     unused);

        if (stoppedByUser()) {
            std::cout << "BeesAlgorithm StoppedByUser" << std::endl;
            done = true;
        }

    }
    ++genNum;
    writeCurrentLoopFile(MSSPMName,
                 genNum,
                 bestFitness,
                 unused);
    return std::move(theBestBee);
}


bool
BeesAlgorithm::isABetterFitness(double& bestFitnessInPopulation,
                                double& bestBeesFitness)
{

    return (bestFitnessInPopulation < bestBeesFitness);  // Minimize the fitness

}

bool
BeesAlgorithm::stoppedByUser()
{
    std::string cmd;
    std::ifstream inputFile(nmfConstantsMSSPM::MSSPMStopRunFile);
    if (inputFile) {
        std::getline(inputFile,cmd);
    }
    inputFile.close();

    return (cmd == "StoppedByUser");
}

void
BeesAlgorithm::writeCurrentLoopFile(std::string &MSSPMName,
                                   int         &NumGens,
                                   double      &BestFitness,
                                   int         &NumGensSinceBestFit)
{
    double adjustedBestFitness; // May need negating if ObjCrit is Model Efficiency
    std::ofstream outputFile(nmfConstantsMSSPM::MSSPMProgressChartFile,
                             std::ios::out|std::ios::app);

    adjustedBestFitness = BestFitness;
    //
    // Model Efficiency is to be maximized instead of minimized.  The
    // best value is 1. Since the code is set up to minimize for Least
    // Squares, I just negated the fitness and ran the minimization code.
    // Now, I just need to negate the fitness again so the plot will
    // show the fitness approaching +1.
    //
    if (m_BeeStruct.ObjectiveCriterion == "Model Efficiency") {
        adjustedBestFitness = -adjustedBestFitness;
    }

    outputFile << MSSPMName   << ", "
               << NumGens     << ", "
               << adjustedBestFitness << ", "
               << NumGensSinceBestFit << std::endl;

    outputFile.close();
}

void
BeesAlgorithm::printBee(double &fitness,
                        std::vector<double> &parameters)
{
    int numParameters = parameters.size();

    std::cout << "Estimated Parameters: " << " ";
    std::cout << fitness;
    for (int i=0; i<numParameters; ++i) {
        std::cout << " " << parameters[i];
    }
    std::cout << std::endl;
}

int
BeesAlgorithm::calculateActualNumEstParameters()
{
    int numParameters = 0;

    for (unsigned i=0; i<m_ParameterRanges.size(); ++i) {
        if (m_ParameterRanges[i].first != m_ParameterRanges[i].second) {
            ++numParameters;
        }
    }
    return numParameters;
}

bool
BeesAlgorithm::estimateParameters(double &bestFitness,
                                  std::vector<double> &bestParameters,
                                  int &runNum,
                                  int &subRunNum,
                                  std::string& errorMsg)
{
    bool ok = false;
    std::unique_ptr<Bee> bestBee;

    m_DefaultFitness =  INT_MAX;
    m_NullFitness    = -999;

    bestBee = searchParameterSpaceForBestBee(runNum,subRunNum,errorMsg);

    if (bestBee->getFitness() != m_NullFitness) {
        bestFitness    = bestBee->getFitness();
        bestParameters = bestBee->getParameters();
        ok = true;
    }

    return ok;
}
