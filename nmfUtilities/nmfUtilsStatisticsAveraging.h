/**
 * @file nmfUtilsStatisticsAveraging.h
 * @brief Definition for the nmfUtilsStatisticsAveraging class
 * @date Mar 04, 2021
 *
 * This class describes the nmfUtilsStatisticsAveraging class for the MultiSpecies tools. It
 * allows the user to save find the timeseries averages of the passed timeseries.
 *
 */
#pragma once

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <fstream>

#include <QList>
#include <QString>

#include "nmfUtils.h"

#include <boost/numeric/ublas/matrix.hpp>

/**
 * @brief This class describes the nmfUtilsStatisticsAveraging class for the MultiSpecies tools. It
 * allows the user to save find the timeseries averages of the passed timeseries.
 */
class nmfUtilsStatisticsAveraging {

private:
    int           m_NumSpecies;
    int           m_NumParameters;
    int           m_NumRuns;
    int           m_NumYears;
    QString       m_Type;
    QList<double> m_WeightData;
    boost::numeric::ublas::matrix<double> m_BiomassData;
    boost::numeric::ublas::matrix<double> m_ParameterData;

    std::map<QString, void(nmfUtilsStatisticsAveraging::*)(
            boost::numeric::ublas::matrix<double>& averagedBiomassData,
            boost::numeric::ublas::matrix<double>& averagedParameterData)> m_FunctionMap;

    void calculateUnweighted(
            boost::numeric::ublas::matrix<double>& averagedBiomassData,
            boost::numeric::ublas::matrix<double>& averagedParameterData);
    void checkData(boost::numeric::ublas::matrix<double>& data);

public:
    nmfUtilsStatisticsAveraging();
   ~nmfUtilsStatisticsAveraging();

    void setAverageType(const QString& type,
                        const QList<double>& weightData);
    void calculate(
            boost::numeric::ublas::matrix<double>& averagedBiomass,
            boost::numeric::ublas::matrix<double>& averagedParameters);
    void loadBiomassData(
            const int& NumRuns,
            const int& NumSpecies,
            const int& NumYears,
            boost::numeric::ublas::matrix<double>& data);
    void loadParameterData(
            const int& NumRuns,
            const int& NumParameters,
            const int& NumYears,
            boost::numeric::ublas::matrix<double>& data);
};



