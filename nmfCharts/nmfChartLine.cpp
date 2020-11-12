
#include "nmfChartLine.h"
#include <nmfConstants.h>
#include <nmfConstantsMSSPM.h>


nmfChartLine::nmfChartLine()
{
    m_tooltips.clear();
}

void
nmfChartLine::clear(QChart* chart)
{
    chart->removeAllSeries();
}

void
nmfChartLine::populateChart(
        QChart*            chart,
        std::string&       type,
        const std::string& style,
        const bool&        ShowFirstPoint,
        const bool&        ShowLegend,
        const double&      XOffset,
        const bool&        XAxisIsInteger,
        const double&      YMinSliderVal,
        const bool&        LeaveGapsWhereNegative,
        const boost::numeric::ublas::matrix<double> &YAxisData,
        const QStringList& RowLabels,
        const QStringList& ColumnLabels,
        const QStringList& HoverLabels,
        std::string&       MainTitle,
        std::string&       XTitle,
        std::string&       YTitle,
        const std::vector<bool>& GridLines,
        const int&         Theme,
        const QColor&      LineColor,
        const std::string& LineColorName,
        const double&      XInc)
{
    //bool showLegend = (ColumnLabels.size() > 0);
    QLineSeries *series = nullptr;
    QPen pen;
    int XStartVal = (ShowFirstPoint) ? 0 : 1;
    XStartVal += XOffset;
    double yVal;
    bool skipRest = false;
    QString lineColorName = QString::fromStdString(LineColorName);
    QString hoverLabel;
    int NumXValues = YAxisData.size1();

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
        series->setPointsVisible();

    } else if (type == "Line") {

        // Load data into series and then add series to the chart
        for (unsigned line=0; line<YAxisData.size2(); ++line) {
            series = new QLineSeries();
            hoverLabel = (HoverLabels.size() <= line) ? "" : HoverLabels[line];
            if (hoverLabel.isEmpty()) {
                hoverLabel = "No Label Set";
            }

            m_HoverLabels[hoverLabel] = hoverLabel;

            // Using the hover help QString as the name of the series so that I can
            // find later on the hover help that corresponds to the series the user is
            // hovering over.
            series->setName(hoverLabel);

            skipRest = false;
            pen = series->pen();
            if (lineColorName == "MonteCarloSimulation") {
                pen.setColor(LineColor);
            } else if (lineColorName == "No Uncertainty Variations") {
                 pen.setColor(Qt::black);
            } else {
                pen.setColor(QColor(QString::fromStdString(nmfConstants::LineColors[line%nmfConstants::LineColors.size()])));
            }
            pen.setWidth(2);
            if (style == "DottedLine") {
                pen.setStyle(Qt::DotLine);
            } else if (style == "DashedLine") {
                pen.setStyle(Qt::DashLine);
            } else {
                pen.setStyle(Qt::SolidLine);
            }
            series->setPen(pen);

            for (unsigned j=XStartVal-XOffset; j<NumXValues; ++j) {
                yVal = YAxisData(j,line);
                if (yVal != nmfConstants::NoValueDouble) {
                    series->append(XOffset+j*XInc,yVal);
                }
            }

            chart->addSeries(series);

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


    if (LeaveGapsWhereNegative) {
        double currentYMax = currentAxisY->max();
        // Replace any y = -1 found in series with 99999
        // Then restore the previous y max value
        // This should have the effect of omitting -1 values but allowing in range values
        // if they appear any time after the -1 values in the series
        QLineSeries* lineSeries;
        QPointF seriesPoint;
        for (QAbstractSeries* series : chart->series()) {
            lineSeries = qobject_cast<QLineSeries*>(series);
            for (int i=0; i<lineSeries->count(); ++i) {
                seriesPoint = lineSeries->at(i);
                if (seriesPoint.y() == nmfConstantsMSSPM::NoFishingMortality) {
                    lineSeries->replace(i,QPointF(seriesPoint.x(),nmfConstantsMSSPM::MaxOutOfBounds));
                }

            }
        }
        currentAxisY->setMax(currentYMax);
    }




    QValueAxis *currentAxisX = qobject_cast<QValueAxis*>(chart->axisX());
    currentAxisX->setTitleFont(titleFont);
    currentAxisX->setTitleText(QString::fromStdString(XTitle));
    currentAxisX->applyNiceNumbers();
    if (XAxisIsInteger) {
        currentAxisX->setLabelFormat("%d");
    }

    // Set range so plot completely fills out chart in the x-direction
    currentAxisX->setRange(XStartVal,XStartVal+NumXValues-1);

    // Set grid line visibility
    chart->axisX()->setGridLineVisible(GridLines[0]);
    chart->axisY()->setGridLineVisible(GridLines[1]);

    // Set legend visibility
    chart->legend()->setVisible(ShowLegend);
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
        if (m_HoverLabels.size() > 0) {
            tooltip = m_HoverLabels[qobject_cast<QLineSeries* >(QObject::sender())->name()];
        }
        else if (m_tooltips.size() > 0) {
            tooltip = m_tooltips[qobject_cast<QLineSeries* >(QObject::sender())->name()];
        }

        // Call singleshot timer
//      int ToolTipDuration = 5*1000; // in seconds
//      QTimer::singleShot(ToolTipDuration, this, SLOT(callback_HideTooltip()));
        QToolTip::showText(pos, tooltip, new QWidget(), QRect(), 10000);
//      QWidget::setToolTipDuration(10000);  // RSK - can't seem to set the duration of the tooltip

    } else {
        QToolTip::hideText();
    }
}

//void
//nmfChartLine::callback_HideTooltip()
//{
//    QToolTip::hideText();
//}

