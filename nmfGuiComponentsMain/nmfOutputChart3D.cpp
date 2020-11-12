

#include "nmfOutputChart3D.h"
//#include "nmfConstants.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QScreen>
#include <QtGui/QGuiApplication>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QComboBox>

#include <QtCore/QTimer>
#include <QtCore/QLocale>
#include <QtCore/qmath.h>

#include <math.h>

using namespace QtDataVisualization;

const int sampleCountX = 27; //50;
const int sampleCountZ = 50;
const int heightMapGridStepX = 6;
const int heightMapGridStepZ = 6;
const float sampleMin = 1982; //-8.0f;
const float sampleMax = 2008; //8.0f;

nmfOutputChart3D::nmfOutputChart3D(Q3DSurface *surface,
                                   nmfLogger *theLogger,
                                   std::string theProjectDir) :
        m_graph(surface)
{
    ProjectDir = theProjectDir;
    logger = theLogger;

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_ssvpaProxy  = new QSurfaceDataProxy();
    m_ssvpaSeries = new QSurface3DSeries(m_ssvpaProxy);

    m_scene  = m_graph->scene();
    m_camera = m_scene->activeCamera();
    m_xangle = 0;
    m_rotateToggle = 0;
    m_lockXSliders = false;
    m_diffXSliders = 0;

    // Start a qt timer to be used later for visualization purposes
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    timer->start(50);
}

nmfOutputChart3D::~nmfOutputChart3D() {
    delete m_graph;
}

Q3DSurface* nmfOutputChart3D::graph() {
    return m_graph;
}

void nmfOutputChart3D::toggleModeNone() {
    graph()->setSelectionMode(QAbstract3DGraph::SelectionNone);
}
void nmfOutputChart3D::toggleModeItem() {
    graph()->setSelectionMode(QAbstract3DGraph::SelectionItem);
}
void nmfOutputChart3D::toggleModeSliceRow() {
    graph()->setSelectionMode(
            QAbstract3DGraph::SelectionItemAndRow
                    | QAbstract3DGraph::SelectionSlice);
    //graph()->scene()->setSecondarySubViewport(QRect(-100,-100,700,900));
}
void nmfOutputChart3D::toggleModeSliceColumn() {
    graph()->setSelectionMode(
            QAbstract3DGraph::SelectionItemAndColumn
                    | QAbstract3DGraph::SelectionSlice);
}

void nmfOutputChart3D::updateTimer() {

    if (abs(m_xangle) > 360) {
        m_xangle = 0;
    }
    if (m_rotateToggle != 0) {
        m_xangle -= m_rotateToggle;
        m_camera->setXRotation(m_xangle);
    }

}

void nmfOutputChart3D::fillSSVPAProxy(const int &FirstCatchYear,
        const int &FirstCatchAge,
        boost::numeric::ublas::matrix<double> &dataMatrix) {
    float x, y, z;

    int nrows = dataMatrix.size1();
    int ncols = dataMatrix.size2();

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(nrows);
    for (int col = 0; col < ncols; col++) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(nrows);
        int index = 0;
        for (int row = 0; row < nrows; ++row) {
            x = FirstCatchYear + row;
            y = dataMatrix(row, col);
            z = FirstCatchAge + col;
            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }

    m_ssvpaProxy->resetArray(dataArray);

}

void nmfOutputChart3D::orbitLeftClicked() {
    resetRotatePresets();
    if (m_rotateToggle == 1) {
        orbitPauseClicked();
    } else {
        m_rotateToggle = 1;
        m_xangle = m_camera->xRotation();
    }
}
void nmfOutputChart3D::orbitPauseClicked() {
    resetRotatePresets();
    m_rotateToggle = 0;
    m_xangle = m_camera->xRotation();
}
void nmfOutputChart3D::orbitRightClicked() {
    resetRotatePresets();
    if (m_rotateToggle == -1) {
        orbitPauseClicked();
    } else {
        m_rotateToggle = -1;
        m_xangle = m_camera->xRotation();
    }
}

