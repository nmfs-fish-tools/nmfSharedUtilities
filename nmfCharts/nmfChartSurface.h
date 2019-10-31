#ifndef NMFCHARTSURFACE_H
#define NMFCHARTSURFACE_H

#include <QtDataVisualization>

//#include "nmfOutputChart3D.h"
#include "nmfProgressWidget.h"
#include "nmfLogger.h"
#include "nmfUtils.h"
#include "nmfUtilsQt.h"

#include <Q3DSurface>
#include <QString>
#include <QStringList>

//#include <boost/numeric/ublas/matrix.hpp>
//#include <boost/numeric/ublas/io.hpp>
//#include <boost/algorithm/string/trim.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/range/algorithm_ext/erase.hpp>

#include <vector>
#include <QtDataVisualization>

QT_CHARTS_USE_NAMESPACE

using namespace QtDataVisualization;

class nmfChartSurface : public QObject
{

    Q_OBJECT

public:
    nmfChartSurface(
            Q3DSurface* graph3D,
            const int&  FirstRow,
            const int&  FirstColumn,
            const QString& XTitle,
            const QString& YTitle,
            const QString& ZTitle,
            const boost::numeric::ublas::matrix<double> &Data);
    virtual ~nmfChartSurface() {}

    void showTest();

//    void populateChart(
//            QChart  *chart,
//            std::string &type,
//            const boost::numeric::ublas::matrix<double> &ChartData,
//            const QStringList &RowLabels,
//            const QStringList &ColumnLabels,
//            std::string &MainTitle,
//            std::string &XTitle,
//            std::string &YTitle,
//            const std::vector<bool> &GridLines,
//            const int Theme);
signals:

public slots:

};

#endif
