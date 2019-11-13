
/**
 @file nmfConstants.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Dec 8, 2016
*/

#ifndef NMF_CONSTANTS
#define EXTERN
#else
#define EXTERN extern
#endif

#include <iostream>
#include <vector>

namespace nmfConstants
{

    EXTERN const bool   nullsAsEmptyString = true;
    EXTERN const bool   nullAsZero = false;
    EXTERN const bool   RoundOff = true;
    EXTERN const bool   NoSignal = false;
    EXTERN const bool   RotateLabels = true;
    EXTERN const bool   DontRotateLabels = false;

    EXTERN const int    TYPE_PREDATOR       = 0;
    EXTERN const int    TYPE_PREY           = 1;
    EXTERN const int    TYPE_OTHER_PREDATOR = 3;

    EXTERN const int    MaxNumberLogFiles   =  50;
    EXTERN const int    DefaultReferenceAge =   2;
    EXTERN const int    MinSliderValue      =   0;
    EXTERN const int    MaxSliderValue      =  99;
    EXTERN const int    MaxNumberSeasons    =   6;
    EXTERN const int    MaxNumberYears      = 100;
    EXTERN const int    MaxNumberAges       =  21;
    EXTERN const int    MaxNumberSpecies    =  12;
    EXTERN const int    MaxNumberOtherPredators =   12;
    EXTERN const int    MaxNumberPrey           =   20;
    EXTERN const int    MaxNumberFleets         =    6;
    EXTERN const int    MaxNumberIndices        =  100;
    EXTERN const int    MaxNumberSizeCats       =   20;
    EXTERN const int    MaxTotalSpecies         =   50;
    EXTERN const int    MaxMSVPALoops           =  210; // This is 30 loops * (8-1) update messages per each loop = 210; (8-1 because there are 8 updates per loop but you must always have one, so need 7 add'l)
    EXTERN const int    MaxLoopsBeforeChecking  =   20; // Number of MSVPA loops to iterate before asking user if they still want to continue if no convergence
    EXTERN const int    NumMSVPASetupSteps      =   30;
    EXTERN const int    NumForecastSetupSteps   =   16;
    EXTERN const int    NumCategoriesForVerticalNotation = 10;
    EXTERN const int    NumFeedingDataRows      =  7;
    EXTERN const int    MinNumForecastYears     =  3;
    EXTERN const int    MaxNumForecastYears     = 20;
    EXTERN const bool   RearrangeTitle          = true;
    EXTERN const bool   DontRearrangeTitle      = false;
    EXTERN const bool   Clear                   = true;
    EXTERN const bool   NoClear                 = false;
    EXTERN const bool   ShowError               = true;
    EXTERN const bool   DontShowError           = false;
    EXTERN const bool   IsProportion            = true;
    EXTERN const bool   IsNotProportion         = false;
    EXTERN const bool   includeTotalColumn      = true;
    EXTERN const bool   dontIncludeTotalColumn  = false;
    EXTERN const bool   autoFillSegments        = true;
    EXTERN const bool   dontAutoFillSegments    = true;
    EXTERN const bool   FirstYearOnly           = true;
    EXTERN const bool   AllYears                = false;

    EXTERN const double KgToLbs       = 2.20462;
    EXTERN const double MortalityMax  = 5.0;
    EXTERN const double MortalityInit = 1.0;
    EXTERN const double MortalityMin  = 0.0;
    EXTERN const double MortalityInc  = 0.01;
    EXTERN const double TerminalMortalityMin = 0.01;
    EXTERN const double TerminalMortalityMax = 5.0;
    EXTERN const double DefaultTerminalMortality = 0.5;
    EXTERN const double DefaultResidualNaturalMortality = 0.25;
    EXTERN const double DefaultPredationMortality = 0.0;
    EXTERN const double SelectivityMin  = 0.0;
    EXTERN const double SelectivityMax  = 1.0;
    EXTERN const double MaturityMin     = 0.0;
    EXTERN const double MaturityMax     = 1.0;
    EXTERN const int    NoValueInt      = -9999;
    EXTERN const double NoValueDouble   = -9999.0;
    EXTERN const int    ProgressUpdateFrequency = 1000; // How often to update progress bar (milliseconds).
    EXTERN const int    NumModelTypes   = 3;            // SSVPA, MSVPA, Forecast
    EXTERN const int    MSVPAFullSpecies     = 0;
    EXTERN const int    MSVPAPreyOnlySpecies = 1;
    EXTERN const int    MSVPABiomassPredator = 3;
    EXTERN const int    SetupLevel    = 0;
    EXTERN const int    SSVPALevel    = 1;
    EXTERN const int    MSVPALevel    = 2;
    EXTERN const int    ForecastLevel = 3;
    EXTERN const int    NumberOfMSVPAPages = 12;

