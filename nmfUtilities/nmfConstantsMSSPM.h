
/**
 * @file nmfConstantsMSSPM.h
 * @brief Definition for constants used in MSSPM tool
 * @date Jan 14, 2020
 *
 * This file defines constants used in the MSSPM application program.
 */

#ifndef NMFCONSTANTSMSSPM_H
#define NMFCONSTANTSMSSPM_H

#include <iostream>
#include <vector>
#include <QStringList>

namespace nmfConstantsMSSPM
{
     const int    Start_Year            = 1900;

     const int    Column_Guild_Name       =  0;
     const int    Column_Guild_GrowthRate =  1;
     const int    Column_Guild_GuildK     =  2;

     const int    Column_Species_SpeName     =  0;
     const int    Column_Species_Guild       =  1;
     const int    Column_Species_InitBiomass =  2;
     const int    Column_Species_GrowthRate  =  3;
     const int    Column_Species_SpeciesK    =  4;

     const int    Column_Supp_Guild_Name             =  0;
     const int    Column_Supp_Guild_GrowthRate       =  1;
     const int    Column_Supp_Guild_CarryingCapacity =  4;

     const int    Column_Supp_Species_Name             =  0;
     const int    Column_Supp_Species_InitBiomass      =  1;
     const int    Column_Supp_Species_GrowthRate       =  4;
     const int    Column_Supp_Species_CarryingCapacity =  8;

     const int    Column_SpeId          =  0;
     const int    Column_SpeName        =  1;
     const int    Column_GuildName      =  2;
     const int    Column_InitBiomass    =  3;
     const int    Column_SurveyQ        =  4;
     const int    Column_SpeciesK       =  5;
     const int    Column_SpeciesKCovarCoeff   = 6;
     const int    Column_GrowthRate           = 7;
     const int    Column_GrowthRateCovarCoeff = 8;
     const int    Column_Habitat        =  9;
     const int    Column_InitBiomassMin = 10;
     const int    Column_InitBiomassMax = 11;
     const int    Column_SurveyQMin     = 12;
     const int    Column_SurveyQMax     = 13;
     const int    Column_GrowthRateMin  = 14;
     const int    Column_GrowthRateMax  = 15;
     const int    Column_SpeciesKMin    = 16;
     const int    Column_SpeciesKMax    = 17;
     const int    Last_Review_Column_Visible = 13;
     const bool   Query_User_For_Filename = true;
     const bool   Dont_Query_User_For_Filename = false;

//     const double epsilon               = 0.0; // 0.001;

