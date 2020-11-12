
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

    std::map<std::string, double(*)(
            int    &TimeT,
            int    &SpeciesNum,
            boost::numeric::ublas::matrix<double> &Catch,
            boost::numeric::ublas::matrix<double> &Effort,
            boost::numeric::ublas::matrix<double> &Exploitation,
            double& biomassAtTime,
            std::vector<double>& catchabilityRate
    )> m_FunctionMap;



public:
    nmfHarvestForm(std::string theType);
   ~nmfHarvestForm() {};

    int getNumParameters();
    void extractParameters(
            const std::vector<double> &parameters,
            int& startPos,
            std::vector<double>& catchabilityRate);
    double evaluate(int    &time,
                    int    &speciesNum,
                    boost::numeric::ublas::matrix<double> &Catch,
                    boost::numeric::ublas::matrix<double> &Effort,
                    boost::numeric::ublas::matrix<double> &Exploitation,
                    double& biomassAtTime,
                    std::vector<double>& catchabilityRate);
    void loadParameterRanges(
                    std::vector<std::pair<double,double> >& parameterRanges,
                    Data_Struct& beeStruct);
    static double ExploitationHarvest(int &time,
                                      int &speciesNum,
                                      boost::numeric::ublas::matrix<double> &Catch,
                                      boost::numeric::ublas::matrix<double> &Effort,
                                      boost::numeric::ublas::matrix<double> &Exploitation,
                                      double& biomassAtTime,
                                      std::vector<double>& catchabilityRate);
    static double CatchHarvest(int &time,
                               int &speciesNum,
                               boost::numeric::ublas::matrix<double> &Catch,
                               boost::numeric::ublas::matrix<double> &Effort,
                               boost::numeric::ublas::matrix<double> &Exploitation,
                               double& biomassAtTime,
                               std::vector<double>& catchabilityRate);
    static double EffortHarvest(int& timeT,
                                int& speciesNum,
                                boost::numeric::ublas::matrix<double> &Catch,
                                boost::numeric::ublas::matrix<double> &Effort,
                                boost::numeric::ublas::matrix<double> &Exploitation,
                                double& biomassAtTime,
                                std::vector<double>& catchabilityRate);

};


