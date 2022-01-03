/**
 * @file nmfChartDonut.h
 * @brief Definition for donut chart widget
 * @date Feb 16, 2018
 *
 * This file contains the definition for a donut chart widget
 *
 * @copyright
 * Public Domain Notice\n
 *
 * National Oceanic And Atmospheric Administration\n\n
 *
 * This software is a "United States Government Work" under the terms of the
 * United States Copyright Act.  It was written as part of the author's official
 * duties as a United States Government employee/contractor and thus cannot be copyrighted.
 * This software is freely available to the public for use. The National Oceanic
 * And Atmospheric Administration and the U.S. Government have not placed any
 * restriction on its use or reproduction.  Although all reasonable efforts have
 * been taken to ensure the accuracy and reliability of the software and data,
 * the National Oceanic And Atmospheric Administration and the U.S. Government
 * do not and cannot warrant the performance or results that may be obtained
 * by using this software or data. The National Oceanic And Atmospheric
 * Administration and the U.S. Government disclaim all warranties, express
 * or implied, including warranties of performance, merchantability or fitness
 * for any particular purpose.\n\n
 *
 * Please cite the author(s) in any work or product based on this material.
 */

#ifndef NMFCHARTDONUT_H
#define NMFCHARTDONUT_H


#include <QChart>
#include <QCursor>
#include <QLine>
#include <QLineSeries>
#include <QPieSeries>
#include <QPieSlice>
#include <QScatterSeries>
#include <QStackedBarSeries>
#include <QString>
#include <QStringList>
#include <QToolTip>
#include <QValueAxis>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <vector>

QT_CHARTS_USE_NAMESPACE

/**
 * @brief The nmfChartDonut class depicts a donut-type pie shaped widget
 */
class nmfChartDonut : public QObject
{

    Q_OBJECT

public:
    /**
     * @brief nmfChartDonut class constructor
     */
    nmfChartDonut();
    virtual ~nmfChartDonut() {}

    /**
     * @brief populates (and draws) the chart with all of the data necessary
     * @param chart : pointer to QChart
     * @param chartData : matrix data for chart
     * @param rowLabels : labels along the x-axis
     * @param columnLabels : labels along the y-axis
     * @param mainTitle : the main title for the chart
     * @param xTitle : the x-axis label
     * @param yTitle : the y-axis label
     * @param gridLines : vector of booleans designating if the gridlines should be visible
     * @param theme : color theme for the chart
     */
    void populateChart(
            QChart  *chart,
            const boost::numeric::ublas::matrix<double> &chartData,
            const QStringList &rowLabels,
            const QStringList &columnLabels,
            std::string &mainTitle,
            std::string &xTitle,
            std::string &yTitle,
            const std::vector<bool> &gridLines,
            const int theme);

public slots:
    /**
     * @brief Callback invoked when the user hovers over a pie slice
     * @param exploded : boolean signifying that the slice is exploded
     */
    void callback_explodeSlice(bool exploded);
};

#endif // NMFCHARTDONUT_H
