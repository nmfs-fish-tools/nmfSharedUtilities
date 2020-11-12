#include "nmfChartMovableLine.h"
#include <QtCore/QtMath>
#include <QtCore/QDebug>

QT_CHARTS_USE_NAMESPACE

nmfChartMovableLine::nmfChartMovableLine(
        QWidget*     parent,
        std::string& mainTitle,
        std::string& xTitle,
        std::string& yTitle)
{
    m_Chart     = new QChart();
    m_ChartView = new QChartView(m_Chart);

    parent->layout()->addWidget(m_ChartView);

    m_Chart->legend()->hide();
    m_MainTitle      = mainTitle;
    m_XTitle         = xTitle;
    m_YTitle         = yTitle;
    m_MinY           = MIN_Y;
    m_MaxY           = MAX_Y;
    m_RoundingFactor = TENTHS;
}

nmfChartMovableLine::~nmfChartMovableLine()
{

}

void
nmfChartMovableLine::addPoint(QPointF currPoint)
{
    QList<QPointF> points = m_Scatter->points();
    int numPoints = points.size();
    bool pointExists=false;

    // Check if point is already in the list, if so, just update it.
    for (QPointF point : points) {
       if (int(currPoint.x()) == int(point.x())) {
           point.setY(currPoint.y());
           pointExists = true;
           break;
       }
    }
    if (! pointExists) {
        points.insert(numPoints-1, currPoint);
    }

    m_Scatter->clear();
    m_Line->clear();
    m_SelectedScatter->clear();

    m_Scatter->append(points);
    m_Line->append(points);
}

void
nmfChartMovableLine::calculateYearlyPoints()
{
    double m;
    double b;
    std::vector<QPointF> points;
    QPointF lastPoint  = m_Scatter->at(m_Scatter->count()-1);

    int numPoints = m_Scatter->count();
    for (int i=0; i<numPoints; ++i) {
        points.push_back(m_Scatter->at(i));
    }

    m_yearlyPoints.clear();
    for (int i = 1; i < numPoints; ++i) {
        m = (points[i-1].y() - points[i].y()) / (points[i-1].x() - points[i].x());
        b = points[i-1].y() - m * (points[i-1].x() - m_MinX);

        for(int j = int(points[i-1].x()) - m_MinX; j < int(points[i].x()) - m_MinX; ++j) {
            m_yearlyPoints.push_back(QPointF(j, m*j+b));
        }
    }
    m_yearlyPoints.push_back(lastPoint);
}

void
nmfChartMovableLine::checkChartBoundaries(QPointF *point)
{
    if (point->y() > m_MaxY) point->setY(m_MaxY);
    if (point->y() < m_MinY) point->setY(m_MinY);
}

int
nmfChartMovableLine::getMaxYScaleFactor()
{
    return m_MaxY;
}


int
nmfChartMovableLine::getNumPoints()
{
    return m_Scatter->count();
}

QList<QPointF>
nmfChartMovableLine::getPoints()
{
    return m_Scatter->points();
}

void
nmfChartMovableLine::getRange(int& xMin, int& xMax)
{
    xMin = m_MinX;
    xMax = m_MaxX;
}

double
nmfChartMovableLine::getYValue(const int& xValue)
{
    if (xValue < m_yearlyPoints.size()) {
        return m_yearlyPoints[xValue].y();
    } else {
        return -1.0;
    }
}

void
nmfChartMovableLine::hide()
{
    m_ChartView->hide();
}

