
/**
 * @file nmfConstantsMSSPM.h
 * @brief Definition for constants used in MSSPM tool
 * @date Jan 14, 2020
 *
 * This file defines constants used in the MSSPM application program.
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

#ifndef NMFCONSTANTSMSSPM_H
#define NMFCONSTANTSMSSPM_H

#include <iostream>
#include <vector>
#include <QStringList>

/**
 * @brief Constants used specifically in the MSSPM application
 */
namespace nmfConstantsMSSPM
{
     const int    Start_Year            = 1900;
     const int    Num_Columns_Species   = 5;
     const int    Num_Columns_Guilds    = 3;
     const int    Num_Estimatable_Parameter_Groups = 13;

     const int    Column_Guild_Name       =  0;
     const int    Column_Guild_GrowthRate =  1;
     const int    Column_Guild_GuildK     =  2;

     const int    Column_Species_Guild    =  2;

     const int    Column_Supp_Guild_Name             =  0;
     const int    Column_Supp_Guild_GrowthRate       =  1;
     const int    Column_Supp_Guild_CarryingCapacity =  4;

     const int    Column_Supp_Species_Name                =   0;
     const int    Column_Supp_Species_InitBiomass         =   1;
     const int    Column_Supp_Species_GrowthRate          =   4;
     const int    Column_Supp_Species_CarryingCapacity    =   8;
     const int    Column_Supp_Species_CarryingCapacityMin =   9;
     const int    Column_Supp_Species_SurveyQ             =  12;
     const int    Column_Supp_Species_SurveyQMin          =  13;
     const int    Column_Supp_Species_SurveyQMax          =  14;

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
     const int    Model_Review_Column_rSquared     =  1;
     const int    Model_Review_Column_SSResiduals  =  2;
     const int    Model_Review_Column_AIC          =  3;
     const int    Model_Review_Column_DB_Snapshot  = 13;
     const int    Model_Review_Column_Last_Visible = 14;
     const int    Model_Review_Column_Estimated_Parameters = 64;
     const bool   Query_User_For_Filename      = true;
     const bool   Dont_Query_User_For_Filename = false;
//   const double epsilon              = 0.0; // 0.001;
     const double NoFishingMortality   = -1.0;
     const double MaxOutOfBounds       = 9999999.0;
     const double MaxValidProgressYValue = 90000.0;
     const double NoData               = -99999.0;
     const double DefaultTolerance     = 1E-06;
     const bool   RotateLabels         = true;
     const bool   DontRotateLabels     = false;
     const int    MaxNumberLogFiles    =  50;
     const int    MinSliderValue       =   0;
     const int    MaxSliderValue       =  99;
     const int    MaxNumberSeasons     =   6;
     const int    MaxNumberYears       = 100;
     const int    MaxNumberSpecies     =  12;
     const int    MaxTotalSpecies      =  50;
     const int    ToolTipDuration      = 5000; // milliseconds
     const double ValueToStartEE       = 1000000.0;
     const int    DefaultDBTimeoutDays = 5;
     const int    NotEnabledAndNotChecked =  0;
     const int    NotEnabledButChecked    =  1;
     const int    EnabledAndNotChecked    = 10;
     const int    EnabledAndChecked       = 11;
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
     const bool   ClearModelName       = true;
     const bool   DontClearModelName   = false;
     const QString ChartType2d         = "2d";
     const QString ChartType3d         = "3d";
     const bool   MultiplyTableByCatchability      = true;
     const bool   DontMultiplyTableByCatchability  = false;
     const bool   MultiplyTableByOutputBiomass     = true;
     const bool   DontMultiplyTableByOutputBiomass = false;
     const bool   DivideTableByOutputBiomass       = true;
     const bool   DontDivideTableByOutputBiomass   = false;
     const bool   RemoveCommas                     = true;
     const bool   DontRemoveCommas                 = false;
     const bool   Editable                         = true;
     const bool   NotEditable                      = false;
     const bool   GrayedIfNotEditable              = true;
     const bool   WhiteIfEditable                  = false;
     const bool   ShowHistoricalDelimiterLine      = true;
     const bool   DontShowHistoricalDelimiterLine  = false;
     const bool   AllowBlanks                      = true;
     const bool   DontAllowBlanks                  = false;
     const bool   ConvertAll                       = true;
     const bool   DontConvertBlanks                = false;
     const bool   UseLogData                       = true;
     const bool   DontUseLogData                   = false;
     const bool   ReadHarvestTypeFromTable         = true;
     const bool   DontReadHarvestTypeFromTable     = false;
     const bool   RunChecks                        = true;
     const bool   DontRunChecks                    = false;
     const bool   ResetCheckboxes                  = true;
     const bool   DontResetCheckboxes              = false;

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
     const std::string  FilenameMultiRun               = "MultiRunParameter.csv";
     const std::string  FilenameMohnsRhoRun            = "MohnsRhoMultiRunParameter.csv";
     const std::string  FilenameModelReview            = "ModelReview.csv";
     const std::string  FilenameSummaryModelFit        = "SummaryModelFit.csv";
     const std::string  FilenameSummaryDiagnostic      = "SummaryDiagnostic.csv";
     const std::string  FilenameEstimatedParameters    = "EstimatedParameters.csv";
     const std::string  TitleChartBiomass              = "B(calc) with B(obs) points";
     const std::string  TitleChartBiomassAndCatch      = "Catch with B(obs) points";
     const QString      OutputChartBiomass             = "Biomass vs Time";
     const QString      OutputChartBiomassAndCatch     = "Biomass & Catch vs Time";
     const QString      OutputChartHarvest             = "Harvest vs Time";
     const QString      OutputChartExploitation        = "Exploitation Rate";
     const QString      OutputChartExploitationCatchAverageTitle = "Exploitation Rate Ave(C/Bc)";
     const QString      OutputChartExploitationCatchTitle        = "Exploitation Rate (C/Bc)";
     const QString      OutputChartExploitationEffortTitle       = "Exploitation Rate ((qE)/Bc)";
     const QString      HarvestNone                    = "Null";
     const QString      HarvestCatch                   = "Catch";
     const QString      HarvestEffort                  = "Effort (qE)";
     const QString      HarvestEffortFitToCatch        = "Effort Fit to Catch";
     const QString      HarvestExploitation            = "Exploitation Rate (Harvest/Bc)";
     const QString      ForecastHarvestTypeCatch       = "Catch";
     const QString      ForecastHarvestTypeEffort      = "Effort";
     const QString      DistributionNone               = "None";
     const QString      DistributionLognormal          = "Lognormal";
     const QString      DistributionUniform            = "Uniform";

