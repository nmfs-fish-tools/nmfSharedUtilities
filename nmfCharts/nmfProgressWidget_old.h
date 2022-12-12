
/**
 * @file nmfProgressWidget.h
 * @brief Definition for the Progress Chart dock widget
 * @date Feb 16, 2018
 *
 * This file contains the definition for the Progress Chart dock widget.
 *
 * @copyright
 * Public Domain Notice\n
 *
 * National Oceanic And Atmospheric Administration\n\n
 *
 * This software is a "United States Government Work" under the terms of the
 * United States Copyright Act.  It was written as part of the author's official
 * duties as a United States Government employee/contractor and thus cannot be copyrighted.
 * This software is freely available to the public for use. The National Oceanic
 * And Atmospheric Administration and the U.S. Government have not placed any
 * restriction on its use or reproduction.  Although all reasonable efforts have
 * been taken to ensure the accuracy and reliability of the software and data,
 * the National Oceanic And Atmospheric Administration and the U.S. Government
 * do not and cannot warrant the performance or results that may be obtained
 * by using this software or data. The National Oceanic And Atmospheric
 * Administration and the U.S. Government disclaim all warranties, express
 * or implied, including warranties of performance, merchantability or fitness
 * for any particular purpose.\n\n
 *
 * Please cite the author(s) in any work or product based on this material.
 */

#ifndef NMFPROGRESSWIDGET_H
#define NMFPROGRESSWIDGET_H

#include <QtCharts/QChartView>
#include <QtCharts/QChart>

#include <QAbstractSeries>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLineSeries>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QMessageBox>
#include <QScatterSeries>
#include <QSlider>
#include <QSpinBox>
#include <QSpacerItem>
#include <QTimer>
#include <QValueAxis>

#include "nmfUtils.h"
#include "nmfUtilsQt.h"
#include "nmfLogger.h"

QT_CHARTS_USE_NAMESPACE

/**
 * @brief The progress GUI that shows the user the status of the current run
 */
class nmfProgressWidget : public QObject
{

    Q_OBJECT

private:
    nmfLogger   *m_logger;
    QChartView  *m_chartView;
    QChart      *m_chart;

    void adjustYAxisLabelPrecision(QValueAxis *yAxis, double yMin, double yMax);

public:
    QGroupBox   *controlsGB;
    QHBoxLayout *hMainLayt;
    QVBoxLayout *vChartLayt;
    QHBoxLayout *hMinLayt;
    QHBoxLayout *hMaxLayt;
//  QHBoxLayout *hRangeLayt;
    QHBoxLayout *yRangeLayt;
    QHBoxLayout *statusLayt;
    QVBoxLayout *vGroupLayt;
    QHBoxLayout *buttonLayt;
    QHBoxLayout *timeLayt;
    QHBoxLayout *pointsLayt;
    QLabel      *statusLBL;
    QLabel      *timeLBL;
    QLabel      *tolLBL;
    QLabel      *pointLBL;
    QLabel      *rangeLBL;
    QLabel      *minLBL;
    QLabel      *maxLBL;
    QLabel      *yAxisLBL;
    QLabel      *runStatusLBL;
    QCheckBox   *markersCB;
    QCheckBox   *labelsCB;
    QCheckBox   *validPointsCB;
    QDoubleSpinBox *yMinSB;
    QDoubleSpinBox *yMaxSB;
    QSpinBox    *minSB;
    QSpinBox    *maxSB;
    QPushButton *clearPB;
    QPushButton *stopPB;
    QPushButton *rangeSetPB;
    QLineEdit   *timeLE;
    QLineEdit   *tolLE;
    QLineEdit   *runLE;
    QLineEdit   *subRunLE;

    bool        m_wasStopped;
    bool        m_yAxisAdjusted;
    int         m_maxNumGenerations;
    int         m_lastX;
    double      m_xMin;
    double      m_xMax;
    double      m_yMin;
    double      m_yMax;
    double      m_xInc;
    double      m_yInc;
    std::string m_RunType;
    std::string m_elapsedTime;
    QTimer*     m_timer;
    QDateTime   m_startTime;
    QString     m_mainTitle;
    QString     m_xTitle;
    QString     m_yTitle;

    /**
     * @brief nmfProgressWidget class constructor
     * @param timer : pointer to the current Qt timer
     * @param logger : pointer to the application logger
     * @param runType : the type of run (i.e., MSSPM Estimation, Forecast)
     * @param title : main title of the Progress plot
     * @param XAxisLabel : label for the x-axis of the Progress plot
     * @param YAxisLabel : label for the y-axis of the Progress plot
     * @param xMin : minimum x value
     * @param xMax : maximum x value
     * @param xInc : increment of x values
     * @param yMin : minimum y value
     * @param yMax : maximum y value
     * @param yInc : increment of y value
     */
    nmfProgressWidget(QTimer* timer,
                      nmfLogger* logger,
                      std::string runType,
                      QString title,
                      QString XAxisLabel,
                      QString YAxisLabel,
                      double xMin,
                      double xMax,
                      double xInc,
                      double yMin,
                      double yMax,
                      double yInc);
   ~nmfProgressWidget();

