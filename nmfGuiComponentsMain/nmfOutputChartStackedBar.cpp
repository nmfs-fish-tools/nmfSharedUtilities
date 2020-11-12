
#include "nmfConstants.h"
#include "nmfOutputChartStackedBar.h"


nmfOutputChartStackedBar::nmfOutputChartStackedBar(nmfLogger *theLogger):
    nmfOutputChartBase()
{
    logger = theLogger;
}

nmfOutputChartStackedBar::~nmfOutputChartStackedBar()
{
}



void nmfOutputChartStackedBar::loadChartWithData(
        QChart *chart,
        QStackedBarSeries *series,
        QStringList &PreyList,
        boost::numeric::ublas::matrix<double> &ChartData)
{
    QBarSet *newSet;

    series = new QStackedBarSeries();
    for (unsigned int pred=0; pred<ChartData.size2(); ++pred) {
        newSet = new QBarSet(PreyList[pred]);
        for (unsigned int val=0; val<ChartData.size1(); ++val) {
            *newSet << ChartData(val,pred);
        }
        series->append(newSet);
    }
    chart->addSeries(series);
} // end loadChartWithData


void nmfOutputChartStackedBar::setTitles(
        QChart *chart,
        QStackedBarSeries *series, QStringList categories,
        std::string species, std::string byVariables,
        bool rearrangeTitle, std::string dataType,
        std::string xLabel, std::string yLabel,
        std::string suffix)
{
    std::stringstream ss;
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    if (rearrangeTitle) {
        ss << byVariables << " " << dataType << " " << species;
    } else {
        ss << byVariables << " " << species << " " << dataType << " By " << xLabel << suffix;
    }
    chart->setTitle(ss.str().c_str());

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QAbstractAxis *axisX = chart->axisX();
    QFont titleFont = axisX->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    axisX->setTitleFont(titleFont);
    axisX->setTitleText(xLabel.c_str());
    if (categories.count() > nmfConstants::NumCategoriesForVerticalNotation)
        axis->setLabelsAngle(-90);
    else
        axis->setLabelsAngle(0);

    // Rescale vertical axis....0 to 1 in increments of 0.2
    QValueAxis *newAxisY = new QValueAxis();
    newAxisY->setTitleFont(titleFont);
    newAxisY->setTitleText(yLabel.c_str());
    newAxisY->setRange(0,1.0);
    newAxisY->setTickCount(6);
    chart->setAxisY(newAxisY,series);

}


