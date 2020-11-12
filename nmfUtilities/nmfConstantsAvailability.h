
/**
 * @file nmfConstantsAvailability.h
 * @brief Definition for constants used in Availability tool
 * @date Jan 14, 2020
 *
 * This file defines constants used in the Availability application program.
 */

#ifndef NMFCONSTANTSAvailability_H
#define NMFCONSTANTSAvailability_H

#include <iostream>
#include <vector>

namespace nmfConstantsAvailability
{
     const bool ResetPositionAlso      = true;
     const bool DontResetPosition      = false;

     const std::string  HiddenDir                      = ".Availability";
     const std::string  HiddenDataDir                  = ".Availability/data";
     const std::string  HiddenLogDir                   = ".Availability/logs";
     const std::string  AvailabilityProgressBarFile           = ".Availability/AvailabilityProgress.dat";
     const std::string  AvailabilityProgressChartFile         = ".Availability/AvailabilityProgressChart.dat";
     const std::string  AvailabilityProgressChartLabelFile    = ".Availability/AvailabilityProgressChartLabel.dat";
     const std::string  AvailabilityStopRunFile               = ".Availability/AvailabilityStopRun.dat";
     const std::string  AvailabilityCurrentLoopFile           = ".Availability/AvailabilityCurrentLoop.dat";
     const std::string  SettingsDirWindows             = "C:\\.QtSettings";
     const std::string  LogDir                         = ".Availability/logs";
     const std::string  LogFilter                      = ".Availability/logs/*.log";
     const std::string  InputDataDir                   = "inputData";
     const std::string  OutputDataDir                  = "outputData";
     const std::string  OutputImagesDir                = "outputImages";

}


#endif