     const QString      ParameterNameInitialBiomass              = "Initial Absolute Biomass";
     const QString      ParameterNameSurveyQ                     = "SurveyQ";
     const QString      ParameterNameGrowthRate                  = "Growth Rate";
     const QString      ParameterNameGrowthRateShape             = "Growth Rate Shape";
     const QString      ParameterNameCarryingCapacity            = "Carrying Capacity";
     const QString      ParameterNameCatchability                = "Catchability";
     const QString      ParameterNameCompetitionAlpha            = "Competition Alpha";
     const QString      ParameterNameCompetitionBetaSpecies      = "Competition Beta Species";
     const QString      ParameterNameCompetitionBetaGuilds       = "Competition Beta Guilds";
     const QString      ParameterNameCompetitionBetaGuildsGuilds = "Competition Beta GuildsGuilds";
     const QString      ParameterNamePredationRho                = "Predation Rho";
     const QString      ParameterNamePredationHandling           = "Predation Handling";
     const QString      ParameterNamePredationExponent           = "Predation Exponent";
     const QString      ParameterNameGrowthRateCovCoeff          = "Growth Rate (r) Covariate Coefficient";
     const QString      ParameterNameCarryingCapacityCovCoeff    = "Carrying Capacity (K) Covariate Coefficient";
     const QString      ParameterNameCatchabilityCovCoeff        = "Catchability (q) Covariate Coefficient";
     const QString      ParameterNameSurveyQCovCoeff             = "SurveyQ Covariate Coefficient";

