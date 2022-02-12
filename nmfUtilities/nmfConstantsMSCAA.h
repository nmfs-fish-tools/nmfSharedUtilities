/**
 * @file nmfConstantsMSCAA.h
 * @brief Definition for constants used in MSCAA tool
 * @date Jan 14, 2020
 *
 * This file defines constants used in the MSCAA application program.
 */

#pragma once

#include <iostream>
#include <vector>
#include <QString>

namespace nmfConstantsMSCAA
{
    const double       Kg2Mt                      = 0.001;
    const int          MaxNumYears                = 100;
    const int          MaxNumSpecies              =  12;
    const int          MaxNumAges                 =  30;
    const int          DefaultDBTimeoutDays       =   5;
    const bool         ClearSummary               = true;
    const bool         DontClearSummary           = false;
    const bool         ShowFirstPoint             = true;
    const bool         HideFirstPoint             = false;
    const bool         LeaveGapsWhereNegative     = true;
    const bool         DontLeaveGapsWhereNegative = false;

    const std::string  HiddenDir                      = ".MSCAA";
    const std::string  HiddenDataDir                  = ".MSCAA/data";
    const std::string  HiddenLogDir                   = ".MSCAA/logs";
    const std::string  MSCAAProgressBarFile           = ".MSCAA/MSCAAProgress.dat";
    const std::string  MSCAAProgressChartFile         = ".MSCAA/MSCAAProgressChart.dat";
    const std::string  MSCAAProgressChartLabelFile    = ".MSCAA/MSCAAProgressChartLabel.dat";
    const std::string  MSCAAStopRunFile               = ".MSCAA/MSCAAStopRun.dat";
    const std::string  MSCAACurrentLoopFile           = ".MSCAA/MSCAACurrentLoop.dat";
    const std::string  LogDir                         = ".MSCAA/logs";
    const std::string  LogFilter                      = ".MSCAA/logs/*.log";
    const std::string  SettingsDirWindows             = "C:\\.QtSettings";
    const std::string  InputDataDir                   = "inputData";
    const std::string  OutputDataDir                  = "outputData";
    const std::string  OutputImagesDir                = "outputImages";

    const std::string TableAgeLengthKey                = QString::fromStdString("AgeLengthKey").toLower().toStdString();
    const std::string TableApplication                 = QString::fromStdString("Application").toLower().toStdString();
    const std::string TableCatchAtLengthFishery        = QString::fromStdString("CatchAtLengthFishery").toLower().toStdString();
    const std::string TableCatchFishery                = QString::fromStdString("CatchFishery").toLower().toStdString();
    const std::string TableCatchFisheryProportion      = QString::fromStdString("CatchFisheryProportion").toLower().toStdString();
    const std::string TableCatchFisheryTotal           = QString::fromStdString("CatchFisheryTotal").toLower().toStdString();
    const std::string TableCatchSurvey                 = QString::fromStdString("CatchSurvey").toLower().toStdString();
    const std::string TableCatchSurveyProportion       = QString::fromStdString("CatchSurveyProportion").toLower().toStdString();
    const std::string TableCatchSurveyTotal            = QString::fromStdString("CatchSurveyTotal").toLower().toStdString();
    const std::string TableConsumption                 = QString::fromStdString("Consumption").toLower().toStdString();
    const std::string TableCovariates                  = QString::fromStdString("Covariates").toLower().toStdString();
    const std::string TableDiet                        = QString::fromStdString("Diet").toLower().toStdString();
    const std::string TableFleets                      = QString::fromStdString("Fleets").toLower().toStdString();
    const std::string TableInitialAbundance            = QString::fromStdString("InitialAbundance").toLower().toStdString();
    const std::string TableMaturity                    = QString::fromStdString("Maturity").toLower().toStdString();
    const std::string TableMortalityFishing            = QString::fromStdString("MortalityFishing").toLower().toStdString();
    const std::string TableMortalityNatural            = QString::fromStdString("MortalityNatural").toLower().toStdString();
    const std::string TablePredatorPreyInteractions    = QString::fromStdString("PredatorPreyInteractions").toLower().toStdString();
    const std::string TablePredatorPreyInteractionsVec = QString::fromStdString("PredatorPreyInteractionsVec").toLower().toStdString();
    const std::string TablePredatorPreyPreferredRatio  = QString::fromStdString("PredatorPreyPreferredRatio").toLower().toStdString();
    const std::string TablePredatorPreyVarianceGTRatio = QString::fromStdString("PredatorPreyVarianceGTRatio").toLower().toStdString();
    const std::string TablePredatorPreyVarianceLTRatio = QString::fromStdString("PredatorPreyVarianceLTRatio").toLower().toStdString();
    const std::string TablePredatorPreyVulnerability   = QString::fromStdString("PredatorPreyVulnerability").toLower().toStdString();
    const std::string TableSimulationParametersSpecies = QString::fromStdString("SimulationParametersSpecies").toLower().toStdString();
    const std::string TableSimulationParametersYearly  = QString::fromStdString("SimulationParametersYearly").toLower().toStdString();
    const std::string TableSpecies                     = QString::fromStdString("Species").toLower().toStdString();
    const std::string TableSurveyMonth                 = QString::fromStdString("SurveyMonth").toLower().toStdString();
    const std::string TableSurveyWeights               = QString::fromStdString("SurveyWeights").toLower().toStdString();
    const std::string TableModels                      = QString::fromStdString("Models").toLower().toStdString();
    const std::string TableWeight                      = QString::fromStdString("Weight").toLower().toStdString();


     const std::vector<std::string> ColLabelsEstimations = {
         "aAge1ph", // aAge1 estimation phase
         "aFtph",   // aFt estimation phase
         "dAge1ph", // dAge1 estimation phase
         "dFtph",   // dFt estimation phase
         "ficph",   // Survey selectivity, FICsel, estimation phase
         "fishph",  // Fishery selectivity, agesel, estimation phase
         "Yr1ph"    // Yr1 estimation phase
//       "Rhoph"    // Phase in which each rho is estimated
     };

     const std::vector<std::string> ColLabelsWeightings = {
         "TCwt",    // Total annual commercial catch in weight
         "CPwt",    // Commercial catch proportions at age
         "Bwt",     // Weight for Biomass Penalty Term, Bpen
         "Ywt",     // Weight for Yr1 Penalty Term, Ypen)
         "Rwt",     // Weight for Recruitment Penalty Term, Rpen
//       "FHwt",    // Food habits proportions by weight
         "Bthres",  // Biomass threshold used in the penalty function to avoid B == 0, which would cause M2 calc to crash
         "Rthres"   // Threshold for the coefficient of variation of recruitment
     };

}


