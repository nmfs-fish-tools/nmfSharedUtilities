
#include "nmfChartLine.h"
#include <nmfConstants.h>
#include <nmfConstantsMSSPM.h>


nmfChartLine::nmfChartLine()
{
    m_Tooltips.clear();

    // Create custom tool tip
    m_CustomToolTip = new nmfToolTip();
}

void
nmfChartLine::clear(QChart* chart)
{
    chart->removeAllSeries();
}

void
nmfChartLine::overlayVerticalLine(
        QChart* Chart,
        const std::string& LineStyle,
        const int&         DataLineWidth,
        const bool&        ShowFirstPoint,
        const bool&        ShowLegend,
        const double&      XOffset,
        const bool&        XAxisApplyNiceNumbers,
        const bool&        XAxisIsInteger,
        const double&      XPos,
        const double&      YMinVal,
        double&            YMaxVal,
        const bool&        LeaveGapsWhereNegative,
        int&               NumXValues,
        const QString&     HoverLabel,
        std::string&       XTitle,
        std::string&       YTitle,
        const int&         FontSizeLabel,
        const int&         FontSizeNumber,
        const QString&     FontLabel,
        const int&         AxisLineWidth,
        const int&         AxisLineColor,
        const std::string& LineColor,
        const std::vector<bool>& GridLines,
        const double&      XInc)
{
    QString hoverLabel;
    int XStartVal = (ShowFirstPoint) ? XOffset : XOffset+1;
    QLineSeries *series = new QLineSeries();;
    QPen pen = series->pen();

    // Set pen color and style for the vertical line
    pen.setWidth(DataLineWidth);
    pen.setColor(QColor(QString::fromStdString(LineColor)));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    if (LineStyle == "DottedLine") {
        pen.setStyle(Qt::DotLine);
    } else if (LineStyle == "DashedLine") {
        pen.setStyle(Qt::DashLine);
    } else {
        pen.setStyle(Qt::SolidLine);
    }
    series->setPen(pen);

    // Add points for the dotted line
    series->append(XPos,YMinVal);
    series->append(XPos,YMaxVal);
    Chart->addSeries(series);

    // Set hover help
    hoverLabel = (HoverLabel.isEmpty()) ? "No Label Set" : HoverLabel;
    m_HoverLabels[hoverLabel] = hoverLabel;

    // Using the hover help QString as the name of the series so that I can
    // find later on the hover help that corresponds to the series the user is
    // hovering over.
    series->setName(hoverLabel);

    // Connect callback for this series
    disconnect(series,0,0,0);
    connect(series, SIGNAL(hovered(const QPointF&,bool)),
            this,   SLOT(callback_hoveredLine(const QPointF&,bool)));

    // Set the axes' labels and scale.
    // N.B. When adding a series you MUST make sure axes have been
    // explicitly added.  If not, the line from the series will not
    // be drawn correctly.
    resetAxes(Chart, ShowLegend, XAxisApplyNiceNumbers, XAxisIsInteger, XStartVal,
              NumXValues, YMinVal, YMaxVal, LeaveGapsWhereNegative,
              XTitle, YTitle, FontSizeLabel, FontSizeNumber, FontLabel,
              AxisLineWidth, AxisLineColor,
              GridLines, XInc);
}

