
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
    int  m_numSpecies;
    int  m_numGuilds;
    int  m_numberParameters;
    int  m_NumSpeciesOrGuilds;
    std::string m_type; // "Type I", "Type II", ...
    std::vector<std::pair<double,double> > m_parameterRanges;
    std::map<std::string,std::string> m_PredationMap;
    std::map<std::string,std::string> m_PredationKey;

    std::map<std::string, double(nmfPredationForm::*)(
            const int &timeMinus1,
            const int &SpeciesNum,
            const boost::numeric::ublas::matrix<double> &EstPredation,
            const boost::numeric::ublas::matrix<double> &EstHandling,
            const std::vector<double> &EstExponent,
            const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
            const double &EstimatedBiomassTimeMinus1
    )> m_FunctionMap;



public:
    nmfPredationForm(std::string theType);
   ~nmfPredationForm() {};

    int getNumParameters();
    void setType(std::string newType);
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
            nmfStructsQt::ModelDataStruct& beeStruct);

    double evaluate(const int &timeMinus1,
                    const int &SpeciesNum,
                    const boost::numeric::ublas::matrix<double> &EstPredation,
                    const boost::numeric::ublas::matrix<double> &EstHandling,
                    const std::vector<double> &EstExponent,
                    const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                    const double &EstimatedBiomassTimeMinus1);

    double TypeNullPredation(const int &timeMinus1,
                             const int &SpeciesNum,
                             const boost::numeric::ublas::matrix<double> &EstPredation,
                             const boost::numeric::ublas::matrix<double> &EstHandling,
                             const std::vector<double> &EstExponent,
                             const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                             const double &EstimatedBiomassTimeMinus1);

    double TypeIPredation(const int &timeMinus1,
                          const int &SpeciesNum,
                          const boost::numeric::ublas::matrix<double> &EstPredation,
                          const boost::numeric::ublas::matrix<double> &EstHandling,
                          const std::vector<double> &EstExponent,
                          const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                          const double &biomassAtTime);

    double TypeIIPredation(const int &timeMinus1,
                           const int &SpeciesNum,
                           const boost::numeric::ublas::matrix<double> &EstPredation,
                           const boost::numeric::ublas::matrix<double> &EstHandling,
                           const std::vector<double> &EstExponent,
                           const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                           const double &EstimatedBiomassTimeMinus1);

    double TypeIIIPredation(const int &timeMinus1,
                            const int &SpeciesNum,
                            const boost::numeric::ublas::matrix<double> &EstPredation,
                            const boost::numeric::ublas::matrix<double> &EstHandling,
                            const std::vector<double> &EstExponent,
                            const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                            const double &EstimatedBiomassTimeMinus1);
    void setupFormMaps();
    void setAggProd(bool isAggProd);
    std::string getExpression();
    std::string getKey();
};


