
#include "nmfPredationForm.h"

nmfPredationForm::nmfPredationForm(std::string predationType)
{
    m_Type             = predationType;
    m_NumSpecies       = 0;
    m_NumParameters = 0;
    m_ParameterRanges.clear();
    m_isAggProd        = false;
    m_PredationMap.clear();
    m_PredationKey.clear();

    m_FunctionMap["Null"]     = &nmfPredationForm::FunctionMap_Null;
    m_FunctionMap["Type I"]   = &nmfPredationForm::FunctionMap_TypeI;
    m_FunctionMap["Type II"]  = &nmfPredationForm::FunctionMap_TypeII;
    m_FunctionMap["Type III"] = &nmfPredationForm::FunctionMap_TypeIII;

    setupFormMaps();
}

void
nmfPredationForm::setupFormMaps()
{
    std::string index1    = (m_isAggProd) ? "I" : "i";
    std::string index2    = (m_isAggProd) ? "J" : "j";
    std::string index3    = (m_isAggProd) ? "K" : "k";

    std::string Bit       = "B<sub>"+index1+",t</sub>";
    std::string Bjt       = "B<sub>"+index2+",t</sub>";
    std::string Bkt       = "B<sub>"+index3+",t</sub>";
    std::string hkj       = "h<sub>"+index3+","+index2+"</sub>";
    std::string rhoij     = "&rho;<sub>"+index1+","+index2+"</sub>";
    std::string rhokj     = "&rho;<sub>"+index3+","+index2+"</sub>";
    std::string supSubi   = (m_isAggProd) ? "<sup>b&#x2081;</sup>" : "<sup>b&#x1d62;+1</sup>";
    std::string supSubk   = (m_isAggProd) ? "<sup>b&#x2082;</sup>" : "<sup>b&#x2096;+1</sup>";
    std::string predj     = (m_isAggProd) ? "Predator Guild J" : "Predator Species j";
    std::string preyk     = (m_isAggProd) ? "Prey Guild K" : "Prey Species k";
    std::string preyi     = (m_isAggProd) ? "Prey Guild I" : "Prey Species i";

    m_PredationMap["Null"]        = "";
    m_PredationMap["Type I"]      = " - " + Bit + "&sum;"  + rhoij + Bjt;
    m_PredationMap["Type II"]     = " - " + Bit + "&sum;[" + rhoij + Bjt + "/" +
                       "(1+&sum;" + hkj + rhokj + Bkt + ")]";
    m_PredationMap["Type III"]    = " - " + Bit + supSubi + " &sum;[" + rhoij + Bjt + "/\
            (1+&sum;" + hkj + rhokj + Bkt + supSubk +")]";

    m_PredationKey["Null"]        = "";
    m_PredationKey["Type I"]      = rhoij + " = Predation Coefficient for " + predj + " on " + preyi + "<br/>";
    m_PredationKey["Type II"]     = rhoij + " = Predation Coefficient for " + predj + " on " + preyi + "<br/>" + hkj + " = Handling time for " + predj + " with " + preyk + "<br/>";
    m_PredationKey["Type III"]    = rhoij + " = Predation Coefficient for " + predj + " on " + preyi + "<br/>" + hkj + " = Handling time for " + predj + " with " + preyk + "<br/>b = Predator Dependent Parameter<br/>";
}

std::string
nmfPredationForm::getExpression()
{
    return m_PredationMap[m_Type];
}

std::string
nmfPredationForm::getKey()
{
    return m_PredationKey[m_Type];
}

void
nmfPredationForm::setType(std::string newType)
{
    m_Type = newType;
}

void
nmfPredationForm::setAggProd(bool isAggProd)
{
    m_isAggProd = isAggProd;
}

int
nmfPredationForm::getNumParameters()
{
    return m_NumParameters; // m_parameterRanges.size();
}

