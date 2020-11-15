
/**
 * @file nmfUtilsQt.h
 * @brief Definition for Qt widgets and functions that operate on widgets.
 * @date Jul 26, 2017
 *
 * This file contains common Qt widgets and functions that operate on Qt widgets
 * as used by the MultiSpecies applications.
 *
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <QApplication>
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QtCharts/QChart>
#include <QClipboard>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QLineSeries>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QProcess>
#include <QPushButton>
#include <QScrollBar>
#include <QSettings>
#include <QSpacerItem>
#include <QStackedBarSeries>
#include <QStandardItemModel>
#include <QString>
#include <QTableView>
#include <QTableWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QValueAxis>

#include "nmfUtils.h"
#include "nmfConstantsMSCAA.h"
#include "nmfConstantsMSSPM.h"
#include "nmfConstantsMSVPA.h"
#include "nmfConstantsDonut.h"

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


QT_CHARTS_USE_NAMESPACE

/**
 * @brief This class creates a dialog that contains a text edit widget displaying
 * the contents of the passed filename.  The dialog also includes a font combo button
 * which allows the user to change the font size of the displayed text. There's an OK
 * button to close the dialog.
 */
class nmfFileViewer : public QDialog {

    Q_OBJECT

    QTextEdit* m_TextEdit;

public:
    /**
     * @brief Constructor class for the file viewer dialog widget
     * @param parent : parent widget over which to display this dialog widget
     * @param filename : name of file to read and display in this widget's text edit box
     */
    nmfFileViewer(QWidget* parent = 0,
                  const char* filename = 0);

private slots:
    void callback_FontSizePB(QString fontSize);
};


/*
// Currently unused and TBD. Will be further developed in
// upcoming releases.
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
*/

/**
 * @brief Namespace containing Qt utility functions
 */
namespace nmfUtilsQt {

    /**
     * @brief Background color specification for a read only line edit widget
     */
    const QString ReadOnlyLineEditBgColor = "QLineEdit { background: rgb(247,247,247);}";
    /**
     * @brief adds a widget tree item under the passed parent item
     * @param parent : parent widget item
     * @param label : text label for widget item
     */
    void addTreeItem(QTreeWidgetItem* parent,
                     QString          label);
    /**
     * @brief Adds a widget tree root item to the passed tree widget
     * @param parent : parent tree widget
     * @param label : text label for root tree widget item
     * @return Returns the newly created tree widget item
     */
    QTreeWidgetItem* addTreeRoot(QTreeWidget* parent,
                                 QString      label);

