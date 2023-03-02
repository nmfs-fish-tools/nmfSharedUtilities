/**
 * @file nmfStructsQt.h
 * @brief Definition for common qt gui widgets
 * @date Jan 14, 2020
 *
 * This file contains Qt widget definitions that are common across applications.
 *
 * @copyright
 * Public Domain Notice\n
 *
 * National Oceanic And Atmospheric Administration\n\n
 *
 * This software is a "United States Government Work" under the terms of the
 * United States Copyright Act.  It was written as part of the author's official
 * duties as a United States Government employee/contractor and thus cannot be copyrighted.
 * This software is freely available to the public for use. The National Oceanic
 * And Atmospheric Administration and the U.S. Government have not placed any
 * restriction on its use or reproduction.  Although all reasonable efforts have
 * been taken to ensure the accuracy and reliability of the software and data,
 * the National Oceanic And Atmospheric Administration and the U.S. Government
 * do not and cannot warrant the performance or results that may be obtained
 * by using this software or data. The National Oceanic And Atmospheric
 * Administration and the U.S. Government disclaim all warranties, express
 * or implied, including warranties of performance, merchantability or fitness
 * for any particular purpose.\n\n
 *
 * Please cite the author(s) in any work or product based on this material.
 */

#ifndef NMF_STRUCTSQT
#define NMF_STRUCTSQT

#include <iostream>
#include <string>
#include <vector>

#include <QComboBox>
#include <QDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

#include "nmfLogger.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/multi_array.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

class nmfDatabase;

/**
 * @brief These are common Qt structures and classes
 */
namespace nmfStructsQt {

/**
 * @brief The MSVPA data struct
 */
struct UpdateDataStruct {
    nmfDatabase* databasePtr;
    nmfLogger*   logger;
    std::string  MSVPAName;
    std::string  ForecastName;
    std::string  ScenarioName;
    std::string  ModelName;
    int          Theme;
    int          NumAgeSizeClasses;
    bool         HorizontalGridLines;
    bool         VerticalGridLines;
    std::string  DataTypeLabel;
    std::string  XLabel;
    std::string  YLabel;
    std::string  TitlePrefix;
    std::string  TitleSuffix;
    QString      SelectDataType;
    QString      SelectPredator;
    QString      SelectVariable;
    QString      SelectByVariables;
    QString      SelectSeason;
    QString      SelectPredatorAgeSizeClass;
    QString      SelectPreyName;
    QString      SelectYPRAnalysisType;
    QString      SelectFullyRecruitedAge;
    QString      MaxScaleY;
    QString      SelectMode;
    std::vector<std::string> SelectedYears;
};

/**
 * @brief The data structure that represents the state of each parameter's "Is estimated?" checkbox.
 */
struct EstimateRunBox {
    std::string parameter;
    std::pair<bool,bool> state;
};

/**
 * @brief The data structure used for the Model Review table.
 */
struct ModelReviewStruct {
    QString ModelName;
    QString ObjectiveCriterion;
    QString EstimationAlgorithm;
    QString MinimizerAlgorithm;
    QString ScalingAlgorithm;
    QString DatabaseSnapshot;
    QString setToDeterministicBees;
    int     maxGenerations;
    int     numBees;
    int     numBestSites;
    int     numEliteSites;
    int     numEliteBees;
    int     numOtherBees;
    int     neighborhoodSize;
    int     numSubRuns;
    QString setToDeterministicNLopt;
    QString isStopAfterValue;
    QString stopAfterValue;
    QString isStopAfterTime;
    QString stopAfterTime;
    QString isStopAfterIter;
    QString stopAfterIter;
    QString isEstInitialBiomassEnabled;
    QString isEstInitialBiomassChecked;
    QString isEstGrowthRateEnabled;
    QString isEstGrowthRateShapeChecked;
    QString isEstGrowthRateShapeEnabled;
    QString isEstGrowthRateChecked;
    QString isEstCarryingCapacityEnabled;
    QString isEstCarryingCapacityChecked;
    QString isEstCatchabilityEnabled;
    QString isEstCatchabilityChecked;
    QString isEstCompetitionAlphaEnabled;
    QString isEstCompetitionAlphaChecked;
    QString isEstCompetitionBetaSpeciesEnabled;
    QString isEstCompetitionBetaSpeciesChecked;
    QString isEstCompetitionBetaGuildsEnabled;
    QString isEstCompetitionBetaGuildsChecked;
    QString isEstCompetitionBetaGuildsGuildsEnabled;
    QString isEstCompetitionBetaGuildsGuildsChecked;
    QString isEstPredationRhoEnabled;
    QString isEstPredationRhoChecked;
    QString isEstPredationHandlingEnabled;
    QString isEstPredationHandlingChecked;
    QString isEstPredationExponentEnabled;
    QString isEstPredationExponentChecked;
    QString isEstSurveyQEnabled;
    QString isEstSurveyQChecked;
    QString isAMultiRun;
    QString ensembleAveragingAlgorithm;
    QString ensembleAverageBy;
    QString ensembleUsingBy;
    QString ensembleUsingAmountValue;
    QString isEnsembleUsingPct;
    QString ensembleFilename;
};


struct CovariateStruct {
    double CoeffValue;
    double CoeffMinValue;
    double CoeffMaxValue;
};

/**
 * @brief The data structure used for parameter estimation. It contains the parameter
 * min max limits as well as the input data.
 */
struct ModelDataStruct {

