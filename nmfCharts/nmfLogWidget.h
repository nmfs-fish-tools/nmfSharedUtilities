
/**
 @file nmfMatrixPlot.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This contains the class definition for the main Qt window.  If this file changes, moc must be re-run.
 @date Dec 8, 2016
 */

#ifndef NMFLOGWIDGET_H
#define NMFLOGWIDGET_H

#include <QtCharts/QChartView>
#include <QtCharts/QChart>

#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QScrollBar>
#include <QSpacerItem>
#include <QTextEdit>
#include <QTextStream>

#include "nmfUtils.h"
#include "nmfLogger.h"

QT_CHARTS_USE_NAMESPACE

class nmfLogWidget : public QObject
{

    Q_OBJECT

private:
    nmfLogger*  m_logger;
    std::string m_logDir;

public:

    QLabel      *filenameLBL;
    QTextEdit   *logTE;
    QGroupBox   *controlsGB;
    QHBoxLayout *hMainLayt;
    QVBoxLayout *vMainLayt;
    QVBoxLayout *vGroupLayt;
    QPushButton *browsePB;
    QPushButton *refreshPB;

    nmfLogWidget(nmfLogger*  logger,
                 std::string logDir);
   ~nmfLogWidget();
    void loadLogFile(QString filename);

public Q_SLOTS:
    void callback_refreshPB(bool unused);
    void callback_browsePB(bool unused);

};


#endif // NMFLOGWIDGET_H
