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

public:
    nmfChartMovableLine();
    ~nmfChartMovableLine();

    void populateChart(QWidget *parent, QWidget* window);
    double roundToTenths(double value);

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

private:
    QChart         *m_chart;
    QChartView     *m_chartView;
    QPointF         m_currPoint;
    QScatterSeries *m_scatter;
    QScatterSeries *m_selectedScatter;
    QLineSeries    *m_line;
    bool m_pointPressed;
    const int MaxX = 1998;
    const int MaxY = 4;
    const int MinX = 1990;
    const int MinY = 0;
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


