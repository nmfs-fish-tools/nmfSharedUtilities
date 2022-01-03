/**
 * @file nmfChartBar.h
 * @brief Definition for bar chart widget
 * @date Feb 16, 2018
 *
 * This file contains the definition for a bar chart widget
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

#ifndef NMFCHARTBAR_H
#define NMFCHARTBAR_H

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QLine>
#include <QLineSeries>
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
#include <math.h>

#include "nmfUtilsQt.h"

QT_CHARTS_USE_NAMESPACE

const int NumCategoriesForVerticalNotation = 10;

/**
 * @brief This class draws a bar chart
 */
class nmfChartBar : public QObject
{

    Q_OBJECT

    bool m_areDataPercentages;

public:
    /**
     * @brief Class constructor
     */
    nmfChartBar();
    virtual ~nmfChartBar() {}

    /**
     * @brief populates a Qt bar chart with appropriate data and annotation
     * @param chart : pointer to Qt chart widget
     * @param type : type of bar chart desired
     * @param maxYVal : maximum y value used for scaling the y-axis
     * @param chartData : matrix representing the chart's data
     * @param rowLabels : labels for rows
     * @param columnLabels : labels for columns
     * @param mainTitle : main chart title
     * @param xTitle : title for x-axis
     * @param yTitle : title for y-axis
     * @param areDataPercentages : flag denoting if data represent percentages
     * @param gridLines : grid lines visibility boolean
     * @param theme : theme value for chart
     */
    void populateChart(
            QChart *chart,
            const std::string& type,
            const double& maxYVal,
            const boost::numeric::ublas::matrix<double>& chartData,
            const QStringList& rowLabels,
            const QStringList& columnLabels,
            const std::string& mainTitle,
            const std::string& xTitle,
            const std::string& yTitle,
            const bool& areDataPercentages,
            const std::vector<bool>& gridLines,
            const int& theme);

public slots:
    /**
     * @brief callback_BarSetHovered : callback invoked when user hovers over a barset
     * @param hovered : boolean describing if user is hovering over a barset
     * @param index : index of barset
     */
    void callback_BarSetHovered(bool hovered,
                                int  index);

};

#endif // NMFCHARTBAR_H
