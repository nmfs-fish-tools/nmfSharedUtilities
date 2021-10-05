#ifndef NMFCHARTBAR_H
#define NMFCHARTBAR_H

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QLine>
#include <QLineSeries>
#include <QScatterSeries>
#include <QStackedBarSeries>
#include <QString>
#include <QStringList>
#include <QToolTip>
#include <QValueAxis>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <vector>
#include <math.h>

#include "nmfUtilsQt.h"

QT_CHARTS_USE_NAMESPACE

const int NumCategoriesForVerticalNotation = 10;

/**
 * @brief This class draws a bar chart
 */
class nmfChartBar : public QObject
{

    Q_OBJECT

    bool m_areDataPercentages;

public:
    /**
     * @brief Class constructor
     */
    nmfChartBar();
    virtual ~nmfChartBar() {}

    /**
     * @brief Populates a Qt bar chart with appropriate data and annotation
     * @param chart : pointer to Qt chart widget
     * @param type : type of bar chart desired
     * @param maxYVal : maximum y value used for scaling the y-axis
     * @param ChartData : matrix representing the chart's data
     * @param RowLabels : labels for rows
     * @param ColumnLabels : labels for columns
     * @param MainTitle : main chart title
     * @param XTitle : title for x-axis
     * @param YTitle : title for y-axis
     * @param areDataPercentages : flag denoting if data represent percentages
     * @param GridLines : grid lines visibility boolean
     * @param Theme : theme value for chart
     */
    void populateChart(
            QChart *chart,
            const std::string& type,
            const double& maxYVal,
            const boost::numeric::ublas::matrix<double>& ChartData,
            const QStringList& RowLabels,
            const QStringList& ColumnLabels,
            const std::string& MainTitle,
            const std::string& XTitle,
            const std::string& YTitle,
            const bool& areDataPercentages,
            const std::vector<bool>& GridLines,
            const int& Theme);

public slots:
    /**
     * @brief callback_BarSetHovered : callback invoked when user hovers over a barset
     * @param hovered : boolean describing if user is hovering over a barset
     * @param index : index of barset
     */
    void callback_BarSetHovered(bool hovered,
                                int  index);

};

#endif // NMFCHARTBAR_H
