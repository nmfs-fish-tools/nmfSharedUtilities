
/**
 @file nmfConstantsMSSPM.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Dec 8, 2016
*/

#ifndef NMFCONSTANTSMSSPM_H
#define NMFCONSTANTSMSSPM_H

//#define
//#else
//#define  extern
//#endif

#include <iostream>
#include <vector>

namespace nmfConstantsMSSPM
{
     const int    Start_Year            = 1900;

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

     const bool   RotateLabels         = true;
     const bool   DontRotateLabels     = false;
     const int    MaxNumberLogFiles    =  50;
     const int    MinSliderValue       =   0;
     const int    MaxSliderValue       =  99;
     const int    MaxNumberSeasons     =   6;
     const int    MaxNumberYears       = 100;
     const int    MaxNumberSpecies     =  12;
     const int    MaxTotalSpecies      =  50;
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

     const int    Normal    = 0;
     const int    Bold      = 1;
     const int    Warning   = 2;
     const int    Error     = 3;
     const int    Highlight = 4;
     const int    Section   = 5;

     const std::string  ClearWindow = "";
     const std::string  BlankLine = " ";
     const std::string  HiddenDir                      = ".MSSPM";
     const std::string  HiddenDataDir                  = ".MSSPM/data";
     const std::string  HiddenLogDir                   = ".MSSPM/logs";
     const std::string  MSSPMProgressBarFile           = ".MSSPM/MSSPMProgress.dat";
     const std::string  MSSPMProgressChartFile         = ".MSSPM/MSSPMProgressChart.dat";
     const std::string  MSSPMProgressChartLabelFile    = ".MSSPM/MSSPMProgressChartLabel.dat";
     const std::string  MSSPMStopRunFile               = ".MSSPM/MSSPMStopRun.dat";
     const std::string  MSSPMCurrentLoopFile           = ".MSSPM/MSSPMCurrentLoop.dat";
     const QString      SettingsDirWindows             = "C:\\.QtSettings";
     const std::string  LogDir                         = ".MSSPM/logs";
     const std::string  LogFilter                      = ".MSSPM/logs/*.log";
     const std::string  InputDataDir                   = "inputData";
     const std::string  OutputDataDir                  = "outputData";
     const std::string  OutputImagesDir                = "outputImages";

     const std::vector<std::string> SurfaceThemes = {
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


#endif // NMFCONSTANTSMSSPM_H