void nmfOutputChart3D::resetRotatePresets() {

    foreach (QWidget * widget, QApplication::allWidgets())
    {
        if (widget->objectName() == "hRotateCMB") {
            QComboBox *hcombobox = (QComboBox*) widget;
            hcombobox->setCurrentIndex(0);
        }
        /*    	if (widget->objectName() == "vRotateCMB") {
         QComboBox *vcombobox = (QComboBox*)widget;
         vcombobox->setCurrentIndex(0);
         }*/
    }
}

void nmfOutputChart3D::resetClicked() {
    lockXSliders(0);
    orbitPauseClicked();
    m_axisMinSliderX->setValue(0);
    m_axisMaxSliderX->setValue(99);
    m_axisMaxSliderYScale->setValue(99);
    m_axisMinSliderZ->setValue(0);
    m_axisMaxSliderZ->setValue(99);
    m_xangle = 0;
    m_camera->setXRotation(0);
    m_camera->setYRotation(0);
    //m_camera->setCameraPreset(Q3DCamera::CameraPresetFrontBelow);
    m_camera->setZoomLevel(100);
    m_scaleCB->setChecked(true);

    // Find widgets and reset them

    foreach(QWidget * widget, QApplication::allWidgets())
    {
        if (widget->objectName() == "modeItemRB") {
            QRadioButton *rbutton = (QRadioButton*) widget;
            rbutton->setChecked(true);
        } else if (widget->objectName() == "hRotateCMB") {
            QComboBox *hcombobox = (QComboBox*) widget;
            hcombobox->setCurrentIndex(0);
            m_xangle = 0;
        } else if (widget->objectName() == "vRotateCMB") {
            QComboBox *vcombobox = (QComboBox*) widget;
            vcombobox->setCurrentIndex(0);
        } else if (widget->objectName() == "themeCMB") {
            QComboBox *tcombobox = (QComboBox*) widget;
            tcombobox->setCurrentIndex(0);
        }
    }
    m_maxValueLE->setText(
            QLocale(QLocale::English).toString(qlonglong(m_maxMatrixValue)));


}

void nmfOutputChart3D::snapshotClicked() {
    bool ok;

    QScreen *screen = QGuiApplication::primaryScreen();
    //QApplication::beep();
    QPixmap pixmap = screen->grabWindow(QApplication::focusWindow()->winId());
    QString path = QDir(QString::fromStdString(ProjectDir)).filePath("outputImages");
    if (! QDir(path).exists()) {
        ok = QDir().mkdir(path);
        if (! ok) {
            nmfUtils::printError("snapshotClicked: Couldn't create directory: "+path.toStdString(), "");
        } else {
            logger->logMsg(0,"snapshotClicked: Created directory: "+ path.toStdString());
        }
    }
    QString filename = QDir(path).filePath("MSVPA_X2.jpg");
    pixmap.save(filename);
    QString msg = "\nSnapshot image saved to file:\n\n" + filename;
    QMessageBox::information(0,
                             tr("Image Saved"),
                             tr(msg.toLatin1()),
                             QMessageBox::Ok);

} // end snapshotClicked


void nmfOutputChart3D::hRotateClicked(QString item) {
    if (!item.contains("Rot")) {
        m_xangle = -item.toFloat();
        m_camera->setXRotation(m_xangle);
    }
}

void nmfOutputChart3D::vRotateClicked(QString item) {
    m_camera->setYRotation(item.toFloat());
}

void nmfOutputChart3D::lockXSliders(bool checked) {
    m_lockXSliders = checked;
    m_diffXSliders = m_axisMaxSliderX->value() - m_axisMinSliderX->value();

}

void nmfOutputChart3D::enableAxisBoxes(bool enableBoxes)
{
    Q3DTheme *myTheme;
    myTheme = m_graph->activeTheme();
    myTheme->setLabelBorderEnabled(enableBoxes);
}

