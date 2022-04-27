/**
 * @file nmfHarvestForm.h
 * @brief Definition for the Harvest Form of the model
 * @date Nov 24, 2021
 *
 * This file contains the class definition for the Harvest Form of the user
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

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multi_array.hpp>

#include "nmfUtils.h"

/**
 * @brief This class contains the logic to calculate the Harvest term of the currently selected model
 */
class nmfHarvestForm {

private:
    bool m_isAGGPROD;
    int m_NumSpecies;
    std::string m_Type; //  Catch, Effort, ...
    std::vector<std::pair<double,double> > m_ParameterRanges;
    std::map<std::string,std::string> m_HarvestMap;
    std::map<std::string,std::string> m_HarvestKey;

    std::map<std::string,double(nmfHarvestForm::*)(
            const std::string& CovariateAlgorithmType,
            const int& TimeMinus1,
            const int& Species,
            const double& BiomassAtTime,
            const boost::numeric::ublas::matrix<double>& Catch,
            const boost::numeric::ublas::matrix<double>& Effort,
            const boost::numeric::ublas::matrix<double>& Exploitation,
            const double& EstCatchability,
            const double& EstCatchabilityCovariateCoeff,
            const double& CatchabilityCovariate
            )> m_FunctionMap;

    void setupFormMaps();

public:
    /**
     * @brief Constructor method for class
     * @param type : type of harvest algorithm to be used
     */
    nmfHarvestForm(std::string type);
   ~nmfHarvestForm() {};

