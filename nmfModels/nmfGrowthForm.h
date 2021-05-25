
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
    std::map<std::string,std::string> m_GrowthMap;
    std::map<std::string,std::string> m_GrowthKey;
    bool m_isAGGPROD;
    std::string m_Prefix;

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
    void setType(std::string newType);
    std::string getType();
    void extractParameters(
            const std::vector<double>& parameters,
            int&                       startPos,
            std::vector<double>&       growthRate,
            std::vector<double>&       carryingCapacity,
            double&                    systemCarryingCapacity);
    void loadParameterRanges(
            std::vector<std::pair<double,double> >& parameterRanges,
            const nmfStructsQt::ModelDataStruct& beeStruct);
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
    void setupFormMaps();
    void setAggProd(bool isAggProd);
    std::string getExpression();
    std::string getKey();
    std::string getPrefix();

};


