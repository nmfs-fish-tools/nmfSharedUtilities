/**
 * @file nmfConstantsMSVPA.h
 * @brief Definition for constants used in MSVPA_X2 tool
 * @date Jan 14, 2020
 *
 * This file defines constants used in the MSVPA_X2 application program.
 */

#ifndef NMFCONSTANTSMSVPA_H
#define NMFCONSTANTSMSVPA_H

#include <iostream>
#include <vector>

namespace nmfConstantsMSVPA
{
    const int          MaxMSVPALoops          =  210; // This is 30 loops * (8-1) update messages per each loop = 210; (8-1 because there are 8 updates per loop but you must always have one, so need 7 add'l)
    const int          MaxLoopsBeforeChecking =   20; // Number of MSVPA loops to iterate before asking user if they still want to continue if no convergence
    const int          NumMSVPASetupSteps     =   30;
    const int          NumForecastSetupSteps  =   16;
    const int          NumModelTypes          =    3; // SSVPA, MSVPA, Forecast
    const int          MSVPAFullSpecies       =    0;
    const int          MSVPAPreyOnlySpecies   =    1;
    const int          MSVPABiomassPredator   =    3;
    const int          SetupLevel             =    0;
    const int          SSVPALevel             =    1;
    const int          MSVPALevel             =    2;
    const int          ForecastLevel          =    3;
    const int          NumberOfMSVPAPages     =   12;

    // RSK change this logic, should really be a map by name and
    // not an array since the species indeces don't go un numeric order
    const int          MaxNumSmodels = 200;

    const std::string  SettingsDirWindows             = "C:\\.QtSettings";
    const std::string  HiddenDir                      = ".MSVPA_X2";
    const std::string  HiddenDataDir                  = ".MSVPA_X2/data";
    const std::string  HiddenLogDir                   = ".MSVPA_X2/logs";
    const std::string  MSVPAProgressBarFile           = ".MSVPA_X2/MSVPAProgress.dat";
    const std::string  MSVPAProgressChartFile         = ".MSVPA_X2/MSVPAProgressChart.dat";
    const std::string  MSVPAProgressChartLabelFile    = ".MSVPA_X2/MSVPAProgressChartLabel.dat";
    const std::string  MSVPAStopRunFile               = ".MSVPA_X2/MSVPAStopRun.dat";
    const std::string  MSVPACurrentLoopFile           = ".MSVPA_X2/MSVPACurrentLoop.dat";
    const std::string  ForecastProgressBarFile        = ".MSVPA_X2/ForecastProgress.dat";
    const std::string  ForecastProgressChartFile      = ".MSVPA_X2/ForecastProgressChart.dat";
    const std::string  ForecastProgressChartLabelFile = ".MSVPA_X2/ForecastProgressChartLabel.dat";
    const std::string  ForecastStopRunFile            = ".MSVPA_X2/ForecastStopRun.dat";
    const std::string  LogDir                         = ".MSVPA_X2/logs";
    const std::string  LogFilter                      = ".MSVPA_X2/logs/*.log";
    const std::string  TableNamesFile                 = ".MSVPA_X2/data/TableNames.dat";
    const std::string  TableDescSSVPAFile             = ".MSVPA_X2/data/TableDescriptionsSSVPA.dat";
    const std::string  TableDescMSVPAFile             = ".MSVPA_X2/data/TableDescriptionsMSVPA.dat";
    const std::string  TableDescForecastFile          = ".MSVPA_X2/data/TableDescriptionsForecast.dat";
    const std::string  ProgramDataDir                 = ".MSVPA_X2/data";
    const std::string  InputDataDir                   = "inputData";
    const std::string  OutputDataDir                  = "outputData";
    const std::string  OutputImagesDir                = "outputImages";

     const std::vector<std::string> AllTables = {
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

     const std::vector<std::string> AllNonSpeciesTables = {
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

     const std::vector<std::string> SSVPANonSpeciesTables = {
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

     const std::vector<std::string> SSVPATables = {
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

     const std::vector<std::string> MSVPATables = {
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

     const std::vector<std::string> ForecastTables = {
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

     const std::vector<std::string> DefaultVPATypes = {
             "Cohort Analysis/SVPA",
             "Separable VPA",
             "Laurec-Shepard Tuned VPA",
             "Extended Survivors Analysis"};
}


#endif