    EXTERN const int    Normal    = 0;
    EXTERN const int    Bold      = 1;
    EXTERN const int    Warning   = 2;
    EXTERN const int    Error     = 3;
    EXTERN const int    Highlight = 4;
    EXTERN const int    Section   = 5;

    EXTERN const std::string  ClearWindow = "";
    EXTERN const std::string  BlankLine = " ";
    EXTERN const std::string  HiddenDir                      = ".MSVPA_X2";
    EXTERN const std::string  HiddenDataDir                  = ".MSVPA_X2/data";
    EXTERN const std::string  HiddenLogDir                   = ".MSVPA_X2/logs";
    EXTERN const std::string  MSVPAProgressBarFile           = ".MSVPA_X2/MSVPAProgress.dat";
    EXTERN const std::string  MSVPAProgressChartFile         = ".MSVPA_X2/MSVPAProgressChart.dat";
    EXTERN const std::string  MSVPAProgressChartLabelFile    = ".MSVPA_X2/MSVPAProgressChartLabel.dat";
    EXTERN const std::string  MSVPAStopRunFile               = ".MSVPA_X2/MSVPAStopRun.dat";
    EXTERN const std::string  MSVPACurrentLoopFile           = ".MSVPA_X2/MSVPACurrentLoop.dat";
    EXTERN const std::string  ForecastProgressBarFile        = ".MSVPA_X2/ForecastProgress.dat";
    EXTERN const std::string  ForecastProgressChartFile      = ".MSVPA_X2/ForecastProgressChart.dat";
    EXTERN const std::string  ForecastProgressChartLabelFile = ".MSVPA_X2/ForecastProgressChartLabel.dat";
    EXTERN const std::string  ForecastStopRunFile            = ".MSVPA_X2/ForecastStopRun.dat";
    EXTERN const std::string  MSSPMProgressChartFile         = ".MSSPM/MSSPMProgressChart.dat";
    EXTERN const std::string  MSSPMProgressChartLabelFile    = ".MSSPM/MSSPMProgressChartLabel.dat";
    EXTERN const std::string  MSSPMStopRunFile               = ".MSSPM/MSSPMStopRun.dat";
    EXTERN const std::string  LogDir                         = ".MSVPA_X2/logs";
    EXTERN const std::string  LogFilter                      = ".MSVPA_X2/logs/*.log";
    EXTERN const std::string  TableNamesFile                 = ".MSVPA_X2/data/TableNames.dat";
    EXTERN const std::string  TableDescSSVPAFile             = ".MSVPA_X2/data/TableDescriptionsSSVPA.dat";
    EXTERN const std::string  TableDescMSVPAFile             = ".MSVPA_X2/data/TableDescriptionsMSVPA.dat";
    EXTERN const std::string  TableDescForecastFile          = ".MSVPA_X2/data/TableDescriptionsForecast.dat";
    EXTERN const std::string  ProgramDataDir                 = ".MSVPA_X2/data";
    EXTERN const std::string  InputDataDir                   = "inputData";
    EXTERN const std::string  OutputDataDir                  = "outputData";
    EXTERN const std::string  OutputImagesDir                = "outputImages";

