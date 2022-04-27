/**
 * @file nmfGrowthForm.h
 * @brief Definition for the Growth Form of the model
 * @date Nov 24, 2021
 *
 * This file contains the class definition for the Growth Form of the user
 * defined model.
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
#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <functional>

#include "nmfUtils.h"
#include "nmfConstantsMSSPM.h"

/**
 * @brief This class contains the logic to calculate the Growth term of the currently selected model
 */
class nmfGrowthForm {

private:
    bool m_isAGGPROD;
    int m_NumParameters;
    int m_NumSpecies;
    std::string m_Prefix;
    std::string m_Type; // "Null","Linear","Logistic"
    std::vector<std::pair<double,double> > m_ParameterRanges;
    std::map<std::string,std::string> m_GrowthMap;
    std::map<std::string,std::string> m_GrowthKey;

    std::map<std::string, double(nmfGrowthForm::*)(
            const std::string& CovariateAlgorithmType,
            const double& BiomassAtTime,
            const double& EstGrowthRate,
            const double& EstGrowthRateCovariateCoeff,
            const double& GrowthRateCovariate,
            const double& EstCarryingCapacity,
            const double& EstCarryingCapacityCovariateCoeff,
            const double& CarryingCapacityCovariate
    )> FunctionMap;

    void setupFormMaps();

public:
    /**
     * @brief Constructor method for class
     * @param type : type of growth algorithm to be used
     */
    nmfGrowthForm(std::string type);
   ~nmfGrowthForm() {};

