/**
 * @file nmfCompetitionForm.h
 * @brief Definition for the Competition Form of the model
 * @date Nov 24, 2021
 *
 * This file contains the class definition for the Competition Form of the user
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
 * @brief This class contains the logic to calculate the Competition term of the currently selected model
 */
class nmfCompetitionForm {

private:
    bool m_isAGGPROD;
    int m_NumParameters;
    int m_NumSpecies;
    int m_NumGuilds;
    std::string m_Type; // "Null","NO_K",...
    std::vector<std::pair<double,double> > m_ParameterRanges;
    std::map<std::string,std::string> m_CompetitionMap;
    std::map<std::string,std::string> m_CompetitionKey;

    std::map<std::string, long double(nmfCompetitionForm::*)(
            const int& TimeMinus1,
            const int& SpeciesOrGuildNum,
            const double& BiomassAtTime,
            const std::vector<double>& EstGrowthRates,
            const boost::numeric::ublas::matrix<double> &GrowthRateCovariates,
            const double& GuildCarryingCapacity,
            const double& SystemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate
    )> m_FunctionMap;

    void setupFormMaps();

public:
    /**
     * @brief Constructor method for class
     * @param type : type of competition algorithm to be used
     */
    nmfCompetitionForm(std::string type);
   ~nmfCompetitionForm() {};