void
nmfPredationForm::extractPredationParameters(
        const std::vector<double> &parameters,
        int& startPos,
        boost::numeric::ublas::matrix<double> &predation)
{
    if (m_Type != "Null") {
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
    if ((m_Type == "Type II") || (m_Type == "Type III"))
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
    if (m_Type == "Type III")
    {
        exponents.clear();
        for (int i=0; i<m_NumSpeciesOrGuilds; ++i) {
            exponents.push_back(parameters[startPos++]);
        }
    }
}

void
nmfPredationForm::loadParameterRanges(
        std::vector<double>& parameterInitialValues,
        std::vector<std::pair<double,double> >& parameterRanges,
        nmfStructsQt::ModelDataStruct& dataStruct)
{
    bool isCheckedRho      = nmfUtils::isEstimateParameterChecked(dataStruct,"PredationRho");
    bool isCheckedHandling = nmfUtils::isEstimateParameterChecked(dataStruct,"Handling");
    bool isCheckedExponent = nmfUtils::isEstimateParameterChecked(dataStruct,"PredationExponent");
    double initialValue;
    std::pair<double,double> aPair;

    if (m_Type == "Null")
        return;

    m_NumSpecies = dataStruct.NumSpecies;
    m_NumGuilds  = dataStruct.NumGuilds;
    m_isAggProd  = (dataStruct.CompetitionForm == "AGG-PROD");
    m_NumSpeciesOrGuilds = (m_isAggProd) ? m_NumGuilds : m_NumSpecies;

    for (unsigned i=0; i<dataStruct.PredationRhoMin.size(); ++i) {
        for (unsigned int j=0; j<dataStruct.PredationRhoMin[0].size(); ++j) {
            if (isCheckedRho) {
                aPair = std::make_pair(dataStruct.PredationRhoMin[i][j],
                                       dataStruct.PredationRhoMax[i][j]);
            } else {
                initialValue = dataStruct.PredationRho[i][j];
                aPair = std::make_pair(initialValue,initialValue);
            }
            parameterRanges.emplace_back(aPair);
            m_ParameterRanges.emplace_back(aPair);
            parameterInitialValues.emplace_back(dataStruct.PredationRho[i][j]);
        }
    }
    m_NumParameters = dataStruct.PredationRhoMin.size() *
                      dataStruct.PredationRhoMin[0].size();

    if ((m_Type == "Type II") || (m_Type == "Type III")) {
        for (unsigned i=0; i<dataStruct.PredationHandlingMin.size(); ++i) {
            for (unsigned int j=0; j<dataStruct.PredationHandlingMin[0].size(); ++j) {
                if (isCheckedHandling) {
                    aPair = std::make_pair(dataStruct.PredationHandlingMin[i][j],
                                           dataStruct.PredationHandlingMax[i][j]);
                } else {
                    initialValue = dataStruct.PredationHandling[i][j];
                    aPair = std::make_pair(initialValue,initialValue);
                }
                parameterRanges.emplace_back(aPair);
                m_ParameterRanges.emplace_back(aPair);
                parameterInitialValues.emplace_back(dataStruct.PredationHandling[i][j]);
            }
        }
        m_NumParameters += dataStruct.PredationHandlingMin.size() *
                           dataStruct.PredationHandlingMin[0].size();
    }

    if (m_Type == "Type III") {
        for (unsigned i=0; i<dataStruct.PredationExponentMin.size(); ++i) {
            if (isCheckedExponent) {
                aPair = std::make_pair(dataStruct.PredationExponentMin[i],
                                       dataStruct.PredationExponentMax[i]);
            } else {
                initialValue = dataStruct.PredationExponent[i];
                aPair = std::make_pair(initialValue,initialValue);
            }
            parameterRanges.emplace_back(aPair);
            m_ParameterRanges.emplace_back(aPair);
            parameterInitialValues.emplace_back(dataStruct.PredationExponent[i]);
        }
        m_NumParameters += dataStruct.PredationExponentMin.size();
    }
}

double
nmfPredationForm::evaluate(const std::string& covariateAlgorithmType,
                           const int& timeMinus1,
                           const int& species,
                           const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                           const double& EstimatedBiomassTimeMinus1,
                           const boost::numeric::ublas::matrix<double>& EstPredationRho,
                           const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
                           const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                           const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
                           const std::vector<double>& EstPredationExponent,
                           const boost::numeric::ublas::matrix<double>& PredationExponentCovariate)
{

    if (m_FunctionMap.find(m_Type) == m_FunctionMap.end()) {
        return 0;
    } else {
        return (this->*m_FunctionMap[m_Type])(
                    covariateAlgorithmType,
                    timeMinus1,species,
                    EstimatedBiomass,EstimatedBiomassTimeMinus1,
                    EstPredationRho,PredationRhoCovariate,
                    EstPredationHandling,PredationHandlingCovariate,
                    EstPredationExponent,PredationExponentCovariate);
    }
}

double
nmfPredationForm::FunctionMap_Null(const std::string& covariateAlgorithmType,
                                   const int& timeMinus1,
                                   const int& species,
                                   const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                                   const double& EstimatedBiomassTimeMinus1,
                                   const boost::numeric::ublas::matrix<double>& EstPredationRho,
                                   const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
                                   const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                                   const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
                                   const std::vector<double>& EstPredationExponent,
                                   const boost::numeric::ublas::matrix<double>& PredationExponentCovariate)
{
    return 0.0;
}

double
nmfPredationForm::FunctionMap_TypeI(const std::string& covariateAlgorithmType,
                                    const int& timeMinus1,
                                    const int& species,
                                    const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                                    const double& EstimatedBiomassTimeMinus1,
                                    const boost::numeric::ublas::matrix<double>& EstPredationRho,
                                    const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
                                    const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                                    const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
                                    const std::vector<double>& EstPredationExponent,
                                    const boost::numeric::ublas::matrix<double>& PredationExponentCovariate)
{
    //  B(i,t)∑ρ(i,j)B(j,t)
    double PredationSum = 0;
    double EstPredationRhoTerm;
    double PredationRhoCovariateCoeff = 0.0; // RSK estimate this later

    for (int row=0; row<int(EstPredationRho.size2()); ++row) {
//      EstPredationRhoTerm = EstPredationRho(row,species) * (1.0+PredationRhoCovariateCoeff*PredationRhoCovariate(timeMinus1,species));
        EstPredationRhoTerm = nmfUtils::applyCovariate(nullptr,
                    covariateAlgorithmType,EstPredationRho(row,species),
                    PredationRhoCovariateCoeff,PredationRhoCovariate(timeMinus1,species));
        PredationSum += EstPredationRhoTerm * EstimatedBiomass(timeMinus1,row);
    }

    return EstimatedBiomassTimeMinus1*PredationSum;
}

double
nmfPredationForm::FunctionMap_TypeII(const std::string& covariateAlgorithmType,
                                     const int& timeMinus1,
                                     const int& species,
                                     const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                                     const double& EstimatedBiomassTimeMinus1,
                                     const boost::numeric::ublas::matrix<double>& EstPredationRho,
                                     const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
                                     const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                                     const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
                                     const std::vector<double>& EstPredationExponent,
                                     const boost::numeric::ublas::matrix<double>& PredationExponentCovariate)
{
    // B(i,t)∑[ρ(i,j)B(j,t)/(1+∑h(k,j)ρ(k,j)B(k,t))]

    int    NumSpecies   = EstPredationRho.size2();
    double predationSum = 0;
    double handlingSum;
    double numerator;
    double denominator;
    double EstPredationRhoTerm;
    double EstPredationHandlingTerm;
    double PredationRhoCovariateCoeff      = 0.0; // RSK estimate this later
    double PredationHandlingCovariateCoeff = 0.0; // RSK estimate this later

    for (int row=0; row<NumSpecies; ++row) {
//      EstPredationRhoTerm = EstPredationRho(row,species) * (1.0+PredationRhoCovariateCoeff*PredationRhoCovariate(timeMinus1,species));
        EstPredationRhoTerm = nmfUtils::applyCovariate(nullptr,
                    covariateAlgorithmType,EstPredationRho(row,species),
                    PredationRhoCovariateCoeff,PredationRhoCovariate(timeMinus1,species));

        numerator   = EstPredationRhoTerm * EstimatedBiomass(timeMinus1,row);
        handlingSum = 0;
        for (int j=0; j<NumSpecies; ++j) {
//          EstPredationRhoTerm      = EstPredationRho(j,row)      * (1.0+PredationRhoCovariateCoeff     *PredationRhoCovariate(timeMinus1,row));
//          EstPredationHandlingTerm = EstPredationHandling(j,row) * (1.0+PredationHandlingCovariateCoeff*PredationHandlingCovariate(timeMinus1,row));
            EstPredationRhoTerm = nmfUtils::applyCovariate(nullptr,
                        covariateAlgorithmType,EstPredationRho(j,row),
                        PredationRhoCovariateCoeff,PredationRhoCovariate(timeMinus1,row));
            EstPredationHandlingTerm = nmfUtils::applyCovariate(nullptr,
                        covariateAlgorithmType,EstPredationHandling(j,row),
                        PredationHandlingCovariateCoeff,PredationHandlingCovariate(timeMinus1,row));
            handlingSum += EstPredationHandlingTerm *
                           EstPredationRhoTerm *
                           EstimatedBiomass(timeMinus1,row);
        }
        denominator   = (1.0 + handlingSum);
        predationSum += (numerator / denominator);
    }

    return EstimatedBiomassTimeMinus1*predationSum;
}

double
nmfPredationForm::FunctionMap_TypeIII(const std::string& covariateAlgorithmType,
                                      const int& timeMinus1,
                                      const int& species,
                                      const boost::numeric::ublas::matrix<double>& EstimatedBiomass,
                                      const double& EstimatedBiomassTimeMinus1,
                                      const boost::numeric::ublas::matrix<double>& EstPredationRho,
                                      const boost::numeric::ublas::matrix<double>& PredationRhoCovariate,
                                      const boost::numeric::ublas::matrix<double>& EstPredationHandling,
                                      const boost::numeric::ublas::matrix<double>& PredationHandlingCovariate,
                                      const std::vector<double>& EstPredationExponent,
                                      const boost::numeric::ublas::matrix<double>& PredationExponentCovariate)
{
    // B(i,t)^(bᵢ+1) ∑[ρ(i,j)B(j,t)/ (1+∑h(k,j)ρ(k,j)B(k,t)^(bₖ+1))]

    int    NumSpecies   = EstPredationRho.size1();
    double predationSum = 0;
    double handlingSum;
    double numerator;
    double denominator;
    double PredationRhoCovariateCoeff      = 0.0; // RSK estimate this later
    double PredationHandlingCovariateCoeff = 0.0; // RSK estimate this later
    double PredationExponentCovariateCoeff = 0.0; // RSK estimate this later
    double EstPredationRhoTerm;
    double EstPredationHandlingTerm;
    double EstPredationExponentTerm;

    for (int col=0; col<NumSpecies; ++col) {
//      EstPredationRhoTerm = EstPredationRho(species,col) * (1.0+PredationRhoCovariateCoeff*PredationRhoCovariate(timeMinus1,species));
        EstPredationRhoTerm = nmfUtils::applyCovariate(nullptr,
                    covariateAlgorithmType,EstPredationRho(species,col),
                    PredationRhoCovariateCoeff,PredationRhoCovariate(timeMinus1,species));

        numerator   = EstPredationRhoTerm * EstimatedBiomass(timeMinus1,col);
        handlingSum = 0;
        for (int j=0; j<NumSpecies; ++j) {
//          EstPredationRhoTerm      = EstPredationRho(j,col)      * (1.0+PredationRhoCovariateCoeff     *PredationRhoCovariate(timeMinus1,j));
//          EstPredationHandlingTerm = EstPredationHandling(j,col) * (1.0+PredationHandlingCovariateCoeff*PredationHandlingCovariate(timeMinus1,j));
//          EstPredationExponentTerm = EstPredationExponent[j]     * (1.0+PredationExponentCovariateCoeff*PredationExponentCovariate(timeMinus1,j));
            EstPredationRhoTerm      = nmfUtils::applyCovariate(nullptr,
                        covariateAlgorithmType,EstPredationRho(j,col),
                        PredationRhoCovariateCoeff,PredationRhoCovariate(timeMinus1,j));
            EstPredationHandlingTerm = nmfUtils::applyCovariate(nullptr,
                        covariateAlgorithmType,EstPredationHandling(j,col),
                        PredationHandlingCovariateCoeff,PredationHandlingCovariate(timeMinus1,j));
            EstPredationExponentTerm = nmfUtils::applyCovariate(nullptr,
                        covariateAlgorithmType,EstPredationExponent[j],
                        PredationExponentCovariateCoeff,PredationExponentCovariate(timeMinus1,j));

            handlingSum += EstPredationHandlingTerm *
                           EstPredationRhoTerm      *
                           std::pow(EstimatedBiomass(timeMinus1,col),EstPredationExponentTerm+1);
        }
        denominator   = (1.0 + handlingSum);
        predationSum += (numerator / denominator);
    }

//  EstPredationExponentTerm = EstPredationExponent[species]*(1.0+PredationExponentCovariateCoeff*PredationExponentCovariate(timeMinus1,species));
    EstPredationExponentTerm = nmfUtils::applyCovariate(nullptr,
                covariateAlgorithmType,EstPredationExponent[species],
                PredationExponentCovariateCoeff,PredationExponentCovariate(timeMinus1,species));

    return std::pow(EstimatedBiomassTimeMinus1,EstPredationExponentTerm+1)*predationSum;
}
