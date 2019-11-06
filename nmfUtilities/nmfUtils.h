/*
#ifndef NMFUTILITIES_H
#define NMFUTILITIES_H

#include "nmfutilities_global.h"

class NMFUTILITIESSHARED_EXPORT nmfUtilities
{

public:
    nmfUtilities();
};

#endif // NMFUTILITIES_H
*/

/**
 @file nmfUtils.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This header file defines MSVPA_X2 utility functions.
 @date Nov 29, 2016
*/

#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "nmfLogger.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multi_array.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

typedef boost::multi_array<double, 3> Boost3DArrayDouble;
typedef boost::multi_array<double, 4> Boost4DArrayDouble;
typedef boost::multi_array<double, 5> Boost5DArrayDouble;

struct Gradient_Struct {
    int RunLength;
    int NumSpecies;
    int NumParameters;
    int MaxIterations;
    int MaxLineSearches;

    std::string GrowthForm;
    std::string HarvestForm;
    std::string CompetitionForm;
    std::string PredationForm;

    std::string Algorithm;
    std::string Minimizer;
    std::string ObjectiveCriterion;

    boost::numeric::ublas::matrix<double> ObservedBiomass;
    boost::numeric::ublas::matrix<double> OutputBiomass;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::vector<double> GrowthRateMin;
    boost::numeric::ublas::vector<double> GrowthRateMax;
    boost::numeric::ublas::vector<double> GrowthRateInit;
    boost::numeric::ublas::vector<double> CarryingCapacityMin;
    boost::numeric::ublas::vector<double> CarryingCapacityMax;
    boost::numeric::ublas::vector<double> CarryingCapacityInit;
    boost::numeric::ublas::vector<double> CompetitionMin;
    boost::numeric::ublas::vector<double> CompetitionMax;
    boost::numeric::ublas::vector<double> CompetitionInit;
    boost::numeric::ublas::vector<double> PredationMin;
    boost::numeric::ublas::vector<double> PredationMax;
    boost::numeric::ublas::vector<double> PredationInit;
//    boost::numeric::ublas::vector<double> CompetitionPosition; // Position of parameters in each row (i.e., per Species)

};


struct SystemData {
    std::string Name;
    std::string CarryingCapacity;
    std::string GrowthForm;
    std::string HarvestForm;
    std::string CompetitionForm;
    std::string PredationForm;
    std::string Algorithm;
    std::string Minimizer;
    std::string ObjectiveCriterion;
    std::string Scaling;
    int         NumberOfParameters;
    int         StartYear;
    int         NumberOfRuns;
    int         RunLength;
    int         TimeStep;
    int         NumGenerations;
    int         PopulationSize;
    double      MutationRate;
    int         ConvergenceNum;
    int         BeesNumTotal;
    int         BeesNumEliteSites;
    int         BeesNumBestSites;
    int         BeesNumElite;
    int         BeesNumOther;
    int         BeesNumRepetitions;
    int         BeesMaxGenerations;
    float       BeesNeighborhoodSize;
    int         GradMaxIterations;
    int         GradMaxLineSearches;
    int         NLoptUseStopVal;
    int         NLoptUseStopAfterTime;
    int         NLoptUseStopAfterIter;
    double      NLoptStopVal;
    int         NLoptStopAfterTime;
    int         NLoptStopAfterIter;
};


struct Data_Struct {

    bool   NLoptUseStopVal;
    bool   NLoptUseStopAfterTime;
    bool   NLoptUseStopAfterIter;
    double NLoptStopVal;
    int    NLoptStopAfterTime;
    int    NLoptStopAfterIter;

    int    RunLength;
    int    NumSpecies;
    int    NumGuilds;

    int    BeesNumTotal;
    int    BeesNumElite;
    int    BeesNumOther;
    int    BeesNumEliteSites;
    int    BeesNumBestSites;
    int    BeesNumRepetitions;
    int    BeesMaxGenerations;
    float  BeesNeighborhoodSize;

    int    GAGenerations;
    int    GAConvergence;

    std::string Scaling;
    int         TotalNumberParameters;
    std::string Benchmark;
    std::string EstimationType;

    std::string GrowthForm;
    std::string HarvestForm;
    std::string CompetitionForm;
    std::string PredationForm;

    std::string Minimizer;
    std::string ObjectiveCriterion;

