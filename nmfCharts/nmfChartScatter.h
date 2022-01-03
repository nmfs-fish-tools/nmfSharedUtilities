/**
 * @file nmfChartScatter.h
 * @brief Definition for the Scatter Chart widget
 * @date Feb 16, 2018
 *
 * This file contains the definition for the Scatter Chart widget. It allows the user
 * to view data displayed as a 2d scatter diagram.
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

#ifndef NMFCHARTSCATTER_H
#define NMFCHARTSCATTER_H

#include <QChart>
#include <QLine>
#include <QLineSeries>
#include <QScatterSeries>
#include <QString>
#include <QStringList>
#include <QValueAxis>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <vector>

QT_CHARTS_USE_NAMESPACE


class nmfChartScatter : public QObject
{

    Q_OBJECT

public:
    /**
     * @brief nmfChartScatter constructor that defines a 2d scatter diagram
     */
    nmfChartScatter();
    virtual ~nmfChartScatter() {}

    /**
     * @brief populates (and draws) the chart with all of the data necessary
     * @param chart : pointer to QChart
     * @param type : type of chart (current option is Scatter)
     * @param showFirstPoint : boolean to show the first point in the series
     * @param chartData : matrix representing all of the chart's data
     * @param color : color of the scatter points
     * @param rowLabels : labels along the x-axis
     * @param columnLabels : labels along the y-axis
     * @param mainTitle : the main title for the chart
     * @param xTitle : the x-axis label
     * @param yTitle : the y-axis label
     * @param gridLines : vector of booleans designating if the gridlines should be visible
     * @param theme : color theme for the chart
     */
    void populateChart(
            QChart *chart,
            std::string &type,
            const bool &showFirstPoint,
            const boost::numeric::ublas::matrix<double> &chartData,
            const QColor &color,
            const QStringList &rowLabels,
            const QStringList &columnLabels,
            std::string &mainTitle,
            std::string &xTitle,
            std::string &yTitle,
            const std::vector<bool> &gridLines,
            const int theme);
};

#endif