     const QString      SectionSetup                           = "Setup";
     const QString      SectionEstimation                      = "Estimation Data Input";
     const QString      SectionDiagnostic                      = "Diagnostic Data Input";
     const QString      SectionForecast                        = "Forecast";

     const int          PageSetupProject                       = 1;
     const int          PageSetupSpecies                       = 2;
     const int          PageSetupModel                         = 3;

     const int          PageEstimationPopulation               = 0;
     const int          PageEstimationHarvest                  = 1;
     const int          PageEstimationCompetition              = 2;
     const int          PageEstimationPredation                = 3;
     const int          PageEstimationObservation              = 4;
     const int          PageEstimationCovariate                = 5;
     const int          PageEstimationRun                      = 6;
     const int          PageEstimationModelReview              = 7;

     const int          PublishLineWidthAxes                   =  2;
     const int          PublishLineWidthData                   =  3;
     const int          PublishLineWidthPoint                  = 11;
     const int          PublishFontSizeLabel                   = 15;
     const int          PublishFontSizeNumber                  = 11;
     const QString      PublishFont                            = "Unicode";

     const std::string Linear                                  = "Linear";
     const std::string Exponential                             = "Exponential";

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
     const std::string TableCovariateAssignment                = QString::fromStdString("CovariateAssignment").toLower().toStdString();
     const std::string TableCovariateInitialValuesAndRanges    = QString::fromStdString("CovariateInitialValuesAndRanges").toLower().toStdString();
     const std::string TableDiagnosticCarryingCapacity         = QString::fromStdString("DiagnosticCarryingCapacity").toLower().toStdString();
     const std::string TableDiagnosticCatchability             = QString::fromStdString("DiagnosticCatchability").toLower().toStdString();
     const std::string TableDiagnosticGrowthRate               = QString::fromStdString("DiagnosticGrowthRate").toLower().toStdString();
     const std::string TableDiagnosticGrowthRateShape          = QString::fromStdString("DiagnosticGrowthRateShape").toLower().toStdString();
     const std::string TableDiagnosticInitBiomass              = QString::fromStdString("DiagnosticInitBiomass").toLower().toStdString();
     const std::string TableDiagnosticSurface                  = QString::fromStdString("DiagnosticSurface").toLower().toStdString();
     const std::string TableDiagnosticSurveyQ                  = QString::fromStdString("DiagnosticSurveyQ").toLower().toStdString();
     const std::string TableDiagnosticGrowthRateCovCoeff       = QString::fromStdString("DiagnosticGrowthRateCovCoeff").toLower().toStdString();
     const std::string TableDiagnosticCarryingCapacityCovCoeff = QString::fromStdString("DiagnosticCarryingCapacityCovCoeff").toLower().toStdString();
     const std::string TableDiagnosticCatchabilityCovCoeff     = QString::fromStdString("DiagnosticCatchabilityCovCoeff").toLower().toStdString();
     const std::string TableDiagnosticSurveyQCovCoeff          = QString::fromStdString("DiagnosticSurveyQCovCoeff").toLower().toStdString();

