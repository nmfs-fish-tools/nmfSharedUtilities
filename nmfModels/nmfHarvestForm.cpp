
#include "nmfHarvestForm.h"

nmfHarvestForm::nmfHarvestForm(std::string harvestType)
{
    m_numParameters = 0;
    m_type = harvestType;
    m_parameterRanges.clear();

    m_FunctionMap["Null"]             = &nmfHarvestForm::NoHarvest;
    m_FunctionMap["Catch"]            = &nmfHarvestForm::CatchHarvest;
    m_FunctionMap["Effort (qE)"]      = &nmfHarvestForm::EffortHarvest;
    m_FunctionMap["Exploitation (F)"] = &nmfHarvestForm::ExploitationHarvest;
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
        Data_Struct& dataStruct)

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
nmfHarvestForm::evaluate(const int& timeMinus1,
                         const int& speciesNum,
                         const boost::numeric::ublas::matrix<double>& Catch,
                         const boost::numeric::ublas::matrix<double>& Effort,
                         const boost::numeric::ublas::matrix<double>& Exploitation,
                         const double& biomassAtTime,
                         const std::vector<double>& catchabilityRate)
{
    if (m_FunctionMap.find(m_type) == m_FunctionMap.end()) {
        return 0;
    } else {
        return (this->*m_FunctionMap[m_type])(timeMinus1,speciesNum,Catch,Effort,Exploitation,
                                              biomassAtTime,catchabilityRate);
    }
}


double
nmfHarvestForm::NoHarvest(const int &timeMinus1,
                          const int &speciesNum,
                          const boost::numeric::ublas::matrix<double> &Catch,
                          const boost::numeric::ublas::matrix<double> &Effort,
                          const boost::numeric::ublas::matrix<double> &Exploitation,
                          const double& biomassAtTime,
                          const std::vector<double>& catchabilityRate)
{
    return 0.0;
}

double
nmfHarvestForm::CatchHarvest(const int &timeMinus1,
                             const int &speciesNum,
                             const boost::numeric::ublas::matrix<double> &Catch,
                             const boost::numeric::ublas::matrix<double> &Effort,
                             const boost::numeric::ublas::matrix<double> &Exploitation,
                             const double& biomassAtTime,
                             const std::vector<double>& catchabilityRate)
{
   return Catch(timeMinus1,speciesNum);
}


double
nmfHarvestForm::EffortHarvest(const int& timeMinus1,
                              const int& speciesNum,
                              const boost::numeric::ublas::matrix<double> &Catch,
                              const boost::numeric::ublas::matrix<double> &Effort,
                              const boost::numeric::ublas::matrix<double> &Exploitation,
                              const double& biomassAtTime,
                              const std::vector<double>& catchabilityRate)
{
    if (catchabilityRate.size() == 0) {
        std::cout << "ERROR: No catchabilityRate rate found.  Please update code." << std::endl;
        return 0;
    }

    return (catchabilityRate[speciesNum]*
            Effort(timeMinus1,speciesNum)*
            biomassAtTime);
}


double
nmfHarvestForm::ExploitationHarvest(const int& timeMinus1,
                                    const int& speciesNum,
                                    const boost::numeric::ublas::matrix<double> &Catch,
                                    const boost::numeric::ublas::matrix<double> &Effort,
                                    const boost::numeric::ublas::matrix<double> &Exploitation,
                                    const double& biomassAtTime,
                                    const std::vector<double>& catchabilityRate)
{
   return Exploitation(timeMinus1,speciesNum)*biomassAtTime;
}
