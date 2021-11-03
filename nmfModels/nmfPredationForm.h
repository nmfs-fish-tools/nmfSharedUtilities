
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
            const int& timeMinus1,
            const int& species,
            const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
            const double& EstimatedBiomassAtTimeMinus1,
            const boost::numeric::ublas::matrix<double>& EstPredationRho,
            const boost::numeric::ublas::matrix<double>& EstPredationRhoCovariate,
            const boost::numeric::ublas::matrix<double>& EstPredationHandling,
            const boost::numeric::ublas::matrix<double>& EstPredationHandlingCovariate,
            const std::vector<double>& EstPredationExponent,
            const boost::numeric::ublas::matrix<double>& EstPredationExponentCovariate
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

    double evaluate(const int& timeMinus1,
                    const int& species,
                    const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                    const double& EstimatedBiomassAtTimeMinus1,
                    const boost::numeric::ublas::matrix<double>& EstPredationRho,
                    const boost::numeric::ublas::matrix<double>& EstPredationRhoCovariate,
                    const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                    const boost::numeric::ublas::matrix<double>& EstPredationHandlingCovariate,
                    const std::vector<double>& EstPredationExponent,
                    const boost::numeric::ublas::matrix<double>& EstPredationExponentCovariate);

    double TypeNullPredation(const int& timeMinus1,
                             const int& species,
                             const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                             const double& EstimatedBiomassAtTimeMinus1,
                             const boost::numeric::ublas::matrix<double>& EstPredationRho,
                             const boost::numeric::ublas::matrix<double>& EstPredationRhoCovariate,
                             const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                             const boost::numeric::ublas::matrix<double>& EstPredationHandlingCovariate,
                             const std::vector<double>& EstPredationExponent,
                             const boost::numeric::ublas::matrix<double>& EstPredationExponentCovariate);

    double TypeIPredation(const int& timeMinus1,
                          const int& species,
                          const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                          const double& EstimatedBiomassAtTimeMinus1,
                          const boost::numeric::ublas::matrix<double>& EstPredationRho,
                          const boost::numeric::ublas::matrix<double>& EstPredationRhoCovariate,
                          const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                          const boost::numeric::ublas::matrix<double>& EstPredationHandlingCovariate,
                          const std::vector<double>& EstPredationExponent,
                          const boost::numeric::ublas::matrix<double>& EstPredationExponentCovariate);

    double TypeIIPredation(const int& timeMinus1,
                           const int& species,
                           const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                           const double& EstimatedBiomassAtTimeMinus1,
                           const boost::numeric::ublas::matrix<double>& EstPredationRho,
                           const boost::numeric::ublas::matrix<double>& EstPredationRhoCovariate,
                           const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                           const boost::numeric::ublas::matrix<double>& EstPredationHandlingCovariate,
                           const std::vector<double>& EstPredationExponent,
                           const boost::numeric::ublas::matrix<double>& EstPredationExponentCovariate);

    double TypeIIIPredation(const int& timeMinus1,
                            const int& species,
                            const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                            const double& EstimatedBiomassAtTimeMinus1,
                            const boost::numeric::ublas::matrix<double>& EstPredationRho,
                            const boost::numeric::ublas::matrix<double>& EstPredationRhoCovariate,
                            const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                            const boost::numeric::ublas::matrix<double>& EstPredationHandlingCovariate,
                            const std::vector<double>& EstPredationExponent,
                            const boost::numeric::ublas::matrix<double>& EstPredationExponentCovariate);
    void setupFormMaps();
    void setAggProd(bool isAggProd);
    std::string getExpression();
    std::string getKey();
};


