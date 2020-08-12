#include "nmfChartMovableLine.h"
#include <QtCore/QtMath>
#include <QtCore/QDebug>

QT_CHARTS_USE_NAMESPACE

nmfChartMovableLine::nmfChartMovableLine(
        std::string mainTitle,
        std::string xTitle,
        std::string yTitle)
{
    m_chart = new QChart();
    m_chart->legend()->hide();
    m_MainTitle = mainTitle;
    m_XTitle    = xTitle;
    m_YTitle    = yTitle;
}

nmfChartMovableLine::~nmfChartMovableLine()
{

}

void
nmfChartMovableLine::populateChart(
        QWidget* parent,
        QWidget* window,
        int&     startYear,
        int&     endYear)
{
    MovableLineEventFilter* movableLineEventFilter = new MovableLineEventFilter();

    m_MinX = startYear,
    m_MaxX = endYear;

    QVBoxLayout* vLayout = new QVBoxLayout();
    m_chartView          = new QChartView(m_chart);

    vLayout->addWidget(m_chartView);
    parent->setLayout(vLayout);

    m_pointPressed = false;
    m_yearlyPoints.clear();

    m_scatter = new QScatterSeries();
    m_scatter->setName("Data Plot");
    m_scatter->installEventFilter(movableLineEventFilter);

    m_selectedScatter = new QScatterSeries();
    m_selectedScatter->setName("Selected Points");
    m_selectedScatter->setPointLabelsVisible(true);

    m_line = new QLineSeries();
    m_line->setName("Slope Line");

    for (int i : {startYear,endYear})
    {
        *m_scatter << QPoint(i, 1);
        *m_line    << QPoint(i, 1);
    }

    QValueAxis* chartVAxis = new QValueAxis();
    chartVAxis->setMin(MinY);
    chartVAxis->setMax(MaxY);
    chartVAxis->setRange(MinY, MaxY);
    chartVAxis->setTickCount(5);

    QValueAxis* chartHAxis = new QValueAxis();
    QString xLabelFormat = "%i";
    chartHAxis->setMin(m_MinX);
    chartHAxis->setMax(m_MaxX);
    chartHAxis->setRange(m_MinX, m_MaxX);
    chartHAxis->setLabelFormat(xLabelFormat);
    chartHAxis->setTickCount(5);

    m_chart->addSeries(m_line);
    m_chart->addSeries(m_scatter);
    m_chart->addSeries(m_selectedScatter);
    m_chart->addAxis(chartHAxis, Qt::AlignBottom);
    m_chart->addAxis(chartVAxis, Qt::AlignLeft);
    QFont boldFont = m_chart->titleFont();
    boldFont.setWeight(QFont::Bold);
    m_chart->setTitleFont(boldFont);
    m_chart->setTitle(QString::fromStdString(m_MainTitle));
    m_chart->axisX()->setTitleText(QString::fromStdString(m_XTitle));
    m_chart->axisY()->setTitleText(QString::fromStdString(m_YTitle));

    m_line->attachAxis(chartHAxis);
    m_line->attachAxis(chartVAxis);
    m_scatter->attachAxis(chartHAxis);
    m_scatter->attachAxis(chartVAxis);
    m_selectedScatter->attachAxis(chartHAxis);
    m_selectedScatter->attachAxis(chartVAxis);

    connect(m_scatter,          &QScatterSeries::pressed,
            this,               &nmfChartMovableLine::callback_pointPressed);
    connect(m_selectedScatter,  &QScatterSeries::pressed,
            this,               &nmfChartMovableLine::callback_selectedPointsPressed);
    connect(m_scatter,          &QScatterSeries::released,
            this,               &nmfChartMovableLine::callback_pointReleased);
    connect(m_selectedScatter,  &QScatterSeries::released,
            this,               &nmfChartMovableLine::callback_selectedPointsReleased);
    connect(m_line,             &QLineSeries::pressed,
            this,               &nmfChartMovableLine::callback_linePressed);
}



