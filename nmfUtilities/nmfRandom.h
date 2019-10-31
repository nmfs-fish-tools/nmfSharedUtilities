#pragma once

/**
 @file nmfRandom.h
 @author rklasky
 @copyright 2018 NOAA - National Marine Fisheries Service
 @brief This file contains the Random number definition.
 @date Sept 19, 2018
*/

#include <iostream>
#include <ctime>
#include <random>
#include <chrono>


class nmfRandom {

private:
    std::uniform_real_distribution<double> dist;
    std::mt19937_64 rng;

public:
    nmfRandom(int seedValue, double lowerLimit, double upperLimit);
    nmfRandom();
   ~nmfRandom();

    void setLimits(double lowerLimit, double upperLimit);
    void setSeed(int seedValue);
    double value();

};


