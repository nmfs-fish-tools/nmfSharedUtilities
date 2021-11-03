
#include "nmfGrowthForm.h"

nmfGrowthForm::nmfGrowthForm(std::string growthType)
{
    m_type = growthType;
    m_parameterRanges.clear();
    m_numberParameters = 0;
    m_isAGGPROD = false;
    m_Prefix.clear();
    m_GrowthMap.clear();
    m_GrowthKey.clear();

    FunctionMap["Null"]     = &nmfGrowthForm::NoGrowth;
    FunctionMap["Linear"]   = &nmfGrowthForm::LinearGrowth;
    FunctionMap["Logistic"] = &nmfGrowthForm::LogisticGrowth;

    setupFormMaps();
}

void
nmfGrowthForm::setupFormMaps()
{
    std::string index1    = (m_isAGGPROD) ? "I" : "i";
    std::string lhs       = "B<sub>"+index1+",t+1</sub> = ";  // left hand side of equation
    std::string Bit       = "B<sub>"+index1+",t</sub>";       // right hand side of equation
    std::string Ki        = "K<sub>"+index1+"</sub>";
    std::string riBit     = "r<sub>"+index1+"</sub>" + Bit;
    m_Prefix              = lhs + Bit;

    m_GrowthMap["Null"]     = "";
    m_GrowthMap["Linear"]   = " + " + riBit;
    m_GrowthMap["Logistic"] = " + " + riBit + "(1 - " + Bit + "/" + Ki + ")";

    m_GrowthKey["Null"]     = "";
    m_GrowthKey["Linear"]   = "B = Biomass<br/>r = Growth Rate<br/>";
    m_GrowthKey["Logistic"] = "B = Biomass<br/>r = Growth Rate<br/>K = Carrying Capacity<br/>";
}

std::string
nmfGrowthForm::getPrefix()
{
    return m_Prefix;
}

std::string
nmfGrowthForm::getExpression()
{
    return m_GrowthMap[m_type];
}

std::string
nmfGrowthForm::getKey()
{
    return m_GrowthKey[m_type];
}

void
nmfGrowthForm::setAggProd(bool isAggProd)
{
    m_isAGGPROD = isAggProd;
}

void
nmfGrowthForm::loadParameterRanges(
        std::vector<std::pair<double,double> >& parameterRanges,
        const nmfStructsQt::ModelDataStruct& dataStruct)
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
nmfGrowthForm::evaluate(const double &biomassAtTimeT,
                        const double &growthRate,
                        const double &growthRateCovariate,
                        const double &carryingCapacity,
                        const double &carryingCapacityCovariate)
{
    if (FunctionMap.find(m_type) == FunctionMap.end()) {
        return 0;
    } else {
        return (this->*FunctionMap[m_type])(biomassAtTimeT,
                                            growthRate,growthRateCovariate,
                                            carryingCapacity,carryingCapacityCovariate);
    }
}

double
nmfGrowthForm::NoGrowth(const double &biomassAtTime,
                        const double &growthRate,
                        const double &growthRateCovariate,
                        const double &carryingCapacity,
                        const double &carryingCapacityCovariate)
{
    return 0.0;
}

double
nmfGrowthForm::LinearGrowth(const double &biomassAtTime,
                            const double &growthRate,
                            const double &growthRateCovariate,
                            const double &carryingCapacity,
                            const double &carryingCapacityCovariate)
{
    double growthRateCovariateCoeff = 1.0; // RSK - estimate this next
    double growthRateTerm  = growthRate*(1.0 + growthRateCovariateCoeff*growthRateCovariate);

    return growthRateTerm*biomassAtTime;
}

double
nmfGrowthForm::LogisticGrowth(const double &biomassAtTime,
                              const double &growthRate,
                              const double &growthRateCovariate,
                              const double &carryingCapacity,
                              const double &carryingCapacityCovariate)
{
    double growthRateCovariateCoeff       = 1.0; // RSK - estimate this next
    double carryingCapacityCovariateCoeff = 1.0; // RSK - estimate this next
    double growthRateTerm       = growthRate      *(1.0 + growthRateCovariateCoeff*growthRateCovariate);
    double carryingCapacityTerm = carryingCapacity*(1.0 + carryingCapacityCovariateCoeff*carryingCapacityCovariate);

    return (growthRateTerm * biomassAtTime * (1.0-biomassAtTime/(carryingCapacityTerm)));
}
