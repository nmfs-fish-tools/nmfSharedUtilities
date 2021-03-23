
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

    std::map<std::string, double(nmfHarvestForm::*)(
            const int    &TimeT,
            const int    &SpeciesNum,
            const boost::numeric::ublas::matrix<double> &Catch,
            const boost::numeric::ublas::matrix<double> &Effort,
            const boost::numeric::ublas::matrix<double> &Exploitation,
            const double& biomassAtTime,
            const std::vector<double>& catchabilityRate
    )> m_FunctionMap;



public:
    nmfHarvestForm(std::string theType);
   ~nmfHarvestForm() {};

    int getNumParameters();
    void extractParameters(
            const std::vector<double> &parameters,
            int& startPos,
            std::vector<double>& catchabilityRate);
    double evaluate(const int    &time,
                    const int    &speciesNum,
                    const boost::numeric::ublas::matrix<double> &Catch,
                    const boost::numeric::ublas::matrix<double> &Effort,
                    const boost::numeric::ublas::matrix<double> &Exploitation,
                    const double& biomassAtTime,
                    const std::vector<double>& catchabilityRate);
    void loadParameterRanges(
                    std::vector<std::pair<double,double> >& parameterRanges,
                    Data_Struct& beeStruct);
    double NoHarvest(const int &time,
                     const int &speciesNum,
                     const boost::numeric::ublas::matrix<double> &Catch,
                     const boost::numeric::ublas::matrix<double> &Effort,
                     const boost::numeric::ublas::matrix<double> &Exploitation,
                     const double& biomassAtTime,
                     const std::vector<double>& catchabilityRate);
    double ExploitationHarvest(const int &time,
                               const int &speciesNum,
                               const boost::numeric::ublas::matrix<double> &Catch,
                               const boost::numeric::ublas::matrix<double> &Effort,
                               const boost::numeric::ublas::matrix<double> &Exploitation,
                               const double& biomassAtTime,
                               const std::vector<double>& catchabilityRate);
    double CatchHarvest(const int &time,
                        const int &speciesNum,
                        const boost::numeric::ublas::matrix<double> &Catch,
                        const boost::numeric::ublas::matrix<double> &Effort,
                        const boost::numeric::ublas::matrix<double> &Exploitation,
                        const double& biomassAtTime,
                        const std::vector<double>& catchabilityRate);
    double EffortHarvest(const int& timeT,
                         const int& speciesNum,
                         const boost::numeric::ublas::matrix<double> &Catch,
                         const boost::numeric::ublas::matrix<double> &Effort,
                         const boost::numeric::ublas::matrix<double> &Exploitation,
                         const double& biomassAtTime,
                         const std::vector<double>& catchabilityRate);

};


