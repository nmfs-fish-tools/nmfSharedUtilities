
#include "nmfHarvestForm.h"

nmfHarvestForm::nmfHarvestForm(std::string harvestType)
{
    m_NumSpecies = 0;
    m_Type = harvestType;
    m_ParameterRanges.clear();
    m_isAGGPROD = false;
    m_HarvestMap.clear();
    m_HarvestKey.clear();

    m_FunctionMap["Null"]                = &nmfHarvestForm::FunctionMap_Null;
    m_FunctionMap["Catch"]               = &nmfHarvestForm::FunctionMap_Catch;
    m_FunctionMap["Effort (qE)"]         = &nmfHarvestForm::FunctionMap_Effort;
    m_FunctionMap["Effort Fit to Catch"] = &nmfHarvestForm::FunctionMap_Effort;
    m_FunctionMap["Exploitation (F)"]    = &nmfHarvestForm::FunctionMap_Exploitation;

    setupFormMaps();
}


void
nmfHarvestForm::setType(std::string newType)
{
    m_Type = newType;
}

void
nmfHarvestForm::setAggProd(bool isAggProd)
{
    m_isAGGPROD = isAggProd;
}

void
nmfHarvestForm::setupFormMaps()
{
    std::string index1 = (m_isAGGPROD) ? "I" : "i";
    std::string Bit    = "B<sub>"+index1+",t</sub>";       // right hand side of equation
    std::string Fit    = "F<sub>"+index1+",t</sub>";
    std::string qiEit  = "q<sub>"+index1+"</sub>E<sub>"+index1+",t</sub>";
    std::string Cit    = "C<sub>"+index1+",t</sub>";

    m_HarvestMap["Null"]                = "";
    m_HarvestMap["Exploitation (F)"]    = " - " +  Fit  + Bit;
    m_HarvestMap["Effort (qE)"]         = " - " + qiEit + Bit;
    m_HarvestMap["Effort Fit to Catch"] = " - " + qiEit + Bit;
    m_HarvestMap["Catch"]               = " - " +  Cit;

    m_HarvestKey["Null"]                = "";
    m_HarvestKey["Exploitation (F)"]    = "F = Exploitation Rate<br/>";
    m_HarvestKey["Effort (qE)"]         = "q = Catchability<br/>E = Effort<br/>";
    m_HarvestKey["Effort Fit to Catch"] = "q = Catchability<br/>E = Effort<br/>";
    m_HarvestKey["Catch"]               = "C = Catch<br/>";
}

std::string
nmfHarvestForm::getExpression()
{
    return m_HarvestMap[m_Type];
}

std::string
nmfHarvestForm::getKey()
{
    return m_HarvestKey[m_Type];
}

void
nmfHarvestForm::extractParameters(
        const std::vector<double>& parameters,
        int& startPos,
        std::vector<double>& catchability,
        std::vector<double>& catchabilityCovariateCoeffs)
{
    catchability.clear();

//    exploitationRate.clear();
//    if (m_type == "Exploitation (F)") {
//        for (int i=startPos; i<startPos+numParameters; ++i) {
//            exploitationRate.emplace_back(parameters[i]);
//        }
//        startPos += numParameters;
//    } else

    if ((m_Type == "Effort (qE)") || (m_Type == "Effort Fit to Catch")) {
        for (int i=startPos; i<startPos+m_NumSpecies; ++i) {
            catchability.emplace_back(parameters[i]);
        }
        startPos += m_NumSpecies;
        for (int i=startPos; i<startPos+m_NumSpecies; ++i) {
            catchabilityCovariateCoeffs.emplace_back(parameters[i]);
        }
        startPos += m_NumSpecies;
    }
}


void
nmfHarvestForm::loadParameterRanges(
        std::vector<std::pair<double,double> >& parameterRanges,
        nmfStructsQt::ModelDataStruct& dataStruct)

