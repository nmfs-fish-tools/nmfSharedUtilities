#ifndef NMFCHARTDONUT_H
#define NMFCHARTDONUT_H


#include <QChart>
#include <QCursor>
#include <QLine>
#include <QLineSeries>
#include <QPieSeries>
#include <QPieSlice>
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

QT_CHARTS_USE_NAMESPACE

class nmfChartDonut : public QObject
{

    Q_OBJECT

public:
    nmfChartDonut();
    virtual ~nmfChartDonut() {}

    void populateChart(
            QChart  *chart,
            const boost::numeric::ublas::matrix<double> &ChartData,
            const QStringList &RowLabels,
            const QStringList &ColumnLabels,
            std::string &MainTitle,
            std::string &XTitle,
            std::string &YTitle,
            const std::vector<bool> &GridLines,
            const int Theme);
signals:

public slots:
    void callback_ExplodeSlice(bool exploded);
};

#endif // NMFCHARTDONUT_H