    EXTERN const std::vector<std::string> AllTables = {
        "Application",
        "ForeEnergyDens",
        "ForeOutput",
        "ForePredGrowth",
        "ForePredVonB",
        "ForeSRQ",
        "ForeSRR",
        "ForeSuitPreyBiomass",
        "Forecasts",
        "MSVPAEnergyDens",
        "MSVPAOthPrey",
        "MSVPAOthPreyAnn",
        "MSVPASeasBiomass",
        "MSVPASeasInfo",
        "MSVPASizePref",
        "MSVPASpaceO",
        "MSVPAStomCont",
        "MSVPASuitPreyBiomass",
        "MSVPAlist",
        "MSVPAprefs",
        "MSVPAspecies",
        "OthPredSizeData",
        "OtherPredBM",
        "OtherPredSpecies",
        "SSVPAAgeM",
        "ScenarioF",
        "ScenarioOthPred",
        "ScenarioOthPrey",
        "ScenarioRec",
        "Scenarios",
        "SpeCatch",
        "SpeMaturity",
        "SpeSSVPA",
        "SpeSize",
        "SpeTuneCatch",
        "SpeTuneEffort",
        "SpeWeight",
        "SpeXSAData",
        "SpeXSAIndices",
        "Species"
    };

    EXTERN const std::vector<std::string> AllNonSpeciesTables = {
        "Application",
        "ForeEnergyDens",
        "ForeOutput",
        "ForePredGrowth",
        "ForePredVonB",
        "ForeSRQ",
        "ForeSRR",
        "ForeSuitPreyBiomass",
        "Forecasts",
        "MSVPAEnergyDens",
        "MSVPAOthPrey",
        "MSVPAOthPreyAnn",
        "MSVPASeasBiomass",
        "MSVPASeasInfo",
        "MSVPASizePref",
        "MSVPASpaceO",
        "MSVPAStomCont",
        "MSVPASuitPreyBiomass",
        "MSVPAlist",
        "MSVPAprefs",
        "MSVPAspecies",
        "OthPredSizeData",
        "OtherPredBM",
        "SSVPAAgeM",
        "ScenarioF",
        "ScenarioOthPred",
        "ScenarioOthPrey",
        "ScenarioRec",
        "Scenarios",
        "SpeCatch",
        "SpeMaturity",
        "SpeSSVPA",
        "SpeSize",
        "SpeTuneCatch",
        "SpeTuneEffort",
        "SpeWeight",
        "SpeXSAData",
        "SpeXSAIndices"
    };

    EXTERN const std::vector<std::string> SSVPANonSpeciesTables = {
        "OthPredSizeData",
        "OtherPredBM",
        "SSVPAAgeM",
        "SpeCatch",
        "SpeMaturity",
        "SpeSSVPA",
        "SpeSize",
        "SpeTuneCatch",
        "SpeTuneEffort",
        "SpeWeight",
        "SpeXSAData",
        "SpeXSAIndices"
    };

    EXTERN const std::vector<std::string> SSVPATables = {
        "OthPredSizeData",
        "OtherPredBM",
        "OtherPredSpecies",
        "SSVPAAgeM",
        "SpeCatch",
        "SpeMaturity",
        "SpeSSVPA",
        "SpeSize",
        "SpeTuneCatch",
        "SpeTuneEffort",
        "SpeWeight",
        "SpeXSAData",
        "SpeXSAIndices",
        "Species"
    };

    EXTERN const std::vector<std::string> MSVPATables = {
        "MSVPAEnergyDens",
        "MSVPAOthPrey",
        "MSVPAOthPreyAnn",
        "MSVPASeasBiomass",
        "MSVPASeasInfo",
        "MSVPASizePref",
        "MSVPASpaceO",
        "MSVPAStomCont",
        "MSVPASuitPreyBiomass",
        "MSVPAlist",
        "MSVPAprefs",
        "MSVPAspecies"
    };

    EXTERN const std::vector<std::string> ForecastTables = {
        "Forecasts",
        "ForeEnergyDens",
        "ForeOutput",
        "ForePredGrowth",
        "ForeSuitPreyBiomass",
        "ForeSRR",
        "ForeSRQ",
        "ForePredVonB",
        "Scenarios",
        "ScenarioF",
        "ScenarioOthPred",
        "ScenarioOthPrey",
        "ScenarioRec"
    };

    EXTERN const std::vector<std::string> DefaultVPATypes = {
            "Cohort Analysis/SVPA",
            "Separable VPA",
            "Laurec-Shepard Tuned VPA",
            "Extended Survivors Analysis"};
    EXTERN const std::vector<std::string> SurfaceThemes = {
            "Qt",
            "Primary Colors",
            "Digia",
            "Stone Moss",
            "Army Blue",
            "Retro",
            "Ebony",
            "Isabelle"
    };
}


//#endif // NMFCONSTANTS_H
