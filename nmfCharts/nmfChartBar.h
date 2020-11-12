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

QT_CHARTS_USE_NAMESPACE

const int NumCategoriesForVerticalNotation = 10;

class nmfChartBar : public QObject
{

    Q_OBJECT

public:
    nmfChartBar();
    virtual ~nmfChartBar() {}

    void populateChart(
            QChart  *chart,
            const std::string &type,
            double& maxYVal,
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
    void callback_BarSetHovered(bool val1,int val2);
};

#endif // NMFCHARTBAR_H