     const std::string TableFitWeights                         = QString::fromStdString("FitWeights").toLower().toStdString();
     const std::string TableForecastBiomass                    = QString::fromStdString("ForecastBiomass").toLower().toStdString();
     const std::string TableForecastBiomassMonteCarlo          = QString::fromStdString("ForecastBiomassMonteCarlo").toLower().toStdString();
     const std::string TableForecastBiomassMultiScenario       = QString::fromStdString("ForecastBiomassMultiScenario").toLower().toStdString();
     const std::string TableForecastHarvestCatch               = QString::fromStdString("ForecastHarvestCatch").toLower().toStdString();
     const std::string TableForecastHarvestCatchFTC            = QString::fromStdString("ForecastHarvestCatchFTC").toLower().toStdString();
     const std::string TableForecastHarvestEffort              = QString::fromStdString("ForecastHarvestEffort").toLower().toStdString();
     const std::string TableForecastHarvestEffortFTC           = QString::fromStdString("ForecastHarvestEffortFTC").toLower().toStdString();
     const std::string TableForecastHarvestExploitation        = QString::fromStdString("ForecastHarvestExploitation").toLower().toStdString();
     const std::string TableForecastMonteCarloParameters       = QString::fromStdString("ForecastMonteCarloParameters").toLower().toStdString();
     const std::string TableForecastUncertainty                = QString::fromStdString("ForecastUncertainty").toLower().toStdString();
     const std::string TableForecasts                          = QString::fromStdString("Forecasts").toLower().toStdString();
     const std::string TableGuilds                             = QString::fromStdString("Guilds").toLower().toStdString();
     const std::string TableHarvestCatch                       = QString::fromStdString("HarvestCatch").toLower().toStdString();
//   const std::string TableHarvestCatchFTC                    = QString::fromStdString("HarvestCatchFTC").toLower().toStdString();
     const std::string TableHarvestEffort                      = QString::fromStdString("HarvestEffort").toLower().toStdString();
//   const std::string TableHarvestEffortFTC                   = QString::fromStdString("HarvestEffortFTC").toLower().toStdString();
     const std::string TableHarvestExploitation                = QString::fromStdString("HarvestExploitation").toLower().toStdString();
     const std::string TableModelReview                        = QString::fromStdString("ModelReview").toLower().toStdString();
     const std::string TableModels                             = QString::fromStdString("Models").toLower().toStdString();
     const std::string TableOutputBiomass                      = QString::fromStdString("OutputBiomass").toLower().toStdString();
     const std::string TableOutputBiomassEnsemble              = QString::fromStdString("OutputBiomassEnsemble").toLower().toStdString();
     const std::string TableOutputBiomassMohnsRhoOfEnsembles   = QString::fromStdString("OutputBiomassMohnsRhoOfEnsembles").toLower().toStdString();
     const std::string TableOutputCarryingCapacity             = QString::fromStdString("OutputCarryingCapacity").toLower().toStdString();
     const std::string TableOutputCarryingCapacityCovariateCoeffs = QString::fromStdString("OutputCarryingCapacityCovariateCoeffs").toLower().toStdString();
     const std::string TableOutputCatchability                 = QString::fromStdString("OutputCatchability").toLower().toStdString();
     const std::string TableOutputCatchabilityCovariateCoeffs  = QString::fromStdString("OutputCatchabilityCovariateCoeffs").toLower().toStdString();
     const std::string TableOutputCompetitionAlpha             = QString::fromStdString("OutputCompetitionAlpha").toLower().toStdString();
     const std::string TableOutputCompetitionBetaGuilds        = QString::fromStdString("OutputCompetitionBetaGuilds").toLower().toStdString();
     const std::string TableOutputCompetitionBetaGuildsGuilds  = QString::fromStdString("OutputCompetitionBetaGuildsGuilds").toLower().toStdString();
     const std::string TableOutputCompetitionBetaSpecies       = QString::fromStdString("OutputCompetitionBetaSpecies").toLower().toStdString();
     const std::string TableOutputGrowthRate                   = QString::fromStdString("OutputGrowthRate").toLower().toStdString();
     const std::string TableOutputGrowthRateShape              = QString::fromStdString("OutputGrowthRateShape").toLower().toStdString();
     const std::string TableOutputGrowthRateCovariateCoeffs    = QString::fromStdString("OutputGrowthRateCovariateCoeffs").toLower().toStdString();
     const std::string TableOutputInitBiomass                  = QString::fromStdString("OutputInitBiomass").toLower().toStdString();
     const std::string TableOutputMSY                          = QString::fromStdString("OutputMSY").toLower().toStdString();
     const std::string TableOutputMSYBiomass                   = QString::fromStdString("OutputMSYBiomass").toLower().toStdString();
     const std::string TableOutputMSYFishing                   = QString::fromStdString("OutputMSYFishing").toLower().toStdString();
     const std::string TableOutputPredationExponent            = QString::fromStdString("OutputPredationExponent").toLower().toStdString();
     const std::string TableOutputPredationHandling            = QString::fromStdString("OutputPredationHandling").toLower().toStdString();
     const std::string TableOutputPredationRho                 = QString::fromStdString("OutputPredationRho").toLower().toStdString();
     const std::string TableOutputSurveyQ                      = QString::fromStdString("OutputSurveyQ").toLower().toStdString();
     const std::string TableOutputSurveyQCovariateCoeffs       = QString::fromStdString("OutputSurveyQCovariateCoeffs").toLower().toStdString();
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
     const std::string TableSummaryDiagnostic                  = QString::fromStdString("SummaryDiagnostic").toLower().toStdString();
     const std::string TableSummaryModelFit                    = QString::fromStdString("SummaryModelFit").toLower().toStdString();
     const std::string TableUnits                              = QString::fromStdString("Units").toLower().toStdString();