    /**
     * @brief Clear all series and all associated data from the current Progress chart
     */
    void clearChart();
    /**
     * @brief clears the output progress data file
     * @param filename : name of output progress data file
     */
    void clearChartData(std::string filename);
    /**
     * @brief Removes all series from progress chart data file
     */
    void clearChartOnly();
    /**
     * @brief Clears the run line edit boxes
     */
    void clearRunBoxes();
    /**
     * @brief Clears the time line edit box
     */
    void clearTime();
    /**
     * @brief Clears the tolerance line edit box
     */
    void clearTolerance();
    /**
     * @brief Gets the elapsed run time
     * @return Returns the elapsed run time string
     */
    std::string getElapsedTime();
    /**
     * @brief Gets the main title
     * @return Returns the main title bolded
     */
    QString getMainTitle();
    /**
     * @brief Gets the number of x-axis divisions
     * @return The number of x-axis divisions
     */
    int getNumXTicks();
    /**
     * @brief Gets the number of y-axis divisions
     * @return The number of y-axis divisions
     */
    int getNumYTicks();
    /**
     * @brief Gets the x-axis increment size
     * @return The x-axis increment size
     */
    double getXInc();
    /**
     * @brief Gets the range of the x-axis values
     * @return Pair of doubles for x-axis range
     */
    std::pair<double,double> getXRange();
    /**
     * @brief Gets the title for the x-axis
     * @return Returns the x-axis title
     */
    QString getXTitle();
    /**
     * @brief Gets the y-axis increment size
     * @return The y-axis increment size
     */
    double getYInc();
    /**
     * @brief Gets the range of the y-axis values
     * @return Pair of doubles for y-axis range
     */
    std::pair<double,double> getYRange();
    /**
     * @brief Gets the title for the y-axis
     * @return Returns the y-axis title
     */
    QString getYTitle();
    /**
     * @brief Hides the progress chart legend
     */
    void hideLegend();
    /**
     * @brief Boolean describing if the current run has been stopped
     * @return true if stopped, false if not stopped
     */
    bool isStopped();
    /**
     * @brief Reads the data file containing the points for the progress chart's data series
     * @param type : type of progress chart (i.e., MSSPM Estimation, Forecast)
     * @param inputFileName : name of progress chart file
     * @param inputLabelFileName : name of file containing run label
     * @param validPointsOnly : boolean describing if only valid points be used in progress charts,
     * i.e., points < nmfConstantsMSSPM::MaxValidProgressYValue
     */
    void readChartDataFile(std::string type,
                           std::string inputFileName,
                           std::string inputLabelFileName,
                           bool validPointsOnly);
    /**
     * @brief Boolean that describes if the user has checked the read valid points box (i.e., omit any 99999 values)
     * @return true if read valid points checkbox is checked, false if not
     */
    bool readValidPointsOnly();
    /**
     * @brief Sets the main title of the progress chart
     * @param mainTitle : main title of the progress chart
     */
    void setMainTitle(QString mainTitle);
    /**
     * @brief Sets the maximum number of generations for the Bees algorithm
     * @param newMaxNumGenerations : maximum number of generations
     */
    void setMaxNumGenerations(const int& newMaxNumGenerations);
    /**
     * @brief Sets the text of the run boxes with run information
     * @param run : current run number
     * @param subRun : current sub run number
     * @param numSubRuns : number of sub runs
     */
    void setRunBoxes(const int& run,
                     const int& subRun,
                     const int& numSubRuns);
    /**
     * @brief sets the tolerance qlineedit widget
     * @param tolStr : tolerance value as double; tolerance defined as difference between current fitness and previous
     */
    void setTolerance(double tol);
    /**
     * @brief Sets up the signal/slot connections for the class
     */
    void setupConnections();
    /**
     * @brief Sets the whats this message for the chart view
     * @param whatsThis : the message displayed when the user enables the whats this help for the chart
     */
    void setWhatsThis(QString whatsThis);
    /**
     * @brief Sets the scale of the x-axis
     * @param title : title of x-axis
     * @param xMin : minimum value of x-axis
     * @param xMax : maximum value of x-axis
     * @param xInc : x-axis increment
     */
    void setXAxisTitleScale(const QString& title,
                            const double&  xMin,
                            const double&  xMax,
                            const double&  xInc);
    /**
     * @brief Sets the increment of the x-axis
     * @param xInc : x-axis increment
     */
    void setXInc(const double& xInc);
    /**
     * @brief Sets the range of the x-axis
     * @param xMin : minimum value of x-axis
     * @param xMax : maximum value of x-axis
     */
    void setXRange(const double& xMin,const double& xMax);
    /**
     * @brief Sets the title of the x-axis
     * @param title : x-axis title
     */
    void setXTitle(const QString title);
    /**
     * @brief Sets the scale of the y-axis
     * @param title : title of y-axis
     * @param yMin : minimum value of y-axis
     * @param yMax : maximum value of y-axis
     * @param yInc : y-axis increment
     */
    void setYAxisTitleScale(const QString title,
                            const double  yMin,
                            const double  yMax,
                            const double  yInc);
    /**
     * @brief Sets the increment of the y-axis
     * @param yInc : y-axis increment
     */
    void setYInc(const double& yInc);
    /**
     * @brief Sets the range of the y-axis
     * @param yMin : minimum value of y-axis
     * @param yMax : maximum value of y-axis
     */
    void setYRange(const double& yMin,const double& yMax);
    /**
     * @brief Sets the title of the y-axis
     * @param title : y-axis title
     */
    void setYTitle(const QString title);
    /**
     * @brief Sets whether the Qt single step increment will be used or if a user defined increment will be used for the y-axis
     * @param state : if true, a single step increment will be used for the y-axis,
     * if false, the increment will be whatever the user has already defined
     */
    void shouldYRangeEditsStick(bool state);
    /**
     * @brief Shows the legend on the progress chart
     */
    void showLegend();
    /**
     * @brief Sets up the class for a run (i.e., initializes variables and any output files)
     */
    void startRun();
    /**
     * @brief Start the timer in accordance with the passed delay
     * @param delayMillisec : timer delay in milliseconds
     */
    void startTimer(int delayMillisec);
    /**
     * @brief Stops all MSSPM runs
     */
    void stopAllRuns();
    /**
     * @brief Stops the current active timer
     */
    void stopTimer();
    /**
     * @brief Updates the chart with the current class settings
     */
    void updateChart();
    /**
     * @brief Updates the chart status label which describes the number of runs completed out of total
     * @param inputLabelFileName : data file containing run information
     * @param overrideMsg : if not empty, this string will override the auto-generated message
     */
    void updateChartDataLabel(std::string inputLabelFileName,
                              std::string overrideMsg);
    /**
     * @brief Updates the run time string
     */
    void updateTime();
    /**
     * @brief Boolean signifying if the run was stopped by the user
     * @return true if run was stopped by the user, false otherwise
     */
    bool wasStopped();
    /**
     * @brief Writes to the Stop Run file that the run was stopped by the user
     */
    void writeToStopRunFile();

signals:
    /**
     * @brief Signal emitted when the user checks the "use valid points only" checkbox
     * @param checked : state of checkbox
     * @param clear : clears the chart prior to next update (currently always set to true)
     */
    void RedrawValidPointsOnly(bool checked, bool clear);
    /**
     * @brief Signal emitted when the user wishes to stop all runs
     */
    void StopAllRuns();
    /**
     * @brief Signal emitted to stop the current timer
     */
    void StopTheTimer();

public Q_SLOTS:
    /**
     * @brief Callback invoked when the user clicks the clear button
     */
    void callback_clearPB();
    /**
     * @brief Callback invoked when the user checks the Labels checkbox
     * @param state : state of the checkbox
     */
    void callback_labelsCB(int state);
//  void callback_lineHovered(QPointF point, bool state);
    /**
     * @brief Callback invoked when the user checks the show markers checkbox
     * @param state : true to show markers (i.e., dots), false to suppress markers
     */
    void callback_markersCB(int state);
    /**
     * @brief Callback invoked when the user wants to auto set the minimum and maximum scale values
     */
    void callback_rangeSetPB();
//  void callback_scatterSeriesHovered(QPointF point, bool state);
    /**
     * @brief Callback invoked when the user clicks the Stop Run button
     */
    void callback_stopPB();
    /**
     * @brief Callback invoked when the user checks the "Use Valid Points" checkbox for the Progress chart data series values
     * @param checked : true if checked, false if not checked
     */
    void callback_validPointsCB(bool checked);
    /**
     * @brief Callback invoked when the user wants to set the maximum x scale value
     * @param value : maximum x scale value
     */
    void callback_xMaxSB(int value);
    /**
     * @brief Callback invoked when the user wants to set the minimum scale value
     * @param value : minimum scale value
     */
    void callback_xMinSB(int value);
    /**
     * @brief Callback invoked when the user modifies the max y-value range spin box
     * @param value : max y-value range
     */
    void callback_yMaxSB(double value);
    /**
     * @brief Callback invoked when the user modifies the min y-value range spin box
     * @param value : min y-value range
     */
    void callback_yMinSB(double value);
};


#endif // NMFPROGRESSWIDGET_H
