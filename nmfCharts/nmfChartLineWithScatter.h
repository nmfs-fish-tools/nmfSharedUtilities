/**
 * @file nmfChartLineWithScatter.h
 * @brief Definition for line widget with scatter points
 * @date Feb 16, 2018
 *
 * This file contains the definition for a line widget with scatter points overlayed on top
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

#pragma once

#include <QChart>
#include <QLine>
#include <QLineSeries>
#include <QScatterSeries>
#include <QString>
#include <QStringList>
#include <QValueAxis>
#include <QLabel>
#include <QLegend>
#include <QLegendMarker>
#include <QToolTip>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <vector>
#include "nmfUtilsQt.h"

QT_CHARTS_USE_NAMESPACE

/**
 * @brief The class creates a line chart with an overlayed scatter diagram. The user can
 * specify multiple lines worth of data for display.
 */
class nmfChartLineWithScatter : public QObject
{

    Q_OBJECT

private:
    std::map<QString,QString> m_tooltips;
    nmfToolTip*               m_CustomToolTip;

public:
    /**
     * @brief nmfChartLineWithScatter constructor for multi-line chart with
     * overlayed scatter diagram
     */
    nmfChartLineWithScatter();
    virtual ~nmfChartLineWithScatter() {}

    /**
     * @brief populates (and draws) the chart with all of the data necessary
     * @param chart : pointer to QChart
     * @param type : type of chart (currently unused)
     * @param lineStyle : style of line(s)
     * @param isMohnsRho : boolean signifying if plot is of a retrospective analysis
     * @param showFirstPoint : boolean signifying if showing the first point should be shown
     * @param addScatter : boolean signifying if the scatter diagram should be visible
     * @param xOffset : the x offset that should be applied to the data series
     * @param xAxisIsInteger : boolean signifying if the values along the x-axis should be integers
     * @param yMin : minimum y value
     * @param yMax : maximum y value
     * @param lineData : matrix containing the line data
     * @param scatterData : matrix containing the scatter data
     * @param rowLabels : labels along the x-axis
     * @param columnLabels : labels along the y-axis
     * @param mainTitle : the main title for the chart
     * @param xTitle : the x-axis label
     * @param yTitle : the y-axis label
     * @param gridLines : vector of booleans designating if the gridlines should be visible
     * @param theme : color theme for the chart
     * @param dashedLineColor : color of any dashed lines
     * @param scatterColor : color of scatter points
     * @param lineColor : the line color designation
     * @param lineColorName : the name of the line color to be used as a tooltip
     * @param multiRunLineLabels : list of line labels if there are to be more than one
     * @param showLegend : boolean to specify the visibility of the legend
     */
    void populateChart(
            QChart* chart,
            std::string& type,
            const std::string& lineStyle,
            const bool& isMohnsRho,
            const bool& showFirstPoint,
            const bool& addScatter,
            const int& xOffset,
            const bool& xAxisIsInteger,
            const double& yMin,
            const double& yMax,
            const boost::numeric::ublas::matrix<double>& lineData,
            const boost::numeric::ublas::matrix<double>& scatterData,
            const QStringList& rowLabels,
            const QStringList& columnLabels,
            std::string& mainTitle,
            std::string& xTitle,
            std::string& yTitle,
            const std::vector<bool>& gridLines,
            const int& theme,
            const QColor& dashedLineColor,
            const QColor& scatterColor,
            const std::string& lineColor,
            const std::string& lineColorName,
            const QList<QString>& multiRunLineLabels,
            const bool& showLegend);

public slots:
    /**
     * @brief Callback invoked if the user hovers over a plot marker
     * @param hovered : boolean signifying if user is hovering over a marker
     */
    void callback_hoveredLegendMarker(bool hovered);
    /**
     * @brief Callback invoked if user hovers over a line
     * @param point : point at which hovering takes place
     * @param hovered : boolean signifying if user is hovering over a line
     */
    void callback_hoveredLine(const QPointF& point, bool hovered);
    /**
     * @brief Callback invoked if user hovers over a scatter point
     * @param point : point at which hovering takes place
     * @param hovered : boolean signifying if user is hovering over a scatter point
     */
    void callback_hoveredScatter(const QPointF& point, bool hovered);
    /**
     * @brief Callback invoked to hide tooltip after a single shot timeout
     */
    void callback_hideTooltip();
};