void nmfOutputChart3D::enableSSVPAModel(bool enable, int xmin, int xmax, int ymin,
        long ymax, int zmin, int zmax, std::string xLabel, std::string yLabel,
        std::string zLabel, bool autoScale, bool updateRange) {   // RSK updateRange unused, remove it
    if (enable) {

        m_ssvpaSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        //m_ssvpaSeries->setFlatShadingEnabled(true);

        m_graph->axisX()->setLabelFormat("%d");
        m_graph->axisX()->setSegmentCount((xmax - xmin + 1) / 2);
        //m_graph->axisY()->setLabelFormat("%d");
        m_graph->axisZ()->setLabelFormat("Age %d");
        m_graph->axisX()->setRange(xmin, xmax);
        if (autoScale) {
            m_graph->axisY()->setRange(ymin, ymax);
        }
        m_graph->axisZ()->setRange(zmin, zmax);


        m_graph->axisX()->setLabelAutoRotation(30);
        m_graph->axisY()->setLabelAutoRotation(90);
        m_graph->axisZ()->setLabelAutoRotation(30);

        m_graph->axisX()->setTitleVisible(true);
        m_graph->axisY()->setTitleVisible(true);
        m_graph->axisZ()->setTitleVisible(true);
        m_graph->axisX()->setTitle(xLabel.c_str());
        m_graph->axisY()->setTitle(yLabel.c_str());
        m_graph->axisZ()->setTitle(zLabel.c_str());

        //m_graph->axisX()->setReversed(true); // X is Year axis
        m_graph->axisZ()->setReversed(true); // Z is Age axis

        m_graph->addSeries(m_ssvpaSeries);
        m_xmin = xmin;
        m_xmax = xmax;
        m_xcount = m_xmax - m_xmin + 1;
        m_zmin = zmin;
        m_zmax = zmax;
        m_zcount = m_zmax - m_zmin + 1;

        // Turn off boxes around labels (it's done in the theme).
        enableAxisBoxes(false);
    }
}

void nmfOutputChart3D::themeChanged(int newTheme)
{
    // Set new theme
    m_graph->activeTheme()->setType(Q3DTheme::Theme(newTheme));
    enableAxisBoxes(false);
}

void nmfOutputChart3D::enableHeightMapModel(bool enable) {
    if (enable) {
        m_heightMapSeries->setDrawMode(QSurface3DSeries::DrawSurface);
        //m_heightMapSeries->setFlatShadingEnabled(false);

        m_graph->axisX()->setLabelFormat("%.1f N");
        m_graph->axisZ()->setLabelFormat("%.1f E");
        m_graph->axisX()->setRange(34.0f, 40.0f);
        m_graph->axisY()->setAutoAdjustRange(true);
        m_graph->axisZ()->setRange(18.0f, 24.0f);

        m_graph->axisX()->setTitle(QStringLiteral("Latitude"));
        m_graph->axisY()->setTitle(QStringLiteral("Height"));
        m_graph->axisZ()->setTitle(QStringLiteral("Longitude"));

        //m_graph->removeSeries(m_sqrtSinSeries);
        //m_graph->addSeries(m_heightMapSeries);

        // Reset range sliders for height map

        /*        int mapGridCountX = m_heightMapWidth / heightMapGridStepX;
         int mapGridCountZ = m_heightMapHeight / heightMapGridStepZ;
         m_rangeMinX = 34.0f;
         m_rangeMinZ = 18.0f;
         m_stepX = 6.0f / float(mapGridCountX - 1);
         m_stepZ = 6.0f / float(mapGridCountZ - 1);
         m_axisMinSliderX->setMaximum(mapGridCountX - 2);
         m_axisMinSliderX->setValue(0);
         m_axisMaxSliderX->setMaximum(mapGridCountX - 1);
         m_axisMaxSliderX->setValue(mapGridCountX - 1);
         m_axisMinSliderZ->setMaximum(mapGridCountZ - 2);
         m_axisMinSliderZ->setValue(0);
         m_axisMaxSliderZ->setMaximum(mapGridCountZ - 1);
         m_axisMaxSliderZ->setValue(mapGridCountZ - 1);*/
    }
}

