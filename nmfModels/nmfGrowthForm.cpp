
#include "nmfGrowthForm.h"

nmfGrowthForm::nmfGrowthForm(std::string growthType)
{
    m_Type = growthType;
    m_NumSpecies = 0;
    m_isAGGPROD = false;
    m_Prefix.clear();
    m_GrowthMap.clear();
    m_GrowthKey.clear();

    FunctionMap["Null"]     = &nmfGrowthForm::FunctionMap_Null;
    FunctionMap["Linear"]   = &nmfGrowthForm::FunctionMap_Linear;
    FunctionMap["Logistic"] = &nmfGrowthForm::FunctionMap_Logistic;

    setupFormMaps();
}

void
nmfGrowthForm::setupFormMaps()
{
    std::string index1      = (m_isAGGPROD) ? "I" : "i";
    std::string lhs         = "B<sub>"+index1+",t+1</sub> = ";  // left hand side of equation
    std::string Bit         = "B<sub>"+index1+",t</sub>";       // right hand side of equation
    std::string Ki          = "K<sub>"+index1+"</sub>";
    std::string Pi          = "p<sub>"+index1+"</sub>";
    std::string riBit       = "r<sub>"+index1+"</sub>" + Bit;
    m_Prefix                = lhs + Bit;

    m_GrowthMap["Null"]     = "";
    m_GrowthMap["Linear"]   = " + " + riBit;
    m_GrowthMap["Logistic"] = " + " + riBit + "(1 - (" + Bit + "/" + Ki + ")^(" + Pi + "))";

    m_GrowthKey["Null"]     = "";
    m_GrowthKey["Linear"]   = "B = Biomass<br/>r = Growth Rate<br/>";
    m_GrowthKey["Logistic"] = "B = Biomass<br/>r = Growth Rate<br/>p = Growth Rate Shape Exponent<br/>K = Carrying Capacity<br/>";
}

std::string
nmfGrowthForm::getPrefix()
{
    return m_Prefix;
}

std::string
nmfGrowthForm::getExpression()
{
    return m_GrowthMap[m_Type];
}

std::string
nmfGrowthForm::getKey()
{
    return m_GrowthKey[m_Type];
}

void
nmfGrowthForm::setAggProd(bool isAggProd)
{
    m_isAGGPROD = isAggProd;
}

