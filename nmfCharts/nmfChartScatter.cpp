
#include "nmfChartScatter.h"


nmfChartScatter::nmfChartScatter()
{
}


void
nmfChartScatter::populateChart(
        QChart *chart,
        std::string &type,
        const bool &ShowFirstPoint,
        const boost::numeric::ublas::matrix<double> &ChartData,
        const QColor &Color,
        const QStringList &RowLabels,
        const QStringList &ColumnLabels,
        std::string &MainTitle,
        std::string &XTitle,
        std::string &YTitle,
        const std::vector<bool> &GridLines,
        const int Theme)
{
    QScatterSeries *series = NULL;
    int StartVal = (ShowFirstPoint) ? 0 : 1;

    // Set current theme
    chart->setTheme(static_cast<QChart::ChartTheme>(Theme));
    //chart->removeAllSeries();
    if (type == "Scatter") {
        // Load data into series and then add series to the chart
        for (unsigned int line=0; line<ChartData.size2(); ++line) {
            series = new QScatterSeries();
            series->setMarkerSize(10);
            series->setColor(Color);
            for (unsigned int j=StartVal; j<ChartData.size1(); ++j) {
                series->append(j,ChartData(j,line));
            }
            chart->addSeries(series);
//            if (line < ColumnLabels.size())
//                series->setName(ColumnLabels[line]);
        }

    }

    // Set main title
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    chart->setTitle(QString::fromStdString(MainTitle));

    // Setup X and Y axes
    //QBarCategoryAxis *axis = new QBarCategoryAxis();
    //if (RowLabels.size() > 0)
    //    axis->append(RowLabels);
    chart->createDefaultAxes();
    //chart->setAxisX(axis, NULL);
    chart->legend()->setVisible(false);
    //chart->legend()->setAlignment(Qt::AlignRight);

    QAbstractAxis *axisX = chart->axes(Qt::Horizontal).back();
    QFont titleFont = axisX->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    axisX->setTitleFont(titleFont);
    axisX->setTitleText(QString::fromStdString(XTitle));

    QValueAxis *currentAxisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).back());
    currentAxisY->setTitleFont(titleFont);
    currentAxisY->setTitleText(QString::fromStdString(YTitle));
    currentAxisY->applyNiceNumbers();

    QValueAxis *currentAxisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).back());
    currentAxisX->applyNiceNumbers();

    // Set grid line visibility
    chart->axes(Qt::Horizontal).back()->setGridLineVisible(GridLines[0]);
    chart->axes(Qt::Vertical).back()->setGridLineVisible(GridLines[1]);
}

