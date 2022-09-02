/**
 * @file nmfPredationForm.h
 * @brief Definition for the Predation Form of the model
 * @date Nov 24, 2021
 *
 * This file contains the class definition for the Predation Form of the user
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
 * @brief This class contains the logic to calculate the Predation term of the currently selected model
 */
class nmfPredationForm {

private:
    bool m_isAggProd;
    int  m_NumGuilds;
    int  m_NumParameters;
    int  m_NumSpecies;
    int  m_NumSpeciesOrGuilds;
    std::string m_Type; // "Type I", "Type II", ...
    std::vector<std::pair<double,double> > m_ParameterRanges;
    std::map<std::string,std::string> m_PredationMap;
    std::map<std::string,std::string> m_PredationKey;

    std::map<std::string, double(nmfPredationForm::*)(
            const std::string& covariateAlgorithmType,
            const int& TimeMinus1,
            const int& Species,
            const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
            const double& EstimatedBiomassAtTimeMinus1,
            const boost::numeric::ublas::matrix<double>& EstPredationRho,
            const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
            const boost::numeric::ublas::matrix<double>& EstPredationHandling,
            const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
            const std::vector<double>& EstPredationExponent,
            const boost::numeric::ublas::matrix<double>& PredationExponentCovariate
            )> m_FunctionMap;

    void setupFormMaps();

public:
    /**
     * @brief Constructor method for class
     * @param type : type of predation algorithm to be used
     */
    nmfPredationForm(std::string theType);
   ~nmfPredationForm() {};