void
nmfGrowthForm::loadParameterRanges(
        std::vector<double>& parameterInitialValues,
        std::vector<std::pair<double,double> >& parameterRanges,
        const nmfStructsQt::ModelDataStruct& dataStruct)
{
    bool isCheckedGrowthRate       = nmfUtils::isEstimateParameterChecked(dataStruct,"GrowthRate");
    bool isCheckedCarryingCapacity = nmfUtils::isEstimateParameterChecked(dataStruct,"CarryingCapacity");
    std::string speciesName;
    std::map<std::string,nmfStructsQt::CovariateStruct> covariateCoeffMap;
    nmfStructsQt::CovariateStruct covariateStruct;
    std::pair<double,double> aPair;

    m_NumSpecies = (int)dataStruct.SpeciesNames.size();

    if (m_NumSpecies != (int)dataStruct.GrowthRateMin.size()) {
        std::cout << "Error nmfGrowthForm::loadParameterRanges: GrowthRateMin size (" + std::to_string(dataStruct.GrowthRateMin.size()) +
                     ") is not the same as NumSpecies (" + std::to_string(m_NumSpecies) + ")" << std::endl;
        return;
    }
    if (m_NumSpecies != (int)dataStruct.CarryingCapacityMin.size()) {
        std::cout << "Error nmfGrowthForm::loadParameterRanges: CarryingCapacityMin size is not the same as NumSpecies" << std::endl;
        return;
    }

    // Always load growth rate values
    for (int species=0; species<m_NumSpecies; ++species) {
        if (isCheckedGrowthRate) {
            aPair = std::make_pair(dataStruct.GrowthRateMin[species],
                                   dataStruct.GrowthRateMax[species]);
        } else {
            aPair = std::make_pair(dataStruct.GrowthRate[species],  //-nmfConstantsMSSPM::epsilon,
                                   dataStruct.GrowthRate[species]); //+nmfConstantsMSSPM::epsilon);
        }
        parameterRanges.emplace_back(aPair);
        m_ParameterRanges.emplace_back(aPair);
        parameterInitialValues.emplace_back(dataStruct.GrowthRate[species]);
    }

    if (m_Type == "Logistic") {
        // Load growth rate shape parameter values
        for (int species=0; species<m_NumSpecies; ++species) {
            if (isCheckedGrowthRate) {
                aPair = std::make_pair(dataStruct.GrowthRateShapeMin[species],
                                       dataStruct.GrowthRateShapeMax[species]);
            } else {
                aPair = std::make_pair(dataStruct.GrowthRateShape[species],
                                       dataStruct.GrowthRateShape[species]);
            }
            parameterRanges.emplace_back(aPair);
            m_ParameterRanges.emplace_back(aPair);
            parameterInitialValues.emplace_back(dataStruct.GrowthRateShape[species]);
        }
    }

    // Always load growth rate covariate values
    for (int species=0; species<m_NumSpecies; ++species) {
        speciesName       = dataStruct.SpeciesNames[species];
        covariateCoeffMap = dataStruct.GrowthRateCovariateCoeff;
        covariateStruct   = covariateCoeffMap[speciesName];
        if (isCheckedGrowthRate) {
            aPair = std::make_pair(covariateStruct.CoeffMinValue,
                                   covariateStruct.CoeffMaxValue);
        } else {
            aPair = std::make_pair(covariateStruct.CoeffValue,
                                   covariateStruct.CoeffValue);
        }
        parameterRanges.emplace_back(aPair);
        m_ParameterRanges.emplace_back(aPair);
        parameterInitialValues.emplace_back(covariateStruct.CoeffValue);
    }

    if (m_Type == "Logistic") {
        // Load carrying capacity values
        for (int species=0; species<m_NumSpecies; ++species) {
            if (isCheckedCarryingCapacity) {
                aPair = std::make_pair(dataStruct.CarryingCapacityMin[species],
                                       dataStruct.CarryingCapacityMax[species]);
            } else {
                aPair = std::make_pair(dataStruct.CarryingCapacity[species],  //-nmfConstantsMSSPM::epsilon,
                                       dataStruct.CarryingCapacity[species]); //+nmfConstantsMSSPM::epsilon);
            }
            parameterRanges.emplace_back(aPair);
            m_ParameterRanges.emplace_back(aPair);
            parameterInitialValues.emplace_back(dataStruct.CarryingCapacity[species]);
        }

        // Load carrying capacity covariate coefficient values
        for (int species=0; species<m_NumSpecies; ++species) {
            speciesName       = dataStruct.SpeciesNames[species];
            covariateCoeffMap = dataStruct.CarryingCapacityCovariateCoeff;
            covariateStruct   = covariateCoeffMap[speciesName];
            if (isCheckedCarryingCapacity) {
                aPair = std::make_pair(covariateStruct.CoeffMinValue,
                                       covariateStruct.CoeffMaxValue);
            } else {
                aPair = std::make_pair(covariateStruct.CoeffValue,
                                       covariateStruct.CoeffValue);
            }
            parameterRanges.emplace_back(aPair);
            m_ParameterRanges.emplace_back(aPair);
            parameterInitialValues.emplace_back(covariateStruct.CoeffValue);
        }
    }
}

void
nmfGrowthForm::setType(std::string newType)
{
    m_Type = newType;
}

std::string
nmfGrowthForm::getType()
{
    return m_Type;
}

void
nmfGrowthForm::extractParameters(
        const std::vector<double>& parameters,
        int&                       startPos,
        std::vector<double>&       growthRate,
        std::vector<double>&       growthRateShape,
        std::vector<double>&       growthRateCovariateCoeffs,
        std::vector<double>&       carryingCapacity,
        std::vector<double>&       carryingCapacityCovariateCoeffs,
        double&                    systemCarryingCapacity)
{
    double tempK;

    systemCarryingCapacity = 0;
    growthRate.clear();
    growthRateShape.clear();
    carryingCapacity.clear();

    if (m_Type == "Linear") {
        for (int i=startPos; i<startPos+m_NumSpecies; ++i) {
            growthRate.emplace_back(parameters[i]);
            growthRateShape.emplace_back(0);
            carryingCapacity.emplace_back(0);
        }
        startPos += m_NumSpecies;
        for (int i=startPos; i<startPos+m_NumSpecies; ++i) {
            growthRateCovariateCoeffs.emplace_back(parameters[i]);
        }
        startPos += m_NumSpecies;
    } else if (m_Type == "Logistic") {
        for (int i=startPos; i<startPos+m_NumSpecies; ++i) {
            growthRate.emplace_back(parameters[i]);
        }
        startPos += m_NumSpecies;
        for (int i=startPos; i<startPos+m_NumSpecies; ++i) {
            growthRateCovariateCoeffs.emplace_back(parameters[i]);
        }
        startPos += m_NumSpecies;
        for (int i=startPos; i<startPos+m_NumSpecies; ++i) {
            growthRateShape.emplace_back(parameters[i]);
        }
        startPos += m_NumSpecies;
        for (int i=startPos; i<startPos+m_NumSpecies; ++i) {
            tempK = parameters[i];
            systemCarryingCapacity += tempK;
            carryingCapacity.emplace_back(tempK);
        }
        startPos += m_NumSpecies;
        for (int i=startPos; i<startPos+m_NumSpecies; ++i) {
            carryingCapacityCovariateCoeffs.emplace_back(parameters[i]);
        }
        startPos += m_NumSpecies;
    }
}

