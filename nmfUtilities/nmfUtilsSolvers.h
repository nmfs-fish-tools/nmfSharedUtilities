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

#include <iostream>
#include <string>
#include <vector>
#include <map>


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multi_array.hpp>

typedef boost::multi_array<double, 3> Boost3DArrayDouble;
typedef boost::multi_array<double, 4> Boost4DArrayDouble;
typedef boost::multi_array<double, 5> Boost5DArrayDouble;


namespace nmfUtilsSolvers {

/*
 * An iterative solution for a mortality rate (F) given the initial population
 * size N, the total number removed (catch) during the time period, and the
 * total "other mortality", M.
 */
void SolveF(const double Catch, const double N, const double M, double &F);


}

