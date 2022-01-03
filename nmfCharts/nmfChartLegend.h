/**
 * @file nmfChartLegend.h
 * @brief Definition for the legend widget
 * @date Feb 16, 2018
 *
 * This file contains the definition for a custom legend widget. It was necessary
 * for the proper operation of hover help for the legend markers.
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

#ifndef NMFCHARTLEGEND_H
#define NMFCHARTLEGEND_H

#include <QChart>
#include <QLabel>
#include <QLegend>
#include <QLegendMarker>
#include <QLine>
#include <QLineSeries>
#include <QScatterSeries>
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
 * @brief The nmfChartLegend class for a custom legend
 */
class nmfChartLegend : public QObject
{

    Q_OBJECT

private:
    int                       m_maxNumMarkers;
    QChart*                   m_chart;
    std::map<QString,QString> m_tooltips;
    std::vector<QString>      m_markerColors;

public:
    /**
     * @brief nmfChartLegend constructor
     * @param chart : pointer to QChart class
     */
    nmfChartLegend(QChart *chart);
    virtual ~nmfChartLegend() {}

    /**
     * @brief sets the tool tip messages
     * @param markerColors : messages for legend tooltips
     */
    void setToolTips(std::vector<QString>& markerColors);
    /**
     * @brief Sets up the class signal/slot connections
     */
    void setupConnections();

public slots:
    /**
     * @brief Callback invoked when the user hovers over a marker
     * @param hovered : true if hovered over, false if not
     */
    void callback_hoveredLegend(bool hovered);
};

#endif // NMFCHARTLEGEND_H
