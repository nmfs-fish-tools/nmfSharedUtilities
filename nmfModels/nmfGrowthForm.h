
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
    std::map<std::string, double(*)(
            int    &speciesNum,
            double &initBiomass,
            std::vector<double> &growthRate,
            std::vector<double> &carryingCapacity
    )> FunctionMap;



public:
    nmfGrowthForm(std::string theType);
   ~nmfGrowthForm() {};

    double evaluate(int    &speciesNum,
                    double &biomassAtTime,
                    std::vector<double> &growthRate,
                    std::vector<double> &carryingCapacity);
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
    static double LinearGrowth(int    &speciesNum,
                               double &biomassAtTime,
                               std::vector<double> &growthRate,
                               std::vector<double> &carryingCapacity);
    static double LogisticGrowth(int    &speciesNum,
                                 double &biomassAtTime,
                                 std::vector<double> &growthRate,
                                 std::vector<double> &carryingCapacity);
};


