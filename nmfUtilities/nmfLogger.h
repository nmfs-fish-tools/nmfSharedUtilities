/**
 * @file nmfLogger.h
 * @brief Definition for the MultiSpecies Logger class
 * @date Jan 14, 2020
 *
 * This file defines the MS Logger class.  This class allows the user to
 * log (i.e., save) normal output statements, warnings, or errors to an
 * output timestamped log file.
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

#ifndef NMF_LOGGER
#define NMF_LOGGER

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <fstream>

/**
 * @brief This class describes the nmfLogger class for the MultiSpecies tools. It
 * allows the user to save normal, warning, or error statements to a specially encoded
 * ascii file. This file is then read by another class and is displayed into a widget
 * using the encoding to determine which color to display which message.
 */
class nmfLogger {

private:
    std::string filename = "";

public:
    /**
     * @brief Constructor class for nmfLogger
     */
    nmfLogger();
   ~nmfLogger();

    /**
     * @brief Get the log file name
     * @return The name of the output log file
     */
    std::string getLogFile();
    /**
     * @brief Get the current timestamp string
     * @param withUnderscores : boolean signifying if timestamp should have "_" instead of ":" characters
     * @return The timestamp string of the form: "year_month_day hour:min:sec"
     */
    std::string getTimestamp(bool withUnderscores);
    /**
     * @brief Initializes the logger with the passed in application name
     * @param appName : the name of the current application
     */
    void initLogger(std::string appName);
    /**
     * @brief Logs the passed in message
     * @param color : color code that the message should be written in
     * @param msg : the log message
     */
    void logMsg(int color,std::string msg);
};

#endif