    bool   isMohnsRho;
    bool   isRelativeBiomass;
    bool   showDiagnosticChart;
    bool   useApplicationFixedSeedNLopt;
    bool   useApplicationFixedSeedBees;
    bool   useUserFixedSeedNLopt;
    bool   useUserFixedSeedBees;
    int    userFixedSeedVal;
    bool   incrementFixedSeed;
    bool   allowConvergedOnly;

    bool   NLoptUseStopVal;
    bool   NLoptUseStopAfterTime;
    bool   NLoptUseStopAfterIter;
    double NLoptStopVal;
    int    NLoptStopAfterTime;
    int    NLoptStopAfterIter;
    int    NLoptNumberOfRuns;
    int    NLoptUseInitialPopulationSize;
    double NLoptInitialPopulationSize;

    int    NumMohnsRhoMultiRuns;

    std::string MultiRunSpeciesFilename;
    std::string MultiRunModelFilename;
    std::string MultiRunSetupFilename;
    int    RunLength;
    int    NumSpecies;
    int    NumGuilds;

    int    BeesMaxGenerations;
    int    BeesNumTotal;
    int    BeesNumBestSites;
    int    BeesNumEliteSites;
    int    BeesNumElite;
    int    BeesNumOther;
    float  BeesNeighborhoodSize;
    int    BeesNumRepetitions;

    int    GAGenerations;
    int    GAConvergence;

    int         TotalNumberParameters;
    std::string Benchmark;

    std::string GrowthForm;
    std::string HarvestForm;
    std::string CompetitionForm;
    std::string PredationForm;

    std::string EstimationAlgorithm;
    std::string MinimizerAlgorithm;
    std::string ObjectiveCriterion;
    std::string ScalingAlgorithm;
    bool        LogScale;

    std::string CovariateAlgorithmType;
    std::string ForecastHarvestType;

    boost::numeric::ublas::vector<double> SpeciesWeights;
    std::vector<std::string>              SpeciesNames;
    std::map<int,std::vector<int> >       GuildSpecies; // List of species numbers that make up guild num
    std::vector<int>                      GuildNum;     // Specifies which species are members of which guilds
    boost::numeric::ublas::matrix<double> ObservedBiomassBySpecies;
    boost::numeric::ublas::matrix<double> ObservedBiomassByGuilds;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> Effort;
    boost::numeric::ublas::matrix<double> FitWeights;

