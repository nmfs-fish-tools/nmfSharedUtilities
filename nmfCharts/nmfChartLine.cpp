
#include "nmfChartLine.h"
#include <nmfConstants.h>
#include <nmfConstantsMSSPM.h>


nmfChartLine::nmfChartLine()
{
    m_tooltips.clear();
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
        const QColor& LineColor,
        const std::string &LineColorName,
        const double XInc)
{
    bool showLegend = (ColumnLabels.size() > 0);
    QLineSeries *series = nullptr;
    QPen pen;
    int XStartVal = (ShowFirstPoint) ? 0 : 1;
    XStartVal += XOffset;
    double yVal;
    bool skipRest = false;
    QString lineColorName = QString::fromStdString(LineColorName);

    // Set main title
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    chart->setTitle(QString::fromStdString(MainTitle));

    chart->legend()->hide();

    // Set current theme
    chart->setTheme(static_cast<QChart::ChartTheme>(Theme));

    if (type == "FreeLine") { // A FreeLine doesn't use default integers for x axis
          series = new QLineSeries();

          // For now...just use allow line
          pen = series->pen();
          pen.setStyle(Qt::SolidLine);
          pen.setColor(LineColor); //LineColors[0]);
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
                pen.setColor(LineColor); //LineColors[line%NumColors]);
                pen.setColor(QColor(QString::fromStdString(nmfConstants::LineColors[line%nmfConstants::LineColors.size()])));
                pen.setWidth(2);
                series->setPen(pen);
            } else if (style == "DashedLine") {
                pen = series->pen();
                pen.setStyle(Qt::DashLine);
                pen.setColor(LineColor); //(LineColors[line%NumColors]);
                pen.setColor(QColor(QString::fromStdString(nmfConstants::LineColors[line%nmfConstants::LineColors.size()])));
                pen.setWidth(2);
                series->setPen(pen);
            } else {
                pen = series->pen();
                pen.setStyle(Qt::SolidLine);
                pen.setColor(LineColor); //(LineColors[line%NumColors]);
                pen.setColor(QColor(QString::fromStdString(nmfConstants::LineColors[line%nmfConstants::LineColors.size()])));
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
            if (ColumnLabels.size() > 0) {
                m_tooltips[ColumnLabels[0]] = lineColorName;
            }
            disconnect(series,0,0,0);
            connect(series, SIGNAL(hovered(const QPointF&,bool)),
                    this,   SLOT(callback_hoveredLine(const QPointF&,bool)));
        }
    }

    // Setup X and Y axes
    chart->createDefaultAxes();
    QAbstractAxis *axisX = chart->axisX();
    QFont titleFont = axisX->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);

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
    chart->legend()->setVisible(showLegend);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setShowToolTips(true);
    chart->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

}

void
nmfChartLine::callback_hoveredLine(const QPointF& point, bool hovered)
{
    QString tooltip="";
    QPoint pos = QCursor::pos();

    if (hovered) {
        if (m_tooltips.size() > 0) {
            tooltip = m_tooltips[qobject_cast<QLineSeries* >(QObject::sender())->name()];
        }
        QToolTip::showText(pos, tooltip);
    } else {
        QToolTip::hideText();
    }
}


