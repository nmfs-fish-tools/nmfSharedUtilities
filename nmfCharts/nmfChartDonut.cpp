
#include "nmfChartDonut.h"


nmfChartDonut::nmfChartDonut()
{
}


void
nmfChartDonut::populateChart(
        QChart *chart,
        const boost::numeric::ublas::matrix<double> &ChartData,
        const QStringList &RowLabels,
        const QStringList &ColumnLabels,
        std::string &MainTitle,
        std::string &XTitle,
        std::string &YTitle,
        const std::vector<bool> &GridLines,
        const int Theme)
{
    int numDonuts = ChartData.size1();
    double minSize = 0.2;
    double maxSize = 0.9;
    double width = (maxSize - minSize) / numDonuts;
    QString label;
    QStringList parts;

    if (numDonuts != RowLabels.size()) {
        std::cout << "Error (nmfChartDonut): Num species not equal to num row labels" << std::endl;
    }

    // Set current theme
    chart->setTheme(static_cast<QChart::ChartTheme>(Theme));

    for (int i = 0; i < numDonuts; i++) {
        QPieSeries *series = new QPieSeries;
        series->setHoleSize(minSize + (i + 0) * width*1.1);
        series->setPieSize( minSize + (i + 1) * width);

        int sliceCount =  ChartData.size2();
        for (int j = 0; j < sliceCount; j++) {
            qreal value = ChartData(i,j);
            QPieSlice *slice = new QPieSlice(ColumnLabels[j] + QString(" - %1%").arg(value), value);
            slice->setLabelVisible(true);
            slice->setLabelColor(Qt::white);
            slice->setLabelPosition(QPieSlice::LabelInsideTangential);

            // Display the percentage to only 1 place
            value = slice->value();
            label = slice->label();
            parts = label.split("-");
            if (int(10.0*value) == 10.0*int(value)) {
                slice->setLabel(parts[0].trimmed()+" - "+QString::number(value,'f',0)+"%");
            } else {
                slice->setLabel(parts[0].trimmed()+" - "+QString::number(value,'f',1)+"%");
            }

            connect(slice, &QPieSlice::hovered,
                    this,  &nmfChartDonut::callback_explodeSlice);
            series->append(slice);
        }

        chart->addSeries(series);
    }

    chart->legend()->hide();

    // Set main title
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    chart->setTitle(QString::fromStdString(MainTitle));

//  chart->legend()->setVisible(true);
//  chart->legend()->setAlignment(Qt::AlignRight);

    QAbstractAxis *axisX = chart->axes(Qt::Horizontal).back();
    QFont titleFont = axisX->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    axisX->setTitleFont(titleFont);
    axisX->setTitleText(QString::fromStdString(XTitle));
    axisX->setLabelsVisible(false);
    axisX->setLineVisible(false);

//  QValueAxis *currentAxisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).back());
//  currentAxisY->setTitleFont(titleFont);
//  currentAxisY->setTitleText(QString::fromStdString(YTitle));

    // Set grid line visibility
//  chart->axes(Qt::Horizontal).back()->setGridLineVisible(GridLines[0]);
//  chart->axes(Qt::Vertical).back()->setGridLineVisible(GridLines[1]);
    chart->axes(Qt::Horizontal).back()->setGridLineVisible(false);
    chart->axes(Qt::Vertical).back()->setGridLineVisible(false);
    chart->axes(Qt::Horizontal).back()->setShadesVisible(false);
    chart->axes(Qt::Vertical).back()->setShadesVisible(false);
//  chart->axes(Qt::Horizontal).back()->hide();
    chart->axes(Qt::Vertical).back()->hide();
}

void
nmfChartDonut::callback_explodeSlice(bool exploded)
{
    QPieSlice *slice = qobject_cast<QPieSlice *>(sender());
//    slice->setExploded(exploded);

    if (exploded) {
        QToolTip::showText(QCursor::pos(),slice->label());
    }

}
