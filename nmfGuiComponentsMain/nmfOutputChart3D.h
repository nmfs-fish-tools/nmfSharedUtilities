
/**
 @file nmfMatrixPlot.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This contains the class definition for the main Qt window.  If this file changes, moc must be re-run.
 @date Dec 8, 2016
 */

#ifndef NMFMATRIXPLOT_H
#define NMFMATRIXPLOT_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>

#include <QCheckBox>
#include <QDir>
#include <QLineEdit>
#include <QMessageBox>
#include <QSlider>

#include <boost/numeric/ublas/matrix.hpp>

#include "nmfUtils.h"
#include "nmfLogger.h"


using namespace QtDataVisualization;

class nmfOutputChart3D : public QObject
{
    Q_OBJECT

private:

    nmfLogger *logger;

public:

    std::string ProjectDir;

    explicit nmfOutputChart3D(Q3DSurface *surface,
                              nmfLogger  *logger,
                              std::string theProjectDir);
    ~nmfOutputChart3D();

    void enableHeightMapModel(bool enable);
    void enableSSVPAModel(bool enable, int xmin, int xmax, int ymin,
            long ymax, int zmin, int zmax, std::string xLabel,
            std::string yLabel, std::string zLabel, bool autoScale, bool updateRange);

    void toggleModeNone();
    void toggleModeItem();
    void toggleModeSliceRow();
    void toggleModeSliceColumn();

    void setBlackToYellowGradient();
    void setGreenToRedGradient();

    void setAxisMinSliderX(QSlider *slider) { m_axisMinSliderX = slider; }
    void setAxisMaxSliderX(QSlider *slider) { m_axisMaxSliderX = slider; }
    void setAxisMaxSliderYScale(QSlider *slider) { m_axisMaxSliderYScale = slider; }
    void setAxisMinSliderZ(QSlider *slider) { m_axisMinSliderZ = slider; }
    void setAxisMaxSliderZ(QSlider *slider) { m_axisMaxSliderZ = slider; }
    void setMaxValueLE(QLineEdit *lineedit) { m_maxValueLE = lineedit; }
    void setScaleCB(QCheckBox *scaleCB) { m_scaleCB = scaleCB; }

    QSlider* minSliderX() {return m_axisMinSliderX;}
    QSlider* maxSliderX() {return m_axisMaxSliderX;}
    QSlider* maxSliderYScale() {return m_axisMaxSliderYScale;}
    QSlider* minSliderZ() {return m_axisMinSliderZ;}
    QSlider* maxSliderZ() {return m_axisMaxSliderZ;}
    QCheckBox* scaleCB() {return m_scaleCB;}

    QLineEdit *maxValueLE();
    void orbitLeftClicked();
    void orbitPauseClicked();
    void orbitRightClicked();
    void snapshotClicked();
    void resetClicked();
    void resetRotatePresets();
    void setMaxValue(double maxValue);
    double maxValue();
    void enableAxisBoxes(bool enableBoxes);


    void adjustXMin(int min);
    void adjustXMax(int max);
    //void adjustYMin(int min);
    void adjustYMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);
    Q3DSurface* graph();

    void fillSSVPAProxy(
            const int &FirstCatchYear,
            const int &FirstCatchAge,
             boost::numeric::ublas::matrix<double> &dataMatrix);

public Q_SLOTS:
    void changeTheme(int theme);
    void updateTimer();
    void lockXSliders(bool checked);
    void hRotateClicked(QString item);
    void vRotateClicked(QString item);
    void scaleEdited();
    void themeChanged(int item);


private:
    Q3DSurface *m_graph;
    QHeightMapSurfaceDataProxy *m_heightMapProxy;
    QSurfaceDataProxy *m_ssvpaProxy;
    QSurface3DSeries *m_heightMapSeries;
    QSurface3DSeries *m_ssvpaSeries;
    Q3DScene  *m_scene;
    Q3DCamera *m_camera;

    QSlider   *m_axisMinSliderX;
    QSlider   *m_axisMaxSliderX;
    QSlider   *m_axisMaxSliderYScale;
    QSlider   *m_axisMinSliderZ;
    QSlider   *m_axisMaxSliderZ;
    QWidget   *m_chartTab;
    QLineEdit *m_maxValueLE;
    QCheckBox *m_scaleCB;

    bool  m_lockXSliders;
    int   m_xangle;
    int   m_rotateToggle;
    int   m_heightMapWidth;
    int   m_heightMapHeight;
    int   m_diffXSliders;
    float m_rangeMinX;
    float m_rangeMinZ;
    float m_stepX;
    float m_stepZ;
    float m_xmin;
    float m_xmax;
    float m_zmin;
    float m_zmax;
    float m_xcount;
    float m_zcount;
    double m_maxMatrixValue;

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);
};


#endif // NMFMATRIXPLOT_H
