
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


class nmfHarvestForm {

private:

    int m_numParameters;
    std::string m_type; // "Catch, F, QE"
    std::vector<std::pair<double,double> > m_parameterRanges;
    std::map<std::string,std::string> m_HarvestMap;
    std::map<std::string,std::string> m_HarvestKey;
    bool m_isAGGPROD;

    std::map<std::string, double(nmfHarvestForm::*)(
            const int& time,
            const int& species,
            const double& biomassAtTime,
            const boost::numeric::ublas::matrix<double>& Catch,
            const boost::numeric::ublas::matrix<double>& Effort,
            const boost::numeric::ublas::matrix<double>& Exploitation,
            const std::vector<double>& catchabilityRate,
            const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate
    )> m_FunctionMap;



public:
    nmfHarvestForm(std::string theType);
   ~nmfHarvestForm() {};

    int getNumParameters();
    void setType(std::string newType);
    void extractParameters(
            const std::vector<double> &parameters,
            int& startPos,
            std::vector<double>& catchabilityRate);
    double evaluate(const int& time,
                    const int& species,
                    const double& biomassAtTime,
                    const boost::numeric::ublas::matrix<double>& Catch,
                    const boost::numeric::ublas::matrix<double>& Effort,
                    const boost::numeric::ublas::matrix<double>& Exploitation,
                    const std::vector<double>& catchabilityRate,
                    const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate);
    void loadParameterRanges(
                    std::vector<std::pair<double,double> >& parameterRanges,
                    nmfStructsQt::ModelDataStruct& beeStruct);
    double NoHarvest(const int& time,
                     const int& species,
                     const double& biomassAtTime,
                     const boost::numeric::ublas::matrix<double>& Catch,
                     const boost::numeric::ublas::matrix<double>& Effort,
                     const boost::numeric::ublas::matrix<double>& Exploitation,
                     const std::vector<double>& catchabilityRate,
                     const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate);
    /**
     * @brief ExploitationHarvest
     * @param biomassAtTime
     * @param Catch
     * @param Effort : effort matrix; passing matrix because sometimes it may be empty and unused so can't pass the indexed value
     * @param Exploitation
     * @param catchabilityRate
     * @param catchabilityRateCovariate
     * @return
     */
    double ExploitationHarvest(const int& time,
                               const int& species,
                               const double& biomassAtTime,
                               const boost::numeric::ublas::matrix<double>& Catch,
                               const boost::numeric::ublas::matrix<double>& Effort,
                               const boost::numeric::ublas::matrix<double>& Exploitation,
                               const std::vector<double>& catchabilityRate,
                               const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate);
    double CatchHarvest(const int& time,
                        const int& species,
                        const double& biomassAtTime,
                        const boost::numeric::ublas::matrix<double>& Catch,
                        const boost::numeric::ublas::matrix<double>& Effort,
                        const boost::numeric::ublas::matrix<double>& Exploitation,
                        const std::vector<double>& catchabilityRate,
                        const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate);
    double EffortHarvest(const int& time,
                         const int& species,
                         const double& biomassAtTime,
                         const boost::numeric::ublas::matrix<double>& Catch,
                         const boost::numeric::ublas::matrix<double>& Effort,
                         const boost::numeric::ublas::matrix<double>& Exploitation,
                         const std::vector<double>& catchabilityRate,
                         const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate);
    void setupFormMaps();
    std::string getExpression();
    std::string getKey();
    void setAggProd(bool isAggProd);

};


