
#include "nmfChartScatter.h"


nmfChartScatter::nmfChartScatter()
{
}


void
nmfChartScatter::populateChart(
        QChart *chart,
        std::string &type,
        const bool &showFirstPoint,
        const boost::numeric::ublas::matrix<double> &chartData,
        const QColor &color,
        const QStringList &rowLabels,
        const QStringList &columnLabels,
        std::string &mainTitle,
        std::string &xTitle,
        std::string &yTitle,
        const std::vector<bool> &gridLines,
        const int theme)
{
    QScatterSeries *series = NULL;
    int StartVal = (showFirstPoint) ? 0 : 1;

    // Set current theme
    chart->setTheme(static_cast<QChart::ChartTheme>(theme));

    if (type == "Scatter") {
        // Load data into series and then add series to the chart
        for (unsigned int line=0; line<chartData.size2(); ++line) {
            series = new QScatterSeries();
            series->setMarkerSize(10);
            series->setColor(color);
            for (unsigned int j=StartVal; j<chartData.size1(); ++j) {
                series->append(j,chartData(j,line));
            }
            chart->addSeries(series);
        }

    }

    // Set main title
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    chart->setTitle(QString::fromStdString(mainTitle));

    // Setup X and Y axes
    //QBarCategoryAxis *axis = new QBarCategoryAxis();
    //if (rowLabels.size() > 0)
    //    axis->append(rowLabels);
    chart->createDefaultAxes();
    //chart->setAxisX(axis, NULL);
    chart->legend()->setVisible(false);
    //chart->legend()->setAlignment(Qt::AlignRight);

    QAbstractAxis *axisX = chart->axes(Qt::Horizontal).back();
    QFont titleFont = axisX->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    axisX->setTitleFont(titleFont);
    axisX->setTitleText(QString::fromStdString(xTitle));

    QValueAxis *currentAxisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).back());
    currentAxisY->setTitleFont(titleFont);
    currentAxisY->setTitleText(QString::fromStdString(yTitle));
    currentAxisY->applyNiceNumbers();

    QValueAxis *currentAxisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).back());
    currentAxisX->applyNiceNumbers();

    // Set grid line visibility
    chart->axes(Qt::Horizontal).back()->setGridLineVisible(gridLines[0]);
    chart->axes(Qt::Vertical).back()->setGridLineVisible(gridLines[1]);
}

