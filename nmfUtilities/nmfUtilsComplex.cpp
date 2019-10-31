
/**
 @file nmfComplexUtils.cpp
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief These routines were originally from Complex2.bas
 @date June 9, 2017
*/


#include "nmfUtilsComplex.h"



namespace nmfUtilsComplex {


/*
  This subroutine perfroms a maximization of the user defined function
  USERFUNC with NDim Dimenstions under inequality constrants of the form:
  G(i) < X(i) <= H(i) for M parameter values, Xi.
  Matrix X has n+1,Ndim dimensions
  The constraint vectors and number of dimensions are input by the calling routine
  The parameter values for the maximum, X() and the function value MaxF() are returned
*/
void Complex(int NDim,
             int &NYears,
             boost::numeric::ublas::vector<double> &G,
             boost::numeric::ublas::vector<double> &H,
             boost::numeric::ublas::matrix<double> &X,
             boost::numeric::ublas::matrix<double> &SRData,
             double &MaxF,
             boost::numeric::ublas::vector<double> &VarEst, // Parms, in calling routine
             int &ItFlag)
{
std::cout << "in Complex" << std::endl;
    int K = NDim + 1;
    boost::numeric::ublas::matrix<double> R;

    nmfUtils::initialize(R,NDim+2,NDim+1);
    // Generate random numbers between 0 and 1 and fill into R matrix
    for (int i = 1; i <= K; ++i) {
        for (int j = 1; j <= NDim; ++j) {
            R(i,j) = nmfUtils::randomNumber(-1,0.0,1.0);
//            R(i,j) = nmfUtils::randomNumber();
        }
    }


std::cout << "R(1,1): " << R(1,1) << std::endl;

    // Do the maximization
    Cons(X, R, NDim, NYears, MaxF, SRData, G, H, VarEst, ItFlag);
std::cout << "out Complex" << std::endl;
} // end Complex




void Cons(boost::numeric::ublas::matrix<double> &X,
          boost::numeric::ublas::matrix<double> &R,
          int &NDim,
          int &NYears,
          double &MaxF,
          boost::numeric::ublas::matrix<double> &SRData,
          boost::numeric::ublas::vector<double> &G,
          boost::numeric::ublas::vector<double> &H,
          boost::numeric::ublas::vector<double> &VarEst,
          int &ItFlag)
{
    const int    ITMAX  = 500;      // Maximum number of iterations
    const double AALPHA =   1.3;    // Reflection factor
    const double BBETA  =   0.0001; // Convergence parameter
    const int    GAMMA  =   5;      // Convergence parameter

    int K = NDim+1; // Number of points in complex = NDim + 1
    int IT = 1;     // Iteration Index
    int IEV2;       // Index of point with maximum function value
    int IEV1;       // Index of point with minimum function value
    int KOUNT=0;
    int ITempLow=0;

    boost::numeric::ublas::vector<double> XC;
    boost::numeric::ublas::vector<double> Y;
    boost::numeric::ublas::vector<double> TempX;

    nmfUtils::initialize(XC,   NDim+1); // centroid
    nmfUtils::initialize(Y,    NDim+2); // holds function values for evaluation
    nmfUtils::initialize(TempX,NDim+1); // holds variable values for evaluation

    // Generate K starting points from random values
    for (int i = 1; i <= K; ++i) {
        for (int j = 1; j <= NDim; ++j) {
            X(i,j) = G(j) + R(i,j)*(H(j)-G(j));
        }
    }
    // Evaluate the function at these points
    for (int i = 1; i <= K; ++i) {
        for (int j = 1; j <= NDim; ++j) {
            TempX(j) = X(i,j);
        }
        nmfUtilsStatistics::USERFUNC(TempX, NYears, SRData, Y(i));
    }

    while (IT <= ITMAX) {

        // Find the point with the lowest value
        IEV1 = 1;
        for (int i = 1; i <= K; ++i) {
            if (Y(i) < Y(IEV1)) {
                IEV1 = i;
            }
        }

        // Find the point with the highest value
        IEV2 = 1;
        for (int i = 1; i <= K; ++i) {
            if (Y(i) > Y(IEV2)) {
                IEV2 = i;
            }
        }
        // There's a convergence check here.....i dont really get what its doing though
        // I think its counting the number of iterations in a row where the largest and smallest
        // function values are less than beta...resetting if it happens to pop back up

        if (Y(IEV2) - Y(IEV1) < BBETA) {
            ++KOUNT;
        }
        if (KOUNT > GAMMA)
            break;
        else
            KOUNT = 0;
        /*
        if (Y(IEV2) - Y(IEV1) < BBETA) {
            ++KOUNT;
            if (KOUNT > GAMMA)
                break;
        } else {
            KOUNT = 0;
        }
        */
        // Replace Point with Lowest function value - with the centroid of the other values
        CENTR(IEV1, K, NDim, X, XC);
        for (int j = 1; j <= NDim; ++j) {
            X(IEV1,j) = (1 + AALPHA) * (XC(j)) - AALPHA * (X(IEV1,j));
        }
        // Check to see if the points meet constraints - sub check fixes it if it doesn't
        Check(NDim, K, G, H, X);

        // Call the function for point IEV1 - and put it in the Y matrix
        for (int j = 1; j <= NDim; ++j) {
            TempX(j) = X(IEV1,j);
        }
        nmfUtilsStatistics::USERFUNC(TempX, NYears, SRData, Y(IEV1));

        // Check to see if Y(IEV1) is still the lowest value
        ITempLow = IEV1;

        // Evaluate the highest and lowest points as above
        // Find the point with the lowest value
        IEV1 = 1;
        for (int i = 1; i <= K; ++i) {
            if (Y(i) < Y(IEV1)) {
                IEV1 = i;
            }
        }

        // Find the point with the highest value
        IEV2 = 1;
        for (int i = 1; i <= K; ++i) {
            if (Y(i) > Y(IEV2)) {
                IEV2 = i;
            }
        }

        // If the new point is still the lowest point, then move it again
        if (IEV1 == ITempLow) {
            for (int j = 1; j <= NDim; ++j) {
                X(IEV1,j) = (X(IEV1,j) + XC(j)) / 2.0;
            }

            // Check the revised IEV1 for the boundary
            Check(NDim, K, G, H, X);

            // Evaluate the function and put it in the Y matrix
            for (int j = 1; j <= NDim; ++j) {
                TempX(j) = X(IEV1,j);
            }
            nmfUtilsStatistics::USERFUNC(TempX, NYears, SRData, Y(IEV1));
        } // end if IEV1

        // std::cout << "Iteration # " << IT << std::endl;
        // std::cout << "Centroid coords: " << XC(0) << ", " << XC(1) << ", " << XC(2) << std::endl;
        // std::cout << "Corrected point: " << X(IEV1,0) << ", " << X(IEV1,1) << ", " << X(IEV1,2) << std::endl;
        // std::cout << "Min LogLike Value: " << Y(IEV1) << std::endl;
        // std::cout << "Max Loglike Value: " << Y(IEV2) << std::endl;

        ++IT;
    } // end while IT

    MaxF = Y(IEV1);

    for (int i = 1; i <= NDim; ++i) {
        VarEst(i) = X(IEV1,i);
    }

    ItFlag = (IT > ITMAX) ? 1 : 0;

} // end Cons




void CENTR(int &IEV1,
           int &K,
           int &NDim,
           boost::numeric::ublas::matrix<double> &X,
           boost::numeric::ublas::vector<double> &XC)
{
    // Calculates the centroid of the current set of points (X(i,j))
    //double Sum;

    // This calculates the average dimensions of all but the lowest point...
    for (int j = 1; j <= NDim; ++j) {
        XC(j) = 0.0;
        for (int i = 1; i <= K; ++i) {
            if (i != IEV1) {
                XC(j) += X(i,j);
            }
        }
        XC(j) /= (K-1);
    }

} // end CENTR


void Check(int &NDim,
           int &K,
           boost::numeric::ublas::vector<double> &G,
           boost::numeric::ublas::vector<double> &H,
           boost::numeric::ublas::matrix<double> &X)
{

    // Explicit constraint violation correction
    // This checks independent variables against explicit and implicit constraints -- and
    // throws the point back into the good region
    const double DELTA = 0.1;

    // Check against explicit constraints
    for (int i = 1; i <= K; ++i) {

        for (int j = 1; j <= NDim; ++j) {

            if (X(i,j) < G(j)) {
                X(i,j) = G(j) + DELTA;
            }

            if (X(i,j) >= H(j)) {
                X(i,j) = H(j) - DELTA;
            }

        } // end for j

    } // end i

} // end Check




} // end namespace nmfComplexUtils