void
nmfChartMovableLine::calculateYearlyPoints()
{
    std::vector<QPointF> points;
    QPointF firstPoint = m_scatter->at(0);
    QPointF lastPoint  = m_scatter->at(m_scatter->count()-1);

    int numPoints = m_scatter->count();
    for (int i=0; i<numPoints; ++i) {
        points.push_back(m_scatter->at(i));
    }


    // This is just for the 2 point line...need to improve it for an n-point line
    // y = mx + b

    double m;
    double b;

    // Elliot....tbd
    // This is just for the 2 point line...need to improve it for an n-point line.
    // Using y = mx + b to determine intermediate points.
    m_yearlyPoints.clear();
    for (int i = 1; i < numPoints; ++i)
    {
        m = (points[i-1].y() - points[i].y()) / (points[i-1].x() - points[i].x());
        b = points[i-1].y() - m * (points[i-1].x() - m_MinX);

        for(int j = int(points[i-1].x()) - m_MinX; j < int(points[i].x()) - m_MinX; ++j)
        {
            m_yearlyPoints.push_back(QPointF(j, m*j+b));
        }
    }
    m_yearlyPoints.push_back(lastPoint);

//    m_yearlyPoints.clear();
//    m = (lastPoint.y()-firstPoint.y()) / (lastPoint.x()-firstPoint.x());
//    b = firstPoint.y() - m * (firstPoint.x()-m_MinX);
//    for (int i=int(firstPoint.x())-m_MinX; i<=int(lastPoint.x())-m_MinX; ++i) {
//        m_yearlyPoints.push_back(QPointF(i,m*i+b));
//        std::cout << i << std::endl;
//    }
}

double
nmfChartMovableLine::getYValue(int xvalue)
{
    if (xvalue < m_yearlyPoints.size()) {
        return m_yearlyPoints[xvalue].y();
    } else {
        return -1.0;
    }
}

double
nmfChartMovableLine::roundToTenths(double value)
{
    return double(int (value * 10 + 0.5)) / 10;
}

//void
//nmfChartMovableLine::mouseMoveEvent(QMouseEvent *event)
//{
//    std::cout << "nmfChartMovableLine mouse move event activated" << std::endl;

//    if (m_pointPressed) {
//        QPointF newCoords;

//        QPointF pt = event->localPos();
//        QRectF plot = m_chart->plotArea();
//        QPointF plotTopLeftCoords = plot.topLeft();

//        //newCoords.setX((pt.x() - plotTopLeftCoords.x()) / (plot.width() / maxX));
//        newCoords.setX(m_currPoint.x());
//        newCoords.setY((plotTopLeftCoords.y() - pt.y() + plot.height()) / (plot.height() / MaxY));

//        newCoords.setY(roundToTenths(newCoords.y()));

//        m_scatter->replace(m_currPoint, newCoords);
//        m_line->replace(m_currPoint, newCoords);
//        m_selectedScatter->replace(m_currPoint, newCoords);
//        m_currPoint = newCoords;
//    }

//}

/*
void
nmfChartMovableLine::keyPressEvent(QKeyEvent *event)
{
    std::cout << "key press event activated" << std::endl;

    if (event->key() == Qt::Key_Delete)
    {
        QList<QPointF> selectedPoints = m_selectedScatter->points();
        QList<QPointF> points = m_scatter->points();

        for (int i = 0; i < selectedPoints.length(); i++)
        {
            if (points.at(0) == selectedPoints[i] || points.at(points.length() - 1) == selectedPoints[i])
            {
                continue;
            }
            else
            {
                m_scatter->remove(selectedPoints[i]);
                m_line->remove(selectedPoints[i]);
            }
        }

        m_selectedScatter->clear();
        m_chart->update();
    }

    if (event->key() == Qt::Key_Escape)
    {
        m_selectedScatter->clear();
        m_chart->update();
    }

    if (event->key() == Qt::Key_R)
    {
        m_line->clear();
        m_scatter->clear();
        m_selectedScatter->clear();
        m_chart->update();

        for (int i = m_MinX; i <= m_MaxX; i = i + 8)
        {
            *m_scatter << QPoint(i, 1);
            *m_line << QPoint(i, 1);
        }
    }
}
*/