void
nmfChartMovableLine::populateChart(
        int& startYear,
        int& endYear)
{
    m_MinX = startYear,
    m_MaxX = endYear;

    m_PointPressed = false;
    m_yearlyPoints.clear();

    m_Scatter = new QScatterSeries();
    m_Scatter->setName("Data Plot");
//  m_Scatter->setColor(Qt::green);

    m_SelectedScatter = new QScatterSeries();
    m_SelectedScatter->setName("Selected Points");
    m_SelectedScatter->setPointLabelsVisible(true);

    m_Line = new QLineSeries();
    m_Line->setName("Slope Line");

    for (int i : {startYear,endYear})
    {
        *m_Scatter << QPoint(i, 1);
        *m_Line    << QPoint(i, 1);
    }

    QValueAxis* chartVAxis = new QValueAxis();
    chartVAxis->setMin(m_MinY);
    chartVAxis->setMax(m_MaxY);
    chartVAxis->setRange(m_MinY, m_MaxY);
    chartVAxis->setTickCount(5);

    QValueAxis* chartHAxis = new QValueAxis();
    QString xLabelFormat = "%i";
    chartHAxis->setMin(m_MinX);
    chartHAxis->setMax(m_MaxX);
    chartHAxis->setRange(m_MinX, m_MaxX);
    chartHAxis->setLabelFormat(xLabelFormat);
    chartHAxis->setTickCount(5);
    chartHAxis->setMinorTickCount(4);

    m_Chart->addSeries(m_Line);
    m_Chart->addSeries(m_Scatter);
    m_Chart->addSeries(m_SelectedScatter);
    m_Chart->addAxis(chartHAxis, Qt::AlignBottom);
    m_Chart->addAxis(chartVAxis, Qt::AlignLeft);
    QFont boldFont = m_Chart->titleFont();
    boldFont.setWeight(QFont::Bold);
    m_Chart->setTitleFont(boldFont);
    m_Chart->setTitle(QString::fromStdString(m_MainTitle));
    m_Chart->axisX()->setTitleText(QString::fromStdString(m_XTitle));
    m_Chart->axisY()->setTitleText(QString::fromStdString(m_YTitle));
    QMargins chartMargins(35, 10, 20, 10);
    m_Chart->setMargins(chartMargins);

    m_Line->attachAxis(chartHAxis);
    m_Line->attachAxis(chartVAxis);
    m_Scatter->attachAxis(chartHAxis);
    m_Scatter->attachAxis(chartVAxis);
    m_SelectedScatter->attachAxis(chartHAxis);
    m_SelectedScatter->attachAxis(chartVAxis);

    connect(m_Scatter,          &QScatterSeries::pressed,
            this,               &nmfChartMovableLine::callback_PointPressed);
    connect(m_SelectedScatter,  &QScatterSeries::pressed,
            this,               &nmfChartMovableLine::callback_SelectedPointsPressed);
    connect(m_Scatter,          &QScatterSeries::released,
            this,               &nmfChartMovableLine::callback_PointReleased);
    connect(m_SelectedScatter,  &QScatterSeries::released,
            this,               &nmfChartMovableLine::callback_SelectedPointsReleased);
    connect(m_Line,             &QLineSeries::pressed,
            this,               &nmfChartMovableLine::callback_LinePressed);
}

void
nmfChartMovableLine::removeAllPoints()
{
    m_Line->clear();
    m_Scatter->clear();
    m_SelectedScatter->clear();
    m_Chart->update();
}

void
nmfChartMovableLine::resetEndPoints()
{
    int m;
    QList<QPointF> points = m_Scatter->points();
    QList<QPointF> newPoints;
    int NumPoints = points.size();
    points[NumPoints-1].setX(m_MaxX);

    // Remove any points that may have been scaled past the max
    newPoints.insert(0,points[0]);
    m = 1;
    for (int i=1; i<NumPoints-1; ++i) {
        if ((points[i].x() > m_MinX) && (points[i].x() < m_MaxX)) {
            newPoints.insert(m++,points[i]);
        }
    }
    newPoints.insert(m,points[NumPoints-1]);

    // Clear old points and load modified ones
    m_Scatter->clear();
    m_Line->clear();
    m_SelectedScatter->clear();
    m_Scatter->append(newPoints);
    m_Line->append(newPoints);
}

void
nmfChartMovableLine::resetPoints()
{
    removeAllPoints();
    for (int i : {m_MinX,m_MaxX}) {
        *m_Scatter << QPoint(i, 1);
        *m_Line    << QPoint(i, 1);
    }
}

double
nmfChartMovableLine::roundTo(const double& place,
                             const double& value)
{
    return double(int (value * place + 5.0/place)) / place;
}

void
nmfChartMovableLine::setMaxYValue(int maxYValue)
{
    m_MaxY = maxYValue;
    m_Chart->axisY()->setMax(m_MaxY);

    switch (m_MaxY) {
      case 1:
        m_RoundingFactor = HUNDRETHS;
        break;
      case 2:
        m_RoundingFactor = FIFTIETHS;
        break;
      case 3:
        m_RoundingFactor = TWENTIETHS;
        break;
      case 4:
        m_RoundingFactor = TENTHS;
        break;
      default:
        m_RoundingFactor = TENTHS;
        break;
    }
}

void
nmfChartMovableLine::setPointYValue(
        const int&     pointNum,
        const QPointF& newPoint)
{
      m_Scatter->replace(pointNum,newPoint);
      m_Line->replace(pointNum,newPoint);
}

void
nmfChartMovableLine::setRange(const int& numYears)
{
    m_MaxX = m_MinX + numYears;
    m_Chart->axisX()->setRange(m_MinX,m_MaxX);
    resetEndPoints();
}

