#include "nmfChartLineWithScatter.h"

nmfChartLineWithScatter::nmfChartLineWithScatter()
{
    m_tooltips.clear();
    m_CustomToolTip = new nmfToolTip();
}

void
nmfChartLineWithScatter::populateChart(
        QChart *chart,
        std::string &type,
        const std::string &lineStyle,
        const int& dataLineWidth,
        const int& dataPointWidth,
        const bool& isMohnsRho,
        const bool &showFirstPoint,
        const bool &AddScatter,
        const int &XOffset,
        const bool &XAxisApplyNiceNumbers,
        const bool &xAxisIsInteger,
        const double &YMinVal,
        const double &YMaxVal,
        const boost::numeric::ublas::matrix<double> &YAxisData,
        const boost::numeric::ublas::matrix<double> &ScatterData,
        const boost::numeric::ublas::matrix<double> &SkipScatterData,
        const QStringList &rowLabels,
        const QStringList &columnLabels,
        std::string &mainTitle,
        std::string &xTitle,
        std::string &yTitle,
        const int& fontSizeLabel,
        const int& FontSizeNumber,
        const QString& FontLabel,
        const int& axisLineWidth,
        const int& axisLineColor,
        const std::vector<bool> &gridLines,
        const int &theme,
        const QColor &dashedLineColor,
        const QColor &scatterColor,
        const std::string &lineColorValue,
        const std::string &lineColorName,
        const QList<QString>& multiRunLineLabels,
        const bool& showLegend)
{
    int XStartVal = showFirstPoint ? XOffset : XOffset+1;
    int numPointsInLine = 0;
    double currentYMin  = 0;
    QLineSeries    *lineSeries    = nullptr;
    QScatterSeries *scatterSeries = nullptr;
    QString scatterSeriesName = "Obs Biomass";
    QPen pen;
    QColor  LineColor(QString::fromStdString(lineColorValue));
    QString LineColorName = QString::fromStdString(lineColorName);

    m_FirstYear = XStartVal;

    // Set chart attributes
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    chart->setTitle(QString::fromStdString(mainTitle));
    chart->legend()->hide();
    chart->setTheme(static_cast<QChart::ChartTheme>(theme));
    m_Chart = chart;
    m_NumYears = ScatterData.size1();

    // Load data into series and then add series to the chart
    numPointsInLine = YAxisData.size1();
    for (unsigned int line=0; line<YAxisData.size2(); ++line) {
        lineSeries = new QLineSeries();
        if (lineStyle == "DashedLine") {
            pen = lineSeries->pen();
            pen.setStyle(Qt::DashLine);
            pen.setColor(dashedLineColor);
            pen.setWidth(dataLineWidth);
            lineSeries->setPen(pen);
        } else {
            pen = lineSeries->pen();
            pen.setStyle(Qt::SolidLine);
            pen.setColor(LineColor);
            pen.setWidth(dataLineWidth);
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            lineSeries->setPen(pen);
            //series->setColor(LineColor);
        }
        for (int j=XStartVal-XOffset; j<numPointsInLine; ++j) {
            lineSeries->append(j+XOffset,YAxisData(j,line));
        }
        numPointsInLine = (isMohnsRho) ? numPointsInLine-1 : numPointsInLine;
        chart->addSeries(lineSeries);

        if (multiRunLineLabels.size() > 0) {
            lineSeries->setName(multiRunLineLabels[line]);
            m_tooltips[multiRunLineLabels[line]] = multiRunLineLabels[line];
        } else if (columnLabels.size() > 0) {
            lineSeries->setName(columnLabels[0]);
            m_tooltips[columnLabels[0]] = LineColorName;
        }

        disconnect(lineSeries,0,0,0);
        connect(lineSeries, SIGNAL(hovered(const QPointF&,bool)),
                this,       SLOT(callback_hoveredLine(const QPointF&,bool)));
    }

    if ((ScatterData.size1() != 0) && AddScatter) {
        scatterSeries = new QScatterSeries();
        scatterSeries->setMarkerSize(dataPointWidth);
        scatterSeries->setColor(scatterColor);
        for (unsigned int j=XStartVal-XOffset; j<ScatterData.size1(); ++j) {
            if (! SkipScatterData(j,0)) {
                scatterSeries->append(j+XOffset,ScatterData(j,0)); // Change 0 if more than one type of scatter points
            }
        }
        chart->addSeries(scatterSeries);
        scatterSeries->setName(scatterSeriesName);
        if (m_tooltips.size() > 0) {
            m_tooltips[scatterSeriesName] = scatterSeriesName; //"Deep Sky Blue";
        }
        m_ScatterSeries = scatterSeries;

        disconnect(scatterSeries,0,0,0);
        connect(scatterSeries, SIGNAL(hovered(const QPointF&,bool)),
                this,          SLOT(callback_hoveredScatter(const QPointF&,bool)));
    }

    // Setup X and Y axes
    chart->createDefaultAxes();
    QFont titleFont = QFont(FontLabel); // chart->axes(Qt::Horizontal).back()->titleFont();
    titleFont.setPointSize(fontSizeLabel);
    titleFont.setWeight(QFont::Bold);
    QFont axisFont  = QFont(FontLabel);
    axisFont.setPointSize(FontSizeNumber);
    axisFont.setWeight(QFont::Normal);

    chart->axes(Qt::Horizontal).back()->setTitleFont(titleFont);
    chart->axes(Qt::Horizontal).back()->setTitleText(QString::fromStdString(xTitle));

    QValueAxis *currentAxisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).back());
    currentAxisY->setTitleFont(titleFont);
    currentAxisY->setTitleText(QString::fromStdString(yTitle));
    currentYMin = currentAxisY->min();
    if (YMinVal >= 0) {
        currentAxisY->setMin(currentYMin*YMinVal/100.0);
    }
    if (YMaxVal >= 0) {
        currentAxisY->setMax(YMaxVal);
    }
    currentAxisY->applyNiceNumbers();
    currentAxisY->setLabelsFont(axisFont);

    QValueAxis *currentAxisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).back());
    if (XAxisApplyNiceNumbers) {
        currentAxisX->applyNiceNumbers();
    }
    if (xAxisIsInteger) {
        currentAxisX->setLabelFormat("%d");
    }
    currentAxisX->setLabelsFont(axisFont);

    //   Set grid line visibility
    chart->axes(Qt::Horizontal).back()->setGridLineVisible(gridLines[0]);
    chart->axes(Qt::Vertical).back()->setGridLineVisible(gridLines[1]);

    // Set coordinate line attributes
    QBrush lineBrush;
    QPen linePen(lineBrush,(axisLineWidth < 1) ? 1 : axisLineWidth);
    QColor axisColor = QColor(axisLineColor,axisLineColor,axisLineColor);
    currentAxisX->setLinePen(linePen);
    currentAxisX->setLinePenColor(axisColor);
    currentAxisY->setLinePen(linePen);
    currentAxisY->setLinePenColor(axisColor);

    // Set legend visibility
    chart->legend()->setVisible(showLegend);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setShowToolTips(true);
    chart->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

    // Add hovered callbacks to set tooltips
    QList<QLegendMarker* > legendMarkers = chart->legend()->markers();
    for (QLegendMarker* legendMarker : legendMarkers) {
        disconnect(legendMarker,0,0,0);
        connect(legendMarker, SIGNAL(hovered(bool)),
                this,         SLOT(callback_hoveredLegendMarker(bool)));
    }
}

