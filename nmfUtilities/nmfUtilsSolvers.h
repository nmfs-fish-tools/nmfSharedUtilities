/**
 * @file nmfUtilsSolvers.h
 * @brief This header file defines MSVPA_X2 utility functions.
 * @date Nov 29, 2016
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

#ifndef NMF_UTILS_SOLVERS
#define NMF_UTILS_SOLVERS

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

#endif