void
nmfChartMovableLine::callback_mouseReleased(QMouseEvent *event)
{
   m_pointPressed = false;
}

void
nmfChartMovableLine::callback_mouseMoved(QMouseEvent *event)
{
    if (m_pointPressed) {
        QPointF newCoords;

        QPointF pt = event->localPos();
        QRectF plot = m_chart->plotArea();
        QPointF plotTopLeftCoords = plot.topLeft();

        //newCoords.setX((pt.x() - plotTopLeftCoords.x()) / (plot.width() / maxX));
        newCoords.setX(m_currPoint.x());
        newCoords.setY((plotTopLeftCoords.y() - pt.y() + plot.height()) / (plot.height() / MaxY));

        newCoords.setY(roundToTenths(newCoords.y()));

        m_scatter->replace(m_currPoint, newCoords);
        m_line->replace(m_currPoint, newCoords);
        m_selectedScatter->replace(m_currPoint, newCoords);
        m_currPoint = newCoords;
    }

}

void
nmfChartMovableLine::callback_keyPressed(QKeyEvent *event)
{
    std::cout << "key press event activated" << std::endl;

    if (event->key() == Qt::Key_Delete)
    {
        QList<QPointF> selectedPoints = m_selectedScatter->points();
        QList<QPointF> points = m_scatter->points();

        for (int i = 0; i < selectedPoints.length(); i++)
        {
            if (points.at(0) == selectedPoints[i] || points.at(points.length() - 1) == selectedPoints[i])
            {
                continue;
            }
            else
            {
                m_scatter->remove(selectedPoints[i]);
                m_line->remove(selectedPoints[i]);
            }
        }

        m_selectedScatter->clear();
        m_chart->update();
    }

    if (event->key() == Qt::Key_Escape)
    {
        m_selectedScatter->clear();
        m_chart->update();
    }

    if (event->key() == Qt::Key_R)
    {
        m_line->clear();
        m_scatter->clear();
        m_selectedScatter->clear();
        m_chart->update();

        for (int i : {m_MinX,m_MaxX})
        {
            *m_scatter << QPoint(i, 1);
            *m_line    << QPoint(i, 1);
        }
    }
}

void
nmfChartMovableLine::callback_pointPressed(const QPointF &point)
{
    std::cout << "point pressed: " << point.x() << ", " << point.y() << std::endl;

    m_currPoint = point;
    m_pointPressed = true;

    *m_selectedScatter << point;
}

void
nmfChartMovableLine::callback_pointReleased(const QPointF &point)
{
    std::cout << "point released" << std::endl;

    m_pointPressed = false;
}

void
nmfChartMovableLine::callback_selectedPointsPressed(const QPointF &point)
{
    std::cout << "point pressed: " << point.x() << ", " << point.y() << std::endl;

    m_currPoint = point;
    m_pointPressed = true;
}

void
nmfChartMovableLine::callback_selectedPointsReleased(const QPointF &point)
{
    std::cout << "point released" << std::endl;

    m_pointPressed = false;
}

void
nmfChartMovableLine::callback_linePressed(const QPointF &point)
{
    std::cout << "line pressed" << std::endl;

    QPointF currPoint = point;
    currPoint.setX(round(point.x()));
    QList<QPointF> points = m_scatter->points();

    for (int i = 0; i < points.length(); i++)
    {
       if (points[i].x() == currPoint.x())
       {
           break;
       }
       else if (points[i].x() > currPoint.x())
       {
           currPoint.setY(roundToTenths(currPoint.y()));

           points.insert(i, currPoint);

           m_scatter->clear();
           m_line->clear();

           m_scatter->append(points);
           m_line->append(points);
           break;
       }
    }
}

bool
MovableLineEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    std::cout << "................................event filter worked." << std::endl;

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        qDebug("Ate key press %d", keyEvent->key());
        return true;
    } else if (event->type() == QEvent::MouseButtonPress) {
        std::cout << "event filter mouse button press" << std::endl;
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        std::cout << "event filter mouse move" << std::endl;
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

MovableLineEventFilter::MovableLineEventFilter()
{

}

MovableLineEventFilter::~MovableLineEventFilter()
{

}
