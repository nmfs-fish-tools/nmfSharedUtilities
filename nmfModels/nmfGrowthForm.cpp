
#include "nmfGrowthForm.h"

nmfGrowthForm::nmfGrowthForm(std::string growthType)
{
    m_type = growthType;
    m_parameterRanges.clear();
    m_numberParameters = 0;

    FunctionMap["Null"]     = &nmfGrowthForm::NoGrowth;
    FunctionMap["Linear"]   = &nmfGrowthForm::LinearGrowth;
    FunctionMap["Logistic"] = &nmfGrowthForm::LogisticGrowth;
}

void
nmfGrowthForm::loadParameterRanges(
        std::vector<std::pair<double,double> >& parameterRanges,
        const Data_Struct& dataStruct)
{
    bool isCheckedGrowthRate       = nmfUtils::isEstimateParameterChecked(dataStruct,"GrowthRate");
    bool isCheckedCarryingCapacity = nmfUtils::isEstimateParameterChecked(dataStruct,"CarryingCapacity");

    std::pair<double,double> aPair;
    m_numberParameters = 0;

    // Always load growth rate values
    for (unsigned species=0; species<dataStruct.GrowthRateMin.size(); ++species) {
        if (isCheckedGrowthRate) {
            aPair = std::make_pair(dataStruct.GrowthRateMin[species],
                                   dataStruct.GrowthRateMax[species]);
        } else {
            aPair = std::make_pair(dataStruct.GrowthRate[species], //-nmfConstantsMSSPM::epsilon,
                                   dataStruct.GrowthRate[species]); //+nmfConstantsMSSPM::epsilon);
        }
        parameterRanges.emplace_back(aPair);
        m_parameterRanges.emplace_back(aPair);
    }
    m_numberParameters += dataStruct.GrowthRateMin.size();

    if (m_type == "Logistic") {
        for (unsigned species=0; species<dataStruct.CarryingCapacityMin.size(); ++species) {
            if (isCheckedCarryingCapacity) {
                aPair = std::make_pair(dataStruct.CarryingCapacityMin[species],
                                       dataStruct.CarryingCapacityMax[species]);
            } else {
                aPair = std::make_pair(dataStruct.CarryingCapacity[species], //-nmfConstantsMSSPM::epsilon,
                                       dataStruct.CarryingCapacity[species]); //+nmfConstantsMSSPM::epsilon);
            }
            parameterRanges.emplace_back(aPair);
            m_parameterRanges.emplace_back(aPair);
        }
        m_numberParameters += dataStruct.CarryingCapacityMin.size();
    }
}

int
nmfGrowthForm::getNumParameters()
{
    return m_numberParameters; //m_parameterRanges.size();
}

void
nmfGrowthForm::setType(std::string newType)
{
    m_type = newType;
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
    int numGrowthParameters     = getNumParameters()  +startPos;
    int numHalfGrowthParameters = numGrowthParameters/2+startPos;
    double tempK;

    systemCarryingCapacity = 0;
    growthRate.clear();
    carryingCapacity.clear();

    if (m_type == "Linear") {
        for (int i=startPos; i<numGrowthParameters; ++i) {
            growthRate.emplace_back(parameters[i]);
            carryingCapacity.emplace_back(0);
        }
    } else if (m_type == "Logistic") {
        numGrowthParameters     = m_numberParameters  +startPos;
        numHalfGrowthParameters = m_numberParameters/2+startPos;
        for (int i=startPos; i<numHalfGrowthParameters; ++i) {
            growthRate.emplace_back(parameters[i]);
        }
        for (int i=numHalfGrowthParameters; i<numGrowthParameters; ++i) {
            tempK = parameters[i];
            systemCarryingCapacity += tempK;
            carryingCapacity.emplace_back(tempK);
        }
    }
    startPos = numGrowthParameters;
}

double
nmfGrowthForm::evaluate(const int &SpeciesNum,
                        const double &biomassAtTimeT,
                        const std::vector<double> &growthRate,
                        const std::vector<double> &carryingCapacity)
{
    if (FunctionMap.find(m_type) == FunctionMap.end()) {
        return 0;
    } else {
        return (this->*FunctionMap[m_type])(SpeciesNum,biomassAtTimeT,growthRate,carryingCapacity);
    }
}

double
nmfGrowthForm::NoGrowth(const int &speciesNum,
                        const double &biomassAtTime,
                        const std::vector<double> &growthRate,
                        const std::vector<double> &carryingCapacity)
{
    return 0.0;
}

double
nmfGrowthForm::LinearGrowth(const int &speciesNum,
                            const double &biomassAtTime,
                            const std::vector<double> &growthRate,
                            const std::vector<double> &carryingCapacity)
{
   return growthRate[speciesNum]*biomassAtTime;
}

double
nmfGrowthForm::LogisticGrowth(const int &speciesNum,
                              const double &biomassAtTime,
                              const std::vector<double> &growthRate,
                              const std::vector<double> &carryingCapacity)
{
    return growthRate[speciesNum]*biomassAtTime * (1.0-biomassAtTime/carryingCapacity[speciesNum]);
}