    std::map<int,std::vector<int> >       GuildSpecies; // List of species numbers that make up guild num
    std::vector<int>                      GuildNum;     // Specifies which species are members of which guilds
    boost::numeric::ublas::matrix<double> ObservedBiomassBySpecies;
    boost::numeric::ublas::matrix<double> ObservedBiomassByGuilds;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> Effort;
    boost::numeric::ublas::matrix<double> Exploitation;
    boost::numeric::ublas::vector<double> GrowthRateMin;
    boost::numeric::ublas::vector<double> GrowthRateMax;
    boost::numeric::ublas::vector<double> CarryingCapacityInitial;
    boost::numeric::ublas::vector<double> CarryingCapacityMin;
    boost::numeric::ublas::vector<double> CarryingCapacityMax;
    boost::numeric::ublas::vector<double> ExploitationRateMin;
    boost::numeric::ublas::vector<double> ExploitationRateMax;
    boost::numeric::ublas::vector<double> CatchabilityMin;
    boost::numeric::ublas::vector<double> CatchabilityMax;
    std::vector<std::vector<double> >     CompetitionMin;
    std::vector<std::vector<double> >     CompetitionMax;
    std::vector<std::vector<double> >     CompetitionBetaSpeciesMin;
    std::vector<std::vector<double> >     CompetitionBetaSpeciesMax;
    std::vector<std::vector<double> >     CompetitionBetaGuildsMin;
    std::vector<std::vector<double> >     CompetitionBetaGuildsMax;
    std::vector<std::vector<double> >     PredationMin;
    std::vector<std::vector<double> >     PredationMax;
    std::vector<std::vector<double> >     HandlingMin;
    std::vector<std::vector<double> >     HandlingMax;
    std::vector<double>                   ExponentMin;
    std::vector<double>                   ExponentMax;
    std::vector<double>                   Parameters;


    boost::numeric::ublas::matrix<double> OutputBiomass;


};


struct Scenarios {
    int VarF;
    int VarOthPred;
    int VarOthPrey;
    int VarRec;
    int FishAsF;
    std::string Name;
};


namespace nmfUtils {



//std::clock_t startTimer();
//std::string elapsedTime(std::clock_t cl);

//std::string lstrip(std::string str, std::string substr);

std::chrono::time_point<std::chrono::high_resolution_clock> startTimer();
std::string elapsedTime(std::chrono::time_point<std::chrono::high_resolution_clock> startTime);

double matrixSum(const boost::numeric::ublas::matrix<double> &mat);
double vectorSum(const boost::numeric::ublas::vector<double> &vec);

void printError(const std::string msg, const std::string errorMsg);

void printMsg(std::string msg);

void printMapPair(const std::pair<std::string, std::vector<std::string> > &pair);

void printMap(const std::string name,
        const std::map<std::string, std::vector<std::string> > &dataMap);

void printMatrix(const std::string &name,
        const boost::numeric::ublas::matrix<double> &mat,
        const int width,
        const int precision);

void printVector(const std::string &name,
                 const std::vector<std::string> &vec);
void printVector(const std::string &name,
        const boost::numeric::ublas::vector<double> &vec);
void printVector(const std::string &name,
        const boost::numeric::ublas::vector<std::string> &vec);

void print3DArray(const std::string &name,
        const std::vector<int> &dimensions,
        const Boost3DArrayDouble &array,
        const int &width,
        const int &precision);

void print4DArray(const std::string &name,
        const std::vector<int> &dimensions,
        const Boost4DArrayDouble &array);

bool errorCheck(nmfLogger *logger,
                std::string table,
                std::string field,
                std::map<std::string, std::vector<std::string> > &dataMap,
                std::string queryStr);

void rescaleMatrixMinMax(boost::numeric::ublas::matrix<double> &Matrix);
void rescaleMatrixLog(boost::numeric::ublas::matrix<double> &Matrix);
std::string to_string_scientificNotation(double val);

/**
 * @brief Template function used to print out value of any type of variable passed to it.
 * @param name Variable name
 * @param value Variable value
 * @return none
 */
template<typename T>
void printValue(std::string name, T value, const int width, const int precision)
    {std::cout << name << ": "
        << std::setw(width)
        << std::setprecision(precision)
        << value << std::endl;}
/**
 * @brief Template function used to clamp a value to a high and low value.
 * @param value The value to clamp.
 * @param lower The lower limit to which to clamp the value if it is less than.
 * @param upper The upper limit to which to clamp the value if it is greater than.
 * @return none
 */
template<typename T>
void clamp(T &value, double lower, double upper)
    { value = value <= lower ? lower : value >= upper ? upper : value;}


void initialize(boost::numeric::ublas::vector<std::string> &vec, const int &nrows);
void initialize(boost::numeric::ublas::vector<int>    &vec, const int &nrows);
void initialize(boost::numeric::ublas::vector<double> &vec, const int &nrows);
void initialize(boost::numeric::ublas::matrix<int>    &mat, const int &nrows, const int &ncols);
void initialize(boost::numeric::ublas::matrix<double> &mat, const int &nrows, const int &ncols);
void initialize(Boost3DArrayDouble &array3d);
void initialize(Boost4DArrayDouble &array4d);
void initialize(Boost5DArrayDouble &array5d);

double maxMatrixValue(boost::numeric::ublas::matrix<double> &mat, bool roundValue);
double randomNumber(int seed, double lowerLimit, double upperLimit);

double round(double number, int decimalPlaces);
void   split(std::string main, std::string delim, std::string &str1, std::string &str2);

double CatchUnitsStringToValue(std::string val);
double WeightUnitsStringToValue(std::string val);
double SizeUnitsStringToValue(std::string val);
void   readTableNames(std::map<std::string,std::vector<std::string> > *TableNames);
void   readTableDescriptions(std::string TableName, std::map<std::string,std::string> &TableDescription);
std::string getOS();
}

