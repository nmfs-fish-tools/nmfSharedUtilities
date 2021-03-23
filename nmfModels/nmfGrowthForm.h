
#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <functional>

#include "nmfUtils.h"
#include "nmfConstantsMSSPM.h"


class nmfGrowthForm {

private:

    int m_numberParameters;
    std::string m_type; // "Null","Linear","Logistic"
    std::vector<std::pair<double,double> > m_parameterRanges;
    std::map<std::string, double(nmfGrowthForm::*)(
            const int    &speciesNum,
            const double &initBiomass,
            const std::vector<double> &growthRate,
            const std::vector<double> &carryingCapacity
    )> FunctionMap;



public:
    nmfGrowthForm(std::string theType);
   ~nmfGrowthForm() {};

    double evaluate(const int    &speciesNum,
                    const double &biomassAtTime,
                    const std::vector<double> &growthRate,
                    const std::vector<double> &carryingCapacity);
    int getNumParameters();
    std::string getType();
    void extractParameters(
            const std::vector<double>& parameters,
            int&                       startPos,
            std::vector<double>&       growthRate,
            std::vector<double>&       carryingCapacity,
            double&                    systemCarryingCapacity);
    void loadParameterRanges(
            std::vector<std::pair<double,double> >& parameterRanges,
            const Data_Struct& beeStruct);
    double NoGrowth(const int &speciesNum,
                    const double &biomassAtTime,
                    const std::vector<double> &growthRate,
                    const std::vector<double> &carryingCapacity);
    double LinearGrowth(const int &speciesNum,
                        const double &biomassAtTime,
                        const std::vector<double> &growthRate,
                        const std::vector<double> &carryingCapacity);
    double LogisticGrowth(const int &speciesNum,
                          const double &biomassAtTime,
                          const std::vector<double> &growthRate,
                          const std::vector<double> &carryingCapacity);
};


