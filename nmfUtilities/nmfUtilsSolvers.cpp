
/**
 @file nmfUtilsSolvers.cpp
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @date April 18, 2017
*/

#include "nmfUtilsSolvers.h"

namespace nmfUtilsSolvers {

void SolveF(const double Catch, const double N, const double M, double &F)
{
    int ItNum = 0;
    double E; // exploitation rate
    double UppF = 0.0;;
    double LowF = 0.0;;
    double tmpF = 0.0;;
    double Etemp = 0.0;;
    double Z = 0.0;;
    bool done = false;
    const int MaxIt = 10000; // RSK add to nmfConstants
    const double Tol = 0.0000000001;  // RSK add to nmfConstants

    // Calculate the exploitation rate
    E = Catch / N;

    // If its zero than F is zero..right ?
    if (E > Tol) {
        // Start at a really high F..since these are daily..don't have to start as high
        Etemp =  0.0;
        UppF  = 10.0;
        LowF  =  0.0;
        ItNum =  0;
        while ((! done) && (std::fabs(Etemp - E) > Tol)) {
            ++ItNum;
            Z = UppF + M;
            Etemp = (UppF / Z) * (1.0 - std::exp(-Z));

            // Evaluate the Etemp value
            // If its bigger than the input E...
            if (Etemp > E) { // Reduce the upper limit
                tmpF = UppF; // hold the previous upper value
                UppF = (UppF + LowF) / 2.0;
            } else { // bring the lower value up...
                LowF = UppF;
                UppF = tmpF;
            } // end if

            done = (ItNum + 1 > MaxIt);
        } // end while
    }

    // When you get out of the loop...you've solved for F..and you are done
    F = UppF;
}



} // end namespace nmfSolvers
