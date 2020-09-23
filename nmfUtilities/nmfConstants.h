/**
 * @file nmfConstants.h
 * @brief Definition for common constants used in MultiSpecies tools
 * @date Dec 8, 2016
 *
 * This file contains constant declarations used in the MultiSpecies tools using
 * the nmfConstants namespace.
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
    EXTERN const int    NumCategoriesForVerticalNotation = 10;
    EXTERN const int    NumFeedingDataRows      =  7;
    EXTERN const int    MinNumForecastYears     =  3;
    EXTERN const int    MaxNumForecastYears     = 20;
    EXTERN const int    MaxPathLength           = 1000;
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
    EXTERN const bool   Checked                 = true;
    EXTERN const bool   NotChecked              = false;
    EXTERN const bool   ShowShadow              = true;
    EXTERN const bool   DontShowShadow          = false;
    EXTERN const bool   ReverseAxis             = true;
    EXTERN const bool   DontReverseAxis         = false;
    EXTERN const bool   ShowLegend              = true;
    EXTERN const bool   DontShowLegend          = false;

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
    EXTERN const double MaxBiomass      = 1e12;

    EXTERN const int    Normal    = 0;
    EXTERN const int    Bold      = 1;
    EXTERN const int    Warning   = 2;
    EXTERN const int    Error     = 3;
    EXTERN const int    Highlight = 4;
    EXTERN const int    Section   = 5;

    EXTERN const std::string  DeepSkyBlue = "#00BFFF";
    EXTERN const std::string  Blue        = "#0072B2";
    EXTERN const std::string  Orange      = "#E69F00";
    EXTERN const std::string  Black       = "#000000";
    EXTERN const std::string  SkyBlue     = "#56B4E9";
    EXTERN const std::string  Gray        = "#C8C8C8";
    EXTERN const std::string  Red         = "#FF0000";
    EXTERN const std::vector<std::string> LineColors     = {Blue,Orange,Black,SkyBlue,Gray,Red};
    EXTERN const std::vector<std::string> LineColorNames = {"Blue","Orange","Black","Sky Blue","Gray","Red"};

    EXTERN const std::string  ClearWindow = "";
    EXTERN const std::string  BlankLine = " ";

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