    boost::numeric::ublas::vector<double> InitBiomass;
    boost::numeric::ublas::vector<double> InitBiomassMin;
    boost::numeric::ublas::vector<double> InitBiomassMax;
    boost::numeric::ublas::vector<double> GrowthRate;
    boost::numeric::ublas::vector<double> GrowthRateMin;
    boost::numeric::ublas::vector<double> GrowthRateMax;
    boost::numeric::ublas::vector<double> GrowthRateShape;
    boost::numeric::ublas::vector<double> GrowthRateShapeMin;
    boost::numeric::ublas::vector<double> GrowthRateShapeMax;
    boost::numeric::ublas::vector<double> CarryingCapacity;
    boost::numeric::ublas::vector<double> CarryingCapacityMin;
    boost::numeric::ublas::vector<double> CarryingCapacityMax;
    boost::numeric::ublas::matrix<double> Exploitation;
    boost::numeric::ublas::vector<double> ExploitationRateMin; // RSK - change this to matrix?
    boost::numeric::ublas::vector<double> ExploitationRateMax; // RSK - change this to matrix?
    boost::numeric::ublas::vector<double> Catchability;
    boost::numeric::ublas::vector<double> CatchabilityMin;
    boost::numeric::ublas::vector<double> CatchabilityMax;
    boost::numeric::ublas::vector<double> SurveyQ;
    boost::numeric::ublas::vector<double> SurveyQMin;
    boost::numeric::ublas::vector<double> SurveyQMax;

    boost::numeric::ublas::matrix<double> Competition;
    boost::numeric::ublas::matrix<double> CompetitionMin;
    boost::numeric::ublas::matrix<double> CompetitionMax;
    boost::numeric::ublas::matrix<double> CompetitionBetaSpecies;
    boost::numeric::ublas::matrix<double> CompetitionBetaSpeciesMin;
    boost::numeric::ublas::matrix<double> CompetitionBetaSpeciesMax;
    boost::numeric::ublas::matrix<double> CompetitionBetaGuilds;
    boost::numeric::ublas::matrix<double> CompetitionBetaGuildsMin;
    boost::numeric::ublas::matrix<double> CompetitionBetaGuildsMax;
    boost::numeric::ublas::matrix<double> CompetitionBetaGuildsGuilds;
    boost::numeric::ublas::matrix<double> CompetitionBetaGuildsGuildsMin;
    boost::numeric::ublas::matrix<double> CompetitionBetaGuildsGuildsMax;
    boost::numeric::ublas::matrix<double> PredationRho;
    boost::numeric::ublas::matrix<double> PredationRhoMin;
    boost::numeric::ublas::matrix<double> PredationRhoMax;
    boost::numeric::ublas::matrix<double> PredationHandling;
    boost::numeric::ublas::matrix<double> PredationHandlingMin;
    boost::numeric::ublas::matrix<double> PredationHandlingMax;
    std::vector<double>                   PredationExponent;
    std::vector<double>                   PredationExponentMin;
    std::vector<double>                   PredationExponentMax;
    std::vector<double>                   Parameters;

    // Map of covariate name to vector of values
    std::map<std::string,std::vector<double> >          CovariateMap;
    // Map of species+","+parameterName to covariate name
    std::map<std::string,std::string>                   CovariateAssignment;
    // Map of species to covariate struct for the specific parameter.
    // Each covariate struct contains coefficient name (i.e., parameter name),
    // and the initial, minimum, and maximum coefficient values.
    std::map<std::string,CovariateStruct>               GrowthRateCovariateCoeff;
    std::map<std::string,CovariateStruct>               CarryingCapacityCovariateCoeff;
    std::map<std::string,CovariateStruct>               CatchabilityCovariateCoeff;
    std::map<std::string,CovariateStruct>               SurveyQCovariateCoeff;
    std::map<QString,QString> PreviousUnits;
    std::vector<EstimateRunBox> EstimateRunBoxes;
};

}

#endif

