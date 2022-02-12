/**
 * @file nmfUtils.h
 * @brief Definition for common typedefs and structs (i.e., non-graphical)
 * @date Dec 8, 2016
 *
 * This file contains typedefs, structs, and functions common to the MultiSpecies
 * applications. The functions that print various structures are located in this
 * file as well.
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

#include <ctime>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include <QDir>
#include <QRandomGenerator>

#include "nmfUtilsQt.h"
#include "nmfStructsQt.h"
#include "nmfConstantsMSVPA.h"
#include "nmfLogger.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/multi_array.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

typedef boost::multi_array<double, 3> Boost3DArrayDouble;
typedef boost::multi_array<double, 4> Boost4DArrayDouble;
typedef boost::multi_array<double, 5> Boost5DArrayDouble;

/**
 * @brief This structure contains data needed for the summary statitics tables
 */
struct StatStruct {
    std::vector<double> SSresiduals;
    std::vector<double> SSdeviations;
    std::vector<double> SStotals;
    std::vector<double> rsquared;
    std::vector<double> correlationCoeff;
    std::vector<double> aic;
    std::vector<double> rmse;
    std::vector<double> ri;
    std::vector<double> ae;
    std::vector<double> aae;
    std::vector<double> mef;
    std::string         title;
    std::vector<std::vector<double> > mohnsRhoParameters;
};

/**
 * @brief Structure contains all of the parameters in a particular Model
 */
struct ModelData {
    std::string ProjectName;
    std::string ModelName;
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
    int         NLoptNumberOfRuns;
};


/**
 * @brief The MSVPA Scenarios data structure
 */
struct Scenarios {
    int VarF;
    int VarOthPred;
    int VarOthPrey;
    int VarRec;
    int FishAsF;
    std::string Name;
};

/**
 * @brief General purpose utility functions that might be used in all tools
 */