void
nmfChartLine::populateChart(
        QChart*            Chart,
        std::string&       LineType,
        const std::string& LineStyle,
        const int&         DataLineWidth,
        const bool&        ShowFirstPoint,
        const bool&        ShowLegend,
        const double&      XOffset,
        const bool&        XAxisApplyNiceNumbers,
        const bool&        XAxisIsInteger,
        const double&      YMinVal,
        double&            YMaxVal,
        const bool&        LeaveGapsWhereNegative,
        const boost::numeric::ublas::matrix<double> &lineData,
        const QStringList& RowLabels,
        const QStringList& ColumnLabels,
        const QStringList& HoverLabels,
        std::string&       MainTitle,
        std::string&       XTitle,
        std::string&       YTitle,
        const int&         FontSizeLabel,
        const int&         FontSizeNumber,
        const QString&     FontLabel,
        const int&         AxisLineWidth,
        const int&         AxisLineColor,
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
    QString lineColorName = QString::fromStdString(LineColorName);
    QString hoverLabel;
    int NumXValues = lineData.size1();

    // Set main title
    QFont mainTitleFont = Chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    Chart->setTitleFont(mainTitleFont);
    Chart->setTitle(QString::fromStdString(MainTitle));

    Chart->legend()->hide();

    // Set current theme
    Chart->setTheme(static_cast<QChart::ChartTheme>(Theme));

    if (LineType == "FreeLine") { // A FreeLine doesn't use default integers for x axis
          series = new QLineSeries();

          // For now...just use allow line
          pen = series->pen();
          pen.setStyle(Qt::SolidLine);
          pen.setColor(LineColor); //LineColors[0]);
          pen.setWidth(DataLineWidth);
          pen.setCapStyle(Qt::RoundCap);
          pen.setJoinStyle(Qt::RoundJoin);
          series->setPen(pen);

        // Load Data
        for (unsigned i=0; i<lineData.size2(); ++i) {
                series->append(lineData(0,i),
                               lineData(1,i));
        }

        Chart->addSeries(series);
        series->setPointsVisible();

    } else if (LineType == "Line") {

        // Load data into series and then add series to the chart
        for (unsigned line=0; line<lineData.size2(); ++line) {
            series = new QLineSeries();

            // Set hover help for this series
            hoverLabel = (unsigned(HoverLabels.size()) <= line) ? "" : HoverLabels[line];
            if (hoverLabel.isEmpty()) {
                hoverLabel = "No Label Set";
            }
            m_HoverLabels[hoverLabel] = hoverLabel;

            // Using the hover help QString as the name of the series so that I can
            // find later on the hover help that corresponds to the series the user is
            // hovering over.
            series->setName(hoverLabel);

            // Set pen color and style
            pen = series->pen();
            if (lineColorName == "MonteCarloSimulation") {
                pen.setColor(LineColor);
            } else if (lineColorName == "No Uncertainty Variations") {
                pen.setColor(Qt::black);
            } else {
                pen.setColor(QColor(QString::fromStdString(nmfConstants::LineColors[line%nmfConstants::LineColors.size()])));
            }
            pen.setWidth(DataLineWidth);
            if (LineStyle == "DottedLine") {
                pen.setStyle(Qt::DotLine);
            } else if (LineStyle == "DashedLine") {
                pen.setStyle(Qt::DashLine);
            } else {
                pen.setStyle(Qt::SolidLine);
            }
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            series->setPen(pen);

            // Add data points to the series
            for (int j=XStartVal-XOffset; j<NumXValues; ++j) {
                yVal = lineData(j,line);
                if (yVal != nmfConstants::NoValueDouble) {
                    series->append(XOffset+j*XInc,yVal);
                }
            }

            // Add the series to the chart (each curve (i.e., forecast run) is a series)
            Chart->addSeries(series);

            // Connect callback for this series
            if (ColumnLabels.size() > 0) {
                m_Tooltips[ColumnLabels[0]] = lineColorName;
            }
            disconnect(series,0,0,0);
            connect(series, SIGNAL(hovered(const QPointF&,bool)),
                    this,   SLOT(callback_hoveredLine(const QPointF&,bool)));
        }

    }

    // Set the axes' labels and scale. N.B. This must be called after adding
    // any series to a chart.
    resetAxes(Chart, ShowLegend,
              XAxisApplyNiceNumbers, XAxisIsInteger, XStartVal,
              NumXValues, YMinVal, YMaxVal, LeaveGapsWhereNegative,
              XTitle, YTitle, FontSizeLabel, FontSizeNumber,
              FontLabel, AxisLineWidth, AxisLineColor,
              GridLines, XInc);
}


