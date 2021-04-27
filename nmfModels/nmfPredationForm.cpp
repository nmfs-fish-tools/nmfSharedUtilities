
#include "nmfPredationForm.h"

nmfPredationForm::nmfPredationForm(std::string predationType)
{
    m_type             = predationType;
    m_numSpecies       = 0;
    m_numberParameters = 0;
    m_parameterRanges.clear();
    m_isAggProd        = false;

    m_FunctionMap["Null"]     = &nmfPredationForm::TypeNullPredation;
    m_FunctionMap["Type I"]   = &nmfPredationForm::TypeIPredation;
    m_FunctionMap["Type II"]  = &nmfPredationForm::TypeIIPredation;
    m_FunctionMap["Type III"] = &nmfPredationForm::TypeIIIPredation;
}

void
nmfPredationForm::setType(std::string newType)
{
    m_type = newType;
}

int
nmfPredationForm::getNumParameters()
{
    return m_numberParameters; // m_parameterRanges.size();
}

void
nmfPredationForm::extractPredationParameters(
        const std::vector<double> &parameters,
        int& startPos,
        boost::numeric::ublas::matrix<double> &predation)
{
    if (m_type != "Null") {
        for (int i=0; i<m_NumSpeciesOrGuilds; ++i) {
            for (int j=0; j<m_NumSpeciesOrGuilds; ++j) {
                predation(i,j) = parameters[startPos++];
            }
        }
    }
}

void
nmfPredationForm::extractHandlingParameters(
        const std::vector<double> &parameters,
        int& startPos,
        boost::numeric::ublas::matrix<double> &handling)
{
    if ((m_type == "Type II") || (m_type == "Type III"))
    {
        for (int i=0; i<m_NumSpeciesOrGuilds; ++i) {
            for (int j=0; j<m_NumSpeciesOrGuilds; ++j) {
                handling(i,j) = parameters[startPos++];
            }
        }
    }
}

void
nmfPredationForm::extractExponentParameters(
        const std::vector<double> &parameters,
        int& startPos,
        std::vector<double> &exponents)
{
    if (m_type == "Type III")
    {
        exponents.clear();
        for (int i=0; i<m_NumSpeciesOrGuilds; ++i) {
            exponents.push_back(parameters[startPos++]);
        }
    }
}

void
nmfPredationForm::loadParameterRanges(
        std::vector<std::pair<double,double> >& parameterRanges,
        Data_Struct& dataStruct)
{
    bool isCheckedRho      = nmfUtils::isEstimateParameterChecked(dataStruct,"PredationRho");
    bool isCheckedHandling = nmfUtils::isEstimateParameterChecked(dataStruct,"Handling");
    bool isCheckedExponent = nmfUtils::isEstimateParameterChecked(dataStruct,"PredationExponent");
    double min;
    std::pair<double,double> aPair;

    if (m_type == "Null")
        return;

    m_numSpecies = dataStruct.NumSpecies;
    m_numGuilds  = dataStruct.NumGuilds;
    m_isAggProd  = (dataStruct.CompetitionForm == "AGG-PROD");
    m_NumSpeciesOrGuilds = (m_isAggProd) ? m_numGuilds : m_numSpecies;

    for (unsigned i=0; i<dataStruct.PredationMin.size(); ++i) {
        for (unsigned int j=0; j<dataStruct.PredationMin[0].size(); ++j) {
            if (isCheckedRho) {
                aPair = std::make_pair(dataStruct.PredationMin[i][j],
                                       dataStruct.PredationMax[i][j]);
            } else {
                min   = dataStruct.PredationMin[i][j];
                aPair = std::make_pair(min,min);
            }
            parameterRanges.emplace_back(aPair);
            m_parameterRanges.emplace_back(aPair);
        }
    }
    m_numberParameters = dataStruct.PredationMin.size() *
                         dataStruct.PredationMin[0].size();

    if ((m_type == "Type II") || (m_type == "Type III")) {
        for (unsigned i=0; i<dataStruct.HandlingMin.size(); ++i) {
            for (unsigned int j=0; j<dataStruct.HandlingMin[0].size(); ++j) {
                if (isCheckedHandling) {
                    aPair = std::make_pair(dataStruct.HandlingMin[i][j],
                                           dataStruct.HandlingMax[i][j]);
                } else {
                    min   = dataStruct.HandlingMin[i][j];
                    aPair = std::make_pair(min,min);
                }
                parameterRanges.emplace_back(aPair);
                m_parameterRanges.emplace_back(aPair);
            }
        }
        m_numberParameters += dataStruct.HandlingMin.size() *
                              dataStruct.HandlingMin[0].size();
    }

    if (m_type == "Type III") {
        for (unsigned i=0; i<dataStruct.ExponentMin.size(); ++i) {
            if (isCheckedExponent) {
                aPair = std::make_pair(dataStruct.ExponentMin[i],
                                       dataStruct.ExponentMax[i]);
            } else {
                min   = dataStruct.ExponentMin[i];
                aPair = std::make_pair(min,min);
            }
            parameterRanges.emplace_back(aPair);
            m_parameterRanges.emplace_back(aPair);
        }
        m_numberParameters += dataStruct.ExponentMin.size();
    }
}

