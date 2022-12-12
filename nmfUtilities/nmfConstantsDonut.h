
/**
 * @file nmfConstantsDonut.h
 * @brief Definition for constants used in Donut tool
 * @date Jan 14, 2020
 *
 * This file defines constants used in the Donut application program.
 */

#ifndef NMFCONSTANTSDonut_H
#define NMFCONSTANTSDonut_H

#include <iostream>
#include <vector>

/**
 * @brief Constants used in the Donut tool
 */
namespace nmfConstantsDonut
{
     const bool ResetPositionAlso      = true;
     const bool DontResetPosition      = false;
     const bool DataArePercentages     = true;
     const bool DataAreNotPercentages  = false;

     const int COLUMN_SPECIES          =  0;
     const int COLUMN_REL_ABUND        =  1;
     const int COLUMN_OVERLAP          =  2;
     const int COLUMN_RANK_DETECTION   =  3;
     const int COLUMN_RANK_ICING       =  7;
     const int COLUMN_PROP_DETECTION   =  8;
     const int COLUMN_PROP_PRODUCT     = 13;
     const int COLUMN_RPA_MODEL        = 14;
     const int COLUMN_NULL_AMBIENT     = 15;
     const int COLUMN_NULL_SELECTIVITY = 16;

     const std::string  HiddenDir                      = ".Donut";
     const std::string  HiddenDataDir                  = ".Donut/data";
     const std::string  HiddenLogDir                   = ".Donut/logs";
     const std::string  DonutProgressBarFile           = ".Donut/DonutProgress.dat";
     const std::string  DonutProgressChartFile         = ".Donut/DonutProgressChart.dat";
     const std::string  DonutProgressChartLabelFile    = ".Donut/DonutProgressChartLabel.dat";
     const std::string  DonutStopRunFile               = ".Donut/DonutStopRun.dat";
     const std::string  DonutCurrentLoopFile           = ".Donut/DonutCurrentLoop.dat";
     const std::string  SettingsDirWindows             = "C:\\.QtSettings";
     const std::string  LogDir                         = ".Donut/logs";
     const std::string  LogFilter                      = ".Donut/logs/*.log";
     const std::string  InputDataDir                   = "inputData";
     const std::string  OutputDataDir                  = "outputData";
     const std::string  OutputImagesDir                = "outputImages";

}


#endif