    /**
     * @brief This method allows the appropriate function map function to be called such that a series of if...else...
     * statements are not needed
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param BiomassAtTime : biomass at the currrent time
     * @param EstGrowthRate : estimated growth rate value
     * @param EstGrowthRateCovariateCoeff : estimated growth rate covariate coefficient value
     * @param GrowthRateCovariate : growth rate covariate value
     * @param EstCarryingCapacity : estimated carrying capacity values
     * @param EstCarryingCapacityCovariateCoeff : estimated carrying capacity covariate coefficient value
     * @param CarryingCapacityCovariate : carrying capacity covariate value
     * @return Returns the growth value evaluated with the appropriate function map algorithm
     */
    double evaluate(const std::string& CovariateAlgorithmType,
                    const double& BiomassAtTime,
                    const double& EstGrowthRate,
                    const double& EstGrowthRateCovariateCoeff,
                    const double& GrowthRateCovariate,
                    const double& EstCarryingCapacity,
                    const double& EstCarryingCapacityCovariateCoeff,
                    const double& CarryingCapacityCovariate);
    /**
     * @brief Unpacks the parameters from the Parameters list and loads them into their respective data structures
     * @param Parameters : vector of all Growth parameter values
     * @param StartPos : start position from which to extract parameters
     * @param GrowthRate : growth rate parameters
     * @param GrowthRateCovariateCoeff : growth rate covariate coefficient parameters
     * @param CarryingCapacity : carrying capacity parameters
     * @param CarryingCapacityCovariateCoeff : carrying capacity covariate coefficient parameters
     */
    void extractParameters(
            const std::vector<double>& Parameters,
            int& StartPos,
            std::vector<double>& GrowthRate,
            std::vector<double>& GrowthRateCovariateCoeff,
            std::vector<double>& CarryingCapacity,
            std::vector<double>& CarryingCapacityCovariateCoeff,
            double& systemCarryingCapacity);
    /**
     * @brief This function implements the Linear growth rate function
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param BiomassAtTime : biomass at the currrent time
     * @param EstGrowthRate : estimated growth rate value
     * @param EstGrowthRateCovariateCoeff : estimated growth rate covariate coefficient value
     * @param GrowthRateCovariate : growth rate covariate values
     * @param EstCarryingCapacity : estimated carrying capacity value
     * @param EstCarryingCapacityCovariateCoeff : estimated carrying capacity covariate coefficient value
     * @param CarryingCapacityCovariate : carrying capacity covariate value
     * @return Returns the Linear growth rate function
     */
    double FunctionMap_Linear(const std::string& CovariateAlgorithmType,
                              const double& BiomassAtTime,
                              const double& EstGrowthRate,
                              const double& EstGrowthRateCovariateCoeff,
                              const double& GrowthRateCovariate,
                              const double& EstCarryingCapacity,
                              const double& EstCarryingCapacityCovariateCoeff,
                              const double& CarryingCapacityCovariate);
    /**
     * @brief This function implements the Logistic growth rate function
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param BiomassAtTime : biomass at the currrent time
     * @param EstGrowthRate : estimated growth rate value
     * @param EstGrowthRateCovariateCoeff : estimated growth rate covariate coefficient value
     * @param GrowthRateCovariate : growth rate covariate value
     * @param EstCarryingCapacity : estimated carrying capacity values
     * @param EstCarryingCapacityCovariateCoeffs : estimated carrying capacity covariate coefficient values
     * @param CarryingCapacityCovariates : carrying capacity covariate values
     * @return Returns the Logistic growth rate function
     */
    double FunctionMap_Logistic(const std::string& CovariateAlgorithmType,
                                const double& BiomassAtTime,
                                const double& EstGrowthRate,
                                const double& EstGrowthRateCovariateCoeff,
                                const double& GrowthRateCovariate,
                                const double& EstCarryingCapacity,
                                const double& EstCarryingCapacityCovariateCoeffs,
                                const double& CarryingCapacityCovariates);
    /**
     * @brief This function implements the Null growth rate function (i.e., 0)
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param BiomassAtTime : biomass at the currrent time
     * @param EstGrowthRate : estimated growth rate value
     * @param EstGrowthRateCovariateCoeff : estimated growth rate covariate coefficient value
     * @param GrowthRateCovariate : growth rate covariate value
     * @param EstCarryingCapacity : estimated carrying capacity values
     * @param EstCarryingCapacityCovariateCoeffs : estimated carrying capacity covariate coefficient values
     * @param CarryingCapacityCovariates : carrying capacity covariate values
     * @return Returns 0
     */
    double FunctionMap_Null(const std::string& CovariateAlgorithmType,
                            const double& BiomassAtTime,
                            const double& EstGrowthRate,
                            const double& EstGrowthRateCovariateCoeff,
                            const double& GrowthRateCovariate,
                            const double& EstCarryingCapacity,
                            const double& EstCarryingCapacityCovariateCoeffs,
                            const double& CarryingCapacityCovariates);
    /**
     * @brief Gets the expression for the selected Growth Form algorithm
     * @return Returns a string representing the formula of the selected Growth Form algorithm
     */
    std::string getExpression();
    /**
     * @brief Gets the key (i.e., legend) for the selected Growth Form algorithm variables
     * @return Returns a string representing a description of the variables used in the selected
     * Growth Form algorithm's formula
     */
    std::string getKey();
    /**
     * @brief Gets the beginning part of the growth formula
     * @return Returns the string containing the beginning part of the growth formula
     */
    std::string getPrefix();
    /**
     * @brief Gets the type of growth form selected
     * @return Returns the string representing the type of growth form selected
     */
    std::string getType();
    /**
     * @brief Loads the parameter ranges structure with data from the model data structure
     * @param ParameterRanges : parameter ranges
     * @param DataStruct : model data structure
     */
    void loadParameterRanges(
            std::vector<std::pair<double,double> >& ParameterRanges,
            const nmfStructsQt::ModelDataStruct& DataStruct);
    /**
     * @brief Sets the AggProd variable
     * @param isAggProd : boolean describing the AggProd status (i.e., true/false)
     */
    void setAggProd(bool isAggProd);
    /**
     * @brief Sets the type of growth form algorithm
     * @param newType : type of growth form algorithm
     */
    void setType(std::string newType);
};


