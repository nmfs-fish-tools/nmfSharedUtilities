/**
 * @file nmfUtilsSolvers.h
 * @brief This header file defines MSVPA_X2 utility functions.
 * @date Nov 29, 2016
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


/**
 * @brief Namespace for MSVPA_X2 solver functions
 */
namespace nmfUtilsSolvers {

/**
 * @brief An iterative solution for a mortality rate (F) given the initial parameters
 * @param Catch : the total number (of fish) removed during the time period
 * @param N : the initial population size
 * @param M : the total "other mortality"
 * @param F : the solved mortality rate
 */
void SolveF(const double Catch,
            const double N,
            const double M,
            double &F);

}

