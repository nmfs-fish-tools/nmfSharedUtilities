
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
            const double &initBiomass,
            const double &growthRate,
            const double &growthRateCovariate,
            const double &carryingCapacity,
            const double &carryingCapacityCovariate
    )> FunctionMap;



public:
    nmfGrowthForm(std::string theType);
   ~nmfGrowthForm() {};

    double evaluate(const double &biomassAtTime,
                    const double &growthRate,
                    const double &growthRateCovariate,
                    const double &carryingCapacity,
                    const double &carryingCapacityCovariate);
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
    double NoGrowth(const double &biomassAtTime,
                    const double &growthRate,
                    const double &growthRateCovariate,
                    const double &carryingCapacity,
                    const double &carryingCapacityCovariate);
    double LinearGrowth(const double &biomassAtTime,
                        const double &growthRate,
                        const double &growthRateCovariate,
                        const double &carryingCapacity,
                        const double &carryingCapacityCovariate);
    double LogisticGrowth(const double &biomassAtTime,
                          const double &growthRate,
                          const double &growthRateCovariate,
                          const double &carryingCapacity,
                          const double &carryingCapacityCovariate);
    void setupFormMaps();
    void setAggProd(bool isAggProd);
    std::string getExpression();
    std::string getKey();
    std::string getPrefix();

};