{
    std::pair<double,double> aPair;
    bool isCheckedCatchability = nmfUtils::isEstimateParameterChecked(dataStruct,"Catchability");
    std::string speciesName;
    std::map<std::string,nmfStructsQt::CovariateStruct> covariateCoeffMap;
    nmfStructsQt::CovariateStruct covariateStruct;

    m_NumSpecies = (int)dataStruct.SpeciesNames.size();
    if (m_NumSpecies != (int)dataStruct.CatchabilityMin.size()) {
        std::cout << "Error nmfHarvestForm::loadParameterRanges: CatchabilityMin size is not the same as NumSpecies" << std::endl;
        return;
    }

    if (m_Type == "Null") {
        return;
    }

    if ((m_Type == "Effort (qE)") || (m_Type == "Effort Fit to Catch")) {
        // Load the catchability values
        for (int species=0; species<m_NumSpecies; ++species) {
            if (isCheckedCatchability) {
                aPair = std::make_pair(dataStruct.CatchabilityMin[species],
                                       dataStruct.CatchabilityMax[species]);
            } else {
                aPair = std::make_pair(dataStruct.Catchability[species],
                                       dataStruct.Catchability[species]);
            }
            parameterRanges.emplace_back(aPair);
            m_ParameterRanges.emplace_back(aPair);
        }

        // Load catchability covariate coefficient values
        for (int species=0; species<m_NumSpecies; ++species) {
            speciesName       = dataStruct.SpeciesNames[species];
            covariateCoeffMap = dataStruct.CatchabilityCovariateCoeff;
            covariateStruct   = covariateCoeffMap[speciesName];
            if (isCheckedCatchability) {
                aPair = std::make_pair(covariateStruct.CoeffMinValue,
                                       covariateStruct.CoeffMaxValue);
            } else {
                aPair = std::make_pair(covariateStruct.CoeffValue,
                                       covariateStruct.CoeffValue);
            }
            parameterRanges.emplace_back(aPair);
            m_ParameterRanges.emplace_back(aPair);
        }
    }
}


double
nmfHarvestForm::evaluate(const std::string& covariateAlgorithmType,
                         const int& timeMinus1,
                         const int& species,
                         const double& biomassAtTime,
                         const boost::numeric::ublas::matrix<double>& Catch,
                         const boost::numeric::ublas::matrix<double>& Effort,
                         const boost::numeric::ublas::matrix<double>& Exploitation,
                         const double& catchability,
                         const double& catchabilityCovariateCoeff,
                         const double& catchabilityCovariate)
{
    if (m_FunctionMap.find(m_Type) == m_FunctionMap.end()) {
        return 0;
    } else {
        return (this->*m_FunctionMap[m_Type])(
                    covariateAlgorithmType,
                    timeMinus1,species,biomassAtTime,
                    Catch,Effort,Exploitation,
                    catchability,catchabilityCovariateCoeff,
                    catchabilityCovariate);
    }
}


double
nmfHarvestForm::FunctionMap_Null(const std::string& covariateAlgorithmType,
                                 const int& timeMinus1,
                                 const int& species,
                                 const double& biomassAtTime,
                                 const boost::numeric::ublas::matrix<double>& Catch,
                                 const boost::numeric::ublas::matrix<double>& Effort,
                                 const boost::numeric::ublas::matrix<double>& Exploitation,
                                 const double& catchability,
                                 const double& catchabilityCovariateCoeff,
                                 const double& catchabilityCovariate)
{
    return 0.0;
}

double
nmfHarvestForm::FunctionMap_Catch(const std::string& covariateAlgorithmType,
                                  const int& timeMinus1,
                                  const int& species,
                                  const double& biomassAtTime,
                                  const boost::numeric::ublas::matrix<double>& Catch,
                                  const boost::numeric::ublas::matrix<double>& Effort,
                                  const boost::numeric::ublas::matrix<double>& Exploitation,
                                  const double& catchability,
                                  const double& catchabilityCovariateCoeff,
                                  const double& catchabilityCovariate)
{
    return Catch(timeMinus1,species);
}


double
nmfHarvestForm::FunctionMap_Effort(const std::string& covariateAlgorithmType,
                                   const int& timeMinus1,
                                   const int& species,
                                   const double& biomassAtTime,
                                   const boost::numeric::ublas::matrix<double>& Catch,
                                   const boost::numeric::ublas::matrix<double>& Effort,
                                   const boost::numeric::ublas::matrix<double>& Exploitation,
                                   const double& catchability,
                                   const double& catchabilityCovariateCoeff,
                                   const double& catchabilityCovariate)
{
//  double catchabilityFactor = catchability*(1.0+catchabilityCovariateCoeff*catchabilityCovariate);
    double catchabilityFactor = nmfUtils::applyCovariate(nullptr,
                       covariateAlgorithmType,catchability,
                       catchabilityCovariateCoeff,catchabilityCovariate);

    return (catchabilityFactor*Effort(timeMinus1,species)*biomassAtTime);
}


double
nmfHarvestForm::FunctionMap_Exploitation(const std::string& covariateAlgorithmType,
                                         const int& timeMinus1,
                                         const int& species,
                                         const double& biomassAtTime,
                                         const boost::numeric::ublas::matrix<double>& Catch,
                                         const boost::numeric::ublas::matrix<double>& Effort,
                                         const boost::numeric::ublas::matrix<double>& Exploitation,
                                         const double& catchability,
                                         const double& catchabilityCovariateCoeff,
                                         const double& catchabilityCovariate)
{
    return Exploitation(timeMinus1,species)*biomassAtTime;
}
