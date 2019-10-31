
#include "nmfChartLine.h"
#include <nmfConstants.h>


nmfChartLine::nmfChartLine()
{
}


void
nmfChartLine::populateChart(
        QChart *chart,
        std::string &type,
        const std::string &style,
        const bool &ShowFirstPoint,
        const double& XOffset,
        const bool& XAxisIsInteger,
        const double& YMinSliderVal,
        const boost::numeric::ublas::matrix<double> &YAxisData,
        const QStringList &RowLabels,
        const QStringList &ColumnLabels,
        std::string &MainTitle,
        std::string &XTitle,
        std::string &YTitle,
        const std::vector<bool> &GridLines,
        const int    &Theme,
        const QColor &DashedLineColor,
        const QList<QColor>& LineColors,
        const double XInc)
{
    QLineSeries *series = NULL;
    QPen pen;
    int XStartVal = (ShowFirstPoint) ? 0 : 1;
    XStartVal += XOffset;
    int NumColors = LineColors.size();
    double yVal;
    bool skipRest = false;

    chart->legend()->hide();

    // Set current theme
    chart->setTheme(static_cast<QChart::ChartTheme>(Theme));

    if (type == "FreeLine") { // A FreeLine doesn't use default integers for x axis
          series = new QLineSeries();

          // For now...just use allow line
          pen = series->pen();
          pen.setStyle(Qt::SolidLine);
          pen.setColor(LineColors[0]);
          pen.setWidth(2);
          series->setPen(pen);

        // Load Data
        for (unsigned i=0; i<YAxisData.size2(); ++i) {
                series->append(YAxisData(0,i),
                               YAxisData(1,i));
        }

        chart->addSeries(series);
//      series->setName(ColumnLabels[0]);
        series->setPointsVisible();

    } else if (type == "Line") {

        // Load data into series and then add series to the chart
        for (unsigned line=0; line<YAxisData.size2(); ++line) {
            skipRest = false;
            series = new QLineSeries();
            if (style == "DottedLine") {
                pen = series->pen();
                pen.setStyle(Qt::DotLine);
                if (DashedLineColor == QColor(0,0,1)) {
                    pen.setColor(LineColors[line%NumColors]);
                } else {
                    pen.setColor(DashedLineColor);
                }
                pen.setWidth(2);
                series->setPen(pen);
            } else if (style == "DashedLine") {
                pen = series->pen();
                pen.setStyle(Qt::DashLine);
                if (DashedLineColor == QColor(0,0,1)) {
                    pen.setColor(LineColors[line%NumColors]);
                } else {
                    pen.setColor(DashedLineColor);
                }
                pen.setWidth(2);
                series->setPen(pen);
            } else {
                pen = series->pen();
                pen.setStyle(Qt::SolidLine);
                pen.setColor(LineColors[line%NumColors]);
                pen.setWidth(2);
                series->setPen(pen);
            }
            for (unsigned j=XStartVal-XOffset; j<YAxisData.size1(); ++j) {
                yVal = YAxisData(j,line);
                if (yVal != nmfConstants::NoValueDouble) {
                    series->append(XOffset+j*XInc,yVal);
                }
            }
            chart->addSeries(series);
            if (line < ColumnLabels.size()) {
                series->setName(ColumnLabels[line]);
            }
        }
    }

    // Set main title
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    chart->setTitle(QString::fromStdString(MainTitle));

    // Setup X and Y axes
    chart->createDefaultAxes();
    QAbstractAxis *axisX = chart->axisX();
    QFont titleFont = axisX->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
//    axisX->setTitleFont(titleFont);
//    axisX->setTitleText(QString::fromStdString(XTitle));

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
    currentAxisX->setTitleFont(titleFont);
    currentAxisX->setTitleText(QString::fromStdString(XTitle));
    currentAxisX->applyNiceNumbers();
    if (XAxisIsInteger) {
        currentAxisX->setLabelFormat("%d");
    }

    // Set grid line visibility
    chart->axisX()->setGridLineVisible(GridLines[0]);
    chart->axisY()->setGridLineVisible(GridLines[1]);

    // Show legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    if (ColumnLabels.size() != chart->series().size()) {
        chart->legend()->setVisible(false);
    }
}

