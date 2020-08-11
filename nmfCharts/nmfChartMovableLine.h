#ifndef NMFCHARTMOVABLELINE_H
#define NMFCHARTMOVABLELINE_H

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <iostream>
#include <cmath>

QT_CHARTS_USE_NAMESPACE

class nmfChartMovableLine : public QObject
{
    Q_OBJECT

    std::vector<QPointF> m_yearlyPoints;

private:
    QChart*         m_chart;
    QChartView*     m_chartView;
    QPointF         m_currPoint;
    QScatterSeries* m_scatter;
    QScatterSeries* m_selectedScatter;
    QLineSeries*    m_line;
    bool            m_pointPressed;
    int             m_MaxX;
    int             m_MinX;
    std::string     m_MainTitle;
    std::string     m_XTitle;
    std::string     m_YTitle;
    const int       MaxY = 4;
    const int       MinY = 0;


public:
    nmfChartMovableLine(
            std::string mainTitle,
            std::string xTitle,
            std::string yTitle);
    ~nmfChartMovableLine();

    void populateChart(QWidget *parent,
                       QWidget* window,
                       int& startYear,
                       int& endYear);
    double roundToTenths(double value);
    void calculateYearlyPoints();
    double getYValue(int xvalue);

protected:
    /*
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    */

private Q_SLOTS:
    void callback_pointPressed(const QPointF &point);
    void callback_pointReleased(const QPointF &point);
    void callback_selectedPointsPressed(const QPointF &point);
    void callback_selectedPointsReleased(const QPointF &point);
    void callback_linePressed(const QPointF &point);
    void callback_keyPressed(QKeyEvent *event);
    void callback_mouseMoved(QMouseEvent *event);
    void callback_mouseReleased(QMouseEvent *event);
};

class MovableLineEventFilter : public QObject
{
    Q_OBJECT

public:
    MovableLineEventFilter();
    ~MovableLineEventFilter();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // NMFCHARTMOVABLELINE_H


