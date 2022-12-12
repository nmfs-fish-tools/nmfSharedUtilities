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
#include <QString>

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

    const std::string TableApplication          = QString::fromStdString("Application").toLower().toStdString();
    const std::string TableForeEnergyDens       = QString::fromStdString("ForeEnergyDens").toLower().toStdString();
    const std::string TableForeOutput           = QString::fromStdString("ForeOutput").toLower().toStdString();
    const std::string TableForePredGrowth       = QString::fromStdString("ForePredGrowth").toLower().toStdString();
    const std::string TableForePredVonB         = QString::fromStdString("ForePredVonB").toLower().toStdString();
    const std::string TableForeSRQ              = QString::fromStdString("ForeSRQ").toLower().toStdString();
    const std::string TableForeSRR              = QString::fromStdString("ForeSRR").toLower().toStdString();
    const std::string TableForeSuitPreyBiomass  = QString::fromStdString("ForeSuitPreyBiomass").toLower().toStdString();
    const std::string TableForecasts            = QString::fromStdString("Forecasts").toLower().toStdString();
    const std::string TableMSVPAEnergyDens      = QString::fromStdString("MSVPAEnergyDens").toLower().toStdString();
    const std::string TableMSVPAOthPrey         = QString::fromStdString("MSVPAOthPrey").toLower().toStdString();
    const std::string TableMSVPAOthPreyAnn      = QString::fromStdString("MSVPAOthPreyAnn").toLower().toStdString();
    const std::string TableMSVPASeasBiomass     = QString::fromStdString("MSVPASeasBiomass").toLower().toStdString();
    const std::string TableMSVPASeasInfo        = QString::fromStdString("MSVPASeasInfo").toLower().toStdString();
    const std::string TableMSVPASizePref        = QString::fromStdString("MSVPASizePref").toLower().toStdString();
    const std::string TableMSVPASpaceO          = QString::fromStdString("MSVPASpaceO").toLower().toStdString();
    const std::string TableMSVPAStomCont        = QString::fromStdString("MSVPAStomCont").toLower().toStdString();
    const std::string TableMSVPASuitPreyBiomass = QString::fromStdString("MSVPASuitPreyBiomass").toLower().toStdString();
    const std::string TableMSVPAlist            = QString::fromStdString("MSVPAlist").toLower().toStdString();
    const std::string TableMSVPAprefs           = QString::fromStdString("MSVPAprefs").toLower().toStdString();
    const std::string TableMSVPAspecies         = QString::fromStdString("MSVPAspecies").toLower().toStdString();
    const std::string TableOthPredSizeData      = QString::fromStdString("OthPredSizeData").toLower().toStdString();
    const std::string TableOtherPredBM          = QString::fromStdString("OtherPredBM").toLower().toStdString();
    const std::string TableOtherPredSpecies     = QString::fromStdString("OtherPredSpecies").toLower().toStdString();
    const std::string TableSSVPAAgeM            = QString::fromStdString("SSVPAAgeM").toLower().toStdString();
    const std::string TableScenarioF            = QString::fromStdString("ScenarioF").toLower().toStdString();
    const std::string TableScenarioOthPred      = QString::fromStdString("ScenarioOthPred").toLower().toStdString();
    const std::string TableScenarioOthPrey      = QString::fromStdString("ScenarioOthPrey").toLower().toStdString();
    const std::string TableScenarioRec          = QString::fromStdString("ScenarioRec").toLower().toStdString();
    const std::string TableScenarios            = QString::fromStdString("Scenarios").toLower().toStdString();
    const std::string TableSpeCatch             = QString::fromStdString("SpeCatch").toLower().toStdString();
    const std::string TableSpeMaturity          = QString::fromStdString("SpeMaturity").toLower().toStdString();
    const std::string TableSpeSSVPA             = QString::fromStdString("SpeSSVPA").toLower().toStdString();
    const std::string TableSpeSize              = QString::fromStdString("SpeSize").toLower().toStdString();
    const std::string TableSpeTuneCatch         = QString::fromStdString("SpeTuneCatch").toLower().toStdString();
    const std::string TableSpeTuneEffort        = QString::fromStdString("SpeTuneEffort").toLower().toStdString();
    const std::string TableSpeWeight            = QString::fromStdString("SpeWeight").toLower().toStdString();
    const std::string TableSpeXSAData           = QString::fromStdString("SpeXSAData").toLower().toStdString();
    const std::string TableSpeXSAIndices        = QString::fromStdString("SpeXSAIndices").toLower().toStdString();
    const std::string TableSpecies              = QString::fromStdString("Species").toLower().toStdString();

     const std::vector<std::string> AllTables = {
         TableApplication,
         TableForeEnergyDens,
         TableForeOutput,
         TableForePredGrowth,
         TableForePredVonB,
         TableForeSRQ,
         TableForeSRR,
         TableForeSuitPreyBiomass,
         TableForecasts,
         TableMSVPAEnergyDens,
         TableMSVPAOthPrey,
         TableMSVPAOthPreyAnn,
         TableMSVPASeasBiomass,
         TableMSVPASeasInfo,
         TableMSVPASizePref,
         TableMSVPASpaceO,
         TableMSVPAStomCont,
         TableMSVPASuitPreyBiomass,
         TableMSVPAlist,
         TableMSVPAprefs,
         TableMSVPAspecies,
         TableOthPredSizeData,
         TableOtherPredBM,
         TableOtherPredSpecies,
         TableSSVPAAgeM,
         TableScenarioF,
         TableScenarioOthPred,
         TableScenarioOthPrey,
         TableScenarioRec,
         TableScenarios,
         TableSpeCatch,
         TableSpeMaturity,
         TableSpeSSVPA,
         TableSpeSize,
         TableSpeTuneCatch,
         TableSpeTuneEffort,
         TableSpeWeight,
         TableSpeXSAData,
         TableSpeXSAIndices,
         TableSpecies
     };

     const std::vector<std::string> AllNonSpeciesTables = {
         TableApplication,
         TableForeEnergyDens,
         TableForeOutput,
         TableForePredGrowth,
         TableForePredVonB,
         TableForeSRQ,
         TableForeSRR,
         TableForeSuitPreyBiomass,
         TableForecasts,
         TableMSVPAEnergyDens,
         TableMSVPAOthPrey,
         TableMSVPAOthPreyAnn,
         TableMSVPASeasBiomass,
         TableMSVPASeasInfo,
         TableMSVPASizePref,
         TableMSVPASpaceO,
         TableMSVPAStomCont,
         TableMSVPASuitPreyBiomass,
         TableMSVPAlist,
         TableMSVPAprefs,
         TableMSVPAspecies,
         TableOthPredSizeData,
         TableOtherPredBM,
         TableSSVPAAgeM,
         TableScenarioF,
         TableScenarioOthPred,
         TableScenarioOthPrey,
         TableScenarioRec,
         TableScenarios,
         TableSpeCatch,
         TableSpeMaturity,
         TableSpeSSVPA,
         TableSpeSize,
         TableSpeTuneCatch,
         TableSpeTuneEffort,
         TableSpeWeight,
         TableSpeXSAData,
         TableSpeXSAIndices
     };

     const std::vector<std::string> SSVPANonSpeciesTables = {
         TableOthPredSizeData,
         TableOtherPredBM,
         TableSSVPAAgeM,
         TableSpeCatch,
         TableSpeMaturity,
         TableSpeSSVPA,
         TableSpeSize,
         TableSpeTuneCatch,
         TableSpeTuneEffort,
         TableSpeWeight,
         TableSpeXSAData,
         TableSpeXSAIndices
     };

     const std::vector<std::string> SSVPATables = {
         TableOthPredSizeData,
         TableOtherPredBM,
         TableOtherPredSpecies,
         TableSSVPAAgeM,
         TableSpeCatch,
         TableSpeMaturity,
         TableSpeSSVPA,
         TableSpeSize,
         TableSpeTuneCatch,
         TableSpeTuneEffort,
         TableSpeWeight,
         TableSpeXSAData,
         TableSpeXSAIndices,
         TableSpecies
     };

     const std::vector<std::string> MSVPATables = {
         TableMSVPAEnergyDens,
         TableMSVPAOthPrey,
         TableMSVPAOthPreyAnn,
         TableMSVPASeasBiomass,
         TableMSVPASeasInfo,
         TableMSVPASizePref,
         TableMSVPASpaceO,
         TableMSVPAStomCont,
         TableMSVPASuitPreyBiomass,
         TableMSVPAlist,
         TableMSVPAprefs,
         TableMSVPAspecies
     };

     const std::vector<std::string> ForecastTables = {
         TableForecasts,
         TableForeEnergyDens,
         TableForeOutput,
         TableForePredGrowth,
         TableForeSuitPreyBiomass,
         TableForeSRR,
         TableForeSRQ,
         TableForePredVonB,
         TableScenarios,
         TableScenarioF,
         TableScenarioOthPred,
         TableScenarioOthPrey,
         TableScenarioRec
     };

     const std::vector<std::string> DefaultVPATypes = {
             "Cohort Analysis/SVPA",
             "Separable VPA",
             "Laurec-Shepard Tuned VPA",
             "Extended Survivors Analysis"};
}


#endif
