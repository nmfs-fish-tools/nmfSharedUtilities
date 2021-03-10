
#include "nmfConstants.h"
#include "nmfUtilsStatisticsAveraging.h"


nmfUtilsStatisticsAveraging::nmfUtilsStatisticsAveraging()
{
    m_Type.clear();
    m_WeightData.clear();
    m_BiomassData.clear();
    m_ParameterData.clear();
    m_NumSpecies    = 0;
    m_NumParameters = 0;
    m_NumRuns       = 0;
    m_NumYears      = 0;

    // Set up function map
    m_FunctionMap["Unweighted"] = &nmfUtilsStatisticsAveraging::calculateUnweighted;

}

nmfUtilsStatisticsAveraging::~nmfUtilsStatisticsAveraging()
{
}

void
nmfUtilsStatisticsAveraging::calculateUnweighted(
        boost::numeric::ublas::matrix<double>& averagedBiomassData,
        boost::numeric::ublas::matrix<double>& averagedParameterData)
{
    double sum;
    std::vector<boost::numeric::ublas::matrix<double> > averagedData = {averagedBiomassData,averagedParameterData};
    std::vector<boost::numeric::ublas::matrix<double> > inputData    = {m_BiomassData,m_ParameterData};
    std::vector<int> numItems = {m_NumYears,m_NumParameters};

    for (int group=0; group<int(numItems.size()); ++group) {
        for (int i=0; i<m_NumSpecies; ++i) {
            for (int j=0; j<numItems[group]; ++j) {
                sum = 0;
                for (int k=0; k<m_NumRuns; ++k) {
                    sum += inputData[group](i*m_NumRuns+k,j);
                }
                averagedData[group](j,i) = sum/m_NumRuns;
            }
        }
    }
    averagedBiomassData   = averagedData[0];
    averagedParameterData = averagedData[1];

    // checkData(averagedBiomassData);
}

void
nmfUtilsStatisticsAveraging::calculate(
        boost::numeric::ublas::matrix<double>& averagedBiomass,
        boost::numeric::ublas::matrix<double>& averagedParameters)
{
    nmfUtils::initialize(averagedBiomass,   m_NumYears,     m_NumSpecies);
    nmfUtils::initialize(averagedParameters,m_NumParameters,m_NumSpecies);

    // Using a function map instead of if...else.. statements
    (this->*m_FunctionMap[m_Type])(averagedBiomass,averagedParameters);
}

void
nmfUtilsStatisticsAveraging::loadBiomassData(
        const int& NumRuns,
        const int& NumSpecies,
        const int& NumYears,
        boost::numeric::ublas::matrix<double>& data)
{
    m_NumRuns     = NumRuns;
    m_NumSpecies  = NumSpecies;
    m_NumYears    = NumYears;
    m_BiomassData = data;
}

void
nmfUtilsStatisticsAveraging::loadParameterData(
        const int& NumRuns,
        const int& NumSpecies,
        const int& NumParameters,
        boost::numeric::ublas::matrix<double>& data)
{
    m_NumRuns       = NumRuns;
    m_NumSpecies    = NumSpecies;
    m_NumParameters = NumParameters;
    m_ParameterData = data;
}

void
nmfUtilsStatisticsAveraging::setAverageType(
        const QString& type,
        const QList<double>& weightData)
{
    m_Type       = type;
    m_WeightData = weightData;
}

void
nmfUtilsStatisticsAveraging::checkData(boost::numeric::ublas::matrix<double>& data)
{
    std::cout << "Data Check:" << std::endl;
    for (int i=0; i<int(data.size1()); ++i) {
        for (int j=0; j<int(data.size2()); ++j) {
            std::cout << data(i,j) << " ";
        }
        std::cout << std::endl;
    }
}
