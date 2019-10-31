
#include "nmfGrowthForm.h"

nmfGrowthForm::nmfGrowthForm(std::string growthType)
{
    m_type = growthType;
    m_parameterRanges.clear();
    m_numberParameters = 0;

    FunctionMap["Linear"]   = LinearGrowth;
    FunctionMap["Logistic"] = LogisticGrowth;
}

void
nmfGrowthForm::loadParameterRanges(
        std::vector<std::pair<double,double> >& parameterRanges,
        Data_Struct& beeStruct)
{
    std::pair<double,double> aPair;

    if (m_type == "Linear") {
        for (unsigned i=0; i<beeStruct.GrowthRateMin.size(); ++i) {
            aPair = std::make_pair(beeStruct.GrowthRateMin[i],
                                   beeStruct.GrowthRateMax[i]);
            parameterRanges.emplace_back(aPair);
            m_parameterRanges.emplace_back(aPair);
        }
        m_numberParameters = beeStruct.GrowthRateMin.size();
    } else if (m_type == "Logistic") {
        for (unsigned i=0; i<beeStruct.GrowthRateMin.size(); ++i) {
            aPair = std::make_pair(beeStruct.GrowthRateMin[i],
                                   beeStruct.GrowthRateMax[i]);
            parameterRanges.emplace_back(aPair);
            m_parameterRanges.emplace_back(aPair);
        }
        for (unsigned i=0; i<beeStruct.CarryingCapacityMin.size(); ++i) {
            aPair = std::make_pair(beeStruct.CarryingCapacityMin[i],
                                   beeStruct.CarryingCapacityMax[i]);
            parameterRanges.emplace_back(aPair);
            m_parameterRanges.emplace_back(aPair);
        }
        m_numberParameters = beeStruct.GrowthRateMin.size() +
                             beeStruct.CarryingCapacityMin.size();

    }
}

int
nmfGrowthForm::getNumParameters()
{
    return m_numberParameters; //m_parameterRanges.size();
}

std::string
nmfGrowthForm::getType()
{
    return m_type;
}

void
nmfGrowthForm::extractParameters(
        const std::vector<double>& parameters,
        int&                       startPos,
        std::vector<double>&       growthRate,
        std::vector<double>&       carryingCapacity,
        double&                    systemCarryingCapacity)
{
    int numGrowthParameters     = getNumParameters();
    int numHalfGrowthParameters = numGrowthParameters/2;
    double tempK;

    systemCarryingCapacity = 0;
    growthRate.clear();
    carryingCapacity.clear();

    if (m_type == "Linear") {
        for (int i=startPos; i<numGrowthParameters; ++i) {
            growthRate.emplace_back(parameters[i]);
            carryingCapacity.emplace_back(0);
        }
        startPos = numGrowthParameters;
    } else if (m_type == "Logistic") {
        numGrowthParameters     = m_numberParameters;
        numHalfGrowthParameters = m_numberParameters/2;
        for (int i=startPos; i<numHalfGrowthParameters; ++i) {
            growthRate.emplace_back(parameters[i]);
        }
        for (int i=numHalfGrowthParameters; i<numGrowthParameters; ++i) {
            tempK = parameters[i];
            systemCarryingCapacity += tempK;
            carryingCapacity.emplace_back(tempK);
        }
        startPos = numGrowthParameters;
    }
}

double
nmfGrowthForm::evaluate(int    &SpeciesNum,
                        double &biomassAtTimeT,
                        std::vector<double> &growthRate,
                        std::vector<double> &carryingCapacity)
{
    if (FunctionMap.find(m_type) == FunctionMap.end()) {
        return 0;
    } else {
        return FunctionMap[m_type](SpeciesNum,biomassAtTimeT,growthRate,carryingCapacity);
    }
}

double
nmfGrowthForm::LinearGrowth(int    &speciesNum,
                            double &biomassAtTime,
                            std::vector<double> &growthRate,
                            std::vector<double> &carryingCapacity)
{
   return growthRate[speciesNum]*biomassAtTime;
}

double
nmfGrowthForm::LogisticGrowth(int    &speciesNum,
                              double &biomassAtTime,
                              std::vector<double> &growthRate,
                              std::vector<double> &carryingCapacity)
{
    return growthRate[speciesNum]*biomassAtTime * (1.0-biomassAtTime/carryingCapacity[speciesNum]);
}
