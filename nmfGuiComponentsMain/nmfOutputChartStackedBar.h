#ifndef NMFOUTPUTCHARTSTACKEDBAR_H
#define NMFOUTPUTCHARTSTACKEDBAR_H

#include <iostream>
#include <vector>
#include <map>

#include <QChart>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QComboBox>
#include <QPushButton>
#include <QStackedBarSeries>
#include <QValueAxis>
#include <QCheckBox>
#include <QLabel>

#include <boost/algorithm/string/replace.hpp>
#include <boost/format.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/filesystem.hpp>

#include "nmfDatabase.h"
#include "nmfUtils.h"
#include "nmfOutputChartBase.h"
#include "nmfLogger.h"

QT_CHARTS_USE_NAMESPACE


class nmfOutputChartStackedBar: public nmfOutputChartBase {

private:
    nmfLogger *logger;

public:

    nmfOutputChartStackedBar(nmfLogger *logger);
   ~nmfOutputChartStackedBar();

    void getForecastChartData(
          nmfDatabase *databasePtr,
          std::string &selectedByVariables,
          boost::numeric::ublas::matrix<double> &ChartData,
          int &NumAgeSizeGroups,
          QStringList &categories,
          std::string &xLabel,
          std::string &yLabel);

    void getMSVPAChartData(
          nmfDatabase *databasePtr,
          std::string &selectedByVariables,
          boost::numeric::ublas::matrix<double> &ChartData,
          int &NumAgeSizeGroups,
          QStringList &categories,
          std::string &xLabel,
          std::string &yLabel);

    void getMSVPAInitData(
            nmfDatabase *databasePtr,
            std::string &MSVPAName,
            int &MSVPA_FirstYear,
            int &MSVPA_LastYear,
            int &MSVPA_NYears,
            int &MSVPA_NSeasons);

//    void getForecastInitData(
//          nmfDatabase *databasePtr,
//          std::string &MSVPAName,
//          std::string &ForecastName,
//          int &Forecast_FirstYear,
//          int &Forecast_LastYear,
//          int &Forecast_NYears,
//          int &Forecast_Growth);

    void getMsvpaPreyList(
            nmfDatabase *databasePtr,
            std::string selectedSpeciesAgeSizeClass,
            std::string selectedSpecies,
            std::string MSVPAName,
            std::string &PredAge, int &NPrey,
            std::vector<std::string> &PreyList,
            bool includePredAge);

//    void getForecastPreyList(
//            nmfDatabase *databasePtr,
//            std::string &MSVPAName,
//            std::string &ForecastName,
//            std::string &ScenarioName,
//            std::string &Species,
//            bool includePredAgeClause,
//            std::string &PredAge,
//            std::vector<std::string> &PreyList);

    void redrawChart(
            std::string caller,
            nmfDatabase *databasePtr,
            std::string theModelName,
            QChart *chart,
            std::map<std::string, QLabel* > &AllLabels,
            std::map<std::string, QComboBox* > &AllComboBoxes,
            std::map<std::string, QPushButton*> &AllButtons,
            std::map<std::string, QCheckBox*> &AllCheckBoxes,
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
            boost::numeric::ublas::matrix<double> &ChartData,
            boost::numeric::ublas::matrix<double> &GridData);


    void loadChartWithData(
            QChart *chart,
            QStackedBarSeries *series,
            QStringList &PreyList,
            boost::numeric::ublas::matrix<double> &ChartData);

    void setTitles(
            QChart *chart, QStackedBarSeries *series, QStringList categories,
            std::string species, std::string byVariables,
            bool rearrangeTitle,
            std::string type,
            std::string xLabel,  std::string yLabel,
            std::string suffix);

    void setComboBoxStates(
            const std::string &caller,
            std::map<std::string, QLabel*> &AllLabels,
            std::map<std::string, QComboBox*> &AllComboBoxes,
            std::map<std::string, QPushButton*> &AllButtons,
            std::map<std::string, QCheckBox*> &AllCheckBoxes,
            const std::string &selectedDataType,
            const std::string &selectedSpecies,
            const std::string &selectedVariable,
            const std::string &selectedByVariables,
            const std::string &selectedSeason,
            const std::string &selectedSpeciesAgeSizeClass);
};

#endif // NMFOUTPUTCHARTSTACKEDBAR_H
