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
#include <QPainter>
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

#include <iostream>
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

    void resetAxes(
            QChart*            chart,
            const bool&        ShowLegend,
            const bool&        XAxisApplyNiceNumbers,
            const bool&        XAxisIsInteger,
            const int&         XStartVal,
            const int&         NumXValues,
            const double&      YMinVal,
            double&            YMaxVal,
            const bool&        LeaveGapsWhereNegative,
            std::string&       XTitle,
            std::string&       YTitle,
            const int&         FontSizeLabel,
            const int&         FontSizeNumber,
            const QString&     FontLabel,
            const int&         AxisLineWidth,
            const int&         AxisLineColor,
            const std::vector<bool>& GridLines,
            const double&      XInc);
public:
    /**
     * @brief nmfChartLine constructor for multi-line chart widget
     */
    nmfChartLine();
    virtual ~nmfChartLine() {delete m_CustomToolTip;}

    /**
     * @brief Overlays a vertical line onto the current chart data
     * @param Chart : pointer to QChart
     * @param LineStyle : style of line (i.e., DottedLine, DashedLine)
     * @param DataLineWidth : width of vertical line
     * @param ShowFirstPoint : boolean signifying if the first point should be shown
     * @param ShowLegend : legend visibility boolean
     * @param XOffset : start year used as an offset for the x-axis scale
     * @param XAxisApplyNiceNumbers: boolean signifiying if x axis numbers should be scaled "nicely"
     * @param XAxisIsInteger : boolean signifiying if x axis numbers are integers
     * @param XPos : x position of the vertical line
     * @param YMinVal : minimum y value for the line
     * @param YMaxVal : maximum y value for the line
     * @param LeaveGapsWhereNegative
     * @param NumXValues : number of values along x axis
     * @param HoverLabel : hover label for the overlay line
     * @param XTitle : title of x axis
     * @param YTitle : title of y axis
     * @param FontSizeLabel : the point size of the label titles
     * @param FontSizeNumber : the point size of the axes' scale values
     * @param FontLabel : the font of the label titles and axes values
     * @param AxisLineWidth : width of the x and y axes
     * @param AxisLineColor : value to be used for r,g,b values for axes line color
     * @param LineColor : color of the line
     * @param GridLines : gridline visibility booleans
     * @param XInc : increment of x-axis
     */
    void overlayVerticalLine(
            QChart* Chart,
            const std::string& LineStyle,
            const int&         DataLineWidth,
            const bool&        ShowFirstPoint,
            const bool&        ShowLegend,
            const double&      XOffset,
            const bool&        XAxisApplyNiceNumbers,
            const bool&        XAxisIsInteger,
            const double&      XPos,
            const double&      YMinVal,
            double&            YMaxVal,
            const bool&        LeaveGapsWhereNegative,
            int&               NumXValues,
            const QString&     HoverLabel,
            std::string&       XTitle,
            std::string&       YTitle,
            const int&         FontSizeLabel,
            const int&         FontSizeNumber,
            const QString&     FontLabel,
            const int&         AxisLineWidth,
            const int&         AxisLineColor,
            const std::string& LineColor,
            const std::vector<bool>& GridLines,
            const double&      XInc);
    /**
     * @brief populates (and draws) the chart with all of the data necessary
     * @param Chart : pointer to QChart
     * @param LineType : type of line chart
     * @param LineStyle : style of line (i.e., DottedLine, DashedLine)
     * @param DataLineWidth : width of dotted (MSY) line
     * @param ShowFirstPoint : boolean signifying if the first point should be shown
     * @param ShowLegend : boolean to specify the visibility of the legend
     * @param XOffset : the x offset that should be applied to the data series
     * @param XAxisApplyNiceNumbers: boolean signifying if the scale of the x-axis should be rounded to the nearest "nicest" numbers
     * @param XAxisIsInteger : boolean signifying if the values along the x-axis should be integers
     * @param YMinVal : minimum y value
     * @param YMaxVal : maximum y value
     * @param LeaveGapsWhereNegative
     * @param LineData : matrix containing the line data
     * @param RowLabels : labels along the x-axis
     * @param ColumnLabels : labels along the y-axis
     * @param HoverLabels : labels for line tooltips
     * @param MainTitle : the main title for the chart
     * @param XTitle : the x-axis label
     * @param YTitle : the y-axis label
     * @param FontSizeLabel : the point size of the label titles
     * @param FontSizeNumber : the point size of the axes' scale values
     * @param LabelFont : the font of the label titles and axes values
     * @param AxisLineWidth : width of the x and y axes
     * @param AxisLineColor : value to be used for r,g,b values for axes line color
     * @param GridLines : vector of booleans designating if the gridlines should be visible
     * @param Theme : color theme for the chart
     * @param LineColor : the line color designation
     * @param LineColorName : the name of the line color to be used as a tooltip
     * @param XInc : increment along the x-axis
     */
    void populateChart(
            QChart*            Chart,
            std::string&       LineType,
            const std::string& LineStyle,
            const int&         DataLineWidth,
            const bool&        ShowFirstPoint,
            const bool&        ShowLegend,
            const double&      XOffset,
            const bool&        XAxisApplyNiceNumbers,
            const bool&        XAxisIsInteger,
            const double&      YMinVal,
            double&            YMaxVal,
            const bool&        LeaveGapsWhereNegative,
            const boost::numeric::ublas::matrix<double> &LineData,
            const QStringList& RowLabels,
            const QStringList& ColumnLabels,
            const QStringList& HoverLabels,
            std::string&       MainTitle,
            std::string&       XTitle,
            std::string&       YTitle,
            const int&         FontSizeLabel,
            const int&         FontSizeNumeric,
            const QString&     LabelFont,
            const int&         AxisLineWidth,
            const int&         AxisLineColor,
            const std::vector<bool>& GridLines,
            const int&         Theme,
            const QColor&      LineColor,
            const std::string& LineColorName,
            const double&      XInc);
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
