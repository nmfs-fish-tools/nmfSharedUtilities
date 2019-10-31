/*
 * This is the output chart base class.
 *
 * Using polymorphism to define the output charts.  I chose
 * to do it this way so that I could use the same pointer
 * to point to different type of output charts (i.e. stacked bar,
 * regular bar, or line charts).
 *
 */
#ifndef NMFOUTPUTCHARTBASE_H
#define NMFOUTPUTCHARTBASE_H

#include <iostream>
#include <map>

#include "nmfDatabase.h"

#include <QChart>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

#include <boost/algorithm/string/replace.hpp>
#include <boost/format.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/filesystem.hpp>

QT_CHARTS_USE_NAMESPACE


struct ChartAndTableArgs {
    std::string caller;
    nmfDatabase *databasePtr;
    std::string theModelName;
    QChart *chart;
    std::map<std::string, QLabel*>      AllLabels;
    std::map<std::string, QComboBox*>   AllComboBoxes;
    std::map<std::string, QPushButton*> AllButtons;
    std::map<std::string, QCheckBox*>   AllCheckBoxes;
    std::string MSVPAName;
    std::string ForecastName;
    std::string ScenarioName;
    int Forecast_FirstYear;
    int Forecast_NYears;
    int FirstYear;
    std::string dataType;
    std::string species;
    int selectedSpeciesNum;
    std::string variable;
    std::string byVariables;
    std::string season;
    std::string speciesAgeSizeClass;
    std::string selectedPreyName;
    std::string selectedYPRAnalysisType;
    std::vector<std::string> selectedYears;
    std::string selectedFullyRecruitedAge;
    double YMaxForAllAges;
    QStringList rowLabels;   // was categories
    QStringList colLabels;   // was PreyList
    boost::numeric::ublas::matrix<double> ChartData;     // stores data for charts
    boost::numeric::ublas::matrix<double> GridData;
    QStringList rowLabels2;       // Necessary since some charts have 2 associated tables of data
    QStringList colLabels2;
    boost::numeric::ublas::matrix<double> GridData2;
};


class nmfOutputChartBase {

protected:
    bool hGridLine;

public:

    nmfOutputChartBase ();


    // Must be implemented in subclasses
    virtual void redrawChart (
            std::string caller,
            nmfDatabase *databasePtr,
            std::string theModelName,
            QChart *chart,
            std::map<std::string, QLabel*>      &AllLabels,
            std::map<std::string, QComboBox*>   &AllComboBoxes,
            std::map<std::string, QPushButton*> &AllButtons,
            std::map<std::string, QCheckBox*>   &AllCheckBoxes,
            std::string MSVPAName,
            std::string ForecastName,
            std::string ScenarioName,
            int Forecast_FirstYear,
            int Forecast_NYears,
            int FirstYear,
            std::string dataType,
            std::string species,
            int selectedSpeciesNum,
            std::string variable,
            std::string byVariables,
            std::string season,
            std::string speciesAgeSizeClass,
            std::string selectedPreyName,
            std::string selectedYPRAnalysisType,
            std::vector<std::string> &selectedYears,
            std::string selectedFullyRecruitedAge,
            double YMaxForAllAges,
            bool hGridLine,
            bool vGridLine,
            QStringList &categories,
            QStringList &PreyList,
            boost::numeric::ublas::matrix<double> &ChartData,     // stores data for charts
            boost::numeric::ublas::matrix<double> &GridData) = 0; // stores data for table shown in pane for the respective chart, not necessarily same as ChartData

};



#endif // NMFOUTPUTCHARTBASE_H
