/**
 * @file nmfUtilsComplex.h
 * @brief These routines were originally from Complex2.bas
 * @date Dec 8, 2016
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multi_array.hpp>

#include "nmfUtils.h"
#include "nmfUtilsStatistics.h"

/**
 * @brief Routines used in MSVPA calculations
 */
namespace nmfUtilsComplex {

/**
 * @brief This subroutine perfroms a maximization of the user defined function USERFUNC
 * with NDim Dimenstions under inequality constrants of the form:
 * G(i) < X(i) <= H(i) for M parameter values, Xi. Matrix X has (n+1,Ndim) dimensions.
 * The constraint vectors and number of dimensions are input by the calling routine
 * The parameter values for the maximum, X() and the function value MaxF() are returned.
 * @param NDim
 * @param NYears
 * @param G
 * @param H
 * @param X
 * @param SRData
 * @param MaxF
 * @param VarEst
 * @param ItFlag
 */
void Complex(int NDim,
             int &NYears,
             boost::numeric::ublas::vector<double> &G,
             boost::numeric::ublas::vector<double> &H,
             boost::numeric::ublas::matrix<double> &X,
             boost::numeric::ublas::matrix<double> &SRData,
             double &MaxF,
             boost::numeric::ublas::vector<double> &VarEst,
             int &ItFlag);

void Cons(boost::numeric::ublas::matrix<double> &X,
          boost::numeric::ublas::matrix<double> &R,
          int &NDim,
          int &NYears,
          double &MaxF,
          boost::numeric::ublas::matrix<double> &SRData,
          boost::numeric::ublas::vector<double> &G,
          boost::numeric::ublas::vector<double> &H,
          boost::numeric::ublas::vector<double> &VarEst,
          int &ItFlag);
/**
 * @brief Explicit constraint violation correction. This checks independent
 * variables against explicit and implicit constraints -- and throws the
 * point back into the good region.
 * @param NDim
 * @param K
 * @param G
 * @param H
 * @param X
 */
void Check(int &NDim,
           int &K,
           boost::numeric::ublas::vector<double> &G,
           boost::numeric::ublas::vector<double> &H,
           boost::numeric::ublas::matrix<double> &X);
/**
 * @brief Calculates the centroid of the current set of points (X(i,j)). This
 * calculates the average dimensions of all but the lowest point.
 * @param IEV1
 * @param K
 * @param NDim
 * @param X
 * @param XC
 */
void Centroid(int &IEV1,
              int &K,
              int &NDim,
              boost::numeric::ublas::matrix<double> &X,
              boost::numeric::ublas::vector<double> &XC);

}


