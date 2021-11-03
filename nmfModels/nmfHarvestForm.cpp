
#include "nmfHarvestForm.h"

nmfHarvestForm::nmfHarvestForm(std::string harvestType)
{
    m_numParameters = 0;
    m_type = harvestType;
    m_parameterRanges.clear();
    m_isAGGPROD = false;
    m_HarvestMap.clear();
    m_HarvestKey.clear();

    m_FunctionMap["Null"]             = &nmfHarvestForm::NoHarvest;
    m_FunctionMap["Catch"]            = &nmfHarvestForm::CatchHarvest;
    m_FunctionMap["Effort (qE)"]      = &nmfHarvestForm::EffortHarvest;
    m_FunctionMap["Exploitation (F)"] = &nmfHarvestForm::ExploitationHarvest;

    setupFormMaps();
}


void
nmfHarvestForm::setType(std::string newType)
{
    m_type = newType;
}

int
nmfHarvestForm::getNumParameters()
{
    return m_numParameters;
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

    m_HarvestMap["Null"]             = "";
    m_HarvestMap["Exploitation (F)"] = " - " +  Fit  + Bit;
    m_HarvestMap["Effort (qE)"]      = " - " + qiEit + Bit;
    m_HarvestMap["Catch"]            = " - " +  Cit;

    m_HarvestKey["Null"]             = "";
    m_HarvestKey["Exploitation (F)"] = "F = Exploitation Rate<br/>";
    m_HarvestKey["Effort (qE)"]      = "q = Catchability<br/>E = Effort<br/>";
    m_HarvestKey["Catch"]            = "C = Catch<br/>";
}

std::string
nmfHarvestForm::getExpression()
{
    return m_HarvestMap[m_type];
}

std::string
nmfHarvestForm::getKey()
{
    return m_HarvestKey[m_type];
}

void
nmfHarvestForm::extractParameters(
        const std::vector<double> &parameters,
        int& startPos,
        std::vector<double> &catchabilityRate)
{
    int numParameters = getNumParameters();

    catchabilityRate.clear();

//    exploitationRate.clear();
//    if (m_type == "Exploitation (F)") {
//        for (int i=startPos; i<startPos+numParameters; ++i) {
//            exploitationRate.emplace_back(parameters[i]);
//        }
//        startPos += numParameters;
//    } else

    if (m_type == "Effort (qE)") {
        for (int i=startPos; i<startPos+numParameters; ++i) {
            catchabilityRate.emplace_back(parameters[i]);
        }
        startPos += numParameters;
    }
}


void
nmfHarvestForm::loadParameterRanges(
        std::vector<std::pair<double,double> >& parameterRanges,
        nmfStructsQt::ModelDataStruct& dataStruct)

{
    std::pair<double,double> aPair;
    bool isCheckedCatchability = nmfUtils::isEstimateParameterChecked(dataStruct,"Catchability");

    m_numParameters = 0;

    if (m_type == "Null")
        return;

    if (m_type == "Effort (qE)") {
        for (unsigned i=0; i<dataStruct.CatchabilityMin.size(); ++i) {
            if (isCheckedCatchability) {
                aPair = std::make_pair(dataStruct.CatchabilityMin[i],
                                       dataStruct.CatchabilityMax[i]);
            } else {
                aPair = std::make_pair(dataStruct.Catchability[i],
                                       dataStruct.Catchability[i]);
            }
            parameterRanges.emplace_back(aPair);
            m_parameterRanges.emplace_back(aPair);
        }
        m_numParameters = dataStruct.CatchabilityMin.size();
    }

}


double
nmfHarvestForm::evaluate(const int& time,
                         const int& species,
                         const double& biomassAtTime,
                         const boost::numeric::ublas::matrix<double>& Catch,
                         const boost::numeric::ublas::matrix<double>& Effort,
                         const boost::numeric::ublas::matrix<double>& Exploitation,
                         const std::vector<double>& catchabilityRate,
                         const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate)
{
    if (m_FunctionMap.find(m_type) == m_FunctionMap.end()) {
        return 0;
    } else {
        return (this->*m_FunctionMap[m_type])(
                    time,species,biomassAtTime,
                    Catch,Effort,Exploitation,
                    catchabilityRate,catchabilityRateCovariate);
    }
}


double
nmfHarvestForm::NoHarvest(const int& time,
                          const int& species,
                          const double& biomassAtTime,
                          const boost::numeric::ublas::matrix<double>& Catch,
                          const boost::numeric::ublas::matrix<double>& Effort,
                          const boost::numeric::ublas::matrix<double>& Exploitation,
                          const std::vector<double>& catchabilityRate,
                          const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate)
{
    return 0.0;
}

double
nmfHarvestForm::CatchHarvest(const int& time,
                             const int& species,
                             const double& biomassAtTime,
                             const boost::numeric::ublas::matrix<double>& Catch,
                             const boost::numeric::ublas::matrix<double>& Effort,
                             const boost::numeric::ublas::matrix<double>& Exploitation,
                             const std::vector<double>& catchabilityRate,
                             const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate)
{
   return Catch(time,species);
}


double
nmfHarvestForm::EffortHarvest(const int& time,
                              const int& species,
                              const double& biomassAtTime,
                              const boost::numeric::ublas::matrix<double>& Catch,
                              const boost::numeric::ublas::matrix<double>& Effort,
                              const boost::numeric::ublas::matrix<double>& Exploitation,
                              const std::vector<double>& catchabilityRate,
                              const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate)
{
    double catchabilityRateCovariateCoeff = 1.0; // RSK estimate this later
    double catchabilityFactor = catchabilityRate[species]*(1.0+catchabilityRateCovariateCoeff*catchabilityRateCovariate(time,species));

    return (catchabilityFactor*Effort(time,species)*biomassAtTime);
}


double
nmfHarvestForm::ExploitationHarvest(const int& time,
                                    const int& species,
                                    const double& biomassAtTime,
                                    const boost::numeric::ublas::matrix<double>& Catch,
                                    const boost::numeric::ublas::matrix<double>& Effort,
                                    const boost::numeric::ublas::matrix<double>& Exploitation,
                                    const std::vector<double>& catchabilityRate,
                                    const boost::numeric::ublas::matrix<double>& catchabilityRateCovariate)
{
   return Exploitation(time,species)*biomassAtTime;
}