     const double NoFishingMortality   = -1.0;
     const double MaxOutOfBounds       = 9999999.0;
     const bool   RotateLabels         = true;
     const bool   DontRotateLabels     = false;
     const int    MaxNumberLogFiles    =  50;
     const int    MinSliderValue       =   0;
     const int    MaxSliderValue       =  99;
     const int    MaxNumberSeasons     =   6;
     const int    MaxNumberYears       = 100;
     const int    MaxNumberSpecies     =  12;
     const int    MaxTotalSpecies      =  50;
     const int    ToolTipDuration      = 2000; // milliseconds
     const double ValueToStartEE       = 1000000.0;
     const bool   RearrangeTitle       = true;
     const bool   DontRearrangeTitle   = false;
     const bool   Clear                = true;
     const bool   DontClear            = false;
     const bool   ShowError            = true;
     const bool   DontShowError        = false;
     const bool   ShowFirstPoint       = true;
     const bool   HideFirstPoint       = false;
     const bool   RescaleData          = true;
     const bool   DontRescaleData      = false;
     const bool   LabelXAxisAsReals    = false;
     const bool   LabelXAxisAsInts     = true;
     const bool   UseDimmedColor       = true;
     const bool   DontUseDimmedColor   = false;
     const bool   VerboseOn            = true;
     const bool   VerboseOff           = false;
     const bool   ShowPopupError       = true;
     const bool   DontShowPopupError   = false;
     const bool   ShowDiagnosticsChart = true;
     const bool   DontShowDiagnosticsChart = false;
     const bool   ShowBlankFields      = true;
     const bool   LoadFirstRowOnly     = true;
     const bool   LoadAllRows          = false;
     const bool   ReadValidPointsOnly  = true;
     const bool   ReadAllPoints        = false;
     const bool   ResetPositionAlso    = true;
     const bool   DontResetPosition    = false;
     const bool   LeaveGapsWhereNegative     = true;
     const bool   DontLeaveGapsWhereNegative = false;
     const bool   FirstLineReadOnly    = true;
     const bool   FirstLineNotReadOnly = false;
     const bool   IsATimeSeries        = true;
     const bool   IsNotATimeSeries     = false;
     const bool   UpdateSetup          = true;
     const bool   DontUpdateSetup      = false;
     const bool   IsAveraged           = true;
     const bool   IsNotAveraged        = false;
     const bool   ScientificNotation   = true;
     const bool   FixedNotation        = false;
     const bool   IsEnsemble           = true;
     const bool   IsNotEnsemble        = false;
     const bool   IsMonteCarlo         = true;
     const bool   IsNotMonteCarlo      = false;
     const bool   PreEstimation        = true;
     const bool   ShowLegend           = true;
     const bool   DontShowLegend       = false;
     const QString ChartType2d         = "2d";
     const QString ChartType3d         = "3d";
     const bool   MultiplyTableByCatchability      = true;
     const bool   DontMultiplyTableByCatchability  = false;
     const bool   MultiplyTableByOutputBiomass     = true;
     const bool   DontMultiplyTableByOutputBiomass = false;
     const bool   DivideTableByOutputBiomass       = true;
     const bool   DontDivideTableByOutputBiomass   = false;
     const bool   ReloadAllWidgets                 = true;
     const bool   DontReloadAllWidgets             = false;

     const std::string  HiddenDir                      = ".MSSPM";
     const std::string  HiddenDataDir                  = ".MSSPM/data";
     const std::string  HiddenLogDir                   = ".MSSPM/logs";
     const std::string  MSSPMProgressBarFile           = ".MSSPM/MSSPMProgress.dat";
     const std::string  MSSPMProgressChartFile         = ".MSSPM/MSSPMProgressChart.dat";
     const std::string  MSSPMProgressChartLabelFile    = ".MSSPM/MSSPMProgressChartLabel.dat";
     const std::string  MSSPMStopRunFile               = ".MSSPM/MSSPMStopRun.dat";
     const std::string  MSSPMCurrentLoopFile           = ".MSSPM/MSSPMCurrentLoop.dat";
     const std::string  SettingsDirWindows             = "C:\\.QtSettings";
     const std::string  LogDir                         = ".MSSPM/logs";
     const std::string  LogFilter                      = ".MSSPM/logs/*.log";
     const std::string  InputDataDir                   = "inputData";
     const std::string  OutputDataDir                  = "outputData";
     const std::string  OutputImagesDir                = "outputImages";
     const std::string  OutputImagesDirMMode           = "Remora/outputImages";
     const std::string  OutputDataDirMMode             = "Remora/outputData";
     const std::string  OutputScenariosDirMMode        = "Remora/scenarios";
     const std::string  MultiRunFilename               = "MultiRunParameter.csv";
     const std::string  MohnsRhoRunFilename            = "MohnsRhoMultiRunParameter.csv";
     const std::string  ModelReviewFilename            = "ModelReview.csv";
     const std::string  EstimatedParametersFilename    = "EstimatedParameters.csv";
     const QString      OutputChartBiomass             = "Biomass vs Time";
     const QString      OutputChartHarvest             = "Harvest vs Time";
     const QString      OutputChartExploitation        = "Exploitation Rate";
     const QString      OutputChartExploitationCatchAverageTitle = "Exploitation Rate Ave(C/Bc)";
     const QString      OutputChartExploitationCatchTitle        = "Exploitation Rate (C/Bc)";
     const QString      OutputChartExploitationEffortTitle       = "Exploitation Rate ((qE)/Bc)";
     const QString      HarvestNone                    = "Null";
     const QString      HarvestCatch                   = "Catch";
     const QString      HarvestEffort                  = "Effort (qE)";
     const QString      HarvestExploitation            = "Exploitation Rate (Harvest/Bc)";

