
/**
 * @file nmfConstantsAvailability.h
 * @brief Definition for constants used in Availability tool
 * @date Jan 14, 2020
 *
 * This file defines constants used in the Availability application program.
 */

#pragma once

#include <iostream>
#include <vector>
#include <QStringList>

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
    const QString EndSection_pPrey   = "## Ontogenetic diet";

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

