
#include "nmfPredationForm.h"

nmfPredationForm::nmfPredationForm(std::string predationType)
{
    m_type             = predationType;
    m_numSpecies       = 0;
    m_numberParameters = 0;
    m_parameterRanges.clear();
    m_isAggProd        = false;

    m_FunctionMap["Type I"]   = TypeIPredation;
    m_FunctionMap["Type II"]  = TypeIIPredation;
    m_FunctionMap["Type III"] = TypeIIIPredation;
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
        Data_Struct& beeStruct)
{
    std::pair<double,double> aPair;

    if (m_type == "Null")
        return;

    m_numSpecies = beeStruct.NumSpecies;
    m_numGuilds  = beeStruct.NumGuilds;
    m_isAggProd  = (beeStruct.CompetitionForm == "AGG-PROD");
    m_NumSpeciesOrGuilds = (m_isAggProd) ? m_numGuilds : m_numSpecies;

    for (unsigned i=0; i<beeStruct.PredationMin.size(); ++i) {
        for (unsigned int j=0; j<beeStruct.PredationMin[0].size(); ++j) {
            aPair = std::make_pair(beeStruct.PredationMin[i][j],
                                   beeStruct.PredationMax[i][j]);
            parameterRanges.emplace_back(aPair);
            m_parameterRanges.emplace_back(aPair);
        }
    }
    m_numberParameters = beeStruct.PredationMin.size() *
                         beeStruct.PredationMin[0].size();

    if ((m_type == "Type II") || (m_type == "Type III")) {
        for (unsigned i=0; i<beeStruct.HandlingMin.size(); ++i) {
            for (unsigned int j=0; j<beeStruct.HandlingMin[0].size(); ++j) {
                aPair = std::make_pair(beeStruct.HandlingMin[i][j],
                                       beeStruct.HandlingMax[i][j]);
                parameterRanges.emplace_back(aPair);
                m_parameterRanges.emplace_back(aPair);
            }
        }
        m_numberParameters += beeStruct.HandlingMin.size() *
                              beeStruct.HandlingMin[0].size();
    }

    if (m_type == "Type III") {
        for (unsigned i=0; i<beeStruct.ExponentMin.size(); ++i) {
            aPair = std::make_pair(beeStruct.ExponentMin[i],
                                   beeStruct.ExponentMax[i]);
            parameterRanges.emplace_back(aPair);
            m_parameterRanges.emplace_back(aPair);
        }
        m_numberParameters += beeStruct.ExponentMin.size();
    }
}

double
nmfPredationForm::evaluate(int &timeMinus1,
                           int &SpeciesNum,
                           boost::numeric::ublas::matrix<double> &EstPredation,
                           boost::numeric::ublas::matrix<double> &EstHandling,
                           std::vector<double> &EstExponent,
                           boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                           double &EstimatedBiomassTimeMinus1)
{

    if (m_FunctionMap.find(m_type) == m_FunctionMap.end()) {
        return 0;
    } else {
        return m_FunctionMap[m_type](timeMinus1,SpeciesNum,
                                     EstPredation,EstHandling,EstExponent,
                                     EstimatedBiomass,EstimatedBiomassTimeMinus1);
    }
}


double
nmfPredationForm::TypeIPredation(int &timeMinus1,
                                 int &SpeciesNum,
                                 boost::numeric::ublas::matrix<double> &EstPredation,
                                 boost::numeric::ublas::matrix<double> &EstHandling,
                                 std::vector<double> &EstExponent,
                                 boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                                 double &EstimatedBiomassTimeMinus1)
{
    //  B(i,t)∑ρ(i,j)B(j,t)
    int    NumSpecies   = EstPredation.size1();
    double PredationSum = 0;

    for (int col=0; col<NumSpecies; ++col) {
        PredationSum += EstPredation(SpeciesNum,col) * EstimatedBiomass(timeMinus1,col);
    }

    return EstimatedBiomassTimeMinus1*PredationSum;
}

double
nmfPredationForm::TypeIIPredation(int &timeMinus1,
                                  int &SpeciesNum,
                                  boost::numeric::ublas::matrix<double> &EstPredation,
                                  boost::numeric::ublas::matrix<double> &EstHandling,
                                  std::vector<double> &EstExponent,
                                  boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                                  double &EstimatedBiomassTimeMinus1)
{
    // B(i,t)∑[ρ(i,j)B(j,t)/(1+∑h(k,j)ρ(k,j)B(k,t))]

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
                          EstimatedBiomass(timeMinus1,col);
        }
        denominator   = (1.0 + handlingSum);
        predationSum += (numerator / denominator);
    }
    return EstimatedBiomassTimeMinus1*predationSum;
}

double
nmfPredationForm::TypeIIIPredation(int &timeMinus1,
                                   int &SpeciesNum,
                                   boost::numeric::ublas::matrix<double> &EstPredation,
                                   boost::numeric::ublas::matrix<double> &EstHandling,
                                   std::vector<double> &EstExponent,
                                   boost::numeric::ublas::matrix<double> &EstimatedBiomass,
                                   double &EstimatedBiomassTimeMinus1)
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