     const QString      ParameterNameInitialBiomass            = "Initial Absolute Biomass";
     const QString      ParameterNameSurveyQ                   = "SurveyQ";
     const QString      ParameterNameGrowthRate                = "Growth Rate";
     const QString      ParameterNameCarryingCapacity          = "Carrying Capacity";
     const QString      ParameterNameCatchability              = "Catchability";
     const QString      ParameterNameCompetitionAlpha          = "Alpha";
     const QString      ParameterNameCompetitionBetaSpecies    = "Beta SpeciesSpecies";
     const QString      ParameterNameCompetitionBetaGuild      = "Beta GuildSpecies";
     const QString      ParameterNameCompetitionBetaGuildGuild = "Beta GuildGuild";
     const QString      ParameterNamePredationRho              = "Predation Effect";
     const QString      ParameterNamePredationHandling         = "Handling Time";
     const QString      ParameterNamePredationExponent         = "Predation Exponent";

     const QString      SectionSetup                           = "Setup";
     const QString      SectionEstimation                      = "Estimation Data Input";
     const QString      SectionDiagnostic                      = "Diagnostic Data Input";
     const QString      SectionForecast                        = "Forecast";

     const std::string TableApplication                        = QString::fromStdString("Application").toLower().toStdString();
     const std::string TableBetweenGuildsInteractionCoeff      = QString::fromStdString("BetweenGuildsInteractionCoeff").toLower().toStdString();
     const std::string TableBiomassAbsolute                    = QString::fromStdString("BiomassAbsolute").toLower().toStdString();
     const std::string TableBiomassRelative                    = QString::fromStdString("BiomassRelative").toLower().toStdString();
     const std::string TableBiomassRelativeDividedByEstSurveyQ = QString::fromStdString("BiomassRelativeDividedByEstSurveyQ").toLower().toStdString();
     const std::string TableBiomassRelativeScalars             = QString::fromStdString("BiomassRelativeScalars").toLower().toStdString();
     const std::string TableCompetitionAlpha                   = QString::fromStdString("CompetitionAlpha").toLower().toStdString();
     const std::string TableCompetitionAlphaMax                = QString::fromStdString("CompetitionAlphaMax").toLower().toStdString();
     const std::string TableCompetitionAlphaMin                = QString::fromStdString("CompetitionAlphaMin").toLower().toStdString();
     const std::string TableCompetitionBetaGuilds              = QString::fromStdString("CompetitionBetaGuilds").toLower().toStdString();
     const std::string TableCompetitionBetaGuildsGuilds        = QString::fromStdString("CompetitionBetaGuildsGuilds").toLower().toStdString();
     const std::string TableCompetitionBetaGuildsGuildsMax     = QString::fromStdString("CompetitionBetaGuildsGuildsMax").toLower().toStdString();
     const std::string TableCompetitionBetaGuildsGuildsMin     = QString::fromStdString("CompetitionBetaGuildsGuildsMin").toLower().toStdString();
     const std::string TableCompetitionBetaGuildsMax           = QString::fromStdString("CompetitionBetaGuildsMax").toLower().toStdString();
     const std::string TableCompetitionBetaGuildsMin           = QString::fromStdString("CompetitionBetaGuildsMin").toLower().toStdString();
     const std::string TableCompetitionBetaSpecies             = QString::fromStdString("CompetitionBetaSpecies").toLower().toStdString();
     const std::string TableCompetitionBetaSpeciesMax          = QString::fromStdString("CompetitionBetaSpeciesMax").toLower().toStdString();
     const std::string TableCompetitionBetaSpeciesMin          = QString::fromStdString("CompetitionBetaSpeciesMin").toLower().toStdString();
     const std::string TableCovariate                          = QString::fromStdString("Covariate").toLower().toStdString();
     const std::string TableCovariateTS                        = QString::fromStdString("CovariateTS").toLower().toStdString();
     const std::string TableDiagnosticCarryingCapacity         = QString::fromStdString("DiagnosticCarryingCapacity").toLower().toStdString();
     const std::string TableDiagnosticCatchability             = QString::fromStdString("DiagnosticCatchability").toLower().toStdString();
     const std::string TableDiagnosticGrowthRate               = QString::fromStdString("DiagnosticGrowthRate").toLower().toStdString();
     const std::string TableDiagnosticInitBiomass              = QString::fromStdString("DiagnosticInitBiomass").toLower().toStdString();
     const std::string TableDiagnosticSurface                  = QString::fromStdString("DiagnosticSurface").toLower().toStdString();
     const std::string TableDiagnosticSurveyQ                  = QString::fromStdString("DiagnosticSurveyQ").toLower().toStdString();
     const std::string TableForecastBiomass                    = QString::fromStdString("ForecastBiomass").toLower().toStdString();
     const std::string TableForecastBiomassMonteCarlo          = QString::fromStdString("ForecastBiomassMonteCarlo").toLower().toStdString();
     const std::string TableForecastBiomassMultiScenario       = QString::fromStdString("ForecastBiomassMultiScenario").toLower().toStdString();
     const std::string TableForecastHarvestCatch               = QString::fromStdString("ForecastHarvestCatch").toLower().toStdString();
     const std::string TableForecastHarvestEffort              = QString::fromStdString("ForecastHarvestEffort").toLower().toStdString();
     const std::string TableForecastHarvestExploitation        = QString::fromStdString("ForecastHarvestExploitation").toLower().toStdString();
     const std::string TableForecastMonteCarloParameters       = QString::fromStdString("ForecastMonteCarloParameters").toLower().toStdString();
     const std::string TableForecastUncertainty                = QString::fromStdString("ForecastUncertainty").toLower().toStdString();
     const std::string TableForecasts                          = QString::fromStdString("Forecasts").toLower().toStdString();
     const std::string TableGuilds                             = QString::fromStdString("Guilds").toLower().toStdString();
     const std::string TableHarvestCatch                       = QString::fromStdString("HarvestCatch").toLower().toStdString();
     const std::string TableHarvestEffort                      = QString::fromStdString("HarvestEffort").toLower().toStdString();
     const std::string TableHarvestExploitation                = QString::fromStdString("HarvestExploitation").toLower().toStdString();
     const std::string TableModels                             = QString::fromStdString("Models").toLower().toStdString();
     const std::string TableOutputBiomass                      = QString::fromStdString("OutputBiomass").toLower().toStdString();
     const std::string TableOutputBiomassEnsemble              = QString::fromStdString("OutputBiomassEnsemble").toLower().toStdString();
     const std::string TableOutputCarryingCapacity             = QString::fromStdString("OutputCarryingCapacity").toLower().toStdString();
     const std::string TableOutputCatchability                 = QString::fromStdString("OutputCatchability").toLower().toStdString();
     const std::string TableOutputCompetitionAlpha             = QString::fromStdString("OutputCompetitionAlpha").toLower().toStdString();
     const std::string TableOutputCompetitionBetaGuilds        = QString::fromStdString("OutputCompetitionBetaGuilds").toLower().toStdString();
     const std::string TableOutputCompetitionBetaGuildsGuilds  = QString::fromStdString("OutputCompetitionBetaGuildsGuilds").toLower().toStdString();
     const std::string TableOutputCompetitionBetaSpecies       = QString::fromStdString("OutputCompetitionBetaSpecies").toLower().toStdString();
     const std::string TableOutputGrowthRate                   = QString::fromStdString("OutputGrowthRate").toLower().toStdString();
     const std::string TableOutputInitBiomass                  = QString::fromStdString("OutputInitBiomass").toLower().toStdString();
     const std::string TableOutputMSY                          = QString::fromStdString("OutputMSY").toLower().toStdString();
     const std::string TableOutputMSYBiomass                   = QString::fromStdString("OutputMSYBiomass").toLower().toStdString();
     const std::string TableOutputMSYFishing                   = QString::fromStdString("OutputMSYFishing").toLower().toStdString();
     const std::string TableOutputPredationExponent            = QString::fromStdString("OutputPredationExponent").toLower().toStdString();
     const std::string TableOutputPredationHandling            = QString::fromStdString("OutputPredationHandling").toLower().toStdString();
     const std::string TableOutputPredationRho                 = QString::fromStdString("OutputPredationRho").toLower().toStdString();
     const std::string TableOutputSurveyQ                      = QString::fromStdString("OutputSurveyQ").toLower().toStdString();
     const std::string TablePredationExponent                  = QString::fromStdString("PredationExponent").toLower().toStdString();
     const std::string TablePredationExponentMax               = QString::fromStdString("PredationExponentMax").toLower().toStdString();
     const std::string TablePredationExponentMin               = QString::fromStdString("PredationExponentMin").toLower().toStdString();
     const std::string TablePredationHandling                  = QString::fromStdString("PredationHandling").toLower().toStdString();
     const std::string TablePredationHandlingMax               = QString::fromStdString("PredationHandlingMax").toLower().toStdString();
     const std::string TablePredationHandlingMin               = QString::fromStdString("PredationHandlingMin").toLower().toStdString();
     const std::string TablePredationRho                       = QString::fromStdString("PredationRho").toLower().toStdString();
     const std::string TablePredationRhoMax                    = QString::fromStdString("PredationRhoMax").toLower().toStdString();
     const std::string TablePredationRhoMin                    = QString::fromStdString("PredationRhoMin").toLower().toStdString();
     const std::string TableSpatialOverlap                     = QString::fromStdString("SpatialOverlap").toLower().toStdString();
     const std::string TableSpecies                            = QString::fromStdString("Species").toLower().toStdString();

