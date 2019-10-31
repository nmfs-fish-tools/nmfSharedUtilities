#pragma once


/**
 @file nmfLogger.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This file contains the Logger definition.
 @date Apr 03, 2017
*/


#include <iostream>
#include <ctime>
#include <stdio.h>
#include <fstream>




class nmfLogger {

private:
    std::string filename="";


public:

    nmfLogger();
    ~nmfLogger();

   std::string getTimestamp(bool withUnderscores);
   void initLogger(std::string appName);
   std::string logFile();
   void logMsg(int color,std::string msg);

};



