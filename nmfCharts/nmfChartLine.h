/**
 * @file nmfChartLine.h
 * @brief Definition for line widget
 * @date Feb 16, 2018
 *
 * This file contains the definition for a line widget
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

#ifndef NMFCHARTLINE_H
#define NMFCHARTLINE_H

#include <QChart>
#include <QHBoxLayout>
#include <QLabel>
#include <QLegend>
#include <QLegendMarker>
#include <QLine>
#include <QLineSeries>
#include <QPushButton>
#include <QScatterSeries>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QToolTip>
#include <QValueAxis>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <vector>
#include "nmfUtilsQt.h"

QT_CHARTS_USE_NAMESPACE

/**
 * @brief The class creates a line chart. The user can
 * specify multiple lines worth of data for display.
 */
class nmfChartLine : public QObject
{

    Q_OBJECT

private:
    std::map<QString,QString> m_HoverLabels;
    std::map<QString,QString> m_Tooltips;
    nmfToolTip*               m_CustomToolTip;

public:
    /**
     * @brief nmfChartLine constructor for multi-line chart widget
     */
    nmfChartLine();
    virtual ~nmfChartLine() {delete m_CustomToolTip;}

    /**
     * @brief populates (and draws) the chart with all of the data necessary
     * @param chart : pointer to QChart
     * @param type : type of line chart
     * @param lineStyle : style of line (i.e., DottedLine, DashedLine)
     * @param showFirstPoint : boolean signifying if the first point should be shown
     * @param showLegend : boolean to specify the visibility of the legend
     * @param xOffset : the x offset that should be applied to the data series
     * @param xAxisIsInteger : boolean signifying if the values along the x-axis should be integers
     * @param yMinVal : minimum y value
     * @param yMaxVal : maximum y value
     * @param leaveGapsWhereNegative
     * @param lineData : matrix containing the line data
     * @param rowLabels : labels along the x-axis
     * @param columnLabels : labels along the y-axis
     * @param hoverLabels : labels for line tooltips
     * @param mainTitle : the main title for the chart
     * @param xTitle : the x-axis label
     * @param yTitle : the y-axis label
     * @param gridLines : vector of booleans designating if the gridlines should be visible
     * @param theme : color theme for the chart
     * @param lineColor : the line color designation
     * @param lineColorName : the name of the line color to be used as a tooltip
     * @param xInc : increment along the x-axis
     */
    void populateChart(
            QChart*            chart,
            std::string&       type,
            const std::string& lineStyle,
            const bool&        showFirstPoint,
            const bool&        showLegend,
            const double&      xOffset,
            const bool&        xAxisIsInteger,
            const double&      yMinVal,
            const double&      yMaxVal,
            const bool&        leaveGapsWhereNegative,
            const boost::numeric::ublas::matrix<double> &lineData,
            const QStringList& rowLabels,
            const QStringList& columnLabels,
            const QStringList& hoverLabels,
            std::string&       mainTitle,
            std::string&       xTitle,
            std::string&       yTitle,
            const std::vector<bool>& gridLines,
            const int&         theme,
            const QColor&      lineColor,
            const std::string& lineColorName,
            const double&      xInc);
    /**
     * @brief clears all series from the passed chart
     * @param chart : point to QChart object to clear
     */
    void clear(QChart* chart);

public slots:
    /**
     * @brief Callback invoked if user hovers over a line
     * @param point : point at which hovering takes place
     * @param hovered : boolean signifying if user is hovering over a line
     */
    void callback_hoveredLine(const QPointF& point,
                              bool hovered);
    /**
     * @brief Callback invoked to hide tooltip after a single shot timeout
     */
    void callback_hideTooltip();
};

#endif // NMFCHARTLINE_H
