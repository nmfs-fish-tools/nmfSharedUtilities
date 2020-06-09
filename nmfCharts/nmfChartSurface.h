#ifndef NMFCHARTSURFACE_H
#define NMFCHARTSURFACE_H

#include <Q3DSurface>
#include <QtDataVisualization>
#include <QString>
#include <QStringList>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <vector>

using namespace QtDataVisualization;

class nmfChartSurface : public QObject
{

    Q_OBJECT

    QSurfaceDataProxy* m_surfaceProxy;
    QSurface3DSeries*  m_surfaceSeries;
    std::pair<int,int> m_minCoord;
    boost::numeric::ublas::matrix<double> m_heightValues;
    void m_swapYandZPointsInLabel(QPoint pt);

public:

    nmfChartSurface(
            Q3DSurface* graph3D,
            const QString& xTitle,
            const QString& yTitle,
            const QString& zTitle,
            const QString& xLabelFormat,
            const QString& zLabelFormat,
            const boost::numeric::ublas::matrix<double> &rowValues,
            const boost::numeric::ublas::matrix<double> &columnValues,
            const boost::numeric::ublas::matrix<double> &zValues,
            const bool& showShadow);

    virtual ~nmfChartSurface() {}

    /**
     * @brief Selects the center point on the 3d surface with Qt's blue marker ball
     */
    void selectCenterPoint();
    /**
     * @brief Selects the minimum point on the 3d surface with Qt's blue marker ball
     */
    void selectMinimumPoint();

};


#endif