void nmfOutputChartStackedBar::setComboBoxStates(
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
        const std::string &selectedSpeciesAgeSizeClass)
{
    if (caller == "dataType") {
        //  Reset all sub-comboboxes to their first item
        AllComboBoxes["selectSpeciesCMB"]->blockSignals(true);
        AllComboBoxes["selectVariableCMB"]->blockSignals(true);
        AllComboBoxes["selectByVariablesCMB"]->blockSignals(true);
        AllComboBoxes["selectSeasonCMB"]->blockSignals(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(true);
        AllComboBoxes["selectSpeciesCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectVariableCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectByVariablesCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectSeasonCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectSpeciesCMB"]->blockSignals(false);
        AllComboBoxes["selectVariableCMB"]->blockSignals(false);
        AllComboBoxes["selectByVariablesCMB"]->blockSignals(false);
        AllComboBoxes["selectSeasonCMB"]->blockSignals(false);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(false);
        AllComboBoxes["selectPreySpeciesCMB"]->setEnabled(false);
        AllLabels["selectPreySpeciesLBL"]->setEnabled(false);
    } else if (caller == "variable") {
        AllComboBoxes["selectByVariablesCMB"]->blockSignals(true);
        AllComboBoxes["selectSeasonCMB"]->blockSignals(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(true);
        AllComboBoxes["selectByVariablesCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectSeasonCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectByVariablesCMB"]->blockSignals(false);
        AllComboBoxes["selectSeasonCMB"]->blockSignals(false);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(false);
    } else if (caller == "byVariables") {
        if (selectedByVariables == "Average") {
            AllComboBoxes["selectSeasonCMB"]->setEnabled(false);
            //AllCheckBoxes["selectSeasonCB"]->setEnabled(false);
            AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(false);
            AllButtons["flipLeftPB"]->setEnabled(false);
            AllButtons["flipRightPB"]->setEnabled(false);
        } else if ((selectedByVariables == "Average by Year") ||
                   (selectedByVariables == "Average by Season")) {
            AllComboBoxes["selectSeasonCMB"]->setEnabled(false);
            //AllCheckBoxes["selectSeasonCB"]->setEnabled(false);
            AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
            AllButtons["flipLeftPB"]->setEnabled(true);
            AllButtons["flipRightPB"]->setEnabled(true);
        } else if (selectedByVariables == "By Year and Season") {
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            //AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
            AllButtons["flipLeftPB"]->setEnabled(true);
            AllButtons["flipRightPB"]->setEnabled(true);
            AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        } else if (selectedByVariables == "Annual") {
            if ((selectedVariable == "Food Availability by Prey Type") ||
                (selectedVariable == "Food Availability by Prey Age"))
            {
                AllComboBoxes["selectSeasonCMB"]->setEnabled(false);
                AllCheckBoxes["selectSeasonCB"]->setEnabled(false);
            }
        } else if (selectedByVariables == "Seasonal") {
            if ((selectedVariable == "Food Availability by Prey Type") ||
                (selectedVariable == "Food Availability by Prey Age"))
            {
                AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
                AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
                AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
                AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
            }
        }
        AllComboBoxes["selectSeasonCMB"]->blockSignals(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(true);
        AllComboBoxes["selectSeasonCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectSeasonCMB"]->blockSignals(false);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(false);
    } else if (caller == "season") {
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(false);
    }
} // end setComboBoxStates



void nmfOutputChartStackedBar::redrawChart(
        std::string caller,
        nmfDatabase *databasePtr,
        std::string theModelName,
        QChart *chart,
        std::map<std::string, QLabel* >     &AllLabels,
        std::map<std::string, QComboBox* >  &AllComboBoxes,
        std::map<std::string, QPushButton*> &AllButtons,
        std::map<std::string, QCheckBox*>   &AllCheckBoxes,
        std::string MSVPAName,
        std::string ForecastName,
        std::string ScenarioName,
        int Forecast_FirstYear,
        int Forecast_NYears,
        int FirstYear,
        std::string selectedDataType,
        std::string selectedSpecies,
        int selectedSpeciesNum,
        std::string selectedVariable,
        std::string selectedByVariables,
        std::string selectedSeason,
        std::string selectedSpeciesAgeSizeClass,
        std::string selectedPreyName,
        std::string selectedYPRAnalysisType,
        std::vector<std::string> &selectedYears,
        std::string selectedFullyRecruitedAge,
        double YMaxForAllAges,
        bool hGridLine,
        bool vGridLine,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &ChartData,
        boost::numeric::ublas::matrix<double> &GridData)

{
    /* tbd later
    std::string caller = args.caller;
    nmfDatabase *databasePtr = &args.databasePtr;
    std::string theModelName = args.theModelName;
    QChart *chart = args.chart;
    AllLabels = args.AllLabels;
    AllComboBoxes = args.AllComboBoxes;
    AllButtons = args.AllButtons;
    AllCheckBoxes = args.AllCheckBoxes;
    MSVPAName = args.MSVPAName;
    ForecastName = args.ForecastName;
    ScenarioName = args.ScenarioName;
    Forecast_FirstYear = args.Forecast_FirstYear;
    std::string selectedDataType,
    std::string selectedSpecies,
    int selectedSpeciesNum,
    std::string selectedVariable,
    std::string selectedByVariables,
    std::string selectedSeason,
    std::string selectedSpeciesAgeSizeClass,
    std::string selectedPreyName,
    QStringList &rowLabels,
    QStringList &colLabels,
    GridData = args.GridData;
            */
    int m = 0;
    int numRecords=0;
    std::string queryStr,queryStr2,queryStr3;
    std::map<std::string, std::vector<std::string> > dataMap,dataMap2,dataMap3;
    std::vector<std::string> fields,fields2,fields3;
    int index;
    int NPrey;
    int Nage = 0;
    int NumRecords = 0;
    int LastYear=0;
    int NageOrSizeCategories;
    int sizeOffset;
    std::string PredAge;
    std::string ageSizePrefix = "";
    std::string xLabel = "";
    QStackedBarSeries *series;
    //boost::numeric::ublas::matrix<double> ChartData;
    int MSVPA_FirstYear=0;
    int MSVPA_LastYear=0;
    int MSVPA_NYears=0;
    int MSVPA_NSeasons=0;
    int Forecast_LastYear=Forecast_FirstYear+Forecast_NYears-1;
    int Forecast_NSeasons=0;
    int Forecast_Growth=0;
    int NPreds;
    int NOthPreds;
    std::string PredAgeStr = "";
    std::string PredName = selectedSpecies;
    int firstYear;
    int nYears;
    double AvgFA = 0.0;

    std::string seasonStr = selectedSeason;

    if (! seasonStr.empty()) {
        int SeasonVal = std::stoi(seasonStr.erase(0,seasonStr.find(" ")+1))-1;
        seasonStr = " AND Season = " + std::to_string(SeasonVal);
    }
    if (selectedByVariables == "Annual") {
        seasonStr = "";
    }

    setComboBoxStates(caller,AllLabels,AllComboBoxes,AllButtons,AllCheckBoxes,selectedDataType,
                      selectedSpecies,selectedVariable,selectedByVariables,
                      selectedSeason,selectedSpeciesAgeSizeClass);

    chart->removeAllSeries();
    if (MSVPAName == "")
        return; // Means that user hasn't yet chosen an MSVPA configuration

    colLabels.clear();

    logger->logMsg(nmfConstants::Normal,
                      theModelName + " " +
                      "nmfOutputChartStackedBar::redrawChart "+
                      selectedDataType + ", " +
                      selectedSpecies + ", " +
                      selectedVariable + ", " +
                      selectedByVariables + ", " +
                      selectedSeason + ", " +
                      selectedSpeciesAgeSizeClass);

    // Get initial MSVPA and Forecast data
    databasePtr->nmfGetMSVPAInitData(MSVPAName,
        MSVPA_FirstYear, MSVPA_LastYear, MSVPA_NYears, MSVPA_NSeasons);

    if (theModelName == "Forecast") {
        Forecast_NSeasons = MSVPA_NSeasons;
    }

    if (selectedVariable == "Food Availability by Prey Type") {
        std::string str1,str2;
        boost::numeric::ublas::vector<double> tempData;

        nmfUtils::split(selectedSpeciesAgeSizeClass," ",str1,str2);
        int PredAgeVal = (str1 == "Size") ? std::stoi(str2)-1 : std::stoi(str2);
        PredAgeStr = " AND PredAge = " + std::to_string(PredAgeVal);

        if (theModelName == "MSVPA") {

            firstYear = MSVPA_FirstYear;
            nYears = MSVPA_NYears;
            // First need to get a list of prey names and a count of prey for the predator.
            databasePtr->nmfQueryMsvpaPreyList(  selectedSpeciesAgeSizeClass,
                                                 selectedSpecies,
                                                 MSVPAName,
                                                 PredAgeStr,
                                                 NPrey,
                                                 colLabels,
                                                 false);
            queryStr = "SELECT Year, Sum(SuitPreyBiomass) AS TotFA FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND PredName = '" + PredName + "'" +
                         PredAgeStr + seasonStr +
                       " GROUP By Year";
            queryStr2 = "SELECT Year, PreyName, Sum(SuitPreyBiomass) AS SBM FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND PredName = '" + PredName + "'" +
                      PredAgeStr + seasonStr +
                    " GROUP By Year, PreyName";

        } else if (theModelName == "Forecast") { // By Prey Type..stacked bar

            firstYear = Forecast_FirstYear;
            nYears    = Forecast_NYears;

            // First need to get a list of prey names and a count of prey for the predator.
            databasePtr->nmfQueryForecastPreyList(MSVPAName, ForecastName, ScenarioName,
                                                  selectedSpecies, true, PredAgeStr, seasonStr, colLabels);
            NPrey = colLabels.size();

            queryStr = "SELECT Year, Sum(SuitPreyBiomass) AS TotFA FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND ForeName = '" + ForecastName + "'" +
                       " AND Scenario = '" + ScenarioName + "'" +
                       " AND PredName = '" + PredName + "'" +
                         PredAgeStr + seasonStr +
                       " GROUP By Year";
            queryStr2 = "SELECT Year, PreyName, Sum(SuitPreyBiomass) AS SBM FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND PredName = '" + PredName + "'" +
                      PredAgeStr + seasonStr +
                    " GROUP By Year, PreyName";
        }

        nmfUtils::initialize(tempData,  nYears);
        nmfUtils::initialize(ChartData, nYears, NPrey);
        nmfUtils::initialize(GridData,  nYears, NPrey);

        // Then want to get total suitable biomass and calculate average as previously
        fields = {"Year","TotFA"};
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < nYears; ++i) {
            tempData(i) = std::stod(dataMap["TotFA"][i]);
            AvgFA += tempData(i);
        }
        AvgFA /= (nYears + 1.0);

        // Then want to get suitable prey biomass by prey species
        fields2 = {"Year","PreyName","SBM"};
        dataMap2 = databasePtr->nmfQueryDatabase(queryStr2, fields2);
        m = 0;
        for (int i = 0; i < nYears; ++i) {
            rowLabels << QString::number(firstYear + i);
            for (int j = 0; j < NPrey; ++j) {
                if (dataMap2["PreyName"][m] == colLabels[j].toStdString()) {
                    ChartData(i,j) = std::stod(dataMap2["SBM"][m++]) / AvgFA;
                } else {
                    ChartData(i,j) = 0.0;
                }
            }
        }
        GridData = ChartData;

        //
        // Draw the chart now that you have all the data.
        //
        loadChartWithData(chart,series,colLabels,ChartData);
        setTitles(chart, series, rowLabels, selectedSpecies,
                  "Food Availability by Prey Type for ",
                  nmfConstants::RearrangeTitle,
                  selectedSpeciesAgeSizeClass,
                  "Year", "Relative Food Availability","");


    } else if (selectedVariable == "Food Availability by Prey Age") {


        //QStringList PreyAges;
        colLabels.clear();
        std::string PreyName = selectedPreyName;
        boost::numeric::ublas::vector<double> PreyAge;
        nmfUtils::initialize(PreyAge,  nmfConstants::MaxNumberAges);
        int NPreyAge;
        boost::numeric::ublas::vector<double> tempData;

        fields  = {"PreyAge"};
        fields2 = {"Year","TotFA"};
        fields3 = {"Year","PreyAge","SBM"};

        if (theModelName == "MSVPA") {

            nYears    = MSVPA_NYears;
            firstYear = MSVPA_FirstYear;

            // First get a list of ages for the prey species...might only be one if it's another prey...
            queryStr = "SELECT DISTINCT PreyAge FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                    PredAgeStr + seasonStr +
                    " AND PreyName = '" + PreyName + "'" +
                    " ORDER By PreyAge";

            // Then want to get total suitable biomass and calculate average as previously
            queryStr2 = "SELECT Year, Sum(SuitPreyBiomass) AS TotFA FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                    PredAgeStr + seasonStr +
                    " GROUP By Year";
            // Then want to get suitable prey biomass by prey age class
            queryStr3 = "SELECT Year, PreyAge, Sum(SuitPreyBiomass) AS SBM FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                    PredAgeStr + seasonStr +
                    " AND PreyName = '" + PreyName + "'" +
                    " GROUP By Year, PreyAge";

        } else if (theModelName == "Forecast") {
            nYears    = Forecast_NYears;
            firstYear = Forecast_FirstYear;

            // First get a list of ages for the prey species...might only be one if it's another prey...
            queryStr = "SELECT DISTINCT PreyAge FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                    PredAgeStr + seasonStr +
                    " AND PreyName = '" + PreyName + "'" +
                    " ORDER By PreyAge";
            // Then want to get total suitable biomass and calculate average as previously
            queryStr2 = "SELECT Year, Sum(SuitPreyBiomass) AS TotFA FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                    PredAgeStr + seasonStr +
                    " GROUP By Year";
            // Then want to get suitable prey biomass by prey age class
            queryStr3 = "SELECT Year, PreyAge, Sum(SuitPreyBiomass) AS SBM FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                    PredAgeStr + seasonStr +
                    " AND PreyName = '" + PreyName + "'" +
                    " GROUP By Year, PreyAge";
        }

        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        NPreyAge = dataMap["PreyAge"].size();

        nmfUtils::initialize(tempData, nYears);
        nmfUtils::initialize(ChartData, nYears, NPreyAge);
        nmfUtils::initialize(GridData,  nYears, NPreyAge);

        dataMap2 = databasePtr->nmfQueryDatabase(queryStr2, fields2);
        for (int i = 0; i < nYears; ++i) {
            tempData(i) = std::stod(dataMap2["TotFA"][i]);
            AvgFA += tempData(i);
        }
        AvgFA /= (nYears + 1.0);

        dataMap3 = databasePtr->nmfQueryDatabase(queryStr3, fields3);
        m = 0;
        for (int i = 0; i < nYears; ++i) {
            rowLabels << QString::number(firstYear + i);
            for (int j = 0; j < NPreyAge; ++j) {
                colLabels << QString::fromStdString("Age "+std::to_string(j));
                if (m < dataMap3["SBM"].size()) {
                    if (std::stoi(dataMap3["PreyAge"][j]) == j) {
                        ChartData(i,j) = std::stod(dataMap3["SBM"][m++]) / AvgFA;
                    }
                }
            } // end for j
        } // end for i
        GridData = ChartData;

        //
        // Draw the chart now that you have all the data.
        //
        loadChartWithData(chart,series,colLabels,ChartData);
        setTitles(chart, series, rowLabels, selectedSpecies,
                  "Availability of " + selectedPreyName + " for ",
                  nmfConstants::RearrangeTitle,
                  selectedSpeciesAgeSizeClass,
                  "Year", "Relative Food Availability","");
     }

    if (selectedByVariables == "Average") {

        if (theModelName == "MSVPA") {

            // Find number of Age groups
            fields = {"NumAges"};
            queryStr = "SELECT COUNT(DISTINCT(Age)) as NumAges from MSVPAprefs WHERE MSVPAname='" + MSVPAName +
                    "' and SpeName='" + selectedSpecies + "'";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            Nage = std::stoi(dataMap["NumAges"][0]);
            databasePtr->nmfQueryMsvpaPreyList(selectedSpeciesAgeSizeClass, selectedSpecies, MSVPAName,
                                               PredAge, NPrey, colLabels, false);

            nmfUtils::initialize(ChartData, Nage, NPrey);
            nmfUtils::initialize(GridData,  Nage, NPrey);

            // Calculate ChartData based upon passed in arguments
            fields = {"PredName","PredAge","PreyName","Year","Season","Diet"};
            for (int i=0; i<Nage; ++i) {
                for (int j=0; j<=NPrey-1; ++j) {
                    queryStr = "SELECT PredName,PredAge,PreyName,Year,Season,Sum(PropDiet) as Diet FROM MSVPASuitPreyBiomass WHERE MSVPAName ='" + MSVPAName + "'" +
                            " AND PredName = '" + selectedSpecies + "'" +
                            " AND PredAge = " + std::to_string(i) +
                            " AND PreyName = '" + colLabels[j].toStdString() + "'" +
                            " GROUP BY PredName,PredAge,PreyName,Year,Season";
                    dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
                    numRecords = dataMap["PredName"].size();
                    if (numRecords > 0) {
                        m = 0;
                        for (int k=0; k<MSVPA_NYears; ++k) {
                            for (int l=0; l<MSVPA_NSeasons; ++l) {
                                if (m < numRecords) {
                                    ChartData(i,j) += std::stod(dataMap["Diet"][m++]);
                                }
                            } // end for l
                        } // end for k
                        ChartData(i,j) /= (MSVPA_NSeasons*(MSVPA_NYears+1));
                    } // end if
                } // end for j
            } // end for i
            GridData = ChartData;

            // if current species is of Type=3 then the x axis is Size Class, else it's Age Class
            ageSizePrefix = "Age ";
            NageOrSizeCategories = Nage;
            sizeOffset = 0;
            fields = {"SpeName"};
            queryStr = "SELECT SpeName from MSVPAspecies WHERE MSVPAName='" + MSVPAName +
                    "' and SpeName='" + selectedSpecies + "' and Type=3";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            if (dataMap["SpeName"].size() > 0) {
                if (dataMap["SpeName"][0] == selectedSpecies) {
                    ageSizePrefix = "Size ";
                }
            }
            fields = {"SpeName","NumSizeCats"};
            queryStr = "SELECT SpeName,NumSizeCats from OtherPredSpecies WHERE SpeName='" + selectedSpecies + "'";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            if (dataMap["SpeName"].size() > 0) {
                if (dataMap["SpeName"][0] == selectedSpecies) {
                    sizeOffset = 1;
                    NageOrSizeCategories = std::stoi(dataMap["NumSizeCats"][0]);
                }
            }
            for (int ageOrSize=0; ageOrSize<NageOrSizeCategories; ++ageOrSize) {
                rowLabels << QString::fromStdString(ageSizePrefix) + QString::number(ageOrSize+sizeOffset);
            }
            xLabel = ageSizePrefix+"Class";


        } else if (theModelName == "Forecast") {

            boost::numeric::ublas::vector<std::string> OthPredList;
            boost::numeric::ublas::vector<int>         NPredAge;
            boost::numeric::ublas::vector<int>         NOthPredAge;
            boost::numeric::ublas::vector<std::string> PredList;
            boost::numeric::ublas::vector<int>         PredType;
            nmfUtils::initialize(PredType,    nmfConstants::MaxNumberSpecies);
            nmfUtils::initialize(PredList,    nmfConstants::MaxNumberSpecies);
            nmfUtils::initialize(OthPredList, nmfConstants::MaxNumberSpecies);
            nmfUtils::initialize(NPredAge,    nmfConstants::MaxNumberSpecies);
            nmfUtils::initialize(NOthPredAge, nmfConstants::MaxNumberSpecies);

            // Load predator name and age lists for use later
            fields = {"SpeName"};
            queryStr = "SELECT SpeName FROM MSVPAspecies WHERE MSVPAname = '" + MSVPAName + "' AND Type = 0";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            NPreds = dataMap["SpeName"].size();

            for (int i = 0; i < NPreds; ++i) {
                PredList(i) = dataMap["SpeName"][i];
                fields2 = {"MaxAge"};
                queryStr2 = "SELECT MaxAge FROM Species WHERE SpeName = '" + PredList(i) + "'";
                dataMap2 = databasePtr->nmfQueryDatabase(queryStr2, fields2);
                NPredAge(i) = std::stoi(dataMap2["MaxAge"][0]);
                PredType(i) = 0;
            } // end for i

            // Load Other Predator Names and Ages
            fields = {"SpeName"};
            queryStr = "SELECT SpeName FROM MSVPAspecies WHERE MSVPAname = '" + MSVPAName + "' AND Type = 3";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            NOthPreds = dataMap["SpeName"].size();

            for (int i = 0; i < NOthPreds; ++i) {
                OthPredList(i) = dataMap["SpeName"][i];
                PredType(i+NPreds) = 1;
                fields2 = {"NumSizeCats"};
                queryStr2 = "SELECT NumSizeCats FROM OtherPredSpecies WHERE SpeName = '" + OthPredList(i) + "'";
                dataMap2 = databasePtr->nmfQueryDatabase(queryStr2, fields2);
                NOthPredAge(i) = std::stoi(dataMap2["NumSizeCats"][0]);
            } // end for i


            int PredNum = selectedSpeciesNum;

            if (PredType(PredNum) == 0) {
                Nage = NPredAge(PredNum);
            } else {
                Nage = (NOthPredAge(PredNum - NPreds)) - 1;
            }

            std::string dummyStr = "";
            databasePtr->nmfQueryForecastPreyList(MSVPAName, ForecastName, ScenarioName,
                                                  selectedSpecies, false, dummyStr, "", colLabels);
            NPrey = colLabels.size();
            nmfUtils::initialize(ChartData, Nage+1, NPrey);
            nmfUtils::initialize(GridData, Nage+1, NPrey);

            for (int i = 0; i <= Nage; ++i) {
                for (int j = 0; j < NPrey; ++j) {
                    fields = {"PredName","PredAge","PreyName","Year","Season","Diet"};
                    queryStr = "SELECT PredName, PredAge, PreyName, Year, Season, Sum(PropDiet) as Diet FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                            " AND ForeName = '" + ForecastName + "'" +
                            " AND Scenario = '" + ScenarioName + "'" +
                            " AND PredName = '" + selectedSpecies + "'" +
                            " AND PredAge = "   + std::to_string(i) +
                            " AND PreyName = '" + colLabels[j].toStdString() + "'" +
                            " GROUP By PredName, PredAge, PreyName, Year, Season ";
                    dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
                    NumRecords = dataMap["Diet"].size();
                    if (NumRecords > 0) {
                        m = 0;
                        for (int k = 0; k<Forecast_NYears; ++k) { // RSK improve these types of loops, very inefficient
                            for (int l = 0; l < Forecast_NSeasons; ++l) {
                                if (m < NumRecords) {
                                    ChartData(i,j) += std::stod(dataMap["Diet"][m++]);
                                } else
                                    break; // end if
                            } // end for l
                        } // end for k
                        ChartData(i,j) /= (Forecast_NSeasons * (Forecast_NYears + 1));
                    } // end if
                } // end j
            } // end i
            GridData = ChartData;

            xLabel = (PredType(selectedSpeciesNum) == 0) ? "Age Class" : "Size Class";
            if (xLabel == "Age Class") {
                for (int ageOrSize=0; ageOrSize<=Nage; ++ageOrSize) {
                    rowLabels << "Age " + QString::number(ageOrSize);
                }
            } else {
                for (int ageOrSize=0; ageOrSize<=Nage; ++ageOrSize) {
                    rowLabels << "Size " + QString::number(ageOrSize);
                }
            }
        } // end if Forecast

        //
        // Draw the chart now that you have all the data.
        //
        loadChartWithData(chart,series,colLabels,ChartData);
        setTitles(chart,series,rowLabels,selectedSpecies,"Average",
                  nmfConstants::DontRearrangeTitle,
                  "Diet", xLabel, "Prop. Diet","");


    } else if (selectedByVariables == "Average by Year") {

        if (theModelName == "MSVPA") {

            databasePtr->nmfQueryMsvpaPreyList(selectedSpeciesAgeSizeClass, selectedSpecies, MSVPAName,
                                              PredAge, NPrey, colLabels, true);

            nmfUtils::initialize(ChartData, MSVPA_NYears, NPrey);
            nmfUtils::initialize(GridData,  MSVPA_NYears, NPrey);
            for (int i=0; i<NPrey; ++i) {
                fields = {"Year","Diet"};
                queryStr = "SELECT Year,Sum(PropDiet) as Diet FROM MSVPASuitPreyBiomass WHERE PredName = '" + selectedSpecies +
                        "' and PredAge = " + PredAge +
                        " and MSVPAName='" + MSVPAName +
                        "' and PreyName='" + colLabels[i].toStdString() +
                        "' GROUP BY Year";
                dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
                for (int j=0; j<MSVPA_NYears; ++j) {
                    ChartData(j,i) = std::stod(dataMap["Diet"][j]) / MSVPA_NSeasons;
                }
            }
            GridData = ChartData;

            FirstYear = MSVPA_FirstYear;
            LastYear  = MSVPA_LastYear;

        } else if (theModelName == "Forecast") {

            PredAge = selectedSpeciesAgeSizeClass.erase(0,selectedSpeciesAgeSizeClass.find(" "));
            PredAgeStr = " AND PredAge = " + PredAge;
            databasePtr->nmfQueryForecastPreyList(MSVPAName, ForecastName, ScenarioName,
                                                  selectedSpecies, true, PredAgeStr, "", colLabels);
            NPrey = colLabels.size();
            nmfUtils::initialize(ChartData, Forecast_NYears, NPrey);
            nmfUtils::initialize(GridData, Forecast_NYears, NPrey);

            for (int i = 0; i<NPrey; ++i) {
                fields = {"Year","PDiet"};
                queryStr = "SELECT Year, Sum(PropDiet) as PDiet FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                        " AND ForeName = '" + ForecastName + "'" +
                        " AND Scenario = '" + ScenarioName + "'" +
                        " AND PredName = '" + selectedSpecies + "'" +
                        " AND PredAge = " + PredAge +
                        " AND PreyName = '" + colLabels[i].toStdString() + "'" +
                        " GROUP By Year";
                dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

                for (int j = 0; j < Forecast_NYears; ++j) {
                    ChartData(j,i) = std::stod(dataMap["PDiet"][j]) / Forecast_NSeasons;
                } // end for j
            } // end for i
            GridData = ChartData;

            FirstYear = Forecast_FirstYear;
            LastYear  = Forecast_LastYear;

        } // end if Forecast

        for (int year=FirstYear; year<=LastYear; ++year) {
            rowLabels << QString::number(year);
        }

        //
        // Draw the chart now that you have all the data.
        //
        loadChartWithData(chart,series,colLabels,ChartData);
        setTitles(chart, series, rowLabels, selectedSpecies,
                  "Average "+selectedSpeciesAgeSizeClass,
                  nmfConstants::DontRearrangeTitle,
                  "Diet","Year","Prop. Diet","");


    } else if (selectedByVariables == "Average by Season") {

        if (theModelName == "MSVPA") {

            databasePtr->nmfQueryMsvpaPreyList(selectedSpeciesAgeSizeClass, selectedSpecies, MSVPAName,
                                               PredAge, NPrey, colLabels, true);

            nmfUtils::initialize(ChartData, MSVPA_NSeasons, NPrey);
            nmfUtils::initialize(GridData,  MSVPA_NSeasons, NPrey);

            for (int i=0; i<NPrey; ++i) {
                fields = {"Season","Diet"};
                queryStr = "SELECT Season,Sum(PropDiet) as Diet FROM MSVPASuitPreyBiomass WHERE PredName = '" + selectedSpecies +
                        "' and PredAge = " + PredAge +
                        "  and MSVPAName='" + MSVPAName +
                        "' and PreyName='" + colLabels[i].toStdString() +
                        "' GROUP BY Season";
                dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
                for (unsigned int j=0; j<dataMap["Season"].size(); ++j) {
                    index = std::stoi(dataMap["Season"][j]);
                    ChartData(index,i) = std::stod(dataMap["Diet"][j]) / (MSVPA_NYears+1);
                }
            }
            GridData = ChartData;

        } else if (theModelName == "Forecast") {

            PredAge = selectedSpeciesAgeSizeClass.erase(0,selectedSpeciesAgeSizeClass.find(" "));
            PredAgeStr = " AND PredAge = " + PredAge;
            databasePtr->nmfQueryForecastPreyList(MSVPAName, ForecastName, ScenarioName,
                                                  selectedSpecies, true, PredAgeStr,  "", colLabels);
            NPrey = colLabels.size();
            nmfUtils::initialize(ChartData, Forecast_NSeasons, NPrey);
            nmfUtils::initialize(GridData, Forecast_NSeasons, NPrey);

            for (int i = 0; i <NPrey; ++i) {
                fields = {"Season","Diet"};
                queryStr = "SELECT Season, Sum(PropDiet) as Diet FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                        " AND ForeName = '" + ForecastName + "'" +
                        " AND Scenario = '" + ScenarioName + "'" +
                        " AND PredName = '" + selectedSpecies + "'" +
                        " AND PredAge = "   + PredAge +
                        " AND PreyName = '" + colLabels[i].toStdString() + "'" +
                        " GROUP By Season";

                dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
                NumRecords = dataMap["Diet"].size();

                for (int j = 0; j < Forecast_NSeasons; ++j) {
                    if (j < NumRecords) {
                        if (std::stoi(dataMap["Season"][j]) == j) {
                            ChartData(j,i) = std::stod(dataMap["Diet"][j]) / (Forecast_NYears + 1);
                        } // end if
                    }
                } // end for j
            } // end for i
            GridData = ChartData;

        }

        for (int season=0; season<MSVPA_NSeasons; ++season) {
            rowLabels << "Season " + QString::number(season+1);
        }

        loadChartWithData(chart,series,colLabels,ChartData);
        setTitles(chart, series, rowLabels, selectedSpecies,
                  "Average "+selectedSpeciesAgeSizeClass,
                  nmfConstants::DontRearrangeTitle,
                  "Diet","Season","Prop. Diet","");


    } else if (selectedByVariables == "By Year and Season") {

        if (theModelName == "MSVPA") {

            selectedSeason.erase(0,selectedSeason.find(" "));
            int season = std::stoi(selectedSeason)-1;

            databasePtr->nmfQueryMsvpaPreyList(selectedSpeciesAgeSizeClass, selectedSpecies, MSVPAName,
                                               PredAge, NPrey, colLabels, true);

            nmfUtils::initialize(ChartData, MSVPA_NYears, NPrey);
            nmfUtils::initialize(GridData,  MSVPA_NYears, NPrey);

            fields = {"PredName","PredAge","Year","PreyName","Diet"};
            queryStr = "SELECT PredName,PredAge,Year,PreyName,Sum(PropDiet) as Diet FROM MSVPASuitPreyBiomass WHERE PredName = '" + selectedSpecies +
                    "' and PredAge = " + PredAge + " and Season = " + std::to_string(season) + " and MSVPAName='" + MSVPAName +
                    "' GROUP BY PredName,PredAge,Year,PreyName";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

            m = 0;
            for (int i=0; i<MSVPA_NYears; ++i) {
                for (int j=0; j<NPrey; ++j) {
                    if (dataMap["PreyName"][m] == colLabels[j].toStdString()) {
                        ChartData(i,j) = std::stod(dataMap["Diet"][m]);
                        ++m;
                    } else {
                        ChartData(i,j) = 0.0;
                    }
                }
            }
            GridData = ChartData;

            for (int year=MSVPA_FirstYear; year<=MSVPA_LastYear; ++year) {
                rowLabels << QString::number(year);
            }

        } else if (theModelName == "Forecast") {

            PredAge = selectedSpeciesAgeSizeClass.erase(0,selectedSpeciesAgeSizeClass.find(" "));
            selectedSeason = selectedSeason.erase(0,selectedSeason.find(" "));

            int selectedSeasonInt = std::stoi(selectedSeason)-1;
            PredAgeStr = " AND PredAge = " + PredAge;
            databasePtr->nmfQueryForecastPreyList(MSVPAName, ForecastName, ScenarioName,
                                                  selectedSpecies, true, PredAgeStr, "", colLabels);
            NPrey = colLabels.size();
            nmfUtils::initialize(ChartData, Forecast_NYears, NPrey);
            nmfUtils::initialize(GridData,  Forecast_NYears, NPrey);

            fields = {"PredName","PredAge","Year","PreyName","Diet"};
            queryStr = "SELECT PredName,PredAge,Year,PreyName,Sum(PropDiet) as Diet FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND ForeName = '" + ForecastName +  "'" +
                       " AND Scenario = '" + ScenarioName + "'" +
                       " AND PredName = '" + selectedSpecies + "'" +
                       " AND PredAge = " + PredAge +
                       " AND Season = " + std::to_string(selectedSeasonInt) +
                       " GROUP By PredName, PredAge, Year, PreyName";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            m = 0;
            NumRecords = dataMap["Diet"].size();
std::cout << "hereee: " << Forecast_NYears << ", " << NPrey << std::endl;

            for (int i = 0; i < Forecast_NYears; ++i) {
                for (int j = 0; j < NPrey; ++j) {
                    if (m < NumRecords) {
                        if (dataMap["PreyName"][m] == colLabels[j].toStdString()) {
                            ChartData(i,j) = std::stod(dataMap["Diet"][m++]);
                        } else {
                            ChartData(i,j) = 0.0;
                        }
                    }
                } // end for j
            } // end for i
            GridData = ChartData;

            for (int year=Forecast_FirstYear; year<=Forecast_LastYear; ++year) {
                rowLabels << QString::number(year);
            }
            selectedSeason = std::to_string(selectedSeasonInt+1);
        }

        loadChartWithData(chart,series,colLabels,ChartData);
        setTitles(chart, series, rowLabels, selectedSpecies,
                  "Average "+selectedSpeciesAgeSizeClass,
                  nmfConstants::DontRearrangeTitle,
                  "Diet","Year","Prop. Diet",
                  ": Season "+selectedSeason);

    }

    // Set grid line visibility
    chart->axisX()->setGridLineVisible(vGridLine);
    chart->axisY()->setGridLineVisible(hGridLine);

} // end redrawChart
