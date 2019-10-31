
#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <functional>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multi_array.hpp>

#include "nmfUtils.h"

class nmfPredationForm {

private:

    bool m_isAggProd;
    int m_numSpecies;
    int m_numGuilds;
    int m_numberParameters;
    int m_NumSpeciesOrGuilds;
    std::string m_type; // "Type I", "Type II", ...
    std::vector<std::pair<double,double> > m_parameterRanges;

    std::map<std::string, double(*)(
            int &timeMinus1,
            int &SpeciesNum,
            boost::numeric::ublas::matrix<double> &EstPredation,
            boost::numeric::ublas::matrix<double> &EstHandling,
            std::vector<double> &EstExponent,
            boost::numeric::ublas::matrix<double> &EstimatedBiomass,
            double &EstimatedBiomassTimeMinus1
    )> m_FunctionMap;



public:
    nmfPredationForm(std::string theType);
   ~nmfPredationForm() {};

    int getNumParameters();
    void extractPredationParameters(
            const std::vector<double> &parameters,
            int& startPos,
            boost::numeric::ublas::matrix<double> &predation);
    void extractHandlingParameters(
            const std::vector<double> &parameters,
            int& startPos,
            boost::numeric::ublas::matrix<double> &handling);
    void extractExponentParameters(
            const std::vector<double> &parameters,
            int& startPos,
            std::vector<double> &exponents);
    void loadParameterRanges(
            std::vector<std::pair<double,double> >& parameterRanges,
            Data_Struct& beeStruct);

    double evaluate(int &timeMinus1,
                    int &SpeciesNum,
                    boost::numeric::ublas::matrix<double> &EstPredation,
                    boost::numeric::ublas::matrix<double> &EstHandling,
                    std::vector<double> &EstExponent,
                    boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                    double &EstimatedBiomassTimeMinus1);

    static double TypeIPredation(int &timeMinus1,
                                 int &SpeciesNum,
                                 boost::numeric::ublas::matrix<double> &EstPredation,
                                 boost::numeric::ublas::matrix<double> &EstHandling,
                                 std::vector<double> &EstExponent,
                                 boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                                 double &EstimatedBiomassTimeMinus1);

    static double TypeIIPredation(int &timeMinus1,
                                  int &SpeciesNum,
                                  boost::numeric::ublas::matrix<double> &EstPredation,
                                  boost::numeric::ublas::matrix<double> &EstHandling,
                                  std::vector<double> &EstExponent,
                                  boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                                  double &EstimatedBiomassTimeMinus1);

    static double TypeIIIPredation(int &timeMinus1,
                                   int &SpeciesNum,
                                   boost::numeric::ublas::matrix<double> &EstPredation,
                                   boost::numeric::ublas::matrix<double> &EstHandling,
                                   std::vector<double> &EstExponent,
                                   boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                                   double &EstimatedBiomassTimeMinus1);
};