double
nmfPredationForm::evaluate(const int &timeMinus1,
                           const int &SpeciesNum,
                           const boost::numeric::ublas::matrix<double> &EstPredation,
                           const boost::numeric::ublas::matrix<double> &EstHandling,
                           const std::vector<double> &EstExponent,
                           const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                           const double &EstimatedBiomassTimeMinus1)
{

    if (m_FunctionMap.find(m_type) == m_FunctionMap.end()) {
        return 0;
    } else {
        return (this->*m_FunctionMap[m_type])(timeMinus1,SpeciesNum,
                                     EstPredation,EstHandling,EstExponent,
                                     EstimatedBiomass,EstimatedBiomassTimeMinus1);
    }
}

double
nmfPredationForm::TypeNullPredation(const int &timeMinus1,
                                    const int &SpeciesNum,
                                    const boost::numeric::ublas::matrix<double> &EstPredation,
                                    const boost::numeric::ublas::matrix<double> &EstHandling,
                                    const std::vector<double> &EstExponent,
                                    const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                                    const double &EstimatedBiomassTimeMinus1)
{
    return 0.0;
}

double
nmfPredationForm::TypeIPredation(const int &timeMinus1,
                                 const int &SpeciesNum,
                                 const boost::numeric::ublas::matrix<double> &EstPredation,
                                 const boost::numeric::ublas::matrix<double> &EstHandling,
                                 const std::vector<double> &EstExponent,
                                 const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                                 const double &biomassAtTimeMinus1)
{
    //  B(i,t)∑ρ(i,j)B(j,t)
    double PredationSum = 0;

    for (int row=0; row<int(EstPredation.size2()); ++row) {
        PredationSum += EstPredation(row,SpeciesNum) * EstimatedBiomass(timeMinus1,row);
    }

    return biomassAtTimeMinus1*PredationSum;
}

double
nmfPredationForm::TypeIIPredation(const int &timeMinus1,
                                  const int &SpeciesNum,
                                  const boost::numeric::ublas::matrix<double> &EstPredation,
                                  const boost::numeric::ublas::matrix<double> &EstHandling,
                                  const std::vector<double> &EstExponent,
                                  const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                                  const double &biomassAtTimeMinus1)
{
    // B(i,t)∑[ρ(i,j)B(j,t)/(1+∑h(k,j)ρ(k,j)B(k,t))]

    int    NumSpecies   = EstPredation.size2();
    double predationSum = 0;
    double handlingSum;
    double numerator;
    double denominator;

    for (int row=0; row<NumSpecies; ++row) {
        numerator   = EstPredation(row,SpeciesNum) * EstimatedBiomass(timeMinus1,row);
        handlingSum = 0;
        for (int j=0; j<NumSpecies; ++j) {
           handlingSum += EstHandling(j,row) *
                          EstPredation(j,row) *
                          EstimatedBiomass(timeMinus1,row);
        }
        denominator   = (1.0 + handlingSum);
        predationSum += (numerator / denominator);
    }
//std::cout << "-----------> TypeIIPredation: " << biomassAtTimeMinus1*predationSum << std::endl;

    return biomassAtTimeMinus1*predationSum;
}

double
nmfPredationForm::TypeIIIPredation(const int &timeMinus1,
                                   const int &SpeciesNum,
                                   const boost::numeric::ublas::matrix<double> &EstPredation,
                                   const boost::numeric::ublas::matrix<double> &EstHandling,
                                   const std::vector<double> &EstExponent,
                                   const boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                                   const double &EstimatedBiomassTimeMinus1)
{
    // B(i,t)^(bᵢ+1) ∑[ρ(i,j)B(j,t)/ (1+∑h(k,j)ρ(k,j)B(k,t)^(bₖ+1))]

    int    NumSpecies     = EstPredation.size1();
    double predationSum = 0;
    double handlingSum;
    double numerator;
    double denominator;

    for (int col=0; col<NumSpecies; ++col) {
        numerator   = EstPredation(SpeciesNum,col) * EstimatedBiomass(timeMinus1,col);
        handlingSum = 0;
        for (int j=0; j<NumSpecies; ++j) {
           handlingSum += EstHandling(j,col) *
                          EstPredation(j,col) *
                          std::pow(EstimatedBiomass(timeMinus1,col),EstExponent[j]+1);
        }
        denominator   = (1.0 + handlingSum);
        predationSum += (numerator / denominator);
    }

    return std::pow(EstimatedBiomassTimeMinus1,EstExponent[SpeciesNum]+1)*predationSum;
}