namespace nmfUtils {
    /**
     * @brief appends the new vector to the current vector
     * @param newVec : vector to be appended
     * @param mainVec : the resultant appended vector
     */
    void append(std::vector<double>& newVec,
                std::vector<double>& currentVec);
    /**
     * @brief applies the appropriate covariate logic to the covariate values
     * @param logger : application logger to which to post error messages
     * @param covariateAlgorithmType : type of covariate algorithm to be used
     * @param parameter : estimated parameter which the covariate operations are applied to
     * @param covariateCoeff : the covariate coefficient value
     * @param covariateValue : the covariate value entered by the user
     * @return the value of the parameter with the covariate applied
     */
    double applyCovariate(nmfLogger *logger,
                          const std::string& covariateAlgorithmType,
                          const double& parameter,
                          const double& covariateCoeff,
                          const double& covariateValue);
    /**
     * @brief Checks that there's at least one item in the passed table's field
     * @param logger : application logger to which to post error messages
     * @param table : table being checked
     * @param field : field in table being checked
     * @param dataMap : data structure holding table data after they've been read from file system
     * @param queryStr : query command used to check the field in table being checked
     * @return True if an error was found, False otherwise
     */
    bool checkForError(nmfLogger *logger,
                       std::string table,
                       std::string field,
                       std::map<std::string, std::vector<std::string> > &dataMap,
                       std::string queryStr);
    /**
     * @brief Template function used to clamp a value to a high and low value.
     * @param value : The value to clamp.
     * @param lower : The lower limit to which to clamp the value if it is less than.
     * @param upper : The upper limit to which to clamp the value if it is greater than.
     */
    template<typename T>
    void clamp(T &value, double lower, double upper)
    {
        value = (value <= lower) ? lower : (value >= upper) ? upper : value;
    }
    /**
     * @brief Converts the catch units string description to the appropriate factor value in thousands of fish
     * @param catchUnits : catch units string (i.e., "Millions of Fish")
     * @return The converted value (i.e., for "Millions of Fish" it returns 1000.0)
     */
    double convertCatchUnitsToValue(std::string catchUnits);
    /**
     * @brief Converts the size units string description to the appropriate factor value in centimeters
     * @param sizeUnits : size units string (i.e., "Inches")
     * @return The converted value (i.e., for "Inches" it returns 2.54)
     */
    double convertSizeUnitsToValue(std::string sizeUnits);
    /**
     * @brief Converts the passed value to a scientific notation formatted string
     * @param val : value to convert to scientific notation
     * @return String representing the passed in value in scientific notation
     */
    std::string convertToScientificNotation(double val);
    /**
     * @brief Formats a vector of double values into a table format
     * @param label : label for values
     * @param values : the vector values
     * @param includeTotal : boolean to include the total or not
     * @return returns the formatted string
     */
    std::string convertValues1DToOutputStr(
            const std::string& label,
            const std::vector<double> &values,
            const bool& includeTotal);
    /**
     * @brief Formats a matrix of double values into a table format
     * @param label : label for values
     * @param matrix : the matrix of values
     * @return returns the formatted string
     */
    std::string convertValues2DToOutputStr(
            const std::string& label,
            const boost::numeric::ublas::matrix<double> &matrix);
    /**
     * @brief Converts a vector to a one dimensional matrix
     * @param vec : vector to convert
     * @return one-dimensional matrix
     */
    boost::numeric::ublas::matrix<double>
        convertVectorToMatrix(std::vector<double>& vec);
    /**
     * @brief Converts the weight units string description to the appropriate factor value in kilograms
     * @param weightUnits : size units string (i.e., "Gram")
     * @return The converted value (i.e., for "Gram" it returns 0.001)
     */
    double convertWeightUnitsToValue(std::string weightUnits);
    /**
     * @brief Copies the first file contents to the second file
     * @param fileA : file to be copied from
     * @param fileB : file to be copied to
     */
    void copyFile(std::string fileA, std::string fileB);
    /**
     * @brief Returns the files in the passed directory having the given extension
     * @param path : directory in which to search for files
     * @param ext : extension of files to list
     * @param filesToCopy : the list of files having the given extension in the given directory
     */
    void getFilesWithExt(std::string path,
                         std::string ext,
                         QStringList& filesToCopy);
    /**
     * @brief Gets the maximum element value from the passed matrix
     * @param mat : matrix in which to find the maximum value
     * @param roundOff : If True, will round max value off to nearest tens or hundrerds or...place (i.e., 12 -> 10, 180 -> 200)
     * @return The max value in matrix
     */
    double getMatrixMax(boost::numeric::ublas::matrix<double> &mat,
                        bool roundOff);
    /**
     * @brief Get the sum over all of the matrix elements
     * @param mat : matrix in which to sum over
     * @return : The sum over all of the matrix elements
     */
    double getMatrixSum(const boost::numeric::ublas::matrix<double> &mat);
    /**
     * @brief Gets the name of the operating system
     * @return The name of the operating system on which the application is running
     */
    std::string getOS();
    /**
     * @brief Returns a random number between the passed limits: [lowerLimit,upperLimit). The
     * random number is generated using the Mersene Twister 19937 generator (64 bit) algorithm.
     * @param seed : If seed < 0, no seed is used and the algorithm is stochastic, else
     * the seed value is used and the algorithm is deterministic.
     * @param lowerLimit : lower limit of random number range
     * @param upperLimit : upper limit of random number range
     * @return The random number generated using the passed parameters
     */
    double getRandomNumber(int seed,
                           double lowerLimit,
                           double upperLimit);
    /**
     * @brief Get the sum over all of the vector elements
     * @param vec : vector in which to sum over
     * @return : The sum over all of the vector elements
     */
    double getVectorSum(const boost::numeric::ublas::vector<double> &vec);
    /**
     * @brief Template function to initialize a multi_array of any size
     * @param multi_array : multi_array to initialize
     */
    template<typename T>
    void initialize(T& multi_array)
    {
        std::fill_n(multi_array.data(), multi_array.num_elements(), 0);
    }
    /**
     * @brief Template function to initialize a vector of various types
     * @param vec : vector to initialize
     * @param numElements : number of elements in vector
     */
    template<typename T>
    void initialize(T& vec,
                    int const& numElements)
    {
        vec.resize(numElements);
        vec.clear();
    }
    /**
     * @brief Template function to initialize a matrix of various types
     * @param matrix : matrix to initialize
     * @param nrows : number of rows in matrix
     * @param ncols : number of cols in matrix
     */
    template<typename T>
    void initialize(T& matrix,
              int const& nrows,
              int const& ncols)
    {
        matrix.resize(nrows,ncols,false);
        matrix.clear();
    }
    /**
     * @brief Finds the inverse of the passed matrix. (In order for an inverse to be found, the determinant of the matrix must not be 0.)
     * @param matrix : matrix whose inverse is desired
     * @param inverseMatrix : the inverse of the passed in matrix
     * @return True if inverse was found, else False
     */
    bool invertMatrix(
            boost::numeric::ublas::matrix<double>& matrix,
            boost::numeric::ublas::matrix<double>& inverseMatrix);
    /**
     * @brief Checks a list from within the passes Data_Struct to see if a specific parameter has has its checkbox checked on the Estimation Tab6 page
     * @param dataStruct : data structure to check a list for the passed parameter name
     * @param ParameterName : name of parameter to check in list in Data_Struct
     * @return True if parameter should be estimated, false otherwise
     */
    bool isEstimateParameterChecked(
            const nmfStructsQt::ModelDataStruct& dataStruct,
            const std::string& ParameterName);
    /**
     * @brief Tests the passed value if there's no decimal part and returns true, else returns false
     * @param value : number to test if there's a decimal part
     * @return True or False value signifying if the passed number is a "whole" number (i.e., there's no decimal part)
     */
    bool isWholeNumber(double value);
    /**
     * @brief Checks to see if the passed double is very close to 0
     * @param value : value to check to see if close to 0
     * @return boolean true if nearly zero, else false
     */
    bool isNearlyZero(const double& value);
    /**
     * @brief Returns True/False if current operating system is Windows (either 32-bit or 64-bit)
     * @return True or False signifying current operating system is Windows
     */
    bool isOSWindows();
    /**
     * @brief Determines if a current run has been stopped
     * @param runName : currently unused, here for future usage
     * @param msg1 : time of run
     * @param msg2 : currently unused; here for future usage
     * @param stopRunFile : name of text file containing the "stop" information
     * @param state : describes how the application was stopped
     * @return True if valid Stop command was written to file, false otherwise
     */
    bool isStopped(std::string& runName,
                   std::string& msg1,
                   std::string& msg2,
                   std::string& stopRunFile,
                   std::string& state);
    /**
     * @brief prints out a 3d double array
     * @param name : name of array
     * @param dimensions : dimensions of array
     * @param array : the 3d double array
     * @param width : the number of characters to use for each array element
     * @param precision : the number of decimal places to use for each array element
     */
    void print3DArray(const std::string& name,
                      const std::vector<int>& dimensions,
                      const Boost3DArrayDouble& array,
                      const int& width,
                      const int& precision);
    /**
     * @brief prints out a 4d double array
     * @param name : name of array
     * @param dimensions : dimensions of array
     * @param array : the 4d double array
     */
    void print4DArray(const std::string& name,
                      const std::vector<int>& dimensions,
                      const Boost4DArrayDouble& array);
    /**
     * @brief prints out an error message
     * @param msg : message to print
     * @param errorMsg : previously captured error message to print
     */
    void printError(const std::string msg,
                    const std::string errorMsg);
    /**
     * @brief prints out all of the key value pairs in the passed data map
     * @param name : name of map
     * @param dataMap : map containing std::string keys and vectors of std::string types
     */
    void printMap(const std::string name,
                  const std::map<std::string, std::vector<std::string> >& dataMap);
    /**
     * @brief prints out the std::string and the corresponding vector of std::string values
     * @param pair : includes a std::string and a vector of std::string types
     */
    void printMapPair(const std::pair<std::string, std::vector<std::string> >& pair);
    /**
     * @brief prints out a matrix of double types
     * @param name : name of matrix
     * @param mat : the matrix of double types
     * @param width : the number of characters to use for each array element
     * @param precision : the number of decimal places to use for each array element
     */
    void printMatrix(const std::string& name,
                     const boost::numeric::ublas::matrix<double>& mat,
                     const int width,
                     const int precision);
    /**
     * @brief prints the passed message
     * @param msg : message to be printed
     */
    void printMsg(std::string msg);
    /**
     * @brief Template function used to print out value of any type of variable passed to it.
     * @param name : variable name
     * @param value : variable value
     * @param width : the number of characters to use for each array element
     * @param precision : the number of decimal places to use for each array element
     */
    template<typename T>
    void printValue(std::string name,
                    T value,
                    const int width,
                    const int precision)
    {
        std::cout << name << ": "
                  << std::setw(width)
                  << std::setprecision(precision)
                  << value << std::endl;
    }
    /**
     * @brief Template function used to print out a vector; suggest to use numCharsPerElement
     * of 30, 4, and 4 for boost vectors of std::string, boost vectors of
     * double, and std::vectors of std::string, respectively
     * @param name : name of the vector data
     * @param numCharsPerElement : the number of characters that the values will contain
     * @param vec : the templated vector type
     */
    template<typename T>
    void printVector(const std::string& name,
                     const int& numCharsPerElement,
                     T& vec)
    {
        std::cout << "\n" << name << ": " << vec.size() << std::endl;
        for (unsigned i = 0; i < vec.size(); ++i) {
            std::cout << "| ";
            std::cout << std::setw(numCharsPerElement) << vec(i) << " | ";
            std::cout << std::endl;
        }
    }
    /**
     * @brief reads a table and creates a map of table names as keys and table descriptions as values
     * @param tableName : name of table
     * @param tableDescription : map of table name and table description
     */
    void readTableDescriptions(std::string tableName,
                               std::map<std::string,std::string>& tableDescription);
    /**
     * @brief reads table names into a map where the key is SSVPA or
     * MSVPA and the value is a vector of associated tables
     * @param TableNames : map of vectors of table names binned by SSVPA or MSVPA
     */
    void readTableNames(std::map<std::string,std::vector<std::string> >* TableNames);
    /**
     * @brief rescales the passed matrix by: log base 10
     * @param matrix to rescale
     */
    void rescaleMatrixLog(boost::numeric::ublas::matrix<double>& matrix);
    /**
     * @brief rescales the passed matrix by: (X - Xmin) / (Xmax - Xmin) where X is an element of the matrix
     * @param matrix : the matrix to rescale
     */
    void rescaleMatrixMinMax(boost::numeric::ublas::matrix<double>& matrix);
    /**
     * @brief Sets the passed matrix values to the passed resetValue
     * @param mat : matrix to reset
     * @param resetValue : value with which to reset the matrix
     */
    void reset(boost::numeric::ublas::matrix<double>& mat,
               const double& resetValue);
    /**
     * @brief rounds a number to a specific number of decimal places
     * @param number : number to round
     * @param decimalPlaces : number of decimal places at which to perform the rounding
     * @return the rounded number
     */
    double round(double number,
                 int decimalPlaces);
    /**
     * @brief splits a string by a specific delimeter
     * @param main : the string to split
     * @param delim : the delimeter to split the string by
     * @param str1 : the substring to the left of the delimeter
     * @param str2 : the substring to the right of the delimeter
     */
    void split(std::string main,
               std::string delim,
               std::string& str1,
               std::string& str2);
}