double
nmfGrowthForm::evaluate(const std::string& covariateAlgorithmType,
                        const double& biomassAtTimeT,
                        const double& growthRate,
                        const double& growthRateShape,
                        const double& growthRateCovariateCoeff,
                        const double& growthRateCovariate,
                        const double& carryingCapacity,
                        const double& carryingCapacityCovariateCoeff,
                        const double& carryingCapacityCovariate)
{
    if (FunctionMap.find(m_Type) == FunctionMap.end()) {
        return 0;
    } else {
        return (this->*FunctionMap[m_Type])(covariateAlgorithmType,
                                            biomassAtTimeT,
                                            growthRate,
                                            growthRateShape,
                                            growthRateCovariateCoeff,
                                            growthRateCovariate,
                                            carryingCapacity,
                                            carryingCapacityCovariateCoeff,
                                            carryingCapacityCovariate);
    }
}

double
nmfGrowthForm::FunctionMap_Null(const std::string& covariateAlgorithmType,
                                const double& biomassAtTime,
                                const double& growthRate,
                                const double& growthRateShape,
                                const double& growthRateCovariateCoeff,
                                const double& growthRateCovariate,
                                const double& carryingCapacity,
                                const double& carryingCapacityCovariateCoeff,
                                const double& carryingCapacityCovariate)
{
    return 0.0;
}

double
nmfGrowthForm::FunctionMap_Linear(const std::string& covariateAlgorithmType,
                                  const double& biomassAtTime,
                                  const double& growthRate,
                                  const double& growthRateShape,
                                  const double& growthRateCovariateCoeff,
                                  const double& growthRateCovariate,
                                  const double& carryingCapacity,
                                  const double& carryingCapacityCovariateCoeff,
                                  const double& carryingCapacityCovariate)
{
//  double growthRateTerm = growthRate*(1.0+growthRateCovariateCoeff*growthRateCovariate);
    double growthRateTerm = nmfUtils::applyCovariate(nullptr,
                covariateAlgorithmType,growthRate,
                growthRateCovariateCoeff,growthRateCovariate);

    return growthRateTerm*biomassAtTime;
}

double
nmfGrowthForm::FunctionMap_Logistic(const std::string& covariateAlgorithmType,
                                    const double& biomassAtTime,
                                    const double& growthRate,
                                    const double& growthRateShape,
                                    const double& growthRateCovariateCoeff,
                                    const double& growthRateCovariate,
                                    const double& carryingCapacity,
                                    const double& carryingCapacityCovariateCoeff,
                                    const double& carryingCapacityCovariate)
{
//  double growthRateTerm       = growthRate       * (1.0+growthRateCovariateCoeff       * growthRateCovariate);
//  double carryingCapacityTerm = carryingCapacity * (1.0+carryingCapacityCovariateCoeff * carryingCapacityCovariate);

    double growthRateTerm = nmfUtils::applyCovariate(nullptr,
                covariateAlgorithmType,growthRate,
                growthRateCovariateCoeff,growthRateCovariate);
//std::cout << "growth term: " << growthRateTerm <<
//             ", covType: " << covariateAlgorithmType <<
//             ", r: " << growthRate <<
//             ", coeff: " <<growthRateCovariateCoeff <<
//             ", cov: " << growthRateCovariate << std::endl;
    double carryingCapacityTerm = nmfUtils::applyCovariate(nullptr,
                covariateAlgorithmType,carryingCapacity,
                carryingCapacityCovariateCoeff,carryingCapacityCovariate);

//  return (growthRateTerm * biomassAtTime * (1.0 - biomassAtTime/carryingCapacityTerm));
    return (growthRateTerm * biomassAtTime * (1.0 - std::pow((biomassAtTime/carryingCapacityTerm),growthRateShape)));

}
