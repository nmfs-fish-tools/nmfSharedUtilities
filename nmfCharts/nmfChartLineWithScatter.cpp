
#include "nmfChartLineWithScatter.h"


nmfChartLineWithScatter::nmfChartLineWithScatter()
{
}


void
nmfChartLineWithScatter::populateChart(
        QChart *chart,
        std::string &type,
        const std::string &style,
        const bool &ShowFirstPoint,
        const bool &AddScatter,
        const int &XOffset,
        const bool &xAxisIsInteger,
        const double &YMinSliderVal,
        const boost::numeric::ublas::matrix<double> &YAxisData,
        const boost::numeric::ublas::matrix<double> &ScatterData,
        const QStringList &RowLabels,
        const QStringList &ColumnLabels,
        std::string &MainTitle,
        std::string &XTitle,
        std::string &YTitle,
        const std::vector<bool> &GridLines,
        const int &Theme,
        const QColor &DashedLineColor,
        const QColor &ScatterColor,
        const QColor &LineColor)
{
    QLineSeries    *lineSeries    = NULL;
    QScatterSeries *scatterSeries = NULL;
    QPen pen;
    int XStartVal = (ShowFirstPoint) ? 0 : 1;
    XStartVal += XOffset;

    chart->legend()->hide();
    // Set current theme
    chart->setTheme(static_cast<QChart::ChartTheme>(Theme));
    //chart->removeAllSeries();

    // Load data into series and then add series to the chart
    for (unsigned int line=0; line<YAxisData.size2(); ++line) {
        lineSeries = new QLineSeries();
        if (style == "DashedLine") {
            pen = lineSeries->pen();
            pen.setStyle(Qt::DashLine);
            pen.setColor(DashedLineColor);
            pen.setWidth(2);
            lineSeries->setPen(pen);
        } else {
            pen = lineSeries->pen();
            pen.setStyle(Qt::SolidLine);
            pen.setColor(LineColor);
            pen.setWidth(2);
            lineSeries->setPen(pen);
            //series->setColor(LineColor);
        }
        for (unsigned j=XStartVal-XOffset; j<YAxisData.size1(); ++j) {
            lineSeries->append(j+XOffset,YAxisData(j,line));
        }
        chart->addSeries(lineSeries);
        lineSeries->setName(ColumnLabels[0]);
    }

    if ((ScatterData.size1() != 0) && AddScatter) {
        scatterSeries = new QScatterSeries();
        scatterSeries->setMarkerSize(10);
        scatterSeries->setColor(ScatterColor);
        for (unsigned int j=XStartVal-XOffset; j<ScatterData.size1(); ++j) {
            scatterSeries->append(j+XOffset,ScatterData(j,0)); // Change 0 if more than one type of scatter points
        }
        chart->addSeries(scatterSeries);
        scatterSeries->setName("Biomass Obs");
    }

    // Set main title
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    chart->setTitle(QString::fromStdString(MainTitle));

    // Setup X and Y axes
    chart->createDefaultAxes();
    QFont titleFont = chart->axisX()->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    chart->axisX()->setTitleFont(titleFont);
    chart->axisX()->setTitleText(QString::fromStdString(XTitle));

    QValueAxis *currentAxisY = qobject_cast<QValueAxis*>(chart->axisY());
    currentAxisY->setTitleFont(titleFont);
    currentAxisY->setTitleText(QString::fromStdString(YTitle));
    currentAxisY->applyNiceNumbers();
    double currentYMin = currentAxisY->min();
    if (YMinSliderVal >= 0) {
        currentAxisY->setMin(currentYMin*YMinSliderVal/100.0);
        currentAxisY->applyNiceNumbers();
    }

    QValueAxis *currentAxisX = qobject_cast<QValueAxis*>(chart->axisX());
    currentAxisX->applyNiceNumbers();
    if (xAxisIsInteger) {
        currentAxisX->setLabelFormat("%d");
    }

    // Set grid line visibility
    chart->axisX()->setGridLineVisible(GridLines[0]);
    chart->axisY()->setGridLineVisible(GridLines[1]);

    // Show legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

}

