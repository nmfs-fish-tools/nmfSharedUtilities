/**
 * @file BeesAlgorithm.h
 * @brief Implementation of Bees Search Algorithm
 *
 * This file contains an implementation of the Bees Search Algorithm. It's based upon
 * the C++ implementation from Marco Castellani downloaded from http://beesalgorithmsite.altervista.org/
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

#include "Bee.h"

#include "nmfUtils.h"
#include "nmfUtilsStatistics.h"
#include "nmfConstantsMSSPM.h"
#include "nmfGrowthForm.h"
#include "nmfHarvestForm.h"
#include "nmfCompetitionForm.h"
#include "nmfPredationForm.h"
#include "nmfUtilsQt.h"

typedef boost::numeric::ublas::matrix<double> Matrix;

/**
 * @brief The beesCompareLT struct is used in comparing the fitness of 2 bees
 * in ascending order of fitness
 */
struct beesCompareLT
{
    inline bool operator() (const std::unique_ptr<Bee>& lhsBee,
                            const std::unique_ptr<Bee>& rhsBee)
    {
        return (lhsBee->getFitness() < rhsBee->getFitness());
    }
};

/**
 * @brief The beesCompareGT struct is used in comparing the fitness of 2 bees
 * in descending order of fitness
 */
struct beesCompareGT
{
    inline bool operator() (const std::unique_ptr<Bee>& lhsBee,
                            const std::unique_ptr<Bee>& rhsBee)
    {
        return (lhsBee->getFitness() > rhsBee->getFitness());
    }
};

/**
 * @brief The BeesAlgorithm class which contains all of the methods necessary to
 * implement the Bees search algorithm.
 */
class BeesAlgorithm
{

    const int kTimeToSpendSearching = 30; //3; // time to look for a bee in microseconds

private:
    long                                   m_Seed;
    int                                    m_DefaultFitness;
    int                                    m_NullFitness;
    double                                 m_PatchSizePct;
    std::string                            m_Scaling;
    boost::numeric::ublas::matrix<double>  m_ObsBiomassBySpeciesOrGuilds;
    boost::numeric::ublas::matrix<double>  m_ObsBiomassByGuilds;
    boost::numeric::ublas::matrix<double>  m_Catch;
    boost::numeric::ublas::matrix<double>  m_Effort;
    boost::numeric::ublas::matrix<double>  m_Exploitation;
    std::vector<std::pair<double,double> > m_ParameterRanges;
    std::vector<double>                    m_PatchSizes;
    nmfStructsQt::ModelDataStruct          m_BeeStruct;
    std::unique_ptr<nmfGrowthForm>         m_GrowthForm;
    std::unique_ptr<nmfHarvestForm>        m_HarvestForm;
    std::unique_ptr<nmfCompetitionForm>    m_CompetitionForm;
    std::unique_ptr<nmfPredationForm>      m_PredationForm;
    std::map<int,std::vector<int> >        m_GuildSpecies;
    std::vector<int>                       m_GuildNum;
    std::map<QString,QString>              m_PreviousUnits;

    void checkAndIncrementSeed();
    std::unique_ptr<Bee> createNeighborhoodBee(std::vector<double> &bestSiteParameters);
    std::unique_ptr<Bee> createRandomBee(bool doWhileLoop,
                                         std::string& errorMsg);
    bool isABetterFitness(double& bestFitnessInPopulation,
                          double& bestBeesFitness);
    void loadInitBiomassParameterRanges(
            std::vector<double>& parameterInitialValues,
            std::vector<std::pair<double,double> >& parameterRanges,
            nmfStructsQt::ModelDataStruct& dataStruct);
    void loadSurveyQParameterRanges(
            std::vector<double>& parameterInitialValues,
            std::vector<std::pair<double,double> >& parameterRanges,
            nmfStructsQt::ModelDataStruct& dataStruct);
    void printBee(double &fitness, std::vector<double> &parameters);
    void printParameterRanges(const int& numSpecies,
                              const int& numGuilds,
                              const std::string& growthForm,
                              const std::string& harvestForm,
                              const std::string& competitionForm,
                              const std::string& predationForm);
    std::unique_ptr<Bee> searchNeighborhoodForBestBee(std::unique_ptr<Bee> bestSite,
                                                      int &neighborhoodSize);
    std::unique_ptr<Bee> searchParameterSpaceForBestBee(int& runNum,
                                                        int& subRunNum,
                                                        std::string& errorMsg);
    bool stoppedByUser();
    void writeCurrentLoopFile(std::string &MSSPMName,
                              int         &NumGens,
                              double      &BestFitness,
                              int         &NumGensSinceBestFit);

public:
    /**
     * @brief BeesAlgorithm class constructor
     * @param BeeStruct : structure containing model data needed for the Bees algorithm
     * @param verbose : if true, additional error statements printed; if false, minimal error statements printed
     */
    BeesAlgorithm(nmfStructsQt::ModelDataStruct BeeStruct,
                  const bool& verbose);
   ~BeesAlgorithm();

    /**
     * @brief calculates the number of parameters
     * @return number of parameters to be estimated
     */
    int calculateActualNumEstParameters();
    bool estimateParameters(
            double &bestFitness,
            std::vector<double> &bestParameters,
            int &RunNum,
            int &subRunNum,
            std::string& errorMsg);
    void extractCompetitionParameters(
            const std::vector<double>& parameters,
            int& startPos,
            boost::numeric::ublas::matrix<double>& alpha,
            boost::numeric::ublas::matrix<double>& betaSpecies,
            boost::numeric::ublas::matrix<double>& betaGuilds,
            boost::numeric::ublas::matrix<double>& betaGuildsGuilds);
    void extractExponentParameters(
            const std::vector<double>& parameters,
            int& startPos,
            std::vector<double>& exponents);
    void extractGrowthParameters(
            const std::vector<double>& parameters,
            int& startPos,
            std::vector<double>& growthRate,
            std::vector<double>& growthRateCovariateCoeff,
            std::vector<double>& carryingCapacity,
            std::vector<double>& carryingCapacityCovariateCoeffs,
            double& systemCarryingCapacity);
    void extractHandlingParameters(
            const std::vector<double>& parameters,
            int& startPos,
            boost::numeric::ublas::matrix<double>& handling);
    void extractHarvestParameters(
            const std::vector<double>& parameters,
            int& startPos,
            std::vector<double>& catchability,
            std::vector<double>& catchabilityCovariateCoeffs);
    void extractInitBiomass(
            const std::vector<double>& parameters,
            int&                       startPos,
            std::vector<double>&       initBiomass);
    void extractPredationParameters(
            const std::vector<double>& parameters,
            int& startPos,
            boost::numeric::ublas::matrix<double>& predation);
    void extractSurveyQParameters(
            const std::vector<double>& parameters,
            int&                       startPos,
            std::vector<double>&       surveyQ,
            std::vector<double>&       surveyQCovariateCoeffs);
    double evaluateObjectiveFunction(
            const std::vector<double> &parameters);
    void initializeParameterRangesAndPatchSizes(
            nmfStructsQt::ModelDataStruct& theBeeStruct);
    void setSeed(
            const long& seed);

};