     const QStringList VectorParameterNames =
         {"Initial Biomass (B₀)",
          "Growth Rate (r)",
          "Carrying Capacity (K)",
          "Catchability (q)",
          "SurveyQ"
         };

     const std::vector<std::string> EstimateCheckboxNames =
         {"InitBiomass",
          "GrowthRate",
          "CarryingCapacity",
          "Catchability",
          "Handling",
          "CompetitionAlpha",
          "CompetitionBetaSpeciesSpecies",
          "CompetitionBetaGuildSpecies",
          "CompetitionBetaGuildGuild",
          "PredationRho",
          "PredationExponent",
          "SurveyQ"};

     const std::vector<std::string> TablesToClean =
        {"BiomassAbsolute",
         "BiomassRelative",
         "BiomassRelativeDividedByEstSurveyQ",
         "BiomassRelativeScalars",
         "CompetitionAlpha",
         "CompetitionAlphaMax",
         "CompetitionAlphaMin",
         "CompetitionBetaGuilds",
         "CompetitionBetaGuildsGuilds",
         "CompetitionBetaGuildsGuildsMax",
         "CompetitionBetaGuildsGuildsMin",
         "CompetitionBetaGuildsMax",
         "CompetitionBetaGuildsMin",
         "CompetitionBetaSpecies",
         "CompetitionBetaSpeciesMax",
         "CompetitionBetaSpeciesMin",
         "HarvestCatch",
         "HarvestEffort",
         "HarvestExploitation",
         "Models",
         "PredationExponent",
         "PredationExponentMax",
         "PredationExponentMin",
         "PredationHandling",
         "PredationHandlingMax",
         "PredationHandlingMin",
         "PredationRho",
         "PredationRhoMax",
         "PredationRhoMin"};
}


#endif
