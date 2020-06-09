
/**
 @file nmfViewerWidget.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This file contains the definition for the screenshot image viewer widget.
 @date Feb 16, 2018
 */

#ifndef NMFVIEWERWIDGET_H
#define NMFVIEWERWIDGET_H

#include <QApplication>
#include <QComboBox>
#include <QDir>
#include <QHBoxLayout>
#include <QImage>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>

#include "nmfLogger.h"
#include "nmfUtilsQt.h"

class nmfViewerWidget : public QObject
{

    Q_OBJECT

private:
    QString      m_ImagePath;
    QWidget*     m_Parent;
    nmfLogger*   m_Logger;

    QPushButton* ScreenShotViewerDelPB;
    QPushButton* ScreenShotViewerRefPB;
    QPushButton* ScreenShotViewerRenPB;
    QComboBox*   ScreenShotViewerCMB;
    QLabel*      ScreenShotViewerPixmapLBL;

public:
    nmfViewerWidget(QWidget* parent,
                    QString imagePath,
                    nmfLogger* logger);
   ~nmfViewerWidget();

    QVBoxLayout* mainLayt;

    void setupOutputScreenShotViewerWidgets();
    void updateScreenShotViewer(QString filename);
    bool fileExists(QString filename);
    bool okToOverwrite(QString filenameWithPath);
    void refreshList();

public Q_SLOTS:
    void callback_ScreenShotViewerRefPB();
    void callback_ScreenShotViewerRenPB();
    void callback_ScreenShotViewerDelPB();
    void callback_ScreenShotViewerCMB(QString filename);
};


#endif // NMFVIEWERWIDGET_H
