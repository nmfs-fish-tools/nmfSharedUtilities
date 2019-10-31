
#include "nmfChartSurface.h"


nmfChartSurface::nmfChartSurface(Q3DSurface* graph3D,
                                 const int&  FirstRow,
                                 const int&  FirstColumn,
                                 const QString& XTitle,
                                 const QString& YTitle,
                                 const QString& ZTitle,
                                 const boost::numeric::ublas::matrix<double> &Data)
{
    float x,y,z;

//    Q3DSurface *graph3D = new Q3DSurface();
//    QWidget *ChartView3d = QWidget::createWindowContainer(graph3D);
//    if (!graph3D->hasContext()) {
//        QMessageBox msgBox;
//        msgBox.setText("Couldn't initialize the OpenGL context.");
//        msgBox.exec();
//        return;
//    }
    QSurfaceDataProxy* surfaceProxy;
    QSurface3DSeries*  surfaceSeries;

    surfaceProxy  = new QSurfaceDataProxy();
    surfaceSeries = new QSurface3DSeries(surfaceProxy);

    int nrows = Data.size1(); // 31 = Years
    int ncols = Data.size2(); // 13 = Age Groups

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(nrows);
    double maxY=0;
    for (int row = 0; row < nrows; row++) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(ncols);
        int index = 0;
        for (int col = 0; col < ncols; ++col) {
            x = row+FirstRow;
            y = Data(row,col);
            maxY = (y > maxY) ? y : maxY;
            z = col+FirstColumn;
            (*newRow)[index++].setPosition(QVector3D(z, y, x)); // If z and x are flipped, only a quarter of surface will be rendered. Qt Bug.
        }
        *dataArray << newRow;
    }

    // Set up surface titles
    graph3D->axisX()->setTitle(XTitle);
    graph3D->axisX()->setTitleVisible(true);
//  graph3D->axisX()->setTitleFixed(false);
    graph3D->axisY()->setTitle(YTitle);
    graph3D->axisY()->setTitleVisible(true);
//  graph3D->axisY()->setTitleFixed(false);
    graph3D->axisZ()->setTitle(ZTitle);
    graph3D->axisZ()->setTitleVisible(true);
//  graph3D->axisZ()->setTitleFixed(false);
    graph3D->axisX()->setLabelFormat("Age %d");
    graph3D->axisZ()->setLabelFormat("%d");
    graph3D->axisX()->setLabelAutoRotation(45.00);
    graph3D->axisY()->setLabelAutoRotation(45.00);
    graph3D->axisZ()->setLabelAutoRotation(45.00);


    // Remove any existing series
    for (QSurface3DSeries* series : graph3D->seriesList()) {
        graph3D->removeSeries(series);
    }
    surfaceProxy->resetArray(dataArray);
//  surfaceSeries->setSelectedPoint(QPoint(nrows,ncols));
    graph3D->addSeries(surfaceSeries);

    // Some logic here to get "nicer" numbers along the vertical scale.
    int power = int(std::log10(maxY));             // Ex. let maxY = 304 -> power = 2
    power = std::pow(10,power);                    // Ex. power = 100
    double upperLim = power*(int(maxY/power + 1)); // Ex. 100*int(304/100+1) => upperLim = 400
    graph3D->axisY()->setRange(0,upperLim);

//    QLayoutItem *child;
//    while ((child=ChartLayt->takeAt(0)) != 0) {
//        delete child;
//    }
//    ChartLayt->insertWidget(0,ChartView3d);

}

void
nmfChartSurface::showTest()
{



}

/*
void
nmfChartBar::populateChart(
        QChart *chart,
        std::string &type,
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
            if (ColumnLabels.size() == ChartData.size2())
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
            if (line < ColumnLabels.size())
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
            if (line < ColumnLabels.size())
                series->setName(ColumnLabels[line]);
        }

    } else if (type == "Bar") {
        QBarSeries *series = new QBarSeries();

        // Load data into series and then add series to the chart
        for (unsigned int i=0; i<ChartData.size2(); ++i) {
            if (ColumnLabels.size() == ChartData.size2())
                newSet = new QBarSet((ColumnLabels[i]));
            else
                newSet = new QBarSet("");
            for (unsigned int j=0; j<ChartData.size1(); ++j) {
                *newSet << ChartData(j,i);
            }
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
    chart->setAxisX(axis, NULL);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QAbstractAxis *axisX = chart->axisX();
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
        chart->setAxisY(newAxisY,series);
    }

    QValueAxis *currentAxisY = qobject_cast<QValueAxis*>(chart->axisY());
    currentAxisY->setTitleFont(titleFont);
    currentAxisY->setTitleText(QString::fromStdString(YTitle));
    currentAxisY->applyNiceNumbers();

    // Set grid line visibility
    chart->axisX()->setGridLineVisible(GridLines[0]);
    chart->axisY()->setGridLineVisible(GridLines[1]);
}

*/