void nmfOutputChart3D::scaleEdited() {
    qlonglong valueEntered = maxValueLE()->text().toLongLong();
    maxValueLE()->setText(QLocale(QLocale::English).toString(valueEntered));
    int sliderVal = (valueEntered / maxValue()) * 99;
    //maxSliderYScale()->blockSignals(true);
    maxSliderYScale()->setValue(sliderVal);
    //maxSliderYScale()->blockSignals(false);
}

QLineEdit* nmfOutputChart3D::maxValueLE() {
    return m_maxValueLE;
}

double nmfOutputChart3D::maxValue() {
    return m_maxMatrixValue;
}

void nmfOutputChart3D::setMaxValue(double maxValue) {
    m_maxMatrixValue = maxValue;
}

/*void nmfMatrixPlot::adjustYMin(int min) {
 float pct = min/100.0;
 m_graph->axisY()->setMin(pct*maxValue());
 }*/

void nmfOutputChart3D::adjustYMax(int max) {
    float pct = max / 100.0;
    long maxValueLong = pct * int(maxValue() / 100.0 + 0.5) * 100;

    if (m_graph->axisY()->title().contains("Abundance")) {
        m_graph->axisY()->setMax(maxValueLong);
        maxValueLE()->setText(
                QLocale(QLocale::English).toString(qlonglong(maxValueLong)));
    }
}

void nmfOutputChart3D::adjustXMin(int min) {

    float maxRangeValue = float(m_axisMinSliderX->maximum());
    float minX = m_xmin + int(m_xcount * (min / maxRangeValue));
    //float lockedMax=0.0;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = m_xmin + int(m_xcount * (max / maxRangeValue)) - 1;

    if (minX < maxX) {
        /*		if (m_lockXSliders) {
         lockedMax = min + m_diffXSliders;
         if (lockedMax <= nmfConstants::MaxSliderValue) {
         setAxisXRange(minX, minX + m_diffXSliders);
         m_axisMaxSliderX->setValue(min + m_diffXSliders);
         }
         } else {
         setAxisXRange(minX, maxX);
         }*/
        setAxisXRange(minX, maxX);
    }
}

void nmfOutputChart3D::adjustXMax(int max) {
    float maxRangeValue = float(m_axisMaxSliderX->maximum());
    float maxX = m_xmin + int(m_xcount * (max / maxRangeValue)) - 1;
    //float lockedMin=0.0;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }

    float minX = m_xmin + int(m_xcount * (min / maxRangeValue));

    if (minX < maxX) {
        /*		if (m_lockXSliders) {
         lockedMin = max-m_diffXSliders;
         if (lockedMin >= MinSliderValue) {
         setAxisXRange(maxX - m_diffXSliders, maxX);
         m_axisMinSliderX->setValue(max - m_diffXSliders);
         }
         } else {
         setAxisXRange(minX, maxX);
         }*/
        setAxisXRange(minX, maxX);
    }
}

void nmfOutputChart3D::adjustZMin(int min) {

    float maxRangeValue = float(m_axisMinSliderZ->maximum());
    float minZ = m_zmin + int(m_zcount * (min / maxRangeValue));

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_zmin + int(m_zcount * (max / maxRangeValue)) - 1;

    if (minZ < maxZ) {
        setAxisZRange(minZ, maxZ);
    }

}

void nmfOutputChart3D::adjustZMax(int max) {

    float maxRangeValue = float(m_axisMaxSliderZ->maximum());
    float maxZ = m_zmin + int(m_zcount * (max / maxRangeValue)) - 1;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }

    float minZ = m_zmin + int(m_zcount * (min / maxRangeValue));

    if (minZ < maxZ) {
        setAxisZRange(minZ, maxZ);
    }

}

void nmfOutputChart3D::setAxisXRange(float min, float max) {
    m_graph->axisX()->setRange(min, max);
}

void nmfOutputChart3D::setAxisZRange(float min, float max) {
    m_graph->axisZ()->setRange(min, max);
}

void nmfOutputChart3D::changeTheme(int theme) {
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void nmfOutputChart3D::setBlackToYellowGradient() {
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(
            Q3DTheme::ColorStyleRangeGradient);
}

void nmfOutputChart3D::setGreenToRedGradient() {
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(
            Q3DTheme::ColorStyleRangeGradient);
}

