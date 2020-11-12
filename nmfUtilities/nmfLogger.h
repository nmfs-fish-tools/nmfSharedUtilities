/**
 * @file nmfLogger.h
 * @brief Definition for the MultiSpecies Logger class
 * @date Jan 14, 2020
 *
 * This file defines the MS Logger class.  This class allows the user to
 * log (i.e., save) normal output statements, warnings, or errors to an
 * output timestamped log file.
 *
 */
#pragma once

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



