
#include "nmfChartBar.h"


nmfChartBar::nmfChartBar()
{
}


void
nmfChartBar::populateChart(
        QChart *chart,
        const std::string &type,
        double& maxYVal,
        const boost::numeric::ublas::matrix<double> &ChartData,
        const QStringList &RowLabels,
        const QStringList &ColumnLabels,
        std::string &MainTitle,
        std::string &XTitle,
        std::string &YTitle,
        const std::vector<bool> &GridLines,
        const int Theme)
{
    QBarSet    *newSet = NULL;
    QBarSeries *series = NULL;

    // Set current theme
    chart->setTheme(static_cast<QChart::ChartTheme>(Theme));
    //chart->removeAllSeries();

    if (type == "StackedBar") {
        QStackedBarSeries *series = new QStackedBarSeries();

        // Load data into series and then add series to the chart
        for (unsigned int i=0; i<ChartData.size2(); ++i) {
            if (ColumnLabels.size() == int(ChartData.size2()))
                newSet = new QBarSet((ColumnLabels[i]));
            else
                newSet = new QBarSet("");
            for (unsigned int val=0; val<ChartData.size1(); ++val) {
                *newSet << ChartData(val,i);
            }
            series->append(newSet);
        }
        chart->addSeries(series);

    } else if (type == "Line") {

        // Load data into series and then add series to the chart
        QLineSeries *series;
        for (unsigned int line=0; line<ChartData.size2(); ++line) {
            series = new QLineSeries();
            for (unsigned int j=0; j<ChartData.size1(); ++j) {
                series->append(j+1,ChartData(j,line));
            }
            chart->addSeries(series);
            if (line < unsigned(ColumnLabels.size()))
                series->setName(ColumnLabels[line]);
        }

    } else if (type == "Scatter") {
        // Load data into series and then add series to the chart
        QScatterSeries *series;
        for (unsigned int line=0; line<ChartData.size2(); ++line) {
            series = new QScatterSeries();
            series->setMarkerSize(10);
            for (unsigned int j=0; j<ChartData.size1(); ++j) {
                series->append(j+1,ChartData(j,line));
            }
            chart->addSeries(series);
            if (line < unsigned(ColumnLabels.size()))
                series->setName(ColumnLabels[line]);
        }

    } else if (type == "Bar") {
        QBarSeries *series = new QBarSeries();

        // Load data into series and then add series to the chart
        for (unsigned int i=0; i<ChartData.size2(); ++i) {
            if (ColumnLabels.size() == int(ChartData.size2())) {
                newSet = new QBarSet((ColumnLabels[i]));
            } else {
                newSet = new QBarSet("");
            }
            for (unsigned int j=0; j<ChartData.size1(); ++j) {
                *newSet << ChartData(j,i);
            }
            connect(newSet, SIGNAL(hovered(bool,int)),
                    this,   SLOT(callback_BarSetHovered(bool,int)));
            series->append(newSet);
        }
        chart->addSeries(series);
    }
    // Set main title
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    chart->setTitle(QString::fromStdString(MainTitle));

    // Setup X and Y axes
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    if (RowLabels.size() > 0)
        axis->append(RowLabels);
    chart->createDefaultAxes();
    //chart->setAxisX(axis, NULL);
    QBarSeries* seriesEmpty = new QBarSeries();
    chart->addSeries(seriesEmpty);
    nmfUtilsQt::setAxisX(chart,axis,seriesEmpty);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QAbstractAxis *axisX = chart->axes(Qt::Horizontal).back();
    QFont titleFont = axisX->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    axisX->setTitleFont(titleFont);
    axisX->setTitleText(QString::fromStdString(XTitle));
    if (RowLabels.count() > NumCategoriesForVerticalNotation)
        axis->setLabelsAngle(-90);
    else
        axis->setLabelsAngle(0);

    // Rescale vertical axis....0 to 1 in increments of 0.2
    if (type == "StackedBar") {
        QValueAxis *newAxisY = new QValueAxis();
        newAxisY->setRange(0,1.0);
        newAxisY->setTickCount(6);
        //chart->setAxisY(newAxisY,series);
        chart->addAxis(newAxisY, Qt::AlignLeft);
        series->attachAxis(newAxisY);
    }

    QValueAxis *currentAxisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).back());
    currentAxisY->setTitleFont(titleFont);
    currentAxisY->setTitleText(QString::fromStdString(YTitle));
    if (maxYVal > 0) {
        currentAxisY->setMax(maxYVal);
    }
    currentAxisY->applyNiceNumbers();

    // Set grid line visibility
    chart->axes(Qt::Horizontal).back()->setGridLineVisible(GridLines[0]);
    chart->axes(Qt::Vertical).back()->setGridLineVisible(GridLines[1]);
}

void
nmfChartBar::callback_BarSetHovered(bool hovered, int index)
{
    if (hovered) {
        QBarSet* barset = qobject_cast<QBarSet* >(QObject::sender());
        double value = barset->at(index);
        int numDecimals = (floor(value) == ceil(value)) ? 0 : 1; // value%1 tells if double has no decimal component (i.e., 20.0)
        QToolTip::showText(QCursor::pos(),barset->label() + " - " +
                           QString::number(value,'f',numDecimals) + "%");
    }
}
