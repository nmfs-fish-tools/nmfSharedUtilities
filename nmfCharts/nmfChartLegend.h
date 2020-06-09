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


class nmfChartLegend : public QObject
{

    Q_OBJECT

private:
    int                       m_maxNumMarkers;
    QChart*                   m_chart;
    std::map<QString,QString> m_tooltips;
    std::vector<QString>      m_markerColors;

public:
    nmfChartLegend(QChart *chart);
    virtual ~nmfChartLegend() {}

    void setToolTips(std::vector<QString>& markerColors);
    void setConnections();

public slots:
    void callback_hoveredLegend(bool hovered);

};

#endif // NMFCHARTLEGEND_H