     const QStringList SummaryModelFitStatisticNames =
         {"SSresiduals",
          "SSdeviations",
          "SStotals",
          "r²",
          "r",
          "AIC",
          "RMSE",
          "RI",
          "AE",
          "AAE",
          "MEF"};

     const QStringList SummaryModelFitStatisticTooltips =
         {"Sum of Squared Residuals",
          "Sum of Squared Deviations",
          "Total Sum of Squares",
          "Determination Coefficient",
          "Correlation Coefficient",
          "Akaike Information Criterion",
          "Root Mean Square Error",
          "Reliability Index",
          "Average Error",
          "Average Absolute Error",
          "Model Efficiency"};

     const QStringList SummaryDiagnosticStatisticNames =
         {"Mohn's Rho (Initial Absolute Biomass)",
          "Mohn's Rho (Growth Rate)",
          "Mohn's Rho (Growth Rate Shape)",
          "Mohn's Rho (Carrying Capacity)",
          "Mohn's Rho (Competition Alpha)",
          "Mohn's Rho (Competition Beta Species)",
          "Mohn's Rho (Competition Beta Guilds)",
          "Mohn's Rho (Competition Beta GuildsGuilds)",
          "Mohn's Rho (Predation Rho)",
          "Mohn's Rho (Predation Handling)",
          "Mohn's Rho (Predation Exponent)",
          "Mohn's Rho (SurveyQ)",
          "Mohn's Rho (Estimated Biomass)"};

     const QStringList VectorParameterNames =
         {"Initial Biomass (B₀)",
          "Growth Rate (r)",
          "Growth Rate Shape (p)",
          "Carrying Capacity (K)",
          "Catchability (q)",
          "SurveyQ"};

     const std::vector<std::string> EstimateCheckboxNames =
         {"InitBiomass",
          "GrowthRate",
          "GrowthRateShape",
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
        {TableBiomassAbsolute,
         TableBiomassRelative,
         TableBiomassRelativeDividedByEstSurveyQ,
         TableBiomassRelativeScalars,
         TableCompetitionAlpha,
         TableCompetitionAlphaMax,
         TableCompetitionAlphaMin,
         TableCompetitionBetaGuilds,
         TableCompetitionBetaGuildsGuilds,
         TableCompetitionBetaGuildsGuildsMax,
         TableCompetitionBetaGuildsGuildsMin,
         TableCompetitionBetaGuildsMax,
         TableCompetitionBetaGuildsMin,
         TableCompetitionBetaSpecies,
         TableCompetitionBetaSpeciesMax,
         TableCompetitionBetaSpeciesMin,
         TableCovariate,
         TableCovariateAssignment,
         TableCovariateInitialValuesAndRanges,
         TableFitWeights,
         TableHarvestCatch,
         TableHarvestEffort,
         TableHarvestExploitation,
         TableModels,
         TableModelReview,
         TablePredationExponent,
         TablePredationExponentMax,
         TablePredationExponentMin,
         TablePredationHandling,
         TablePredationHandlingMax,
         TablePredationHandlingMin,
         TablePredationRho,
         TablePredationRhoMax,
         TablePredationRhoMin,
         TableSummaryDiagnostic,
         TableSummaryModelFit,
         TableUnits};
}


#endif