void
nmfChartLineWithScatter::callback_hideTooltip()
{
    // QToolTip::hideText();
    m_CustomToolTip->hide();
}

void
nmfChartLineWithScatter::callback_hoveredLegendMarker(bool hovered)
{
    QString tooltip;
    QPoint pos = QCursor::pos();

    if (hovered) {
        tooltip = m_tooltips[qobject_cast<QLegendMarker* >(QObject::sender())->label()];
        QToolTip::showText(pos, tooltip);
    } else {
        QToolTip::hideText();
    }
}

void
nmfChartLineWithScatter::callback_hoveredLine(const QPointF& point, bool hovered)
{
    QString tooltip;
    QPoint pos = QCursor::pos();

    if (hovered) {
//      tooltip = m_tooltips[qobject_cast<QLineSeries* >(QObject::sender())->name()];
        tooltip = "(" + QString::number(point.x()) + "," + QString::number(point.y()) + ")";
//        tooltip += ", seed=" + QString::number(m_seed)+", conv="+m_conv;
        // RSK implement this in a later release (would be nice to see the seed value and convergence info as part of the tooltip

        // Workaround code to keep tooltip up for 2 seconds.  See note below.
        m_CustomToolTip->move(pos.x(),pos.y()-20);
        m_CustomToolTip->setLabel(tooltip);
        m_CustomToolTip->show();
        QTimer::singleShot(nmfConstantsMSSPM::ToolTipDuration, this, SLOT(callback_hideTooltip()));

        // The following line doesn't keep the tooltip on for 2 seconds...just 1 second.
        // Could be a QT bug. As a workaround, I created a custom tooltip (m_CustomToolTip) and
        // and showing it in this method (above) and then hiding it with a singleShot timer.
        // QToolTip::showText(pos, tooltip, nullptr, QRect(), 2000);
    } else {
        // QToolTip::hideText();
        m_CustomToolTip->hide();
    }
}

void
nmfChartLineWithScatter::callback_hoveredScatter(const QPointF& point, bool hovered)
{
    QString tooltip;
    QPoint pos = QCursor::pos();

    if (hovered) {
        //tooltip = m_tooltips[qobject_cast<QScatterSeries* >(QObject::sender())->name()];
        tooltip = "(" + QString::number(point.x()) + "," + QString::number(point.y()) + ")";
        m_CustomToolTip->move(pos.x(),pos.y()-20);
        m_CustomToolTip->setLabel(tooltip);
        m_CustomToolTip->show();
        QTimer::singleShot(nmfConstantsMSSPM::ToolTipDuration, this, SLOT(callback_hideTooltip()));
//      QToolTip::showText(pos, tooltip, nullptr, QRect(), 5000);
    } else {
        // QToolTip::hideText();
        m_CustomToolTip->hide();
    }
}

