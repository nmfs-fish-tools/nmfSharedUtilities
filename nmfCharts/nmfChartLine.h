#ifndef NMFCHARTLINE_H
#define NMFCHARTLINE_H


#include <QChart>
#include <QLine>
#include <QLineSeries>
#include <QScatterSeries>
#include <QString>
#include <QStringList>
#include <QValueAxis>
#include <QLabel>

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



public:
    nmfChartLine();
    virtual ~nmfChartLine() {}

    void populateChart(
            QChart       *chart,
            std::string  &type,
            const std::string &style,
            const bool   &showFirstPoint,
            const double& XOffset,
            const bool& XAxisIsInteger,
            const double& YMinSliderVal,
            const boost::numeric::ublas::matrix<double> &ChartData,
            const QStringList &RowLabels,
            const QStringList &ColumnLabels,
            std::string  &MainTitle,
            std::string  &XTitle,
            std::string  &YTitle,
            const std::vector<bool> &GridLines,
            const int    &Theme,
            const QColor &DashedLineColor,
            const QList<QColor>& LineColors,
            const double  XInc);
signals:

public slots:

};

#endif // NMFCHARTLINE_H