    /**
     * @brief This method allows the appropriate function map function to be called such that a series of if...else...
     * statements are not needed
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param TimeMinus1 : one minus the current time
     * @param Species : species number
     * @param EstimatedBiomass : matrix of estimated biomass
     * @param EstimatedBiomassAtTimeMinus1 : estimated biomass of current species and time-1
     * @param EstPredationRho : the estimated predation rho parameters
     * @param PredationRhoCovariate : the predation rho covariate values
     * @param EstPredationHandling : the estimated predation handling parameters
     * @param PredationHandlingCovariate : the predation handling covariate values
     * @param EstPredationExponent : the estimated predation exponent parameters
     * @param PredationExponentCovariate : the predation exponent covariate values
     * @return Returns the Predation value evaluated with the appropriate function map algorithm
     */
    double evaluate(const std::string& covariateAlgorithmType,
                    const int& TimeMinus1,
                    const int& Species,
                    const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                    const double& EstimatedBiomassAtTimeMinus1,
                    const boost::numeric::ublas::matrix<double>& EstPredationRho,
                    const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
                    const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                    const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
                    const std::vector<double>& EstPredationExponent,
                    const boost::numeric::ublas::matrix<double>& PredationExponentCovariate);
    /**
     * @brief Unpacks the Exponent parameters from the Parameters list
     * @param Parameters : vector of all Predation parameter values
     * @param StartPos : start position from which to extract parameters
     * @param Exponents : Exponent parameters
     */
    void extractExponentParameters(
            const std::vector<double> &Parameters,
            int& StartPos,
            std::vector<double> &Exponents);
    /**
     * @brief Unpacks the Handling parameters from the Parameters list
     * @param Parameters : vector of all Predation parameter values
     * @param StartPos : start position from which to extract parameters
     * @param Handling : Handling parameters
     */
    void extractHandlingParameters(
            const std::vector<double> &Parameters,
            int& StartPos,
            boost::numeric::ublas::matrix<double> &Handling);
    /**
     * @brief Unpacks all of the Predation parameters from the Parameters list
     * @param Parameters : vector of all Predation parameter values
     * @param StartPos : start position from which to extract parameters
     * @param Predation : all Predation parameters
     */
    void extractPredationParameters(
            const std::vector<double> &Parameters,
            int& StartPos,
            boost::numeric::ublas::matrix<double> &Predation);
    /**
     * @brief Gets the expression for the selected Predation Form algorithm
     * @return Returns a string representing the formula of the selected Predation Form algorithm
     */
    std::string getExpression();
    /**
     * @brief Gets the key (i.e., legend) for the selected Predation Form algorithm variables
     * @return Returns a string representing a description of the variables used in the selected
     * Predation Form algorithm's formula
     */
    std::string getKey();
    /**
     * @brief Gets the number of parameters in the current Predation Form
     * @return The number of parameters in the current Predation Form
     */
    int getNumParameters();
    /**
     * @brief This method implements the Predation Null algorithm (i.e., 0)
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param TimeMinus1 : one minus the current time
     * @param Species : species number
     * @param EstimatedBiomass : matrix of estimated biomass
     * @param EstimatedBiomassAtTimeMinus1 : estimated biomass of current species and time-1
     * @param EstPredationRho : the estimated predation rho parameters
     * @param PredationRhoCovariate : the predation rho covariate values
     * @param EstPredationHandling : the estimated predation handling parameters
     * @param PredationHandlingCovariate : the predation handling covariate values
     * @param EstPredationExponent : the estimated predation exponent parameters
     * @param PredationExponentCovariate : the predation exponent covariate values
     * @return Returns 0
     */
    double FunctionMap_Null(const std::string& covariateAlgorithmType,
                            const int& TimeMinus1,
                            const int& Species,
                            const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                            const double& EstimatedBiomassAtTimeMinus1,
                            const boost::numeric::ublas::matrix<double>& EstPredationRho,
                            const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
                            const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                            const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
                            const std::vector<double>& EstPredationExponent,
                            const boost::numeric::ublas::matrix<double>& PredationExponentCovariate);
    /**
     * @brief This method implements the Predation Type I algorithm
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param TimeMinus1 : one minus the current time
     * @param Species : species number
     * @param EstimatedBiomass : matrix of estimated biomass
     * @param EstimatedBiomassAtTimeMinus1 : estimated biomass of current species and time-1
     * @param EstPredationRho : the estimated predation rho parameters
     * @param PredationRhoCovariate : the predation rho covariate values
     * @param EstPredationHandling : the estimated predation handling parameters
     * @param PredationHandlingCovariate : the predation handling covariate values
     * @param EstPredationExponent : the estimated predation exponent parameters
     * @param PredationExponentCovariate : the predation exponent covariate values
     * @return Returns the Predation Type I value
     */
    double FunctionMap_TypeI(const std::string& covariateAlgorithmType,
                             const int& TimeMinus1,
                             const int& Species,
                             const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                             const double& EstimatedBiomassAtTimeMinus1,
                             const boost::numeric::ublas::matrix<double>& EstPredationRho,
                             const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
                             const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                             const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
                             const std::vector<double>& EstPredationExponent,
                             const boost::numeric::ublas::matrix<double>& PredationExponentCovariate);
    /**
     * @brief This method implements the Predation Type II algorithm
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param TimeMinus1 : one minus the current time
     * @param Species : species number
     * @param EstimatedBiomass : matrix of estimated biomass
     * @param EstimatedBiomassAtTimeMinus1 : estimated biomass of current species and time-1
     * @param EstPredationRho : the estimated predation rho parameters
     * @param PredationRhoCovariate : the predation rho covariate values
     * @param EstPredationHandling : the estimated predation handling parameters
     * @param PredationHandlingCovariate : the predation handling covariate values
     * @param EstPredationExponent : the estimated predation exponent parameters
     * @param PredationExponentCovariate : the predation exponent covariate values
     * @return Returns the Predation Type II value
     */
    double FunctionMap_TypeII(const std::string& covariateAlgorithmType,
                              const int& TimeMinus1,
                              const int& Species,
                              const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                              const double& EstimatedBiomassAtTimeMinus1,
                              const boost::numeric::ublas::matrix<double>& EstPredationRho,
                              const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
                              const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                              const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
                              const std::vector<double>& EstPredationExponent,
                              const boost::numeric::ublas::matrix<double>& PredationExponentCovariate);
    /**
     * @brief This method implements the Predation Type III algorithm
     * @param CovariateAlgorithmType : the covariate algorithm type to use for subsequent covariate calculations
     * @param TimeMinus1 : one minus the current time
     * @param Species : species number
     * @param EstimatedBiomass : matrix of estimated biomass
     * @param EstimatedBiomassAtTimeMinus1 : estimated biomass of current species and time-1
     * @param EstPredationRho : the estimated predation rho parameters
     * @param PredationRhoCovariate : the predation rho covariate values
     * @param EstPredationHandling : the estimated predation handling parameters
     * @param PredationHandlingCovariate : the predation handling covariate values
     * @param EstPredationExponent : the estimated predation exponent parameters
     * @param PredationExponentCovariate : the predation exponent covariate values
     * @return Returns the Predation Type III value
     */
    double FunctionMap_TypeIII(const std::string& covariateAlgorithmType,
                               const int& TimeMinus1,
                               const int& Species,
                               const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                               const double& EstimatedBiomassAtTimeMinus1,
                               const boost::numeric::ublas::matrix<double>& EstPredationRho,
                               const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
                               const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                               const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
                               const std::vector<double>& EstPredationExponent,
                               const boost::numeric::ublas::matrix<double>& PredationExponentCovariate);
    /**
     * @brief Loads the parameter ranges structure with data from the model data structure
     * @param ParameterInitialValues : parameter initial values (not necessarily the mid-range point)
     * @param ParameterRanges : parameter ranges
     * @param DataStruct : model data structure
     */
    void loadParameterRanges(
            std::vector<double>& ParameterInitialValues,
            std::vector<std::pair<double,double> >& ParameterRanges,
            nmfStructsQt::ModelDataStruct& DataStruct);
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


