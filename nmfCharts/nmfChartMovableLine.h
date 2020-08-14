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
    /**
     * @brief nmfChartMovableLine
     * @param mainTitle
     * @param xTitle
     * @param yTitle
     */
    nmfChartMovableLine(
            std::string mainTitle,
            std::string xTitle,
            std::string yTitle);
    ~nmfChartMovableLine();

    /**
     * @brief populateChart
     * @param parent
     * @param window
     * @param startYear
     * @param endYear
     */
    void populateChart(QWidget *parent,
                       QWidget* window,
                       int& startYear,
                       int& endYear);
    /**
     * @brief roundToTenths
     * @param value
     * @return
     */
    double roundToTenths(double value);
    /**
     * @brief checkChartBoundaries
     * @param point
     */
    void checkChartBoundaries(QPointF *point);
    /**
     * @brief calculateYearlyPoints
     */
    void calculateYearlyPoints();
    /**
     * @brief getYValue
     * @param xvalue
     * @return
     */
    double getYValue(int xvalue);

protected:
    /*
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    */

private Q_SLOTS:
    /**
     * @brief callback_pointPressed
     * @param point
     */
    void callback_pointPressed(const QPointF &point);
    /**
     * @brief callback_pointReleased
     * @param point
     */
    void callback_pointReleased(const QPointF &point);
    /**
     * @brief callback_selectedPointsPressed
     * @param point
     */
    void callback_selectedPointsPressed(const QPointF &point);
    /**
     * @brief callback_selectedPointsReleased
     * @param point
     */
    void callback_selectedPointsReleased(const QPointF &point);
    /**
     * @brief callback_linePressed
     * @param point
     */
    void callback_linePressed(const QPointF &point);
    /**
     * @brief callback_keyPressed
     * @param event
     */
    void callback_keyPressed(QKeyEvent *event);
    /**
     * @brief callback_mouseMoved
     * @param event
     */
    void callback_mouseMoved(QMouseEvent *event);
    /**
     * @brief callback_mouseReleased
     * @param event
     */
    void callback_mouseReleased(QMouseEvent *event);
};

class MovableLineEventFilter : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief MovableLineEventFilter
     */
    MovableLineEventFilter();
    ~MovableLineEventFilter();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // NMFCHARTMOVABLELINE_H


