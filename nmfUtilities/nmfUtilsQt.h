/*
#ifndef NMFUTILITIES_H
#define NMFUTILITIES_H

#include "nmfutilities_global.h"

class NMFUTILITIESSHARED_EXPORT nmfUtilities
{

public:
    nmfUtilities();
};

#endif // NMFUTILITIES_H
*/

/**
 @file nmfUtils.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This header file defines MSVPA_X2 Qt utility functions.
 @date Jul 26, 2017
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QMimeData>
#include <QScrollBar>
#include <QString>
#include <QTableView>
#include <QTableWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QStandardItemModel>
#include <QProcess>
#include <QFileDialog>
#include <QSpacerItem>
#include <QGridLayout>
#include <QDate>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMainWindow>
#include <QDebug>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/multi_array.hpp>
#include <boost/version.hpp>
#include <boost/filesystem.hpp>

typedef boost::multi_array<double, 3> Boost3DArrayDouble;
typedef boost::multi_array<double, 4> Boost4DArrayDouble;
typedef boost::multi_array<double, 5> Boost5DArrayDouble;

//struct ADMBFilesStruct {
//    QString dataFile;
//    QString parameterFile;
//    QString pinFile;
//    QString tplFile;
//    QString reportFile;
//    QString buildOutput;
//    QString runOutput;
//};


class nmfFileViewer : public QDialog {

    Q_OBJECT

    QTextEdit* m_TextEdit;

public:
    nmfFileViewer(QWidget* parent = 0,
                  const char* title = 0);

private slots:
    void callback_FontSizePB(QString fontSize);
};


class nmfTableView: public QTableView
{
    Q_OBJECT

public:
    explicit nmfTableView(QWidget *parent = 0);
//  virtual ~nmfTableView()=0;

    void copy();
    void paste();
    void pasteAll();
    void clearAll();
    void clear();
    void selectAllItems();
    void deselectAllItems();

    void keyPressEvent(QEvent* e);
};

class nmfTableView2: public QTableView
{
    Q_OBJECT

public:
    explicit nmfTableView2(QWidget *parent = 0);
//  virtual ~nmfTableView2()=0;

    void copy();
    void paste();
    void pasteAll();
    void clearAll();
    void clear();
    void selectAllItems();
    void deselectAllItems();
};



namespace nmfUtilsQt {

const QString ReadOnlyLineEditBgColor = "QLineEdit { background: rgb(247,247,247);}";

void sendToOutputWindow(
        QTextEdit *te,
        bool clear);

void sendToOutputWindow(
        QTextEdit *te,
        std::string label,
        std::string content);

void sendToOutputWindow(
        QTextEdit *te,
        std::string title,
        std::vector<std::string> rowTitles,
        std::vector<std::string> colTitles,
        boost::numeric::ublas::matrix<double> &outMatrix,
        int numDigits,
        int numDecimals);

void equalizeQStringLengths(QString &s1, QString &s2);

bool allCellsArePopulated(QTabWidget *tabW,
                          QTableView *tv,
                          bool showError);

bool allCellsArePopulated(QTabWidget *tabW,
                          QTableWidget *tw,
                          bool showError,
                          bool skipRow1);

bool allMaxCellsGreaterThanMinCells(
        QStandardItemModel* smodelMin,
        QStandardItemModel* smodelMax);

int rename(QString fileIn, QString fileOut);

bool clearCSVFile(std::string tableToClear,
                  std::string projectDir,
                  std::string &errorMsg);

QString copy(QApplication* qtApp,
             QTableView*   tableView);
QString paste(QApplication* qtApp,
              QTableView*   tableView);
QString pasteAll(QApplication* qtApp,
                 QTableView* tableView);
QString clearAll(QTableView* tableView);
QString clear(QApplication* qtApp,
              QTableView* tableView);
QString clearSelection(QTableView* tableView,
                       QModelIndexList indexes);
QString selectAll(QTableView* tableView);
QString deselectAll(QTableView* tableView);

QTreeWidgetItem* addTreeRoot(QTreeWidget* NavigatorTree,
                             QString name,
                             QString label);

void addTreeItem(QTreeWidgetItem *parent,
                 QString name,
                 QString label);



void menu_about(const QString& name,
                const QString& operatingSystem,
                const QString& version,
                const QString& specialAcknowledgement,
                const QString& appMsg);


void checkForAndDeleteLogFiles(QString name,
                               std::string theLogDir,
                               std::string theLogFilter);

void checkForAndCreateDirectories(std::string dir,
                                  std::string dataDir,
                                  std::string logDir);

void convertVectorToStrList(const std::vector<std::string>& labels,
                            QStringList& slist);

void checkForAndReplaceInvalidCharacters(QString &stringValue);

} // end namespace

