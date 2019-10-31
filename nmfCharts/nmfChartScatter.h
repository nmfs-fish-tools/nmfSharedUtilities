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
    nmfChartScatter();
    virtual ~nmfChartScatter() {}

    void populateChart(
            QChart  *chart,
            std::string &type,
            const bool &ShowFirstPoint,
            const boost::numeric::ublas::matrix<double> &ChartData,
            const QColor &Color,
            const QStringList &RowLabels,
            const QStringList &ColumnLabels,
            std::string &MainTitle,
            std::string &XTitle,
            std::string &YTitle,
            const std::vector<bool> &GridLines,
            const int Theme);
signals:

public slots:

};

#endif // NMFCHARTSCATTER_H