    /**
     * @brief Checks that all cells in a QTableView table are populated
     * @param tabWidget : parent tab widget of the QTableView widget
     * @param tableView : tableView widget whose cells are being checked
     * @param showError : boolean signifying if error should be shown with a popup
     * @return True if no errors, False if one or more cells are not populated
     */
    bool allCellsArePopulated(QTabWidget* tabWidget,
                              QTableView* tableView,
                              bool        showError);
    /**
     * @brief Checks that all cells in a QTableView table are populated
     * @param tabWidget : parent tab widget of the QTableView widget
     * @param tableView : tableView widget whose cells are being checked
     * @param showError : boolean signifying if error should be shown with a popup
     * @param skipFirstRow : boolean signifying user wants to skip checking the first row of the table
     * @return True if no errors, False if one or more cells are not populated
     */
    bool allCellsArePopulated(
            QTabWidget*   tabWidget,
            QTableWidget* tableWidget,
            bool          showError,
            bool          skipFirstRow);
    /**
     * @brief Checks to see if all of the values in the second model are greater than
     * or equal to all of the values in the first model (on a cell by cell basis)
     * @param smodelMin : the minimum model to check
     * @param smodelMax : the maximum model to check
     * @return True if all the max model cells are greater than or equal to the min model cells, else False
     */
    bool allMaxCellsGreaterThanMinCells(
            QStandardItemModel* smodelMin,
            QStandardItemModel* smodelMax);
    /**
     * @brief Calculates the sum of the widths of the column range
     * @param tableview : tableview whose column widths are to be measured
     * @param firstCol : first column in range to be measured
     * @param lastCol : last column in range to be measured
     * @return
     */
    int calculateMultiColumnWidth(QTableView* tableview,
                                  const int&  firstCol,
                                  const int&  lastCol);
    /**
     * @brief Creates the passed directories if they don't already exist
     * @param dir : typically a hidden application directory
     * @param dataDir : the application data directory
     * @param logDir : the application log directory
     */
    void checkForAndCreateDirectories(std::string dir,
                                      std::string dataDir,
                                      std::string logDir);
    /**
     * @brief Deletes log files after confirming with the user
     * @param name : name of application
     * @param logDir : directory in which to put the log files
     * @param logFilter : file filter for the log files
     */
    void checkForAndDeleteLogFiles(QString     name,
                                   std::string logDir,
                                   std::string logFilter);
    /**
     * @brief Removes any non-alphanumeric characters and replaces any space
     * characters with an underscore character
     * @param stringValue : the string value to check and modify if necessary
     */
    void checkForAndReplaceInvalidCharacters(QString& stringValue);
    /**
     * @brief Clears the current tableView selection or the entire tableView if there's no selection
     * @param qtApp : qt application
     * @param tableView : tableView widget
     * @return Returns error msg if there was an error, else returns the empty string
     */
    QString clear(QApplication* qtApp,
                  QTableView*   tableView);
    /**
     * @brief Clears every cell in the passed tableView widget
     * @param tableView : tableView widget to clear
     * @return Returns error message if there was an error, else returns the empty string
     */
    QString clearAll(QTableView* tableView);
    /**
     * @brief Clear the passed comma separated values (CSV) file
     * @param tableToClear : name of table to clear
     * @param projectDir : name of project directory
     * @param errorMsg : error message (empty string if no error, otherwise the error message)
     * @return True if there was an error, else False
     */
    bool clearCSVFile(std::string  tableToClear,
                      std::string  projectDir,
                      std::string& errorMsg);
    /**
     * @brief Clears the current selection
     * @param tableView : tableView whose selection is to be cleared
     * @param indexes : cell indexes to clear
     * @return Empty string (currently unused)
     */
    QString clearSelection(QTableView*     tableView,
                           QModelIndexList indexes);
    /**
     * @brief Clears the entire tableView of all the passed tableViews
     * @param tables : list of tableViews to clear
     */
    void clearTableView(const QList<QTableView*>& tables);
    /**
     * @brief Converts a vector of std::string types to a QStringList
     * @param stdStringVector : vector of std::string types
     * @param qStringlist : the returned (by reference) QStringList
     */
    void convertVectorToStrList(const std::vector<std::string>& stdStringVector,
                                QStringList& qStringlist);
    /**
     * @brief Copies the selected QTableView cells
     * @param qtApp : Qt application
     * @param tableView : tableView widget
     * @return Returns error message or empty string if there is no error
     */
    QString copy(QApplication* qtApp,
                 QTableView*   tableView);
    /**
     * @brief Creates a Qt settings file
     * @param windowsDir : windows directory for the location of
     * the settings file if running on a Windows operating system
     * @param name : prefix name for Windows settings file
     * @return
     */
    QSettings* createSettings(const std::string& windowsDir,
                              const QString&     name);
    /**
     * @brief deselects all cells in the passed tableView
     * @param tableView : tableView to deselect cells
     * @return Returns error message if an error was found, else returns an empty string
     */
    QString deselectAll(QTableView* tableView);
    /**
     * @brief Equalizes the lengths of the 2 passed strings by
     * padding the shorter with spaces at the beginning of the string
     * @param string1 : first string to compare
     * @param string2 : second string to compare
     */
    void equalizeQStringLengths(QString& string1,
                                QString& string2);
    /**
     * @brief Tests if field is empty or not
     * @return Returns boolean describing if argument is empty string
     */
    bool emptyField(QStringList fields);
    /**
     * @brief Checks to see if the passed in filename exists
     * @param filenameWithPath : file to check for existence
     * @return Boolean signifying if the passed in filename exists
     */
    bool fileExists(QString filenameWithPath);
    /**
     * @brief Gets the name of the current tab
     * @param tabWidget : tab widget containing the tab in question
     * @return Returns the name of the current tab
     */
    QString getCurrentTabName(QTabWidget* tabWidget);
    /**
     * @brief Get the index value for the names tab
     * @param tabWidget : tab widget containing the tab in question
     * @param tabName : the name of the tab whose index is to be returned
     * @return Returns the index of the named tab widget
     */
    int getTabIndex(QTabWidget* tabWidget, QString tabName);
    /**
     * @brief Determine if the passed string is an error message
     * @param errorMsg : return string from previous database call
     * @return True if errorMsg represents an error, False if it does not
     */
    bool isAnError(std::string errorMsg);
    /**
     * @brief Tests the passed fields to see if the parameters are out of range of their min, max fields
     * @param fields : list of parameters and their respective ranges (i.e., GrowthRate,GrowthRateMin,GrowthRateMax,...)
     * @param badParam : name of the out of range parameter
     * @return Boolean signifying if any of the passed parameters were outside of their respective ranges
     */
    bool outOfRange(QStringList fields, QString& badParam);
    /**
     * @brief Pastes what's been saved to the clipboard into the passed tableView
     * @param qtApp : Qt application from which to get the copy buffer
     * @param tableView : tableView to paste into
     * @return Returns error message if an error was found, else returns an empty string
     */
    QString paste(QApplication* qtApp,
                  QTableView*   tableView);
    /**
     * @brief Pastes the first cell of whatever is in the clipboard to every cell in the
     * tableView and then deselects the tableView
     * @param qtApp : Qt application from which to get the copy buffer
     * @param tableView : tableView to paste into
     * @return Returns error message if an error was found, else returns an empty string
     */
    QString pasteAll(QApplication* qtApp,
                     QTableView* tableView);
    /**
     * @brief Removes the Qt settings file from the file system.
     * Useful for resetting the application GUI in case it disappears
     * (i.e., in case some bad GUI settings are saved)
     * @return True if delete worked OK, else False
     */
    bool removeSettingsFile();
    /**
     * @brief Effectively does a file rename...but not quite. It copies the first
     * to the second and then removes the first.  I did this as the std:rename
     * function didn't work on Windows...and a system() mv command caused a shell
     * to pop up briefly over the UI.
     * @param fileIn : input file to rename
     * @param fileOut : the renamed output file
     * @return Returns 1 if rename was successful, -1 if not successful
     */
    int rename(QString fileIn,
               QString fileOut);
    /**
     * @brief Saves the data in the passed model to the appropriate directory and file
     * @param projectDir : directory where application files are written to
     * @param tabName : name of the tab whose data is to be written out; method uses it to determine what to name the vertical headers
     * @param rable : qtableview containing data to be written out
     */
    void saveModelToCSVFile(std::string projectDir,
                            std::string tabName,
                            QTableView* table);
    /**
     * @brief Selects all items in passed table view widget
     * @param tableView : table view widget in which to select all elements
     * @return Returns a message if an error found, else returns an empty string
     */
    QString selectAll(QTableView* tableView);
    /**
     * @brief Clears the passed textEdit widget if clear argument is True
     * @param textEdit : the textEdit widget to clear
     * @param clear : if True, the passed textEdit widget will be cleared, else it won't be cleared
     */
    void sendToOutputWindow(
            QTextEdit* textEdit,
            bool       clear);
    /**
     * @brief Sends text to output textEdit widget for display
     * @param textEdit : textEdit widget in which to place the passed text
     * @param label : text to display as bold text
     * @param content : text to display as normal text
     */
    void sendToOutputWindow(
            QTextEdit*  textEdit,
            std::string label,
            std::string content);
    /**
     * @brief Sends matrix to textEdit widget for display
     * @param textEdit : textEdit widget in which to place the passed widget
     * @param title : title of matrix to display
     * @param rowTitles : row labels of matrix to display
     * @param colTitles : column labels of matrix to display
     * @param outMatrix : output matrix of double type values to display
     * @param numDigits : number of characters to allocate for each matrix value
     * @param numDecimals : number of decimal places that each matrix value will have
     */
    void sendToOutputWindow(
            QTextEdit*               textEdit,
            std::string              title,
            std::vector<std::string> rowTitles,
            std::vector<std::string> colTitles,
            boost::numeric::ublas::matrix<double>& outMatrix,
            int                      numDigits,
            int                      numDecimals);
    /**
     * @brief Convenience function to reset an X axis (the original Qt call was deprecated)
     * @param chart : the chart on which the axis is to be placed
     * @param axisX : the new axis with which to replace the old one
     * @param series : the series to which the axis refers
     */
    void setAxisX(QChart*      chart,
                  QValueAxis*  axisX,
                  QLineSeries* series);
    void setAxisX(QChart*      chart,
                  QBarCategoryAxis*  axisX,
                  QStackedBarSeries* series);
    void setAxisX(QChart*            chart,
                  QBarCategoryAxis*  axisX,
                  QBarSeries*        series);
    /**
     * @brief Convenience function to reset a Y axis (the original Qt call was deprecated)
     * @param chart : the chart on which the axis is to be placed
     * @param axisX : the new axis with which to replace the old one
     * @param series : the series to which the axis refers
     */
    void setAxisY(QChart*      chart,
                  QValueAxis*  axisY,
                  QLineSeries* series);
    void setAxisY(QChart*      chart,
                  QValueAxis*  axisY,
                  QStackedBarSeries* series);
    void setAxisY(QChart*      chart,
                  QValueAxis*  axisY,
                  QBarSeries* series);
    /**
     * @brief Show the About dialog for the application
     * @param parent : parent needed so as to know how to position the popup
     * @param name : name of the application
     * @param operatingSystem : operating system application is running on
     * @param version : version of the application
     * @param specialAcknowledgement : special acknowledgement section
     * @param appMsg : main application message (created in calling routine);
     * lists all toolkits, versions, and links for more information
     */
    void showAboutWidget(
            QWidget* parent,
            const QString& name,
            const QString& operatingSystem,
            const QString& version,
            const QString& specialAcknowledgement,
            const QString& appMsg);
    /*
     * @brief Useful for changing .jpg,.png to .csv
     * @param filename : file name on which to modify the extension
     * @param newExt : the new file name extension
     * @param oldExts : list of old file name extensions to check for and change if found
     */
    void switchFileExtensions(QString& filename,
                              const QString& newExt,
                              const QStringList& oldExts);
} // end namespace

