
/**
 * @file nmfConstantsAvailability.h
 * @brief Definition for constants used in Availability tool
 * @date Jan 14, 2020
 *
 * This file defines constants used in the Availability application program.
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

#ifndef NMF_CONSTANTS_AVAILABILITY
#define NMF_CONSTANTS_AVAILABILITY

#include <iostream>
#include <vector>
#include <QStringList>

/**
 * @brief Constants used in the Availability tool
 */
namespace nmfConstantsAvailability
{
    /**
     * @brief Holds all of the data corresponding to a predator species
     */
    struct PredatorStruct {
        QString growthRate;
        QString initialBiomass;
        QString efficiency;
        QString clearanceRate;
        QString exponent;
        QString ageStructure;
        QString functionalResponse;
    };

    /**
     * @brief Holds all of the data corresponding to a prey only species
     */
    struct PreyOnlyStruct {
        QString initialBiomass;
    };

    const bool ResetPositionAlso      = true;
    const bool DontResetPosition      = false;
    const bool DataArePercentages     = true;
    const bool DataAreNotPercentages  = false;

    const int Predator_Column_Name                = 0;
    const int Predator_Column_GrowthRate          = 1;
    const int Predator_Column_InitialBiomass      = 2;
    const int Predator_Column_Efficiency          = 3;
    const int Predator_Column_ClearanceRate       = 4;
    const int Predator_Column_Exponent            = 5;
    const int Predator_Column_AgeStructure        = 6;
    const int Predator_Column_FunctionalResponse  = 7;

    const int PreyOnly_Column_Name                = 0;
    const int PreyOnly_Column_InitialBiomass      = 1;

    const int DietComposition_Column_Name         = 0;
    const int DietComposition_Column_PredatorType = 1;
    const int DietComposition_Column_PreyType     = 2;
    const int DietComposition_Column_Data         = 3;

    const double MinExponent                      = 1.0;
    const double MaxExponent                      = 2.0;

    const QString BeginSection_pPrey = "# The order of the prey groups is defined by your functional ";
    const QString EndSection_pPrey   = "## Ontogenetic diet preferences";

    const std::string  HiddenDir                      = ".Atlantis_Availability";
    const std::string  HiddenDataDir                  = ".Atlantis_Availability/data";
    const std::string  HiddenLogDir                   = ".Atlantis_Availability/logs";
    const std::string  AvailabilityProgressBarFile        = ".Atlantis_Availability/AvailabilityProgress.dat";
    const std::string  AvailabilityProgressChartFile      = ".Atlantis_Availability/AvailabilityProgressChart.dat";
    const std::string  AvailabilityProgressChartLabelFile = ".Atlantis_Availability/AvailabilityProgressChartLabel.dat";
    const std::string  AvailabilityStopRunFile            = ".Atlantis_Availability/AvailabilityStopRun.dat";
    const std::string  AvailabilityCurrentLoopFile        = ".Atlantis_Availability/AvailabilityCurrentLoop.dat";
    const std::string  SettingsDirWindows             = "C:\\.QtSettings";
    const std::string  LogDir                         = ".Atlantis_Availability/logs";
    const std::string  LogFilter                      = ".Atlantis_Availability/logs/*.log";
    const std::string  InputDataDir                   = "inputData";
    const std::string  OutputDataDir                  = "outputData";
    const std::string  OutputImagesDir                = "outputImages";
    const std::string  PredatorFile                   = "Predators.csv";
    const std::string  PreyOnlyFile                   = "PreyOnly.csv";
    const std::string  AllSpeciesFile                 = "AllSpecies.csv";
    const std::string  DietCompositionFile            = "DietComposition.csv";
    const std::string  AvailabilityFile               = "Availability.csv";
    const QStringList AgeStructureValues = {
        "Juvenile Only","Adult Only","All Ages"};
    const QStringList FunctionalResponseValues = {
        "Standard Holling Type I",
        "Standard Holling Type II",
        "Standard Holling Type II½",
        "Standard Holling Type III",
        "Modified Holling Type II",
        "Modified Holling Type II½",
        "Modified Holling Type III"};
}

#endif
