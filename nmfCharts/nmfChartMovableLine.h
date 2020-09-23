#ifndef NMFCHARTMOVABLELINE_H
#define NMFCHARTMOVABLELINE_H

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>

#include <cmath>
#include <iostream>


QT_CHARTS_USE_NAMESPACE

/**
 * @brief This class allows the user to create a multi-segmented line. The user
 * may add vertices to a line segment, causing the line segment to split into 2 lines. The
 * filled circle representing a vertex may be dragged to modify the slope of the
 * 2 adjacent lines.
 */
class nmfChartMovableLine : public QObject
{
    Q_OBJECT

    std::vector<QPointF> m_yearlyPoints;

private:
    const int       MAX_Y      =   4;
    const int       MIN_Y      =   0;
    const double    HUNDRETHS  = 100.0;
    const double    FIFTIETHS  =  50.0;
    const double    TWENTIETHS =  20.0;
    const double    TENTHS     =  10.0;
    bool            m_PointPressed;
    int             m_MaxX;
    int             m_MinX;
    int             m_MaxY;
    int             m_MinY;
    double          m_RoundingFactor;
    std::string     m_MainTitle;
    std::string     m_XTitle;
    std::string     m_YTitle;
    QChart*         m_Chart;
    QChartView*     m_ChartView;
    QLineSeries*    m_Line;
    QPointF         m_CurrPoint;
    QScatterSeries* m_Scatter;
    QScatterSeries* m_SelectedScatter;

    void checkChartBoundaries(QPointF *point);
    /**
     * @brief Removes all points from line chart. N.B. This will also remove the end points.
     */
    void getRange(int& xMin, int& xMax);
    void removeAllPoints();
    double roundTo(const double& place,
                   const double& value);

public:
    /**
     * @brief Class constructor for multi-segmented line plot
     * @param mainTitle : main title of plot
     * @param xTitle : title for the x-axis
     * @param yTitle : title for the y-axis
     */
    nmfChartMovableLine(
            QWidget*     parent,
            std::string& mainTitle,
            std::string& xTitle,
            std::string& yTitle);
    ~nmfChartMovableLine();

    /**
     * @brief Add a point to the current set of points describing the line
     * @param currPoint : point to add
     */
    void addPoint(QPointF currPoint);
    /**
     * @brief Calculates the y coordinate corresponding to all the x (i.e., year)
     * values. Needs to take into account that between each pair of points there's
     * a line with a potentially different slope than those of its neighbors.
     */
    void calculateYearlyPoints();
    /**
     * @brief Gets the maximum y scale factor value
     * @return Returns the max y scale factor value
     */
    int getMaxYScaleFactor();
    /**
     * @brief Gets the number of points on the plot
     * @return Returns the number of plot points
     */
    int getNumPoints();
    /**
     * @brief Gets all of the points in the plot
     * @return Returns a list of qpoints representing all the points on the plot
     */
    QList<QPointF> getPoints();
    /**
     * @brief Gets the y value corresponding to the passed in x value
     * @param xValue : x value (i.e., year value) of plot
     * @return Returns the corresponding y value to the passed in x value
     */
    double getYValue(const int& xValue);
    /**
     * @brief Hide the plot
     */
    void hide();
    /**
     * @brief Draw the plot given the passed in year range
     * @param startYear : start year (x value) for the plot
     * @param endYear : end year (x value) for the plot
     */
    void populateChart(int& startYear,
                       int& endYear);
    /**
     * @brief Remove any points that may have recently been scaled outside of the end points
     */
    void resetEndPoints();
    /**
     * @brief Removes interior points and resets end points to y=1
     */
    void resetPoints();
    /**
     * @brief Sets the maximum y range value and the y range increment
     * @param maxYValue : the maximum y value
     */
    void setMaxYValue(int maxYValue);
    /**
     * @brief Replaces a point on the line
     * @param xValue : x value of point to replace
     * @param newPoint : the value of the new point that will replace the old point
     */
    void setPointYValue(
            const int&     xValue,
            const QPointF& newPoint);
    /**
     * @brief Sets the range in the x axis
     * @param numYears : number of years to display along the x-axis
     */
    void setRange(const int& numYears);
    /**
     * @brief Show the plot
     */
    void show();
    /**
     * @brief Redraw the chart using the passed range
     * @param startYear : minimum x value for the x-axis range
     * @param endYear : maximum x value for the x-axis range
     */
    void updateChart(
            const int& startYear,
            const int& endYear);

public Q_SLOTS:
    /**
     * @brief Callback invoked when the user presses a key over the plot
     * @param event : key event data
     */
    void callback_KeyPressed(QKeyEvent *event);
    /**
     * @brief Callback invoked when the user clicks on the line
     * @param point : point at which the user clicked
     */
    void callback_LinePressed(const QPointF &point);
    /**
     * @brief Callback invoked when the user moves the mouse over the plot
     * @param event : mouse event data
     */
    void callback_MouseMoved(QMouseEvent *event);
    /**
     * @brief Callback invoked when the user releases the mouse button over the plot
     * @param event : mouse event data
     */
    void callback_MouseReleased(QMouseEvent *event);
    /**
     * @brief Callback invoked when the user clicks on a vertex point
     * @param point : the coordinates of the point clicked
     */
    void callback_PointPressed(const QPointF &point);
    /**
     * @brief Callback invoked when the user releases the mouse after clicking on a vertex point
     * @param point : the coordinates of the vertex point just clicked and released on
     */
    void callback_PointReleased(const QPointF &point);
    /**
     * @brief Callback invoked when the user clicks on a selected vertex point
     * @param point : the coordinates of the point clicked
     */
    void callback_SelectedPointsPressed(const QPointF &point);
    /**
     * @brief Callback invoked when the user releases the mouse over a selected vertex point
     * @param point : the coordinates of the selected point
     */
    void callback_SelectedPointsReleased(const QPointF &point);
};


#endif // NMFCHARTMOVABLELINE_H


