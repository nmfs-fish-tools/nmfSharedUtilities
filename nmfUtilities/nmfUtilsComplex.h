/**
 @file nmfComplexUtils.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief These routines were originally from Complex2.bas
 @date June 9, 2017
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

namespace nmfUtilsComplex {

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

void Check(int &NDim,
           int &K,
           boost::numeric::ublas::vector<double> &G,
           boost::numeric::ublas::vector<double> &H,
           boost::numeric::ublas::matrix<double> &X);

void CENTR(int &IEV1,
           int &K,
           int &NDim,
           boost::numeric::ublas::matrix<double> &X,
           boost::numeric::ublas::vector<double> &XC);

}


