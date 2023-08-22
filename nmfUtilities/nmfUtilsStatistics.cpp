
/**
 @file nmfStatUtils.cpp
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This code should be re-written with better variable names or replaced with math libraries.  It's just a straight conversion from the original code.
 @date Jan 24, 2017
*/

#include "nmfUtilsStatistics.h"

namespace nmfUtilsStatistics {

void ShepSRR(boost::numeric::ublas::matrix<double> &Data,
             int &NYears,
             NLRegOut &NLOut)
{
    int ItFlag;
    int KInd = 0;
    double RegVal;
    double RegSS;
    LinRegOut RegressOut;

    boost::numeric::ublas::vector<double> Parms;
    boost::numeric::ublas::vector<double> G;
    boost::numeric::ublas::vector<double> H;
    boost::numeric::ublas::matrix<double> X;
    boost::numeric::ublas::matrix<double> SRData;
    //boost::numeric::ublas::vector<double> tmpData;
    boost::numeric::ublas::matrix<double> tmpData2;
    std::vector<double> tmpData;

    nmfUtils::initialize(Parms,4);
    nmfUtils::initialize(G,4);
    nmfUtils::initialize(H,4);
    nmfUtils::initialize(X,5,5);
    nmfUtils::initialize(SRData,NYears+1,2);
    //nmfUtils::initialize(tmpData,NYears+1);
    nmfUtils::initialize(tmpData2,int(NYears*0.1+0.5)+2,2);

    //int N = NYears;
    for (int i = 0; i <= NYears-1; ++i) {
        SRData(i,0) = Data(i,0);
        SRData(i,1) = Data(i,1);
    }

    // Initial Guess for params...may try to get fancy here
    // Parms(1) = 3
    // Parms(2) = 20
    // Parms(3) = 1


    // Need to give it reasonable constraints to keep things
    // from getting out of hand..use the descriptions of
    // how to calculate the params in Shepherd 1982 to give
    // it reasonable first guesses and bound around those values

    // A is basically the slope at the origin.  Shepherd recs fitting a line through
    // the origin and the 10% of the points with the lowest biomass..slope = A
    // basically the density-independant effect.
    for (int i = 0; i <= NYears - 1; ++i) {
        //tmpData(i) = SRData(i,0);
        tmpData.push_back(SRData(i,0));
    }

    // Sort the vector
    std::sort(tmpData.begin(),tmpData.end());

    // Put the B and R values for the first 10% of the points in tmpdata2..include 0
    for (int i = 0; i <= int(NYears * 0.1+0.5); ++i) {
        for (int j = 0; j <= NYears; ++j) {
            if (SRData(j,0) == tmpData[i]) {
                tmpData2(i,0) = tmpData[i];
                tmpData2(i,1) = SRData(j,1);
            } // end if
        } // end for j
    } // end for i

    int tmpVal = int(NYears*0.1+0.5)+2;
    LinReg(tmpData2, tmpVal, RegressOut); // this fills the public variable regress with all the info you need

    // OK...so that works pretty good.  RegressOut.Beta is basically the slope at the origin.
    // Could force through zero..but I'm not sure it matters

    // To get a good guess for K..find the level of SSB giving R about 1/2 the level predicted by the regression you just fit
    // .....but need to do this on the sorted matrix...
    nmfUtils::initialize(tmpData2,NYears+1,2);

    // Match values into the sorted matrix
    for (int i = 0; i <= NYears-1; ++i) {
        for (int j = 0; j <= NYears-1; ++j) {
            if (SRData(j,0) == tmpData[i]) {
                tmpData2(i,0) = tmpData[i];
                tmpData2(i,1) = SRData(j,1);
            } // end if
        } // end for j
    } // end for i

    // Look till you find a biomass that is <= 0.5*RegVal
    bool done = false;
    int i=0;
    while ((i <= NYears-1) && (! done)) {
        RegVal = RegressOut.Alpha + RegressOut.Beta * tmpData2(i,0);
        if (tmpData2(i,1) <= 0.5*RegVal) {
            KInd = i;
            done = true;
        } // end if
        ++i;
    } // end while i

    // Set lower bounds

    G(0) = 0.0;
    G(1) = RegressOut.Beta   * 0.5; // A - lower bound is 1/2 the slope at the origin
    G(2) = tmpData2(KInd, 0) * 0.5; // K - lower bound is 1/2 observed level calculated above
    G(3) = 0.5; // B

    // Set upper bound
    H(0) = 0.0; // usused value; using 1-3 instead of 0-2 for ease of code conversion
    H(1) = RegressOut.Beta  * 4.0; // 4x the slope at the origin
    H(2) = tmpData2(KInd,0) * 2.0; // Twice the observed level
    H(3) = 2.5;

    // Call the fitter...
    nmfUtilsComplex::Complex(3, NYears, G, H, X,
                             SRData, RegSS, Parms, ItFlag);
    RegSS = -RegSS;
    // WOW...it works...more or less...


    // Need to calculate sums of squares and stuff...
    double SumX  = 0.0;
    double SumY  = 0.0;
    double SumX2 = 0.0;
    double SumXY = 0.0;
    double SumY2 = 0.0;
    double SSY;
    double SSErr;

    // First calculate sum of Xs, Ys, and XY products
    for (int i = 0; i <= NYears - 1; ++i) {
        SumX  +=  SRData(i,0);
        SumX2 += (SRData(i,0) * SRData(i,0));
        SumY  +=  SRData(i,1);
        SumY2 += (SRData(i,1) * SRData(i,1));
        SumXY += (SRData(i,0) * SRData(i,1));
    }


    // Calculate means
    //MeanX = SumX / NYears; // never used
    //MeanY = SumY / NYears; // never used

    // Calculate sums of squares
    // SSX = SumX2 - ((SumX*SumX) / NYears); // was never used
    // SSxy = SumXY - ((SumX * SumY) / N)

    // Calculate total and regression SSs, F-ratio for regression, and R2
    SSY   = SumY2 - ((SumY*SumY) / NYears); // total sum of squares - total error
    SSErr = SSY - RegSS;  // residual error

    NLOut.A = Parms(1);
    NLOut.K = Parms(2);
    NLOut.B = Parms(3);
    NLOut.RegDf = 3;
    NLOut.RegMS = RegSS / NLOut.RegDf;
    NLOut.ErrorDF = NYears - (3 + 1);
    NLOut.ErrorMS = SSErr / NLOut.ErrorDF;
    NLOut.Rsquare = RegSS / SSY;
    NLOut.ItFlag = ItFlag;
    if (NLOut.ErrorMS > 0) {
        NLOut.ResSE  = std::sqrt(NLOut.ErrorMS);
        NLOut.F      = NLOut.RegMS / NLOut.ErrorMS;
        NLOut.ProbF  = ProbF(NLOut.F, NLOut.RegDf, NLOut.ErrorDF);
    } else {
        NLOut.ResSE  = -9;
        NLOut.F      =  0;
        NLOut.ProbF  = -9;
        NLOut.ItFlag =  2;
    }

} // end ShepSRR



/*
 * This is the function to calculate the sums of squares against the
 * actual data and return -SS to find a minimum.
 */
void USERFUNC(boost::numeric::ublas::vector<double> &Parms,
              int &NYears,
              boost::numeric::ublas::matrix<double> &SRData,
              double &Value)
{
    double A;
    double B;
    double K;
    double Diff2;
    double SumSquares;
    boost::numeric::ublas::vector<double> PredVal;

    nmfUtils::initialize(PredVal,NYears);

    A = Parms(1);
    K = Parms(2);
    B = Parms(3);

    SumSquares = 0.0;
    for (int i = 0; i <= NYears-1; ++i) {
        PredVal(i) = (A * SRData(i,0)) / (1.0 + std::pow((SRData(i,0)/K),B));
        Diff2 = std::pow((PredVal(i) - SRData(i, 1)), 2.0);
        SumSquares += Diff2;
    }

    Value = -SumSquares; // since we are minimizing here

} // end USERUNC


/*
  This is a public procedure to calculate linear regression coefficients and
  significance tests.  Results are output to a publicly declared variable type LinReg that holds the outputs.
  This should be declared both as a type and public variable in the declarations section of the users code.
  This procedure accepts an Nx2 matrix of double data containing X and Y values and the Number of data points, N.
  Any transformations for linearity should be done prior to submitting the data to the procedure.
  N should be > 2.
*/
void LinReg(boost::numeric::ublas::matrix<double> &Data, int &N, LinRegOut &OutVar)
{
    double SumX  = 0.0; // Sum of X values
    double SumY  = 0.0; // Sum of Y values
    double MeanX = 0.0;
    double MeanY = 0.0;
    double SumX2 = 0.0; // Sum of squared Xs
    double SumXY = 0.0; // Sum of products of X and Y
    double SumY2 = 0.0; // Sum of squared Ys
    double SSX   = 0.0; // Sum of squares for X
    double SSY   = 0.0; // Sum of squares for Y
    double SSxy  = 0.0; // Sum of crossprodcut deviations for xy
    double SSReg = 0.0; // Regression sum of squares
    double SSErr = 0.0; // Error sum of squares

    if (N <= 2) {
        std::cout << "Error: LinReg: There should be greater than 2 x and y values." << std::endl;
        return;
    }

    // First calculate sum of Xs, Ys, and XY products
    for (int i = 0; i<N ; ++i) {
        SumX  += Data(i,0);
        SumX2 += Data(i,0) * Data(i,0);
        SumY  += Data(i,1);
        SumY2 += Data(i,1) * Data(i,1);
        SumXY += Data(i,0) * Data(i,1);
    }

    // Calculate means
    MeanX = SumX / N;
    MeanY = SumY / N;

    // Calculate sums of squares
    SSX  = SumX2 - ((SumX*SumX) / N);
    SSxy = SumXY - ((SumX*SumY) / N);

    // Calculate regression coefficients
    OutVar.Beta  = SSxy / SSX;
    OutVar.Alpha = MeanY - OutVar.Beta * MeanX;

    // Calculate total and regression SSs, F-ratio for regression, and R2
    SSY   = SumY2 - (SumY *SumY) / N; // total sum of squares
    SSReg = OutVar.Beta * SSxy;       // regression sum of sruares
    SSErr = SSY - SSReg;

    OutVar.RegDf   = 1;
    OutVar.RegMS   = SSReg;
    OutVar.ErrorDF = N - 2;
    OutVar.ErrorMS = SSErr / OutVar.ErrorDF;
    OutVar.F       = OutVar.RegMS / OutVar.ErrorMS;
    OutVar.Rsquare = SSReg / SSY;
    OutVar.ResSE   = std::sqrt(OutVar.ErrorMS);
    OutVar.MeanX   = MeanX;
    OutVar.SSX     = SSX;
    OutVar.N       = N;
    // Calculate SEs of parmeters
    OutVar.BetaSE  = OutVar.ResSE / std::sqrt(SSX);
    OutVar.AlphaSE = std::sqrt(OutVar.ResSE*OutVar.ResSE * (1.0/N + (MeanX*MeanX)/SSX));

    // Calculate t-values
    OutVar.BetaT   = OutVar.Beta  / OutVar.BetaSE;
    OutVar.AlphaT  = OutVar.Alpha / OutVar.AlphaSE;

} // end LinReg


double BetaI(double A, double B, double X)
{
    double BT = 0;
    double retv = 0.0;

    if ((X == 0) || (X == 1))
        BT = 0;
    else {
        try {
            BT = std::exp(GAMMLN(A + B) - GAMMLN(A) - GAMMLN(B) + (A * std::log(X))	+ (B * std::log(1.0 - X)));
        } catch (...) {
            std::cout << "Error BetaI-1: " << std::endl;
        }
    }

    try {
        if (X < ((A + 1.0) / (A + B + 2.0))) {
            retv = BT * BetaCF(A, B, X) / A;
        } else {
            retv = 1.0 - BT * BetaCF(B, A, 1.0 - X) / B;
        }
    } catch (...) {
        std::cout << "Error BetaI-2: " << std::endl;
    }

    return retv;

} // end BetaI


double BetaCF(double A, double B, double X) {
    const double EPS = 0.0000003;
    const int ITMAX = 100;

    int M = 0;
    int EM;
    int TEM;
    double d;
    double Ap;
    double Bp;
    double App;
    double Bpp;
    double Aold;
    double AM = 1.0;
    double BM = 1.0;
    double AZ = 1.0;
    double QAB = A + B;
    double QAP = A + 1.0;
    double QAM = A - 1.0;
    double BZ = 1.0 - QAB * X / QAP;
    double retv = NAN;

    bool done = false;
    while ((M < ITMAX) && (! done))
    {

        ++M;
        EM = M;
        TEM = EM + EM;

        try {
            d = EM * (B - M) * X / ((QAM + TEM) * (A + TEM));
            Ap = AZ + d * AM;
            Bp = BZ + d * BM;
            d = -(A + EM) * (QAB + EM) * X / ((A + TEM) * (QAP + TEM));
            App = Ap + d * AZ;
            Bpp = Bp + d * BZ;
            Aold = AZ;
            AM = Ap / Bpp;
            BM = Bp / Bpp;
            AZ = App / Bpp;
            BZ = 1.0;
        } catch (const std::exception& e) {
            std::cout << "Error BetaCF: " << e.what() << std::endl;
            retv = 1.0;
            done = true;
        } // end try...catch

        if (std::fabs(AZ - Aold) < (EPS * std::fabs(AZ))) {
            retv = AZ;
            done = true;
        }

    } // end while M

    if (! done) {
        std::cout << "\nBetaCF error: ItMaxReached" << std::endl;
        retv = AZ; // RSK not sure what to return in this case
    }

    return retv;

} // end BetaCF


double GAMMLN(double XX) {
    double retv;
    double X;
    double tmp;
    double SER;
    double COF[6];

    const double STP = 2.50662827465;

    COF[0] =  76.18009173;
    COF[1] = -86.50532033;
    COF[2] =  24.01409822;
    COF[3] =  -1.231739516;
    COF[4] =   0.00120858003;
    COF[5] =  -0.00000536382;

    X = XX - 1.0;
    tmp = X + 5.5;
    tmp = (X + 0.5) * std::log(tmp) - tmp;

    SER = 1.0;

    try {
        for (int j = 0; j < 6; ++j) {
            X = X + 1.0;
            SER += COF[j] / X;
        }
        retv = tmp + std::log(STP * SER);
    } catch (const std::exception& e) {
        std::cout << "Error GAMMLN: " << e.what() << std::endl;
        retv = 0.0;
    } // end try...catch

    return retv;

} // end GAMMLN



// Does the factorial...can still overflow with large values
double FACTRL(int N)
{
    int NTop;
    double A[33];
    double retv = 0.0;

    //On Error GoTo Errhandler
    NTop = 0;
    A[1] = 1;

    try {
        if (N <= NTop) {
            retv = A[N];
        } else {
            if (N <= 32) {
                for (int i = NTop + 1; i <= N; ++i) {
                    A[i+1] = A[i] * i;
                }
                NTop = N;
                retv = A[N + 1];
            } else {
                retv = std::exp(GAMMLN(N + 1));
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error FACTRL: " << e.what() << std::endl;
        retv = 0.0;
    }

    return retv;
} // end FACTRL


/*
 * Returns the log factorial so that you can do really big numbers, for
 * example in the binomial or log-likelihood calcs
 */
double FACTLN(int N) {
    return GAMMLN(N + 1);
} // end FACTLN


double BETAFUNC(double A, double B) {
    return std::exp(GAMMLN(A) + GAMMLN(B) - GAMMLN(A + B));
} // end BETAUNC


double ProbF(double F, int DF1, int DF2)
{
    double A;
    double B;
    double X;
    double Val1;
    double Val2;

    A = 0.5 * DF2;
    B = 0.5 * DF1;
    X = DF2 / (DF2 + DF1 * F);

    Val1 = BetaI(A, B, X);

    X = DF1 / (DF1 + DF2 / F);
    Val2 = BetaI(B, A, X);

    return (Val1 + (1 - Val2)) / 2;

} // end ProbF


double ProbT(double T, int DF) {
    return BetaI(0.5 * DF, 0.5, DF / (DF + T * T));
} // end ProbT

/*
void calculateSSResiduals(const int& NumSpeciesOrGuilds,
                          const int& RunLength,
                          const std::vector<double>& Observed,
                          const std::vector<double>& Estimated,
                          std::vector<double>& SSResiduals)
{
    int m = 1;
    int numObservations=0;
    double sum = 0;
    double diff = 0;

    SSResiduals.clear();
    for (int species=0;species<NumSpeciesOrGuilds;++species) {
        sum = 0;
        for (int time=1; time<=RunLength;++time) {
            if (Observed[m] != nmfConstantsMSSPM::NoData) {
                diff = Observed[m] - Estimated[m];
                sum += diff*diff;
                ++numObservations;
            }
            ++m;
        }
        SSResiduals.push_back(sum);
    }
}
*/
void calculateSSResiduals(const boost::numeric::ublas::matrix<double> & Observed,
                          const boost::numeric::ublas::matrix<double> & Estimated,
                          const bool& SkipFirstYear,
                          std::vector<double>& SSResiduals,
                          std::string& errorMsg)
{
    int StartRow = (SkipFirstYear) ? 1 : 0;
    int NumRowsObs = Observed.size1();
    int NumColsObs = Observed.size2();
    int NumRowsEst = Estimated.size1();
    int NumColsEst = Estimated.size2();
    double sum = 0;
    double diff = 0;

    errorMsg.clear();
    if ((NumRowsObs != NumRowsEst) || (NumColsObs != NumColsEst)) {
        errorMsg = "Error calculateSSResiduals: Matrix mismatch";
        return;
    }

    SSResiduals.clear();
    for (int col=0;col<NumColsObs;++col) {
        sum = 0;
        for (int row=StartRow; row<NumRowsObs; ++row) {
            if (Observed(row,col) != nmfConstantsMSSPM::NoData) {
                diff = Observed(row,col) - Estimated(row,col);
                sum += diff*diff;
            }
        }
        SSResiduals.push_back(sum);
    }
}

bool calculateSSDeviations(const boost::numeric::ublas::matrix<double>& Observed,
                           const boost::numeric::ublas::matrix<double>& Estimated,
                           const std::vector<double>& Means,
                           const bool& SkipFirstYear,
                           std::vector<double>& SSDeviations,
                           std::string& errorMsg)
{
    int StartRow = (SkipFirstYear) ? 1 : 0;
    int NumRowsObs = Observed.size1();
    int NumColsObs = Observed.size2();
    int NumRowsEst = Estimated.size1();
    int NumColsEst = Estimated.size2();
    double sum = 0;
    double diff = 0;

    errorMsg.clear();
    if ((NumRowsObs != NumRowsEst) || (NumColsObs != NumColsEst)) {
        errorMsg = "Error calculateSSDeviations: Matrix mismatch";
        return false;
    }

    SSDeviations.clear();
    for (int col=0;col<NumColsObs;++col) {
        sum = 0;
        for (int row=StartRow; row<NumRowsObs; ++row) {
            diff = Estimated(row,col) - Means[col];
            sum += diff*diff;
        }
        if (sum == 0) {
            errorMsg = "Error calculateSSDeviations: Found SSdeviation sum of 0";
            return false;
        }
        SSDeviations.push_back(sum);
    }

    return true;
}

void calculateSSTotals(const std::vector<double>& SSDeviations,
                       const std::vector<double>& SSResiduals,
                       std::vector<double>& SSTotal,
                       std::string& errorMsg)
{
    int NumDeviations = SSDeviations.size();
    int NumResiduals  = SSResiduals.size();

    errorMsg.clear();
    if (NumDeviations != NumResiduals) {
        errorMsg = "Error calculateSSTotals: Vector mismatch";
        return;
    }

    SSTotal.clear();
    for (int row=0;row<NumDeviations;++row) {
        SSTotal.push_back(SSDeviations[row]+SSResiduals[row]);
    }
}

void calculateRSquared(const std::vector<double>& SSDeviations,
                       const std::vector<double>& SSTotals,
                       std::vector<double>& RSquared,
                       std::string& errorMsg)
{
    int NumDeviations = SSDeviations.size();
    int NumTotals     = SSTotals.size();

    errorMsg.clear();
    if (NumDeviations != NumTotals) {
        errorMsg = "Error calculateRSquared: Vector mismatch";
        return;
    }

    RSquared.clear();
    for (int row=0; row<NumDeviations; ++row) {
        if (SSTotals[row] == 0) {
            errorMsg = "Error calculateRSquared: Found an SSTotal of 0 for species: " + std::to_string(row);
            return;
        }
        RSquared.push_back(SSDeviations[row]/SSTotals[row]);
    }
}

void calculateAIC(const std::vector<int>& NumParameters,
                  const std::vector<int>& NumYearsWithBlanks,
                  const std::vector<double>& SSResiduals,
                  std::vector<double>& AIC)
{
    int NumYears;
    int NumSpecies =  SSResiduals.size();

    AIC.clear();
    for (int species=0;species<NumSpecies;++species) {
        NumYears = NumYearsWithBlanks[species];
        if (SSResiduals[species] == 0) {
            AIC.push_back(0.0);
        } else {
            // AIC (per species i) = nᵢln(SSR/nᵢ) + 2Kᵢ
            AIC.push_back(NumYears*std::log(SSResiduals[species]/NumYears) + 2 * NumParameters[species]);
        }
    }
}

void calculateAIC(const std::vector<int>& NumParameters,
                  const int& TotalNumYearsWithBlanks,
                  const std::vector<double>& SSResiduals,
                  double& AIC)
{
    int NumSpecies = SSResiduals.size();
    int sum_K = 0;
    int sum_N = 0;
    double SSR = 0;

//    sum_N = NumYears * NumSpecies;
    sum_N = TotalNumYearsWithBlanks;

    for (int species=0;species<NumSpecies;++species) {
        SSR   += SSResiduals[species];
        sum_K += NumParameters[species];
    }

    // AIC (per model) = Σnᵢ * ln(ΣSSR/Σnᵢ) + 2ΣKᵢ
    AIC = (sum_N*std::log(SSR/sum_N) + 2 * sum_K);
}


bool calculateR(
        const bool& SkipFirstYear,
        const int& NumSpeciesOrGuilds,
        const std::vector<double>& MeanObserved,
        const std::vector<double>& MeanEstimated,
        const boost::numeric::ublas::matrix<double>& Observed,
        const boost::numeric::ublas::matrix<double>& Estimated,
        std::vector<double>& R)
{
    int NumYears  = Observed.size1();
    int FirstYear = (SkipFirstYear) ? 1 : 0;
    double num,den;
    double diffObs,diffEst;
    double sumObs,sumEst;

    R.clear();
    for (int species=0;species<NumSpeciesOrGuilds;++species) {
        num = 0;
        sumObs = 0;
        sumEst = 0;
        for (int time=FirstYear; time<NumYears; ++time) {
            if (Observed(time,species) != nmfConstantsMSSPM::NoData) {
                diffObs = (Observed(time,species)  - MeanObserved[species]);
                diffEst = (Estimated(time,species) - MeanEstimated[species]);
                num    += diffObs*diffEst;
                sumObs += diffObs*diffObs;
                sumEst += diffEst*diffEst;
            }
        }
        den = sqrt(sumObs*sumEst);
        if (den == 0) {
            R.push_back(-99999);
//            return false;
        }
        R.push_back(num/den);
    }

    return true;
}

bool calculateRMSE(
        const bool& SkipFirstYear,
        const int& NumSpeciesOrGuilds,
        const boost::numeric::ublas::matrix<double>& Observed,
        const boost::numeric::ublas::matrix<double>& Estimated,
        std::vector<double>& RMSE)
{
    int FirstYear = (SkipFirstYear) ? 1 : 0;
    double sum;
    double diff;
    int numYearsWithoutBlanks = 0;

    for (int species=0; species<NumSpeciesOrGuilds; ++species) {
        sum = 0;
        numYearsWithoutBlanks = 0;
        for (unsigned time=FirstYear; time<Observed.size1(); ++time) {
            if (Observed(time,species) != nmfConstantsMSSPM::NoData) {
                diff = Estimated(time,species) - Observed(time,species);
                sum += diff*diff;
                ++numYearsWithoutBlanks;
            }
        }
        if (numYearsWithoutBlanks > 0) {
            RMSE.push_back(sqrt(sum/numYearsWithoutBlanks));
        } else {
            RMSE.push_back(-99999);
        }
    }

    return true;
}

bool calculateRI(
        const bool& SkipFirstYear,
        const int& NumSpeciesOrGuilds,
        const boost::numeric::ublas::matrix<double>& Observed,
        const boost::numeric::ublas::matrix<double>& Estimated,
        std::vector<double>& RI)
{
    int FirstYear = (SkipFirstYear) ? 1 : 0;
    int numYearsWithoutBlanks = 0;
    double sum;
    double val;

    RI.clear();
    for (int species=0; species<NumSpeciesOrGuilds; ++species) {
        sum = 0;
        numYearsWithoutBlanks = 0;
        for (unsigned time=FirstYear; time<Observed.size1(); ++time) {
            if ((Observed(time,species) != nmfConstantsMSSPM::NoData) &&
                (Observed(time,species) != 0)){
                val  = log(Observed(time,species)/Estimated(time,species));
                sum += val*val;
                ++numYearsWithoutBlanks;
            }
        }
        if (numYearsWithoutBlanks > 0) {
            RI.push_back(exp(sqrt((1.0/numYearsWithoutBlanks)*sum)));
        } else {
            RI.push_back(-99999);
        }
    }

    return true;
}

void calculateAE(const int& NumSpeciesOrGuilds,
                 const std::vector<double>& MeanObserved,
                 const std::vector<double>& MeanEstimated,
                 std::vector<double>& AE)
{
    AE.clear();
    for (int i=0; i<NumSpeciesOrGuilds; ++i) {
        AE.push_back(MeanEstimated[i]-MeanObserved[i]);
    }
}

bool calculateAAE(
        const bool& SkipFirstYear,
        const int& NumSpeciesOrGuilds,
        const boost::numeric::ublas::matrix<double>& Observed,
        const boost::numeric::ublas::matrix<double>& Estimated,
        std::vector<double>& AAE)
{
    int numYearsWithoutBlanks = 0;
    int FirstYear = (SkipFirstYear) ? 1 : 0;
    double sum;

    AAE.clear();
    for (int species=0; species<NumSpeciesOrGuilds; ++species) {
        sum = 0;
        numYearsWithoutBlanks = 0;
        for (unsigned time=FirstYear; time<Observed.size1(); ++time) {
            if (Observed(time,species) != nmfConstantsMSSPM::NoData) {
                sum += fabs(Estimated(time,species) - Observed(time,species));
                ++numYearsWithoutBlanks;
            }
        }
        if (numYearsWithoutBlanks > 0) {
            AAE.push_back(sum/numYearsWithoutBlanks);
        } else {
            AAE.push_back(-99999);
        }
    }

    return true;
}

bool calculateMEF(
        const bool& SkipFirstYear,
        const int& NumSpeciesOrGuilds,
        const std::vector<double>& MeanObserved,
        const boost::numeric::ublas::matrix<double>& Observed,
        const boost::numeric::ublas::matrix<double>& Estimated,
        std::vector<double>& MEF)
{
    int FirstYear = (SkipFirstYear) ? 1 : 0;
    bool retv = true;
    double sum1,sum2;
    double diff1,diff2;

    MEF.clear();
    for (int species=0; species<NumSpeciesOrGuilds; ++species) {
        sum1 = sum2 = 0;
        for (unsigned time=FirstYear; time<Observed.size1(); ++time) {
            if (Observed(time,species) != nmfConstantsMSSPM::NoData) {
                diff1 = Observed(time,species)  - MeanObserved[species];
                sum1 += diff1*diff1;
                diff2 = Estimated(time,species) - Observed(time,species);
                sum2 += diff2*diff2;
            }
        }
        if (nmfUtils::isNearlyZero(sum1)) {
            MEF.push_back(-999999);
            retv = false;
        } else {
            MEF.push_back(1.0-sum2/sum1);
        }
    }
    return retv;
}


bool calculateMohnsRhoFor1dParameter(
        std::vector<boost::numeric::ublas::matrix<double> >& VectorOfParameterMatrices,
        std::vector<double>& mohnsRhoVector)
{
    int NumPeels   = VectorOfParameterMatrices.size();
    int NumSpecies = VectorOfParameterMatrices[0].size1();
    double den;
    double value;
    double aveValue=0;

    if (NumPeels == 0) {
        return false;
    }

    // Est Parameters are organized from Max Num Peels to 0.
    // Ex. If NumSpecies = 4, and NumPeels = 3, there should be
    // 16 est params listed as follows in the vector:
    // peel=0, peel=1, peel=2, peel=3
    mohnsRhoVector.clear();
    for (int species=0; species<NumSpecies; ++species) {
        value = 0;
        den = VectorOfParameterMatrices[0](species,0);
        if (den == 0) {
            return false;
        }
        for (int peel=1; peel<NumPeels; ++peel)  {
            value += (VectorOfParameterMatrices[peel](species,0) - den)/den;
        }
        value /= (NumPeels-1);
        aveValue += value;
        mohnsRhoVector.push_back(value);
    }
    aveValue /= NumSpecies;
    mohnsRhoVector.push_back(aveValue);

    return true;
}

bool calculateMohnsRhoFor2dParameter(
        std::vector<boost::numeric::ublas::matrix<double> >& VectorOfParameterMatrices,
        std::vector<double>& mohnsRhoVector)
{
    int NumPeels   = VectorOfParameterMatrices.size();
    int NumSpecies = VectorOfParameterMatrices[0].size1();
    int NumColumns = VectorOfParameterMatrices[0].size2();
    int numNonZeroSubParameters;
    double den;
    double value;
    double aveValue=0;

    if ((NumPeels == 0) || (NumSpecies != NumColumns)) {
        return false;
    }

    // Est Parameters are organized from Max Num Peels to 0.
    // Ex. If NumSpecies = 4, and NumPeels = 3, there should be
    // 16 est params listed as follows in the vector:
    // peel=0, peel=1, peel=2, peel=3
    mohnsRhoVector.clear();
    for (int species=0; species<NumSpecies; ++species) {
        numNonZeroSubParameters = 0;
        value = 0;
        for (int col=0; col<NumColumns; ++col) {
            den = VectorOfParameterMatrices[0](species,col);
            if (den != 0) { // Some of these subparameters may be 0 (i.e., unused)
                for (int peel=1; peel<NumPeels; ++peel)  {
                    value += (VectorOfParameterMatrices[peel](species,col) - den)/den;
                    ++numNonZeroSubParameters;
                }
            }
        }
        value /= numNonZeroSubParameters;
        aveValue += value;
        mohnsRhoVector.push_back(value);
    }
    aveValue /= NumSpecies;
    mohnsRhoVector.push_back(aveValue);

    return true;
}

bool calculateMohnsRhoForTimeSeries(
        const int& NumPeels,
        const int& NumSpecies,
        const std::vector<std::vector<double> >& TimeSeries,
        std::vector<double>& mohnsRhoVector)
{
    int num=0;
    int timeSeriesPeeledLength;
    double den;
    double value=0;
    double aveValue=0;

    if ((TimeSeries.size() == 0) || (NumPeels < 1) || (NumSpecies == 0)) {
        return false;
    }

    mohnsRhoVector.clear();

    for (int species=0; species<NumSpecies; ++species) {
        num = 0;
        for (int peel=1; peel<=NumPeels; ++peel) { // Ex. 4: skip 0, then 1, 2, 3
            timeSeriesPeeledLength = (int)TimeSeries[peel-1].size() - peel;
            for (int i=0; i<timeSeriesPeeledLength; ++i) {
                den = TimeSeries[0][i];
                if (den != 0) {
                    value += (TimeSeries[peel-1][i] - den)/den;
                    ++num;
                }
            }
        }
        value /= num;
        aveValue += value;
        mohnsRhoVector.push_back(value);
    }
    aveValue /= NumSpecies;
    mohnsRhoVector.push_back(aveValue);

    return true;
}


double calculateMean(
        const bool& skipFirstYear,
        const bool& useLogData,
        const boost::numeric::ublas::matrix<double>& ObsBiomass,
        const int speciesNum)
{
    int FirstYear = (skipFirstYear) ? 1 : 0;
    int numYears = ObsBiomass.size1();
    int numYearsWithData = 0; // Not equal to numYears because there may be blanks in observed biomass
    double sumObsBiomassPerSpecies = 0.0;

    for (int time=FirstYear; time<numYears; ++time) {
        if (ObsBiomass(time,speciesNum) != nmfConstantsMSSPM::NoData) {
            sumObsBiomassPerSpecies += ObsBiomass(time,speciesNum);
            ++numYearsWithData;
        }
    }

    return (sumObsBiomassPerSpecies/numYearsWithData);
}

std::vector<double> calculateSigmasSquared(
        const bool& skipFirstYear,
        const boost::numeric::ublas::matrix<double>& ObsBiomass)
{
    int FirstYear = (skipFirstYear) ? 1 : 0;
    int numYears = (int)ObsBiomass.size1();
    int NumPoints = 0;
    double mu;
    double diff;
    double sigmaSquared;
    double sumSq;
    std::vector<double> sigmasSquared;

    for (int species=0; species<(int)ObsBiomass.size2(); ++species) {

        mu = calculateMean(skipFirstYear,nmfConstantsMSSPM::DontUseLogData,ObsBiomass,species);

        // Calculate the sum of the squares of the observed biomass and its mean
        sumSq = 0;
        NumPoints = 0;
        for (int time=FirstYear; time<numYears; ++time) {
            if (ObsBiomass(time,species) != nmfConstantsMSSPM::NoData) {
//              diff   = std::log(ObsBiomass(time,species)) - mu;
                diff   = ObsBiomass(time,species) - mu;
                sumSq += diff*diff;
                ++NumPoints;
            }
        }

        // Calculate standard deviation of the sample; sigma = sqrt(1/(N-1) * sum[(x(i)-x(m))^2])
        sigmaSquared = sumSq/(NumPoints-1);
        if (sigmaSquared == 0) {
            std::cout << "Error: Found sigma=0 in nmfUtilsStatistics::calculateSigmasSquared" << std::endl;
            return {};
        } else {
           sigmasSquared.push_back(sigmaSquared);
        }
    }

    return sigmasSquared;
}

double calculateMaximumLikelihoodNoRescale(
        const bool& skipFirstYear,
        const boost::numeric::ublas::vector<double>& speciesWeights,
        const bool& isEffortFitToCatch,
        const boost::numeric::ublas::matrix<double>& ObsCatch,
        const boost::numeric::ublas::matrix<double>& ObsBiomass,
        const boost::numeric::ublas::matrix<double>& EstCatch,
        const boost::numeric::ublas::matrix<double>& EstBiomass,
        const boost::numeric::ublas::matrix<double>& FitWeights)
{
    double totalMLEForAllSpecies = 0;
    double biomassWeight;
    double catchWeight;
    double mleBiomass;
    double mleCatch;
    int numSpecies = ObsBiomass.size2();

    for (int species=0; species<numSpecies; ++species) {
        biomassWeight = 1.0;
        catchWeight   = 0.0;
        if (isEffortFitToCatch) {
            biomassWeight = FitWeights(species,0);
            catchWeight   = FitWeights(species,1);
        }

        mleBiomass = calculateMaximumLikelihoodNoRescale(skipFirstYear,speciesWeights,species,ObsBiomass,EstBiomass);
        mleCatch   = (isEffortFitToCatch) ? calculateMaximumLikelihoodNoRescale(skipFirstYear,speciesWeights,species,ObsCatch,EstCatch) : 0;
        totalMLEForAllSpecies += speciesWeights(species) * ( biomassWeight*mleBiomass + catchWeight*mleCatch );
    }

    return -totalMLEForAllSpecies;
}

double calculateMaximumLikelihoodNoRescale(
        const bool& skipFirstYear,
        const boost::numeric::ublas::vector<double>& speciesWeights,
        const int& species,
        const boost::numeric::ublas::matrix<double>& ObsMatrix,
        const boost::numeric::ublas::matrix<double>& EstMatrix)
{
    int FirstYear = (skipFirstYear) ? 1 : 0;
    int numYears = EstMatrix.size1();
    double diff;
    double sigma;
    double mlePerSpecies = 0;
    double sumSquares = 0;
    double numPoints  = 0; // Not equal to numYears if there are blanks in observed biomass
    double ObsMinusEstOverSigma;
    double muCurrentSpecies = calculateMean(skipFirstYear,nmfConstantsMSSPM::DontUseLogData,ObsMatrix,species);

    // Calculate the sum of the squares of the observed biomass and its mean
    for (int time=FirstYear; time<numYears; ++time) {
        if (ObsMatrix(time,species) != nmfConstantsMSSPM::NoData) {
            diff = ObsMatrix(time,species) - muCurrentSpecies;
            sumSquares += diff*diff;
            ++numPoints;
        }
    }

    // Calculate standard deviation of the sample; sigma = sqrt(1/(N-1) * sum[(x(i)-x(m))^2])
    sigma = sqrt((1.0/(numPoints-1))*sumSquares);
    if (sigma == 0) {
        std::cout << "Error: Found sigma=0 in nmfUtilsStatistics::calculateMaximumLikelihoodNoRescale" << std::endl;
        return 0;
    }

    // Calculate MLE
    for (int time=FirstYear; time<numYears; ++time) {
        if (ObsMatrix(time,species) != nmfConstantsMSSPM::NoData) {
            diff = ObsMatrix(time,species) - EstMatrix(time,species);
            ObsMinusEstOverSigma = diff / sigma;
            mlePerSpecies += -(nmfConstants::LogRoot2PI + // I could leave this term off, but am including it here for clarity
                               0.5*ObsMinusEstOverSigma*ObsMinusEstOverSigma +
                               log(sigma));
//std::cout << "time: " << time << ", Obs-Est: " << ObsMatrix(time,species) << "-" << EstMatrix(time,species) << std::endl;
        }
    }
//std::cout << "mlePerSpecies: " << mlePerSpecies << std::endl;

    return mlePerSpecies;
}

double calculateModelEfficiency(
        const boost::numeric::ublas::vector<double>& speciesWeights,
        const bool& isEffortFitToCatch,
        const boost::numeric::ublas::matrix<double>& ObsCatch,
        const boost::numeric::ublas::matrix<double>& ObsBiomass,
        const boost::numeric::ublas::matrix<double>& EstCatch,
        const boost::numeric::ublas::matrix<double>& EstBiomass,
        const boost::numeric::ublas::matrix<double>& FitWeights)
{
    int nrows = EstBiomass.size1();
    int ncols = EstBiomass.size2();
    double diff1;
    double diff2;
    double deviation  = 0;
    double sumSquares = 0;
    double biomassWeight;
    double catchWeight;
    double meanObsBiomass = nmfUtils::getMatrixMean(ObsBiomass);
    double meanObsCatch   = nmfUtils::getMatrixMean(ObsCatch);

    for (int time=1; time<nrows; ++time) {
        for (int species=0; species<ncols; ++species) {
            biomassWeight = FitWeights(species,0);
            catchWeight   = FitWeights(species,1);
            if (ObsBiomass(time,species) != nmfConstantsMSSPM::NoData) {
                diff1 = speciesWeights(species)*(ObsBiomass(time,species)-EstBiomass(time,species));
                diff2 = (isEffortFitToCatch) ? speciesWeights(species)*(ObsCatch(time,species)-EstCatch(time,species)) : 0;
                sumSquares += biomassWeight*(diff1*diff1) + catchWeight*(diff2*diff2);
                diff1 = speciesWeights(species)*(ObsBiomass(time,species)-meanObsBiomass);
                diff2 = (isEffortFitToCatch) ? speciesWeights(species)*(ObsCatch(time,species)-meanObsCatch) : 0;;
                deviation += biomassWeight*(diff1*diff1) + catchWeight*(diff2*diff2);
            }
        }
    }

    if (nmfUtils::isNearlyZero(deviation)) {
        return 0;
    } else {
        return (deviation == 0) ? 0 : (1.0 - sumSquares/deviation); // Nash-Sutcliffe Model Efficiency Coefficient
    }
}

double calculateLeastSquares(
        const boost::numeric::ublas::vector<double>& speciesWeights,
        const bool& isEffortFitToCatch,
        const boost::numeric::ublas::matrix<double>& ObsCatch,
        const boost::numeric::ublas::matrix<double>& ObsBiomass,
        const boost::numeric::ublas::matrix<double>& EstCatch,
        const boost::numeric::ublas::matrix<double>& EstBiomass,
        const boost::numeric::ublas::matrix<double>& FitWeights)
{
    double diff1 = 0;
    double diff2 = 0;
    double sumSquares = 0;
    double biomassWeight;
    double catchWeight;
//qDebug() << "\n";
//qDebug() << "ObsCatch: " << ObsCatch.size1() << ObsCatch.size2();
//qDebug() << "EstCatch: " << EstCatch.size1() << EstCatch.size2();

    for (unsigned time=0; time<EstBiomass.size1(); ++time) {
        for (unsigned species=0; species<EstBiomass.size2(); ++species) {
            biomassWeight = FitWeights(species,0);
            catchWeight   = FitWeights(species,1);

            if (ObsBiomass(time,species) != nmfConstantsMSSPM::NoData) {

                // These values have all been rescaled
                diff1         = speciesWeights(species)*(ObsBiomass(time,species)-EstBiomass(time,species));
                diff2         = (isEffortFitToCatch) ? speciesWeights(species)*(ObsCatch(time,species)-EstCatch(time,species)) : 0;
                biomassWeight = (isEffortFitToCatch) ? biomassWeight : 1.0;
                // This will do an average of biomass and catch sum of the squares
                sumSquares += biomassWeight*(diff1*diff1) + catchWeight*(diff2*diff2);
            }
        }
    }

    return sumSquares;
//    return log10(sumSquares+1);
}

} // end namespace nmfStatUtils

