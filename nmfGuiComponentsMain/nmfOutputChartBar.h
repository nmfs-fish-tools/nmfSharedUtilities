#ifndef NMFOUTPUTCHARTBAR_H
#define NMFOUTPUTCHARTBAR_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

#include <QChart>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

#include "nmfDatabase.h"
#include "nmfUtils.h"
#include "nmfLogger.h"

#include "nmfOutputChartBase.h"

QT_CHARTS_USE_NAMESPACE


class nmfOutputChartBar: public nmfOutputChartBase {

    std::map<std::string, void(*)(nmfDatabase* databasePtr,
                                  QChart* chart,
                                  std::string MSVPAName,
                                  std::string ForecastName,
                                  std::string ScenarioName,
                                  int Forecast_NYears,
                                  int Forecast_FirstYear,
                                  int FirstYear,
                                  int NumYears, // These are years selected from the gui
                                  boost::numeric::ublas::vector<int> Years,
                                  boost::numeric::ublas::vector<std::string> LegendNames,
                                  int Nage,
                                  int RecAge,
                                  std::string seasonStr,
                                  std::string selectedSpecies,
                                  double YMaxOverride)> FunctionMap;

    nmfLogger *logger;

  public:

    nmfOutputChartBar(nmfLogger *logger);

    void redrawChart(
            std::string caller,
            nmfDatabase *databasePtr,
            std::string theModelName,
            QChart *chart,
            std::map<std::string, QLabel*> &AllLabels,
            std::map<std::string, QComboBox* > &AllComboBoxes,
            std::map<std::string, QPushButton* > &AllButtons,
            std::map<std::string, QCheckBox* > &AllCheckBoxes,
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

    void getMaturityData(
            nmfDatabase *databasePtr,
            const std::string &modelType,
            const int &Nage, const int &NYears,
            const int &FirstYear, const int &LastYear,
            const std::string &SpeName,
            boost::numeric::ublas::matrix<double> &Maturity);

    void loadChartWithData(
            QChart *chart,
            QBarSeries *series,
            boost::numeric::ublas::matrix<double> &ChartData,
            QStringList &LegendNames,
            bool useMaxValue,
            double maxValue,
            bool inThousands);

    void setTitles(
            QChart *chart, QBarSeries *series, QStringList categories,
            std::string agePrefix,
            std::string species, std::string main, std::string sub,
            std::string xLabel, std::string yLabel,
            bool rearrangeTitle);

    void setComboBoxStates(
            const std::string &caller,
            std::map<std::string, QLabel*> &AllLabels,
            std::map<std::string, QComboBox*> &AllComboBoxes,
            std::map<std::string, QPushButton* > &AllButtons,
            std::map<std::string, QCheckBox* > &AllCheckBoxes,
            std::string &selectedDataType,
            std::string &selectedSpecies,
            std::string &selectedVariable,
            std::string &selectedByVariables,
            std::string &selectedSeason,
            std::string &selectedSpeciesAgeSizeClass);

    void getChartDataOfPredationMortalityByPredator(
          nmfDatabase *databasePtr,
          const std::string &MSVPAName,
          const std::string &type,
          const std::string &ForecastName,
          const std::string &ScenarioName,
          const std::string &SuitPreyBiomassTable,
          const std::string &outputTable,
          const std::string &ageStr,
          const std::string &seasonStr,
          const int &FirstYear,
          const int &NYears,
          const std::string &species,
          const int &speciesAge,
          QStringList &LegendNames,
          QStringList &categories,
          boost::numeric::ublas::matrix<double> &ChartData);

    void getChartData(
            nmfDatabase *databasePtr,
            const std::string &MSVPAName,
            const std::string &theModelName,
            const std::string &ForecastName,
            const std::string &ScenarioName,
            const int &Forecast_FirstYear,
            const int &Forecast_NYears,
            const int &NYears,
            const int &Nage,
            const int &FirstYear,
            const int &LastYear,
            const int &SpeAge,
            const std::string &fieldToSum,
            const std::string &selectedSpecies,
            const int &selectedSpeciesNum,
            const std::string &selectedVariable,
            const std::string &selectedByVariables,
            const std::string &ageStr,
            const std::string &seasonStr,
            const std::string &selectedPreyName,
            const double &scaleFactor,
            boost::numeric::ublas::matrix<double> &ChartData,
            boost::numeric::ublas::matrix<double> &GridData,
            QStringList &categories,
            QStringList &LegendNames,
            std::string &SizeUnitString,
            std::string &WtUnitString);

    void copyChartDataToGridData(
        boost::numeric::ublas::matrix<double> &ChartData,
        boost::numeric::ublas::matrix<double> &GridData);

    void queryDataAndLoadChart(
            std::string caller,
            nmfDatabase *databasePtr,
            boost::numeric::ublas::matrix<double> &ChartData,
            boost::numeric::ublas::matrix<double> &GridData,
            const std::string &theModelName,
            const std::string &ForecastName,
            const std::string &ScenarioName,
            const int &Forecast_FirstYear,
            const int &Forecast_NYears,
            const int &NYears,
            const int &NAge,
            const std::string &fieldToSum,
            const std::string &MSVPAName,
            const std::string &selectedSpecies,
            const int         &selectedSpeciesNum,
            const std::string &selectedVariable,
            const std::string &selectedByVariables,
            const std::string &ageStr,
            const std::string &recruitChar,
            const std::string &seasonStr,
            const int &season,
            const std::string &selectedPreyName,
            const int &FirstYear,
            const int &LastYear,
            const int &SpeAge,
            QChart *chart,
            const std::string &title,
            const std::string &titleSuffix,
            const bool &rearrangeTitle,
            const std::string &yLabel,
            const double &scaleFactor,
            const double YMax,
            QStringList &rowLabels,
            QStringList &colLabels);

    void querySpawningStockBiomassDataAndLoadChart(
            std::string caller,
            nmfDatabase *databasePtr,
            boost::numeric::ublas::matrix<double> &ChartData,
            boost::numeric::ublas::matrix<double> &GridData,
            const std::string &theModelName,
            const std::string &ForecastName,
            const std::string &ScenarioName,
            const int &Forecast_FirstYear,
            const int &Forecast_NYears,
            const int &NYears,
            const int &Nage,
            const std::string &fieldToSum,
            const std::string &MSVPAName,
            const std::string &selectedSpecies,
            const std::string &ageStr,
            const int &season,
            const int &FirstYear,
            const int &LastYear,
            QChart *chart,
            const std::string &title,
            const std::string &titleSuffix,
            const bool &rearrangeTitle,
            const std::string &yLabel,
            const bool useSeasonMaxValue,
            const double YMax,
            QStringList &rowLabels);

    bool isOfTypeAbundance(std::string selectedVariable);



//    static void Forecast_Growth_AverageWeight(
//            nmfDatabase* databasePtr,
//            QChart* chart,
//            std::string MSVPAName,
//            std::string ForecastName,
//            std::string ScenarioName,
//            int Forecast_NYears,
//            int Forecast_FirstYear,
//            int FirstYear,
//            int NumYears, // These are years selected from the gui
//            boost::numeric::ublas::vector<int> Years,
//            boost::numeric::ublas::vector<std::string> LegendNames,
//            int Nage,
//            int RecAge,
//            std::string seasonStr,
//            std::string selectedSpecies,
//            double YMaxOverride);




};

#endif // NMFOUTPUTCHARTBAR_H
