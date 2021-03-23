
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
        Data_Struct& beeStruct)

{
    std::pair<double,double> aPair;

//    if (m_type == "Exploitation (F)") {
//        for (unsigned i=0; i<beeStruct.ExploitationRateMin.size(); ++i) {
//            aPair = std::make_pair(beeStruct.ExploitationRateMin[i],
//                                   beeStruct.ExploitationRateMax[i]);
//            parameterRanges.emplace_back(aPair);
//            m_parameterRanges.emplace_back(aPair);
//        }
//        m_numParameters = beeStruct.ExploitationRateMin.size();
//    } else
    if (m_type == "Effort (qE)") {
        for (unsigned i=0; i<beeStruct.CatchabilityMin.size(); ++i) {
            aPair = std::make_pair(beeStruct.CatchabilityMin[i],
                                   beeStruct.CatchabilityMax[i]);
            parameterRanges.emplace_back(aPair);
            m_parameterRanges.emplace_back(aPair);
        }
        m_numParameters = beeStruct.CatchabilityMin.size();
    } else if (m_type == "Catch") {
        m_numParameters = 0;
    }
}


double
nmfHarvestForm::evaluate(const int& timeT,
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
        return (this->*m_FunctionMap[m_type])(timeT,speciesNum,Catch,Effort,Exploitation,
                                              biomassAtTime,catchabilityRate);
    }
}


double
nmfHarvestForm::NoHarvest(const int &timeT,
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
nmfHarvestForm::CatchHarvest(const int &timeT,
                             const int &speciesNum,
                             const boost::numeric::ublas::matrix<double> &Catch,
                             const boost::numeric::ublas::matrix<double> &Effort,
                             const boost::numeric::ublas::matrix<double> &Exploitation,
                             const double& biomassAtTime,
                             const std::vector<double>& catchabilityRate)
{
   return Catch(timeT,speciesNum);
}


double
nmfHarvestForm::EffortHarvest(const int& timeT,
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
            Effort(timeT,speciesNum)*
            biomassAtTime);
}


double
nmfHarvestForm::ExploitationHarvest(const int& timeT,
                                    const int& speciesNum,
                                    const boost::numeric::ublas::matrix<double> &Catch,
                                    const boost::numeric::ublas::matrix<double> &Effort,
                                    const boost::numeric::ublas::matrix<double> &Exploitation,
                                    const double& biomassAtTime,
                                    const std::vector<double>& catchabilityRate)
{
   return Exploitation(timeT,speciesNum)*biomassAtTime;
}