    /**
     * @brief This method allows the appropriate function map function to be called such that a series of if...else...
     * statements are not needed
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param TimeMinus1 : one minus the current time
     * @param Species : species number
     * @param BiomassAtTime : biomass at the currrent time
     * @param Catch : the Catch matrix (rows are years, columns are species)
     * @param Effort : the Effort matrix (rows are years, columns are species), N.B. Am passing a matrix
     * because sometimes it may be empty and unused so can't pass the indexed value
     * @param Exploitation : the Exploitation matrix (rows are years, columns are species)
     * @param EstCatchability : the estimated catchability parameters
     * @param EstCatchabilityCovariateCoeff : the estimated catchability covariate coefficients
     * @param CatchabilityCovariate : the catchability covariate values
     * @return Returns the harvest value evaluated with the appropriate function map algorithm
     */
    double evaluate(const std::string& CovariateAlgorithmType,
                    const int& TimeMinus1,
                    const int& Species,
                    const double& BiomassAtTime,
                    const boost::numeric::ublas::matrix<double>& Catch,
                    const boost::numeric::ublas::matrix<double>& Effort,
                    const boost::numeric::ublas::matrix<double>& Exploitation,
                    const double& EstCatchability,
                    const double& EstCatchabilityCovariateCoeff,
                    const double& CatchabilityCovariate);
    /**
     * @brief Unpacks the parameters from the Parameters list and loads them into their respective data structures
     * @param Parameters : vector of all Harvest parameter values
     * @param StartPos : start position from which to extract parameters
     * @param Catchability : catchability parameters
     * @param CatchabilityCovariateCoeffs : catchability covariate coefficient parameters
     */
    void extractParameters(
            const std::vector<double>& Parameters,
            int& StartPos,
            std::vector<double>& Catchability,
            std::vector<double>& CatchabilityCovariateCoeffs);
    /**
     * @brief This method implements the Catch harvest algorithm
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param TimeMinus1 : one minus the current time
     * @param Species : species number
     * @param BiomassAtTime : biomass at the currrent time
     * @param Catch : the Catch matrix (rows are years, columns are species)
     * @param Effort : the Effort matrix (rows are years, columns are species), N.B. Am passing a matrix
     * because sometimes it may be empty and unused so can't pass the indexed value
     * @param Exploitation : the Exploitation matrix (rows are years, columns are species)
     * @param EstCatchability : the estimated catchability parameters
     * @param EstCatchabilityCovariateCoeff : the estimated catchability covariate coefficients
     * @param CatchabilityCovariate : the catchability covariate values
     * @return Returns the harvest value evaluated with the Catch algorithm
     */
    double FunctionMap_Catch(const std::string& CovariateAlgorithmType,
                             const int& TimeMinus1,
                             const int& Species,
                             const double& BiomassAtTime,
                             const boost::numeric::ublas::matrix<double>& Catch,
                             const boost::numeric::ublas::matrix<double>& Effort,
                             const boost::numeric::ublas::matrix<double>& Exploitation,
                             const double& EstCatchability,
                             const double& EstCatchabilityCovariateCoeff,
                             const double& CatchabilityCovariate);
    /**
     * @brief This method implements the Effort harvest algorithm
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param TimeMinus1 : one minus the current time
     * @param Species : species number
     * @param BiomassAtTime : biomass at the currrent time
     * @param Catch : the Catch matrix (rows are years, columns are species)
     * @param Effort : the Effort matrix (rows are years, columns are species), N.B. Am passing a matrix
     * because sometimes it may be empty and unused so can't pass the indexed value
     * @param Exploitation : the Exploitation matrix (rows are years, columns are species)
     * @param EstCatchability : the estimated catchability parameters
     * @param EstCatchabilityCovariateCoeff : the estimated catchability covariate coefficients
     * @param CatchabilityCovariate : the catchability covariate values
     * @return Returns the harvest value evaluated with the Effort algorithm
     */
    double FunctionMap_Effort(const std::string& CovariateAlgorithmType,
                              const int& TimeMinus1,
                              const int& Species,
                              const double& BiomassAtTime,
                              const boost::numeric::ublas::matrix<double>& Catch,
                              const boost::numeric::ublas::matrix<double>& Effort,
                              const boost::numeric::ublas::matrix<double>& Exploitation,
                              const double& EstCatchability,
                              const double& EstCatchabilityCovariateCoeff,
                              const double& CatchabilityCovariate);
    /**
     * @brief This method implements the Exploitation harvest algorithm
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param TimeMinus1 : one minus the current time
     * @param Species : species number
     * @param BiomassAtTime : biomass at the currrent time
     * @param Catch : the Catch matrix (rows are years, columns are species)
     * @param Effort : the Effort matrix (rows are years, columns are species), N.B. Am passing a matrix
     * because sometimes it may be empty and unused so can't pass the indexed value
     * @param Exploitation : the Exploitation matrix (rows are years, columns are species)
     * @param EstCatchability : the estimated catchability parameters
     * @param EstCatchabilityCovariateCoeff : the estimated catchability covariate coefficients
     * @param CatchabilityCovariate : the catchability covariate values
     * @return Returns the harvest value evaluated with the Exploitation algorithm
     */
    double FunctionMap_Exploitation(const std::string& CovariateAlgorithmType,
                                    const int& TimeMinus1,
                                    const int& Species,
                                    const double& BiomassAtTime,
                                    const boost::numeric::ublas::matrix<double>& Catch,
                                    const boost::numeric::ublas::matrix<double>& Effort,
                                    const boost::numeric::ublas::matrix<double>& Exploitation,
                                    const double& EstCatchability,
                                    const double& EstCatchabilityCovariateCoeff,
                                    const double& CatchabilityCovariate);
    /**
     * @brief This method implements the Null harvest algorithm (i.e., 0)
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param TimeMinus1 : one minus the current time
     * @param Species : species number
     * @param BiomassAtTime : biomass at the currrent time
     * @param Catch : the Catch matrix (rows are years, columns are species)
     * @param Effort : the Effort matrix (rows are years, columns are species), N.B. Am passing a matrix
     * because sometimes it may be empty and unused so can't pass the indexed value
     * @param Exploitation : the Exploitation matrix (rows are years, columns are species)
     * @param EstCatchability : the estimated catchability parameters
     * @param EstCatchabilityCovariateCoeff : the estimated catchability covariate coefficients
     * @param CatchabilityCovariate : the catchability covariate values
     * @return Returns 0
     */
    double FunctionMap_Null(const std::string& CovariateAlgorithmType,
                            const int& TimeMinus1,
                            const int& Species,
                            const double& BiomassAtTime,
                            const boost::numeric::ublas::matrix<double>& Catch,
                            const boost::numeric::ublas::matrix<double>& Effort,
                            const boost::numeric::ublas::matrix<double>& Exploitation,
                            const double& EstCatchability,
                            const double& EstCatchabilityCovariateCoeff,
                            const double& CatchabilityCovariate);
    /**
     * @brief Gets the expression for the selected Harvest Form algorithm
     * @return Returns a string representing the formula of the selected Harvest Form algorithm
     */
    std::string getExpression();
    /**
     * @brief Gets the key (i.e., legend) for the selected Harvest Form algorithm variables
     * @return Returns a string representing a description of the variables used in the selected
     * Harvest Form algorithm's formula
     */
    std::string getKey();
    /**
     * @brief Loads the parameter ranges structure with data from the model data structure
     * @param ParameterRanges : parameter ranges
     * @param DataStruct : model data structure
     */
    void loadParameterRanges(
                    std::vector<std::pair<double,double> >& ParameterRanges,
                    nmfStructsQt::ModelDataStruct& DataStruct);
    /**
     * @brief Updates the default harvest type to be used for forecasts to whatever
     * the current Forecast Harvest Type Button is set to
     * @param forecastHarvestTypeLabel : current label of the Forecast Harvest type button
     * @return True if harvest type has been correctly set (and harvest model is Effort Fit to Catch);
     * false otherwise
     */
    bool modifyHarvestTypeDueToFitToCatch(
            std::string forecastHarvestTypeLabel);
    /**
     * @brief Sets the AggProd variable
     * @param isAggProd : boolean describing the AggProd status (i.e., true/false)
     */
    void setAggProd(bool isAggProd);
    /**
     * @brief Sets the type of harvest algorithm
     * @param newType : type of harvest algorithm
     */
    void setType(std::string newType);

};