void
nmfChartMovableLine::show()
{
    m_ChartView->show();
}

void
nmfChartMovableLine::updateChart(
        const int& startYear,
        const int& endYear)
{
    m_MinX = startYear;
    m_MaxX = endYear;

    m_Chart->axisX()->setRange(m_MinX,m_MaxX);
}




void
nmfChartMovableLine::callback_KeyPressed(QKeyEvent *event)
{
//    std::cout << "key press event activated" << std::endl;

    QList<QPointF> selectedPoints = m_SelectedScatter->points();

    // Allow the delete key if there are selected points which means
    // that the user is holding down the mouse button
    if (event->key() == Qt::Key_Delete) {
        QList<QPointF> points = m_Scatter->points();
        for (int i = 0; i < selectedPoints.length(); i++) {
            if (points.at(0) == selectedPoints[i] ||
                points.at(points.length() - 1) == selectedPoints[i]) {
                continue;
            } else {
                m_Scatter->remove(selectedPoints[i]);
                m_Line->remove(selectedPoints[i]);
            }
        }
        m_SelectedScatter->clear();
        m_Chart->update();
    }

    else if (event->key() == Qt::Key_Escape) {
        m_SelectedScatter->clear();
        m_Chart->update();
    }

    else if (event->key() == Qt::Key_R) {
        // Allow "r" key if there are selected points which means
        // that the user is holding down the mouse button
        if (selectedPoints.length() > 0) {
            resetPoints();
        }
    }
}

void
nmfChartMovableLine::callback_LinePressed(const QPointF &point)
{
//    std::cout << "line pressed" << std::endl;

    QPointF currPoint = point;
    currPoint.setX(round(point.x()));
    QList<QPointF> points = m_Scatter->points();

    for (int i = 0; i < points.length(); i++) {
       if (points[i].x() == currPoint.x()) {
           break;
       }
       else if (points[i].x() > currPoint.x()) {
           currPoint.setY(roundTo(m_RoundingFactor,currPoint.y()));

           points.insert(i, currPoint);

           m_Scatter->clear();
           m_Line->clear();
           m_SelectedScatter->clear();

           m_Scatter->append(points);
           m_Line->append(points);
           *m_SelectedScatter << currPoint;

           m_CurrPoint = point;
           m_PointPressed = true;

           break;
       }
    }
}

void
nmfChartMovableLine::callback_MouseMoved(QMouseEvent *event)
{
    if (m_PointPressed) {
        QPointF newCoords;

        QPointF pt = event->localPos();
        QRectF plot = m_Chart->plotArea();
        QPointF plotTopLeftCoords = plot.topLeft();

        //newCoords.setX((pt.x() - plotTopLeftCoords.x()) / (plot.width() / maxX));
        newCoords.setX(m_CurrPoint.x());
        newCoords.setY((plotTopLeftCoords.y() - pt.y() + plot.height()) / (plot.height() / m_MaxY));

        newCoords.setY(roundTo(m_RoundingFactor,newCoords.y()));
        checkChartBoundaries(&newCoords);

        m_Scatter->replace(m_CurrPoint, newCoords);
        m_Line->replace(m_CurrPoint, newCoords);
        m_SelectedScatter->replace(m_CurrPoint, newCoords);
        m_CurrPoint = newCoords;
    }
}

void
nmfChartMovableLine::callback_MouseReleased(QMouseEvent *event)
{
    m_PointPressed = false;

    m_SelectedScatter->clear();
    m_Chart->update();
}

void
nmfChartMovableLine::callback_PointPressed(const QPointF &point)
{
//    std::cout << "point pressed: " << point.x() << ", " << point.y() << std::endl;

    m_CurrPoint    = point;
    m_PointPressed = true;

    m_SelectedScatter->clear();
   *m_SelectedScatter << point;
}

void
nmfChartMovableLine::callback_PointReleased(const QPointF &point)
{
//    std::cout << "point released" << std::endl;

    m_PointPressed = false;

    m_SelectedScatter->clear();
    m_Chart->update();
}

void
nmfChartMovableLine::callback_SelectedPointsPressed(const QPointF &point)
{
//    std::cout << "point pressed: " << point.x() << ", " << point.y() << std::endl;

    m_CurrPoint    = point;
    m_PointPressed = true;
}

void
nmfChartMovableLine::callback_SelectedPointsReleased(const QPointF &point)
{
//    std::cout << "point released" << std::endl;

    m_PointPressed = false;
}