void
nmfChartLine::resetAxes(
        QChart*            chart,
        const bool&        ShowLegend,
        const bool&        XAxisApplyNiceNumbers,
        const bool&        XAxisIsInteger,
        const int&         XStartVal,
        const int&         NumXValues,
        const double&      YMinVal,
        double&            YMaxVal,
        const bool&        LeaveGapsWhereNegative,
        std::string&       XTitle,
        std::string&       YTitle,
        const int&         FontSizeLabel,
        const int&         FontSizeNumber,
        const QString&     FontLabel,
        const int&         AxisLineWidth,
        const int&         AxisLineColor,
        const std::vector<bool>& GridLines,
        const double&      XInc)
{
    double currentYMin;

    // Make default axes and set font
    chart->createDefaultAxes();
//    QAbstractAxis *axisX = chart->axes(Qt::Horizontal).back();
    QFont titleFont = QFont(FontLabel); // axisX->titleFont();
    titleFont.setPointSize(FontSizeLabel);
    titleFont.setWeight(QFont::Bold);
    QFont  axisFont  = QFont(FontLabel);
    axisFont.setPointSize(FontSizeNumber);
    axisFont.setWeight(QFont::Normal);

    // Setup Y axis
    QValueAxis *currentAxisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).back());
    currentAxisY->setTitleFont(titleFont);
    currentAxisY->setTitleText(QString::fromStdString(YTitle));
    currentYMin = currentAxisY->min();
    if (YMinVal >= 0) {
        currentAxisY->setMin(currentYMin*YMinVal/100.0);
    }
    if (YMaxVal >= 0) {
        currentAxisY->setMax(YMaxVal);
    }
    currentAxisY->applyNiceNumbers();
    currentAxisY->setLabelsFont(axisFont);

    // Handle any gaps
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

    // Assure x axis numbers are integers (I assume they're years)
    QValueAxis *currentAxisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).back());    
    if (XAxisApplyNiceNumbers) {
        currentAxisX->applyNiceNumbers();
    }
    if (XAxisIsInteger) {
        currentAxisX->setLabelFormat("%d");
    }
    currentAxisX->setLabelsFont(axisFont);
    currentAxisX->setTitleFont(titleFont);
    currentAxisX->setTitleText(QString::fromStdString(XTitle));

    // Set grid line visibility
    chart->axes(Qt::Horizontal).back()->setGridLineVisible(GridLines[0]);
    chart->axes(Qt::Vertical).back()->setGridLineVisible(GridLines[1]);

    // Set coordinate line attributes
    QBrush lineBrush;
    QPen linePen(lineBrush,(AxisLineWidth < 1) ? 1 : AxisLineWidth);
    QColor axisColor = QColor(AxisLineColor,AxisLineColor,AxisLineColor);
    currentAxisX->setLinePen(linePen);
    currentAxisX->setLinePenColor(axisColor);
    currentAxisY->setLinePen(linePen);
    currentAxisY->setLinePenColor(axisColor);

    // Set range so plot completely fills out chart in the x-direction
//    currentAxisX->setRange(XStartVal,XStartVal+(NumXValues-1)*XInc);

    // Set legend visibility
    chart->legend()->setVisible(ShowLegend);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setShowToolTips(true);
    chart->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

    YMaxVal = currentAxisY->max();
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
        else if (m_Tooltips.size() > 0) {
            tooltip = m_Tooltips[qobject_cast<QLineSeries* >(QObject::sender())->name()];
        }

        // Workaround code to keep tooltip up for 2 seconds.  See note below.
        m_CustomToolTip->move(pos.x(),pos.y()-20);
        m_CustomToolTip->setLabel(tooltip);
        m_CustomToolTip->show();
        QTimer::singleShot(nmfConstantsMSSPM::ToolTipDuration, this, SLOT(callback_hideTooltip()));

        // The following line doesn't keep the tooltip on for 5 seconds...just 1 second.
        // Could be a QT bug. As a workaround, I created a custom tooltip (m_CustomToolTip) and
        // and showing it in this method (above) and then hiding it with a singleShot timer.
        // QToolTip::showText(pos, tooltip, nullptr, QRect(), 5000); // 5000 milliseconds = 5 seconds
    } else {
        // QToolTip::hideText();
        m_CustomToolTip->hide();
    }

}

void
nmfChartLine::callback_hideTooltip()
{
    // QToolTip::hideText();
    m_CustomToolTip->hide();
}

