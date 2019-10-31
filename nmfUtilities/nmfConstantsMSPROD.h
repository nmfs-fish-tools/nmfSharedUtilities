
/**
 @file nmfConstants.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Dec 8, 2016
*/

#ifndef NMF_CONSTANTSMSPROD
#define EXTERN
#else
#define EXTERN extern
#endif

#include <iostream>
#include <vector>

namespace nmfConstants
{

    EXTERN const int    Column_SpeId          = 0;
    EXTERN const int    Column_SpeName        = 1;
    EXTERN const int    Column_GuildName      = 2;
    EXTERN const int    Column_InitBiomass    = 3;
    EXTERN const int    Column_SurveyQ        = 4;
    EXTERN const int    Column_SpeciesK       = 5;
    EXTERN const int    Column_SpeciesKCovarCoeff   = 6;
    EXTERN const int    Column_GrowthRate           = 7;
    EXTERN const int    Column_GrowthRateCovarCoeff = 8;
    EXTERN const int    Column_Habitat        = 9;
    EXTERN const int    Column_InitBiomassMin = 10;
    EXTERN const int    Column_InitBiomassMax = 11;
    EXTERN const int    Column_SurveyQMin     = 12;
    EXTERN const int    Column_SurveyQMax     = 13;
    EXTERN const int    Column_GrowthRateMin  = 14;
    EXTERN const int    Column_GrowthRateMax  = 15;
    EXTERN const int    Column_SpeciesKMin    = 16;
    EXTERN const int    Column_SpeciesKMax    = 17;

    EXTERN const bool   RotateLabels         = true;
    EXTERN const bool   DontRotateLabels     = false;
    EXTERN const int    MaxNumberLogFiles    =  50;
    EXTERN const int    MinSliderValue       =   0;
    EXTERN const int    MaxSliderValue       =  99;
    EXTERN const int    MaxNumberSeasons     =   6;
    EXTERN const int    MaxNumberYears       = 100;
    EXTERN const int    MaxNumberSpecies     =  12;
    EXTERN const int    MaxTotalSpecies      =  50;
    EXTERN const bool   RearrangeTitle       = true;
    EXTERN const bool   DontRearrangeTitle   = false;
    EXTERN const bool   Clear                = true;
    EXTERN const bool   NoClear              = false;
    EXTERN const bool   ShowError            = true;
    EXTERN const bool   DontShowError        = false;

    EXTERN const int    Normal    = 0;
    EXTERN const int    Bold      = 1;
    EXTERN const int    Warning   = 2;
    EXTERN const int    Error     = 3;
    EXTERN const int    Highlight = 4;
    EXTERN const int    Section   = 5;

    EXTERN const std::string  ClearWindow = "";
    EXTERN const std::string  BlankLine = " ";
    EXTERN const std::string  HiddenDir                      = ".MSPROD";
    EXTERN const std::string  HiddenDataDir                  = ".MSPROD/data";
    EXTERN const std::string  HiddenLogDir                   = ".MSPROD/logs";
    EXTERN const std::string  MSVPAProgressBarFile           = ".MSPROD/MSVPAProgress.dat";
    EXTERN const std::string  MSVPAProgressChartFile         = ".MSPROD/MSVPAProgressChart.dat";
    EXTERN const std::string  MSVPAProgressChartLabelFile    = ".MSPROD/MSVPAProgressChartLabel.dat";
    EXTERN const std::string  MSPRODStopRunFile              = ".MSPROD/MSVPAStopRun.dat";
    EXTERN const std::string  MSPRODCurrentLoopFile          = ".MSPROD/MSVPACurrentLoop.dat";
    EXTERN const std::string  MSSPMStopRunFile               = ".MSSPM/MSSPMStopRun.dat";
    EXTERN const std::string  LogDir                         = ".MSPROD/logs";
    EXTERN const std::string  LogFilter                      = ".MSPROD/logs/*.log";
    EXTERN const std::string  InputDataDir                   = "inputData";
    EXTERN const std::string  OutputDataDir                  = "outputData";
    EXTERN const std::string  OutputImagesDir                = "outputImages";

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


//#endif // NMFCONSTANTSMSSPM_H