    /**
     * @brief This method allows the appropriate function map function to be called such that a series of if...else...
     * statements are not needed
     * @param TimeMinus1 : one minus the current time
     * @param SpeciesOrGuildNum : species number or guild number
     * @param BiomassAtTime : biomass at the currrent time
     * @param EstGrowthRates : estimated growth rate values
     * @param GrowthRateCovariates : growth rate covariate values
     * @param GuildCarryingCapacity : guild carrying carrying capacity
     * @param SystemCarryingCapacity : system carrying capacity
     * @param EstBiomassSpecies : estimated biomass per species
     * @param EstBiomassGuild : estimated biomass per guild
     * @param EstCompetitionAlpha : estimated competition alpha values
     * @param CompetitionAlphaCovariate : competition alpha covariate values
     * @param EstCompetitionBetaSpecies : estimated competition beta values
     * @param CompetitionBetaSpeciesCovariate : competition beta covariate values
     * @param EstCompetitionBetaGuild : estimated competition beta guild values
     * @param CompetitionBetaGuildCovariate : competition beta guild covariate values
     * @param EstCompetitionBetaGuildGuild : estimated competition beta guild-guild values
     * @param CompetitionBetaGuildGuildCovariate : competition beta guild-guild covariate values
     * @return Returns the competition value evaluated with the appropriate function map algorithm
     */
    long double evaluate(
            const int& TimeMinus1,
            const int& SpeciesOrGuildNum,
            const double& BiomassAtTime,
            const std::vector<double>& EstGrowthRates,
            const boost::numeric::ublas::matrix<double> &GrowthRateCovariates,
            const double& GuildCarryingCapacity,
            const double& SystemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate);
    /**
     * @brief Unpacks the parameters from the Parameters list and loads them into their respective data structures
     * @param Parameters : vector of all Competition parameter values
     * @param StartPos : start position from which to extract parameters
     * @param CompetitionAlpha : alpha parameters
     * @param CompetitionBetaSpecies : beta species parameters
     * @param CompetitionBetaGuilds : beta guild-species parameters
     * @param CompetitionBetaGuildsGuilds : beta guild-guild parameters
     */
    void extractParameters(
            const std::vector<double>& Parameters,
            int& StartPos,
            boost::numeric::ublas::matrix<double>& CompetitionAlpha,
            boost::numeric::ublas::matrix<double>& CompetitionBetaSpecies,
            boost::numeric::ublas::matrix<double>& CompetitionBetaGuilds,
            boost::numeric::ublas::matrix<double>& CompetitionBetaGuildsGuilds);
    /**
     * @brief FunctionMap_NOK : implements the Competition MSPROD functionality according to the
     * formula:  r(i)B(i,t)[(∑β(i,G)B(G,t))/(Kσ - KG)]
     * @param TimeMinus1 : one minus the current time
     * @param SpeciesOrGuildNum : species number or guild number
     * @param BiomassAtTime : biomass at the currrent time
     * @param EstGrowthRates : estimated growth rate values
     * @param GrowthRateCovariates : growth rate covariate values
     * @param GuildCarryingCapacity : guild carrying carrying capacity
     * @param SystemCarryingCapacity : system carrying capacity
     * @param EstBiomassSpecies : estimated biomass per species
     * @param EstBiomassGuild : estimated biomass per guild
     * @param EstCompetitionAlpha : estimated competition alpha values
     * @param CompetitionAlphaCovariate : competition alpha covariate values
     * @param EstCompetitionBetaSpecies : estimated competition beta values
     * @param CompetitionBetaSpeciesCovariate : competition beta covariate values
     * @param EstCompetitionBetaGuild : estimated competition beta guild values
     * @param CompetitionBetaGuildCovariate : competition beta guild covariate values
     * @param EstCompetitionBetaGuildGuild : estimated competition beta guild-guild values
     * @param CompetitionBetaGuildGuildCovariate : competition beta guild-guild covariate values
     * @return Returns the competition value evaluated with the AGGPROD algorithm
     */
    long double FunctionMap_AGGPROD(
            const int& TimeMinus1,
            const int& SpeciesOrGuildNum,
            const double& BiomassAtTime,
            const std::vector<double>& EstGrowthRates,
            const boost::numeric::ublas::matrix<double> &GrowthRateCovariates,
            const double& GuildCarryingCapacity,
            const double& SystemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate);
    /**
     * @brief FunctionMap_NOK : implements the Competition MSPROD functionality according to the
     * formula:  r(i)B(i,t)[(∑β(i,j)B(j,t))/KG - (∑β(i,G)B(G,t))/(K(σ) - K(G))]
     * @param TimeMinus1 : one minus the current time
     * @param SpeciesOrGuildNum : species number or guild number
     * @param BiomassAtTime : biomass at the currrent time
     * @param EstGrowthRates : estimated growth rate values
     * @param GrowthRateCovariates : growth rate covariate values
     * @param GuildCarryingCapacity : guild carrying carrying capacity
     * @param SystemCarryingCapacity : system carrying capacity
     * @param EstBiomassSpecies : estimated biomass per species
     * @param EstBiomassGuild : estimated biomass per guild
     * @param EstCompetitionAlpha : estimated competition alpha values
     * @param CompetitionAlphaCovariate : competition alpha covariate values
     * @param EstCompetitionBetaSpecies : estimated competition beta values
     * @param CompetitionBetaSpeciesCovariate : competition beta covariate values
     * @param EstCompetitionBetaGuild : estimated competition beta guild values
     * @param CompetitionBetaGuildCovariate : competition beta guild covariate values
     * @param EstCompetitionBetaGuildGuild : estimated competition beta guild-guild values
     * @param CompetitionBetaGuildGuildCovariate : competition beta guild-guild covariate values
     * @return Returns the competition value evaluated with the MSPROD algorithm
     */
    long double FunctionMap_MSPROD(
            const int& TimeMinus1,
            const int& SpeciesOrGuildNum,
            const double& BiomassAtTime,
            const std::vector<double>& EstGrowthRates,
            const boost::numeric::ublas::matrix<double> &GrowthRateCovariates,
            const double& GuildCarryingCapacity,
            const double& SystemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate);
    /**
     * @brief FunctionMap_NOK : implements the Competition NOK functionality according to the
     * formula:  B(i,t)[(∑{α(i,j)B(j,t)}]
     * @param TimeMinus1 : one minus the current time
     * @param SpeciesOrGuildNum : species number or guild number
     * @param BiomassAtTime : biomass at the currrent time
     * @param EstGrowthRates : estimated growth rate values
     * @param GrowthRateCovariates : growth rate covariate values
     * @param GuildCarryingCapacity : guild carrying carrying capacity
     * @param SystemCarryingCapacity : system carrying capacity
     * @param EstBiomassSpecies : estimated biomass per species
     * @param EstBiomassGuild : estimated biomass per guild
     * @param EstCompetitionAlpha : estimated competition alpha values
     * @param CompetitionAlphaCovariate : competition alpha covariate values
     * @param EstCompetitionBetaSpecies : estimated competition beta values
     * @param CompetitionBetaSpeciesCovariate : competition beta covariate values
     * @param EstCompetitionBetaGuild : estimated competition beta guild values
     * @param CompetitionBetaGuildCovariate : competition beta guild covariate values
     * @param EstCompetitionBetaGuildGuild : estimated competition beta guild-guild values
     * @param CompetitionBetaGuildGuildCovariate : competition beta guild-guild covariate values
     * @return Returns the competition value evaluated with the NOK algorithm
     */
    long double FunctionMap_NOK(
            const int& TimeMinus1,
            const int& SpeciesOrGuildNum,
            const double& BiomassAtTime,
            const std::vector<double>& EstGrowthRates,
            const boost::numeric::ublas::matrix<double> &GrowthRateCovariates,
            const double& GuildCarryingCapacity,
            const double& SystemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate);
    /**
     * @brief FunctionMap_Null : implements the Competition Null functionality, i.e., returns 0
     * @param TimeMinus1 : one minus the current time
     * @param SpeciesOrGuildNum : species number or guild number
     * @param BiomassAtTime : biomass at the currrent time
     * @param EstGrowthRates : estimated growth rate values
     * @param GrowthRateCovariates : growth rate covariate values
     * @param GuildCarryingCapacity : guild carrying carrying capacity
     * @param SystemCarryingCapacity : system carrying capacity
     * @param EstBiomassSpecies : estimated biomass per species
     * @param EstBiomassGuild : estimated biomass per guild
     * @param EstCompetitionAlpha : estimated competition alpha values
     * @param CompetitionAlphaCovariate : competition alpha covariate values
     * @param EstCompetitionBetaSpecies : estimated competition beta values
     * @param CompetitionBetaSpeciesCovariate : competition beta covariate values
     * @param EstCompetitionBetaGuild : estimated competition beta guild values
     * @param CompetitionBetaGuildCovariate : competition beta guild covariate values
     * @param EstCompetitionBetaGuildGuild : estimated competition beta guild-guild values
     * @param CompetitionBetaGuildGuildCovariate : competition beta guild-guild covariate values
     * @return Returns 0
     */
    long double FunctionMap_Null(
            const int& TimeMinus1,
            const int& SpeciesOrGuildNum,
            const double& BiomassAtTime,
            const std::vector<double>& EstGrowthRates,
            const boost::numeric::ublas::matrix<double> &GrowthRateCovariates,
            const double& GuildCarryingCapacity,
            const double& SystemCarryingCapacity,
            const boost::numeric::ublas::matrix<double> &EstBiomassSpecies,
            const boost::numeric::ublas::matrix<double> &EstBiomassGuild,
            const boost::numeric::ublas::matrix<double> &EstCompetitionAlpha,
            const boost::numeric::ublas::matrix<double> &CompetitionAlphaCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaSpecies,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaSpeciesCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildCovariate,
            const boost::numeric::ublas::matrix<double> &EstCompetitionBetaGuildGuild,
            const boost::numeric::ublas::matrix<double> &CompetitionBetaGuildGuildCovariate);
    /**
     * @brief Gets the expression for the selected Competition Form algorithm
     * @return Returns a string representing the formula of the selected Competition Form algorithm
     */
    std::string getExpression();
    /**
     * @brief Gets the key (i.e., legend) for the selected Competition Form algorithm variables
     * @return Returns a string representing a description of the variables used in the selected
     * Competition Form algorithm's formula
     */
    std::string getKey();
    /**
     * @brief Gets the number of parameters in the selected algorithm. Some parameters may be
     * from a sparsely populated matrix.
     * @return Number of parameters in selected algorithm
     */
    int getNumParameters();
    /**
     * @brief Loads the parameter ranges structure with data from the model data structure
     * @param ParameterRanges : parameter ranges
     * @param DataStruct : model data structure
     */
    void loadParameterRanges(
            std::vector<std::pair<double,double> >& ParameterRanges,
            nmfStructsQt::ModelDataStruct& DataStruct);
    /**
     * @brief Sets the AggProd variable
     * @param isAggProd : boolean describing the AggProd status (i.e., true/false)
     */
    void setAggProd(bool isAggProd);
    /**
     * @brief Sets the type of competition algorithm
     * @param newType : type of competition algorithm
     */
    void setType(std::string newType);
};


