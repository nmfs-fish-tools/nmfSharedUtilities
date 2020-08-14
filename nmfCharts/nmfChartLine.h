#ifndef NMFCHARTLINE_H
#define NMFCHARTLINE_H

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


class nmfChartLine : public QObject
{

    Q_OBJECT

private:
    std::map<QString,QString> m_tooltips;


public:
    nmfChartLine();
    virtual ~nmfChartLine() {}

    void populateChart(
            QChart*            chart,
            std::string&       type,
            const std::string& style,
            const bool&        ShowFirstPoint,
            const bool&        ShowLegend,
            const double&      XOffset,
            const bool&        XAxisIsInteger,
            const double&      YMinSliderVal,
            const boost::numeric::ublas::matrix<double> &YAxisData,
            const QStringList& RowLabels,
            const QStringList& ColumnLabels,
            std::string&       MainTitle,
            std::string&       XTitle,
            std::string&       YTitle,
            const std::vector<bool>& GridLines,
            const int&         Theme,
            const QColor&      LineColor,
            const std::string& LineColorName,
            const double&      XInc);
signals:

public slots:
    void callback_hoveredLine(const QPointF& point,bool hovered);

};

#endif // NMFCHARTLINE_H
