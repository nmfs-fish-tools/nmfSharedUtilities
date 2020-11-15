
#include "nmfConstants.h"
#include "nmfOutputChartBar.h"


nmfOutputChartBar::nmfOutputChartBar(nmfLogger *theLogger) :
    nmfOutputChartBase()
{
    hGridLine = false;
    logger = theLogger;
//    FunctionMap["MSVPA_Growth_Average Weight"]    = MSVPA_Growth_AverageWeight;
//    FunctionMap["MSVPA_Growth_Average Size"]      = MSVPA_Growth_AverageSize;
//    FunctionMap["MSVPA_Growth_Weight at Age"]     = MSVPA_Growth_WeightAtAge;
//    FunctionMap["MSVPA_Growth_Size at Age"]       = MSVPA_Growth_SizeAtAge;

//    FunctionMap["Forecast_Growth_Average Weight"] = Forecast_Growth_AverageWeight;
//    FunctionMap["Forecast_Growth_Average Size"]   = Forecast_Growth_AverageSize;
//    FunctionMap["Forecast_Growth_Weight at Age"]  = Forecast_Growth_WeightAtAge;
//    FunctionMap["Forecast_Growth_Size at Age"]    = Forecast_Growth_SizeAtAge;
}

void nmfOutputChartBar::loadChartWithData(
        QChart *chart,
        QBarSeries *series,
        boost::numeric::ublas::matrix<double> &ChartData,
        QStringList &LegendNames,
        bool useMaxValue,
        double maxValue,
        bool inThousands)
{
    std::string msg;
    QBarSet *newSet;
    //double scaleFactor = (inThousands) ? 1000.0 : 1.0;

    series = new QBarSeries();

    if (LegendNames.size() < int(ChartData.size2())) {
        msg = "Error: nmfOutputChartBar::loadChartWithData array size problem. ";
        msg += "\n       LegendNames size of " + std::to_string(LegendNames.size()) + " < ";
        msg += "ChartData size of " + std::to_string(ChartData.size2());
        std::cout << msg << std::endl;
        return;
    }

    bool showLegend = (ChartData.size2() > 1);

    // There may be bar charts with multiple barsets so ChartData is 2 dimensional.
    for (unsigned int i=0; i<ChartData.size2(); ++i) {
        newSet = new QBarSet(LegendNames[i]);
        for (unsigned int val=0; val<ChartData.size1(); ++val) {
            *newSet << ChartData(val,i); //*scaleFactor;;
        }
        series->append(newSet);
    }
    chart->addSeries(series);

    chart->legend()->setVisible(showLegend);

    QValueAxis *axisY = new QValueAxis();
    // chart->setAxisY(axisY, series);
//    chart->addAxis(axisY, Qt::AlignBottom);
//    series->attachAxis(axisY);
    nmfUtilsQt::setAxisY(chart,axisY,series);

    if (maxValue > 0.0) {
        axisY->setRange(0, maxValue);
    } else {
        axisY->applyNiceNumbers();
    }

} // end loadChartWithData


void nmfOutputChartBar::setTitles(
        QChart *chart, QBarSeries *series,
        QStringList categories, std::string agePrefix,
        std::string species, std::string main, std::string sub,
        std::string xLabel, std::string yLabel,
        bool rearrangeTitle)
{
    if (agePrefix[agePrefix.size()-1] == '+')
        agePrefix += " ";
    else if (agePrefix != "")
        agePrefix += " - ";

    if (! sub.empty()) {
        sub = ": " + sub;
    }

    // Set main title font and text
    std::stringstream ss;
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    if (rearrangeTitle) {
        ss << main << " " << agePrefix << species << sub;
    } else {
        ss << agePrefix << species << " - " << main << " " << sub;
    }
    chart->setTitle(ss.str().c_str());

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    // chart->setAxisX(axis, series);
//    chart->addAxis(axis, Qt::AlignLeft);
//    series->attachAxis(axis);
    nmfUtilsQt::setAxisX(chart,axis,series);

    // Set font and labels of x axis
    QAbstractAxis *axisX = chart->axes(Qt::Horizontal).back();
    QFont titleFont = axisX->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    axisX->setTitleFont(titleFont);
    axisX->setTitleText(xLabel.c_str());
    if (categories.count() > nmfConstants::NumCategoriesForVerticalNotation)
        axis->setLabelsAngle(-90);
    else
        axis->setLabelsAngle(0);

    // Set font and labels of y axis
    QAbstractAxis *axisY = chart->axes(Qt::Vertical).back();
    axisY->setTitleFont(titleFont);
    axisY->setTitleText(yLabel.c_str());

} // end setTitles

void nmfOutputChartBar::setComboBoxStates(
        const std::string &caller,
        std::map<std::string, QLabel*>      &AllLabels,
        std::map<std::string, QComboBox*>   &AllComboBoxes,
        std::map<std::string, QPushButton*> &AllButtons,
        std::map<std::string, QCheckBox*>   &AllCheckBoxes,
        std::string &selectedDataType,
        std::string &selectedSpecies,
        std::string &selectedVariable,
        std::string &selectedByVariables,
        std::string &selectedSeason,
        std::string &selectedSpeciesAgeSizeClass)
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
            AllLabels["selectSeasonLBL"]->setEnabled(false);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(false);
            AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(false);
            AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(false);
            AllButtons["flipLeftPB"]->setEnabled(false);
            AllButtons["flipRightPB"]->setEnabled(false);
        } else if ((selectedByVariables == "Average by Year") ||
                   (selectedByVariables == "Average by Season")) {
            AllLabels["selectSeasonLBL"]->setEnabled(false);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(false);
            AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
            AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
            AllButtons["flipLeftPB"]->setEnabled(true);
            AllButtons["flipRightPB"]->setEnabled(true);
        } else if (selectedByVariables == "By Year and Season") {
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllButtons["flipLeftPB"]->setEnabled(true);
            AllButtons["flipRightPB"]->setEnabled(true);
            AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
            AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
        }
//        if ((selectedDataType == "Consumption Rates") &&
//            (selectedVariable == "Consumption by Prey Age")) {
//            AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(true);
//            AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setCurrentIndex(0);
//            AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(false);
//        }
        AllLabels["selectSeasonLBL"]->setEnabled(true);
        AllComboBoxes["selectSeasonCMB"]->blockSignals(true);
        AllComboBoxes["selectSeasonCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectSeasonCMB"]->blockSignals(false);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->blockSignals(false);
        AllComboBoxes["selectPreySpeciesCMB"]->blockSignals(true);
        AllComboBoxes["selectPreySpeciesCMB"]->setCurrentIndex(0);
        AllComboBoxes["selectPreySpeciesCMB"]->blockSignals(false);

    } else if (caller == "season") {


    }

    // Need to return the updated combo box values since some have changes
    // since the callback was called.
    selectedSpecies             = AllComboBoxes["selectSpeciesCMB"]->currentText().toStdString();
    selectedVariable            = AllComboBoxes["selectVariableCMB"]->currentText().toStdString();
    selectedByVariables         = AllComboBoxes["selectByVariablesCMB"]->currentText().toStdString();
    selectedSeason              = AllComboBoxes["selectSeasonCMB"]->currentText().toStdString();
    selectedSpeciesAgeSizeClass = AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->currentText().toStdString();

} // end setComboBoxStates


void nmfOutputChartBar::getMaturityData(
        nmfDatabase *databasePtr,
        const std::string &modelType,
        const int &Nage, const int &NYears,
        const int &FirstYear, const int &LastYear,
        const std::string &SpeName,
        boost::numeric::ublas::matrix<double> &Maturity)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int m = 0;

    fields = {"PMature"};
    if (modelType == "MSVPA") {
        queryStr = "SELECT PMature from SpeMaturity where SpeName='" + SpeName +
                "' and Year >= " + std::to_string(FirstYear) + " and Year <= " + std::to_string(LastYear) +
                " ORDER By Age,Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i=0; i<Nage; ++i) {
            for (int j=0; j<NYears; ++j) {
                Maturity(i,j) = std::stod(dataMap["PMature"][m++]);
            }
        }
    } else if (modelType == "Forecast") {
        queryStr = "SELECT PMature from SpeMaturity where SpeName='" + SpeName +
                "' and Year = " + std::to_string(FirstYear) +
                " ORDER By Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i=0; i<Nage; ++i) {
            for (int j=0; j<NYears; ++j) {
                    Maturity(i,j) = std::stod(dataMap["PMature"][m]);
            }
            ++m;
        }
    } else {
        std::cout << "Error: nmfOutputChartBar::getMaturityData:  Invalid modelType of " << modelType << std::endl;
        return;
    }

} // end getMaturityData

//void nmfOutputChartBar::setGridLines(
//        QChart *chart,
//        QString whichLines,
//        bool showGridLines)
//{
//    if (whichLines == "horizontal") {
//        hGridLine = showGridLines;
//        chart->axes(Qt::Vertical).back()->setGridLineVisible(showGridLines);
//    } else {
//        chart->axes(Qt::Horizontal).back()->setGridLineVisible(showGridLines);
//    }

//} // setHGridLines

bool nmfOutputChartBar::isOfTypeAbundance(std::string selectedVariable)
{
    return ((selectedVariable == "Total Abundance")  ||
            (selectedVariable == "Abundance by Age") ||
            (selectedVariable == "Recruit Abundance"));
}



void nmfOutputChartBar::getChartData(
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
        const int         &selectedSpeciesNum,
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
        std::string &WtUnitString)
{
    int m;
    int NumRecords = 0;
    double tmpSum;
    double tmpTotCat;
    std::string queryStr,queryStr2;
    std::map<std::string, std::vector<std::string> > dataMap,dataMap2;
    std::vector<std::string> fields,fields2;
    std::string field;
    std::size_t found;
    boost::numeric::ublas::matrix<double> CatchData;
    boost::numeric::ublas::matrix<double> tempData;
    QStringList PreyList;
    int firstYear = 0;
    int nYears    = 0;
    double AvgFA  = 0.0;

    // Get conversion factor
    double WtConversion = 1.0;
    double SizeConversion = 1.0;
    fields = {"WtUnits","SizeUnits"};
    queryStr = "SELECT WtUnits,SizeUnits FROM Species WHERE SpeName = '" + selectedSpecies + "'";
    dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
    if (dataMap["WtUnits"].size() != 0) {
        WtConversion   = std::stod(dataMap["WtUnits"][0]);
        SizeConversion = std::stod(dataMap["SizeUnits"][0]);
    } else {
        fields = {"WtUnits","SizeUnits"};
        queryStr = "SELECT WtUnits,SizeUnits FROM OtherPredSpecies WHERE SpeName = '" + selectedSpecies + "'";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        if (dataMap["WtUnits"].size() != 0) {
            WtConversion   = std::stod(dataMap["WtUnits"][0]);
            SizeConversion = std::stod(dataMap["SizeUnits"][0]);
        }
    }

    // RSK - use constants here...
    SizeUnitString = "";
    if (SizeConversion == 0.1)
        SizeUnitString = "Millimeters";
    else if (SizeConversion == 1.0)
        SizeUnitString = "Centimeters";
    else if (SizeConversion == 2.54)
        SizeUnitString = "Inches";
    WtUnitString = "";
    if (WtConversion == 0.001) // weights in grams..so resuls in 000 grams = kg
        WtUnitString = "Grams";
    else if (WtConversion == 1.0) // weight in kg so metric tons
        WtUnitString = "Kilograms";
    else if (WtConversion == (1.0/2.2)) // want weight in pounds so 000 pounds
        WtUnitString = "Pounds";

    if ((selectedVariable == "Total Biomass")       ||
        (selectedVariable == "Biomass by Age")      ||
        (selectedVariable == "Total Abundance")     ||
        (selectedVariable == "Abundance by Age")    ||
        (selectedVariable == "Recruit Abundance")   ||
        (selectedVariable == "Recruit Biomass")     ||
        (selectedVariable == "Fishing Mortality")   ||
        (selectedVariable == "Predation Mortality"))
    {

        //
        // RSK continue here and maybe introduce GridData and don't use ChartData for the table data
        // frmData2.frm line 2261
        //
        //

        if (theModelName == "MSVPA") {

            nmfUtils::initialize(ChartData, NYears, 1);
            fields = {"Year", "SumField"};

            // RSK  - make lstrip function in nmfUtils
            field = fieldToSum;
            found = field.find("Recruit");
            if (found != std::string::npos) { // then Recruit was found...
                field.erase(0,field.find("_")+1);
            }
            queryStr = "SELECT Year, Sum(" + field + ") as SumField from MSVPASeasBiomass WHERE MSVPAname='" + MSVPAName + "'"
                       " and SpeName='" + selectedSpecies + "' " +
                       ageStr + seasonStr + " GROUP BY YEAR ";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            for (int i=0; i<NYears; ++i) {
                categories << QString::number(FirstYear + i);
                ChartData(i,0) = std::stod(dataMap["SumField"][i])/1000.0; //scaleFactor;
            }
            //copyChartDataToGridData(ChartData,GridData);


        } else if (theModelName == "Forecast") {

            nmfUtils::initialize(ChartData, Forecast_NYears, 1);

            fields = {"Year","Biomass"};
            queryStr = "SELECT Year, Sum(InitBiomass) AS Biomass FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                            " AND ForeName = '" + ForecastName + "'" +
                            " AND Scenario = '" + ScenarioName + "'" +
                            " AND SpeName = '" + selectedSpecies + "' " +
                            ageStr + seasonStr +
                            "  GROUP By Year";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

            for (int i = 0; i<Forecast_NYears; ++i) {
                categories << QString::number(Forecast_FirstYear + i);
                ChartData(i,0) = std::stod(dataMap["Biomass"][i]) / 1000.0; //scaleFactor; //1000;
            }
            //copyChartDataToGridData(ChartData,GridData);
        }

    } else if (selectedVariable == "Average Recruited F") {

        if (theModelName == "MSVPA") {

            nmfUtils::initialize(CatchData, NYears, Nage);
            nmfUtils::initialize(tempData,  NYears, Nage);
            nmfUtils::initialize(ChartData, NYears, 1);

            // Need the catch data
            fields = {"MinCatAge","MaxCatAge"};
            queryStr = "SELECT MinCatAge,MaxCatAge from Species WHERE SpeName = '" + selectedSpecies + "' ";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            int FirstCatchAge = std::stoi(dataMap["MinCatAge"][0]);
            int LastCatchAge  = std::stoi(dataMap["MaxCatAge"][0]);
            fields = {"Catch"};
            queryStr = "SELECT Catch from SpeCatch WHERE SpeName = '" + selectedSpecies + "' " +
                    " AND (Year >= " + std::to_string(FirstYear) +
                    " AND  Year <= " + std::to_string(LastYear)  + ") " +
                    " ORDER by Year, Age";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            m = 0;
            for (int i=0; i<NYears; ++i) {
                for (int j=FirstCatchAge; j<=LastCatchAge; ++j) {
                    CatchData(i,j) = std::stod(dataMap["Catch"][m++]);
                }
            }

            // Get the species data
            fields = {"Year","Age","SumField"};
            queryStr = "SELECT Year, Age, Sum(SeasF) as SumField from MSVPASeasBiomass WHERE MSVPAname='" + MSVPAName + "' "                                                                                                                                     " and SpeName='" + selectedSpecies + "' " +
                    ageStr + seasonStr + " GROUP BY YEAR, Age ";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            m = 0;
            for (int i=0; i<NYears; ++i) {
                for (int j=SpeAge; j<Nage; ++j) {
                    tempData(i,j) = std::stod(dataMap["SumField"][m++]);
                }
            }

            // Calculate the ChartData values
            for (int i=0; i<NYears; ++i) {
                tmpSum = 0.0;
                tmpTotCat = 0.0;
                for (int j=SpeAge; j<Nage; ++j) {
                    tmpSum += tempData(i,j)*CatchData(i,j);
                    tmpTotCat += CatchData(i,j);
                }
                categories << QString::number(FirstYear + i);
                ChartData(i,0) = tmpSum / tmpTotCat;
            }

        } else if (theModelName == "Forecast") {

            // a little tricky..want it to be weighted by catch at age...
            nmfUtils::initialize(CatchData, NYears, Nage+1);
            nmfUtils::initialize(tempData,  NYears, Nage+1);
            nmfUtils::initialize(ChartData, Forecast_NYears, 1);

            // Go Ahead and get the annual catch data...fill with zeros where appropriate
            fields = {"Year","Catch"};
            queryStr = "SELECT Year, Sum(SeasCatch) AS Catch FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND SpeName = '" +  selectedSpecies + "'" +
                      seasonStr +
                    " GROUP By Year,Age";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            m = 0;
            for (int i = 0; i < Forecast_NYears; ++i) {
                for (int j=0; j<Nage; ++j) {
                    CatchData(i,j) = std::stod(dataMap["Catch"][m++]);

                }
            }
            fields = {"Year","Age","F"};
            queryStr = "SELECT Year,Age,Sum(SeasF) AS F FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND SpeName = '" +  selectedSpecies + "'" +
                      seasonStr + ageStr +
                    " GROUP By Year,Age";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            m = 0;
            for (int i = 0; i < Forecast_NYears; ++i) {
             for (int j = SpeAge; j < Nage; ++j) {
              tempData(i,j) = nmfUtils::round(std::stod(dataMap["F"][m++]),3);
             }
            }

            for (int i = 0; i < Forecast_NYears; ++i) {
                tmpSum = 0.0;
                tmpTotCat = 0.0;
                for (int j = SpeAge; j <= Nage; ++j) {
                    tmpSum += (tempData(i,j) * CatchData(i,j));
                    tmpTotCat += CatchData(i,j);
                }
                categories << QString::number(Forecast_FirstYear + i);
                if (tmpTotCat == 0.0) {
                    std::cout << "Error: Got denominator tmpTotCat of 0. Setting ChartData(" << i << ",0) to 0." << std::endl;
                    ChartData(i,0) = 0.0;
                } else {
                    ChartData(i,0) = tmpSum / tmpTotCat;
                }
            }
        }

    } else if (selectedVariable == "Predation Mortality by Predator") {

        if (theModelName == "MSVPA") {

            getChartDataOfPredationMortalityByPredator(
                  databasePtr,
                   MSVPAName,
                  "MSVPA",
                  "",
                  "",
                  "MSVPASuitPreyBiomass",
                  "MSVPASeasBiomass",
                   ageStr,
                   seasonStr,
                   FirstYear,
                   NYears,
                   selectedSpecies,
                   SpeAge,
                   LegendNames,
                   categories,
                   ChartData);


        } else if (theModelName == "Forecast") { // M2 by predator species.

            getChartDataOfPredationMortalityByPredator(
                  databasePtr,
                   MSVPAName,
                  "Forecast",
                   ForecastName,
                   ScenarioName,
                  "ForeSuitPreyBiomass",
                  "ForeOutput",
                   ageStr,
                   seasonStr,
                   Forecast_FirstYear,
                   Forecast_NYears,
                   selectedSpecies,
                   SpeAge,
                   LegendNames,
                   categories,
                   ChartData);

        } // end if (theModelName == "Forecast")

    } else if (selectedVariable == "Total Fishery Catch (Numbers)") {

        nmfUtils::initialize(ChartData, Forecast_NYears, 1);

        fields = {"Year","Catch"};
        queryStr = "SELECT Year, Sum(SeasCatch) AS Catch FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '" +  selectedSpecies + "'" +
                  seasonStr +
                " GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < Forecast_NYears; ++i) {
            categories << QString::number(Forecast_FirstYear + i);
            ChartData(i,0) = std::stod(dataMap["Catch"][i]) / scaleFactor;
        }

    } else if (selectedVariable == "Fishery Catch at Age (Numbers)") {  // Total annual catch

        nmfUtils::initialize(ChartData, Forecast_NYears, 1);

        fields = {"Year","Catch"};
        queryStr = "SELECT Year, Sum(SeasCatch) AS Catch FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '" +  selectedSpecies + "'" +
                  seasonStr + ageStr +
                " GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < Forecast_NYears; ++i) {
            categories << QString::number(Forecast_FirstYear + i);
            ChartData(i,0) = std::stod(dataMap["Catch"][i]) / scaleFactor;
        }

    } else if (selectedVariable == "Total Fishery Yield (Biomass)") {

        nmfUtils::initialize(ChartData, Forecast_NYears, 1);

        fields = {"Year","Yield"};
        queryStr = "SELECT Year, Sum(SeasYield) AS Yield FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '" +  selectedSpecies + "'" +
                  seasonStr +
                " GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < Forecast_NYears; ++i) {
            categories << QString::number(Forecast_FirstYear + i);
            ChartData(i,0) = std::stod(dataMap["Yield"][i]) / scaleFactor;
        }

    } else if (selectedVariable == "Fishery Yield at Age (Biomass)") {

        nmfUtils::initialize(ChartData, Forecast_NYears, 1);

        fields = {"Year","Yield"};
        queryStr = "SELECT Year, Sum(SeasYield) AS Yield FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '" +  selectedSpecies + "'" +
                  seasonStr + ageStr +
                " GROUP By Year";
        //std::cout << queryStr << std::endl;
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < Forecast_NYears; ++i) {
            categories << QString::number(Forecast_FirstYear + i);
            ChartData(i,0) = std::stod(dataMap["Yield"][i]) / scaleFactor;
        }

    } else if (selectedVariable == "Fishery Mortality Rate at Age") {

        nmfUtils::initialize(ChartData, Forecast_NYears, 1);

        fields = {"Year","F"};
        queryStr = "SELECT Year, Sum(SeasF) AS F FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '" +  selectedSpecies + "'" +
                  seasonStr + ageStr +
                " GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < Forecast_NYears; ++i) {
            categories << QString::number(Forecast_FirstYear + i);
            ChartData(i,0) = std::stod(dataMap["F"][i]) / scaleFactor;
            //std::cout << "ChartData(" << i << ",0): " << ChartData(i,0) << std::endl;
        }

    } else if (selectedVariable == "Predation Mortality Rate") {

        nmfUtils::initialize(ChartData, Forecast_NYears, 1);

        fields = {"Year","M2"};
        queryStr = "SELECT Year, Sum(SeasM2) AS M2 FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '" +  selectedSpecies + "'" +
                  seasonStr + ageStr +
                " GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < Forecast_NYears; ++i) {
            categories << QString::number(Forecast_FirstYear + i);
            ChartData(i,0) = ((dataMap["M2"][i]).empty()) ? 0 : nmfUtils::round(std::stod(dataMap["M2"][i]), 3);
        }

    } else if (selectedVariable == "Total Biomass Consumed") {

        fields = {"Year","TotCons"};

        if (theModelName == "MSVPA") {
            nYears = LastYear-FirstYear+1;
            firstYear = FirstYear;
            queryStr = "SELECT Year, Sum(BMConsumed) AS TotCons FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND PredName = '" +  selectedSpecies + "'" +
                      seasonStr + ageStr +
                    " GROUP By Year";
        } else if (theModelName == "Forecast") {
            nYears = Forecast_NYears;
            firstYear = Forecast_FirstYear;
            queryStr = "SELECT Year, Sum(TotalBMConsumed) AS TotCons FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND SpeName = '" +  selectedSpecies + "'" +
                      seasonStr + ageStr +
                    " GROUP By Year";
        }
        nmfUtils::initialize(ChartData, nYears, 1);
std::cout << "qf1: " << queryStr << std::endl;
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < nYears; ++i) {
            categories << QString::number(firstYear + i);
            ChartData(i,0) = std::stod(dataMap["TotCons"][i])/1000.0; // Replace 1000 with a variable
        }

    } else if (selectedVariable == "Consumption by Predator Age") {

        fields = {"Year","TotCons"};

        if (theModelName == "MSVPA") {
            nYears    = LastYear-FirstYear+1;
            firstYear = FirstYear;
            queryStr = "SELECT Year, Sum(BMConsumed) AS TotCons FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND PredName = '" +  selectedSpecies + "'" +
                      seasonStr +
                    " AND PredAge = " + std::to_string(SpeAge) +
                    " GROUP By Year";

        } else if (theModelName == "Forecast") {
            nYears    = Forecast_NYears;
            firstYear = Forecast_FirstYear;
            queryStr = "SELECT Year, Sum(TotalBMConsumed) AS TotCons FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND SpeName = '" +  selectedSpecies + "'" +
                      seasonStr + ageStr +
                    " GROUP By Year";
std::cout << "qf2: " << queryStr << std::endl;
        }
        nmfUtils::initialize(ChartData, nYears, 1);

        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        for (int i = 0; i < nYears; ++i) {
            categories << QString::number(firstYear + i);
            ChartData(i,0) = std::stod(dataMap["TotCons"][i])/1000.0; // Replace 1000 with a variable
        }

    } else if ((selectedVariable == "Consumption by Prey Type") ||
               (selectedVariable == "Cons. by Pred. Age and Prey Type")) {
        int NPrey;
        bool inclPredAge = (selectedVariable == "Cons. by Pred. Age and Prey Type");
        std::string predAgeStr = inclPredAge ? ageStr : "";

        fields = {"Year","PreyName","TotCons"};

        if (theModelName == "MSVPA") {
            nYears    = LastYear-FirstYear+1;
            firstYear = FirstYear;

            // First need to get a list of prey names and a count of prey for the predator.
            databasePtr->nmfQueryMsvpaPreyList( "", selectedSpecies, MSVPAName, predAgeStr,
                                                NPrey, PreyList, false);
            queryStr = "SELECT Year, PreyName, Sum(BMConsumed) as TotCons FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND PredName = '" + selectedSpecies + "'" +
                          seasonStr + predAgeStr +
                       " GROUP BY Year, PreyName";

        } else if (theModelName == "Forecast") {
            nYears    = Forecast_NYears;
            firstYear = Forecast_FirstYear;

            // First need to get a list of prey names and a count of prey for the predator.
            databasePtr->nmfQueryForecastPreyList(MSVPAName, ForecastName, ScenarioName,
                                                  selectedSpecies, inclPredAge, ageStr, "", PreyList);
            queryStr = "SELECT Year, PreyName, Sum(BMConsumed) as TotCons FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND ForeName = '" + ForecastName + "'" +
                       " AND Scenario = '" + ScenarioName + "'" +
                       " AND PredName = '" + selectedSpecies + "'" +
                          seasonStr + predAgeStr +
                       " GROUP BY Year, PreyName";
        }

        NPrey = PreyList.size();
        nmfUtils::initialize(ChartData, nYears, NPrey);
        //nmfUtils::initialize(GridData,  nYears, NPrey);
        LegendNames.clear();
        for (int i=0; i<NPrey; ++i) {
            LegendNames << PreyList[i];
        }
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        m = 0;
        for (int i = 0; i< nYears; ++i) {
            categories << QString::number(firstYear + i);
            for (int j = 0; j < NPrey; ++j) {
                // Need this check since there may be data for some missing species
                if (dataMap["PreyName"][m] == PreyList[j].toStdString()) {
                    ChartData(i,j) = std::stod(dataMap["TotCons"][m++]) / 1000.0;
                }
                //GridData(i,j) = ChartData(i,j);
            }
        }


    } else if (selectedVariable == "Consumption by Prey Age") {
        int NPreyAge;
        std::string newAgeStr = ageStr;
        int PredAgeVal = (! newAgeStr.empty()) ? std::stoi(newAgeStr.erase(0,newAgeStr.find("=")+1)) : 0;

        boost::numeric::ublas::vector<int> PreyAge;
        nmfUtils::initialize(PreyAge, nmfConstants::MaxNumberAges);

        // First get a list of ages for the prey species...might only be one if it's another prey...
        fields = {"PreyAge"};
        fields2 = {"Year","PreyAge","TotCons"};

        if (theModelName == "MSVPA") {
            nYears    = LastYear-FirstYear+1;
            firstYear = FirstYear;
            queryStr = "SELECT DISTINCT(PreyAge) FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                    " AND PredAge = "   + std::to_string(PredAgeVal) +
                    " AND PreyName = '" + selectedPreyName + "'" + seasonStr +
                    " ORDER BY PreyAge";
            // Then select the biomass consumed of that prey age by predator age across the byvar
            queryStr2 = "SELECT Year, PreyAge, Sum(BMConsumed) as TotCons FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                    " AND PredAge = "   + std::to_string(PredAgeVal) +
                    " AND PreyName = '" + selectedPreyName + "'" + seasonStr +
                    " GROUP By Year, PreyAge";
        } else if (theModelName == "Forecast") {
            nYears    = Forecast_NYears;
            firstYear = Forecast_FirstYear;

            queryStr = "SELECT DISTINCT(PreyAge) FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                    " AND PredAge = "   + std::to_string(PredAgeVal) +
                    " AND PreyName = '" + selectedPreyName + "'" + seasonStr +
                    " ORDER BY PreyAge";
            // Then select the biomass consumed of that prey age by predator age across the byvar
            queryStr2 = "SELECT Year, PreyAge, Sum(BMConsumed) as TotCons FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                    " AND PredAge = "   + std::to_string(PredAgeVal) +
                    " AND PreyName = '" + selectedPreyName + "'" + seasonStr +
                    " GROUP By Year, PreyAge";
        }

        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        NPreyAge = dataMap["PreyAge"].size();
        LegendNames.clear();
        for (int i = 0; i < NPreyAge; ++i) {
            PreyAge(i) = std::stoi(dataMap["PreyAge"][i]);
            LegendNames << QString::fromStdString("Age " + std::to_string(PreyAge(i)));
        }

        nmfUtils::initialize(ChartData, nYears, NPreyAge);
        nmfUtils::initialize(GridData, nYears, NPreyAge);
        dataMap2 = databasePtr->nmfQueryDatabase(queryStr2, fields2);
        m = 0;
        NumRecords = dataMap2["PreyAge"].size();
        for (int i = 0; i < nYears; ++i) {
            categories << QString::number(firstYear + i);
            for (int j = 0; j < NPreyAge; ++j) {
                if (m < NumRecords) {
                    if (std::stod(dataMap2["PreyAge"][m]) == PreyAge(j)) {
                        ChartData(i,j) = std::stod(dataMap2["TotCons"][m]) / 1000.0; // RSK - use a constant here
                        GridData(i,j) = ChartData(i,j);
                        ++m;
                    }
                } else {
                    break;
                }
            }
        }

    } else if (selectedVariable == "Wt. Specific Consumption") {
       // weight specific consumption rates..load params and calculate for a particular season
       // include age class in the grid matrix, but plot for a single age.

        boost::numeric::ublas::matrix<double> AgeStomCont;
        boost::numeric::ublas::matrix<double> AgeConsump;
        boost::numeric::ublas::vector<double> SeasTemp;
        boost::numeric::ublas::vector<int>    PredType;

        if (theModelName == "MSVPA") {
            nYears    = LastYear-FirstYear+1;
            firstYear = FirstYear;

            queryStr2 = "SELECT Year, Age, StomCont FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND SpeName = '"  + selectedSpecies +  "'" +
                         seasonStr +
                       " ORDER By Year, Age";

        } else if (theModelName == "Forecast") {
            nYears    = Forecast_NYears;
            firstYear = Forecast_FirstYear;

            queryStr2 = "SELECT Year, Age, StomCont FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND ForeName = '" + ForecastName + "'" +
                       " AND Scenario = '" + ScenarioName + "'" +
                       " AND SpeName = '"  + selectedSpecies +  "'" +
                         seasonStr +
                       " ORDER By Year, Age";
        }

        nmfUtils::initialize(AgeStomCont, nYears, Nage+1);
        nmfUtils::initialize(AgeConsump,  Nage+1,  2);
        nmfUtils::initialize(SeasTemp,    nYears);
        nmfUtils::initialize(ChartData,   nYears, 1);
        nmfUtils::initialize(GridData,    nYears, Nage);
        nmfUtils::initialize(PredType,    nmfConstants::MaxNumberSpecies);

        // Load predator name and age lists for use later
        fields = {"SpeName"};
        queryStr = "SELECT SpeName FROM MSVPAspecies WHERE MSVPAname = '" + MSVPAName + "' AND Type = 0";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        int NPreds = dataMap["SpeName"].size();
        for (int i = 0; i < NPreds; ++i) {
            PredType(i) = 0;
        } // end for i

        // Load Other Predator Names and Ages
        fields = {"SpeName"};
        queryStr = "SELECT SpeName FROM MSVPAspecies WHERE MSVPAname = '" + MSVPAName + "' AND Type = 3";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        int NOthPreds = dataMap["SpeName"].size();
        for (int i = 0; i < NOthPreds; ++i) {
            PredType(i+NPreds) = 1;
        } // end for i

        fields2 = {"Year","Age","StomCont"};
        dataMap2 = databasePtr->nmfQueryDatabase(queryStr2, fields2);
        m = 0;
        for (int i = 0; i < nYears; ++i) {
            for (int j = 0; j < Nage; ++j) {
                AgeStomCont(i,j) = std::stod(dataMap2["StomCont"][m++]);
            }
        }
        if (PredType(selectedSpeciesNum) == 0) {
            fields = {"Age","EvacAlpha","EvacBeta"};
            queryStr = "SELECT Age, EvacAlpha, EvacBeta FROM MSVPASizePref WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND SpeName = '" + selectedSpecies + "'" +
                       " ORDER By Age";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            for (int j = 0; j < Nage; ++j) {
                AgeConsump(j,0) = std::stod(dataMap["EvacAlpha"][j]);
                AgeConsump(j,1) = std::stod(dataMap["EvacBeta"][j]);
            }
        } else {
            fields = {"SizeCat","ConsAlpha","ConsBeta"};
            queryStr = "SELECT SizeCat, ConsAlpha, ConsBeta FROM OthPredSizeData WHERE SpeName = '" + selectedSpecies + "'" +
                       " ORDER BY SizeCat";
            dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
            for (int j = 0; j < Nage; ++j) {
                AgeConsump(j, 0) = std::stod(dataMap["ConsAlpha"][j]);
                AgeConsump(j, 1) = std::stod(dataMap["ConsBeta"][j]);
            }
        }

        fields = {"Year","Season","Value"};
        std::string newSeasonStr = seasonStr;
        int SeasonVal = (! newSeasonStr.empty()) ? std::stoi(newSeasonStr.erase(0,newSeasonStr.find("=")+1))+1 : 0;
        queryStr = "SELECT Year, Season, Value FROM MSVPASeasInfo WHERE MSVPAname = '" + MSVPAName + "'" +
                   " AND Season = " + std::to_string(SeasonVal) +
                   " AND Variable = 'SeasTemp' AND Year = " + std::to_string(FirstYear);
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        std::string newAgeStr = ageStr;
        int PredAgeVal = (! newAgeStr.empty()) ? std::stoi(newAgeStr.erase(0,newAgeStr.find("=")+1)) : 0;

        for (int i = 0; i < nYears; ++i) {
            categories << QString::number(firstYear + i);
            SeasTemp(i) = std::stod(dataMap["Value"][0]);
            ChartData(i,0) = AgeStomCont(i,PredAgeVal) * 24.0 * AgeConsump(PredAgeVal,0) *
                             std::exp( AgeConsump(PredAgeVal,1) * SeasTemp(i) );
        }

        for (int i = 0; i < nYears; ++i) {
            for (int j = 0; j < Nage; ++j) {
                if (i == 0) {
                    LegendNames << "Age " + QString::number(j);
                }
                GridData(i,j) = AgeStomCont(i,j) * 24.0 * AgeConsump(j,0) *
                                std::exp(AgeConsump(j,1) * SeasTemp(i));
            }
        }

    } else if (selectedVariable == "Relative Food Availability") {
        boost::numeric::ublas::vector<double> tempData;
        fields = {"Year","TotFA"};
        if (theModelName == "MSVPA") {
            firstYear = FirstYear;
            nYears = LastYear-FirstYear+1;
            nmfUtils::initialize(tempData,  nYears);
            nmfUtils::initialize(ChartData, nYears, 1);

            queryStr = "SELECT Year, Sum(SuitPreyBiomass) AS TotFA FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND PredName = '" + selectedSpecies + "'" +
                         ageStr + seasonStr +
                       " GROUP By Year";
        } else if (theModelName == "Forecast") {
            firstYear = Forecast_FirstYear;
            nYears = Forecast_NYears;

            // First we are going to get tot suit bm and average across years
            nmfUtils::initialize(tempData,  nYears);
            nmfUtils::initialize(ChartData, nYears, 1);
            nmfUtils::initialize(GridData,  nYears, 1);

            queryStr = "SELECT Year, Sum(SuitPreyBiomass) AS TotFA FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND ForeName = '" + ForecastName + "'" +
                       " AND Scenario = '" + ScenarioName + "'" +
                       " AND PredName = '" + selectedSpecies + "'" +
                         ageStr + seasonStr +
                       " GROUP By Year";
        }
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < nYears; ++i) {
            tempData(i) = std::stod(dataMap["TotFA"][i]);
            AvgFA += tempData(i);
        }
        AvgFA /= double(nYears + 1.0);

        // Convert this to a relative amount
        for (int i = 0; i < nYears; ++i) {
            categories << QString::number(firstYear + i);
            ChartData(i,0) = tempData(i) / AvgFA;
        }
        GridData = ChartData;

    } else if (selectedVariable == "Per Capita Food Availability") {
        boost::numeric::ublas::vector<double> tempData;
        boost::numeric::ublas::vector<double> TotBM;

        std::string newAgeStr = ageStr;
        int PredAgeVal = (! newAgeStr.empty()) ? std::stoi(newAgeStr.erase(0,newAgeStr.find("=")+1)) : 0;
        fields  = {"Year","Biomass"};
        fields2 = {"Year","TotFA"};

        if (theModelName == "MSVPA") {
            firstYear = FirstYear;
            nYears = LastYear-FirstYear+1;
            queryStr = "SELECT Year, Avg(Biomass) AS Biomass FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND SpeName = '"  + selectedSpecies + "'" +
                    " AND Age = " + std::to_string(PredAgeVal) +
                      seasonStr +
                    " GROUP By Year";
            queryStr2 = "SELECT Year, Sum(SuitPreyBiomass) AS TotFA FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                      ageStr + seasonStr +
                    " GROUP By Year";
        } else if (theModelName == "Forecast") {
            firstYear = Forecast_FirstYear;
            nYears    = Forecast_NYears;
            queryStr = "SELECT Year, Avg(InitBiomass) AS Biomass FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND SpeName = '"  + selectedSpecies + "'" +
                    " AND Age = " + std::to_string(PredAgeVal) +
                      seasonStr +
                    " GROUP By Year";
            queryStr2 = "SELECT Year, Sum(SuitPreyBiomass) AS TotFA FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND PredName = '" + selectedSpecies + "'" +
                      ageStr + seasonStr +
                    " GROUP By Year";
        }

        nmfUtils::initialize(tempData,  nYears);
        nmfUtils::initialize(ChartData, nYears, 1);
        nmfUtils::initialize(GridData,  nYears, 1);
        nmfUtils::initialize(TotBM, nYears);

        // Get total biomass for predator/age class..in this case its the average of the seasonal biomasses
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        for (int i = 0; i < nYears; ++i) {
            TotBM(i) = std::stod(dataMap["Biomass"][i]);
        }

        // Then get and calculate food availability stuff
        dataMap2 = databasePtr->nmfQueryDatabase(queryStr2, fields2);
        for (int i = 0; i < nYears; ++i) {
            tempData(i) = std::stod(dataMap2["TotFA"][i]) / TotBM(i);
            AvgFA += tempData(i);
        }
        AvgFA /= (nYears + 1.0);

        // Convert this to a relative amount
        for (int i = 0; i < nYears; ++i) {
            categories << QString::number(firstYear + i);
            ChartData(i,0) = tempData(i) / AvgFA;
        }
        GridData = ChartData;


    } else if (selectedVariable == "Food Availability by Prey Type") {

       // Implemented in nmfOutputChartStacked class

    } else if (selectedVariable == "Food Availability by Prey Age") {

         // implemented in nmfOutputChartStacked

    } else if (selectedVariable == "Stomach Content Weight") {

        if (theModelName == "MSVPA") {
            firstYear = FirstYear;
            nYears = LastYear-FirstYear+1;
            queryStr = "SELECT Year, Avg(StomCont) as Stom FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND SpeName = '"  + selectedSpecies + "'" +
                       ageStr + seasonStr +
                       " Group By Year";
        } else if (theModelName == "Forecast") {
            firstYear = Forecast_FirstYear;
            nYears = Forecast_NYears;
            queryStr = "SELECT Year, Avg(StomCont) as Stom FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND ForeName = '" + ForecastName + "'" +
                       " AND Scenario = '" + ScenarioName + "'" +
                       " AND SpeName = '"  + selectedSpecies + "'" +
                       ageStr + seasonStr +
                       " Group By Year";
        }
        nmfUtils::initialize(ChartData, nYears, 1);
        nmfUtils::initialize(GridData,  nYears, 1);

        fields = {"Year","Stom"};
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < nYears; ++i) {
            categories << QString::number(firstYear + i);
            ChartData(i,0) = std::stod(dataMap["Stom"][i]);
        }
        GridData = ChartData;

    } else if (selectedVariable == "Average Weight") { // Average weight at age

        nmfUtils::initialize(ChartData, Nage, 1);
        fields = {"Age","Weight"};
        if (theModelName == "MSVPA") {
            queryStr = "SELECT Age, Avg(SeasWt) as Weight FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND SpeName = '"  + selectedSpecies + "'" +
                    seasonStr +
                    " Group By Age";
        } else if (theModelName == "Forecast") {
            queryStr = "SELECT Age, Avg(InitWt) as Weight FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND SpeName = '"  + selectedSpecies + "'" +
                    seasonStr +
                    " Group By Age";
        }
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < Nage; ++i) {
           categories << QString::fromStdString("Age " + std::to_string(i));
           ChartData(i,0) = std::stod(dataMap["Weight"][i]) / WtConversion;
        }

    } else if (selectedVariable == "Average Size") {

        nmfUtils::initialize(ChartData, Nage, 1);
        fields = {"Age","ASize"};
        if (theModelName == "MSVPA") {
            queryStr = "SELECT Age, Avg(SeasSize) as ASize FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                       " AND SpeName = '"  + selectedSpecies + "'" +
                       seasonStr +
                       " Group By Age";
        } else if (theModelName == "Forecast") {
            queryStr = "SELECT Age, Avg(AvgSize) as ASize FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND SpeName = '"  + selectedSpecies + "'" +
                    seasonStr +
                    " Group By Age";
        }
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < Nage; ++i) {
           categories << QString::fromStdString("Age " + std::to_string(i));
           ChartData(i,0) = std::stod(dataMap["ASize"][i]) / WtConversion;
        }

    } else if (selectedVariable == "Weight at Age") {
        fields = {"Year","AvgWeight"};
        if (theModelName == "MSVPA") {
            firstYear = FirstYear;
            nYears = LastYear-FirstYear+1;
            nmfUtils::initialize(ChartData, nYears, 1);
            queryStr = "SELECT Year, SeasWt FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND SpeName = '"  + selectedSpecies + "'" +
                    ageStr + seasonStr +
                    " Order By Year";
        } else if (theModelName == "Forecast") {
            firstYear = Forecast_FirstYear;
            nYears = Forecast_NYears;
            nmfUtils::initialize(ChartData, nYears, 1);
            queryStr = "SELECT Year, AvgWeight FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND SpeName = '"  + selectedSpecies + "'" +
                    ageStr + seasonStr +
                    " Order By Year";
        }
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < nYears; ++i) {
            categories << QString::number(firstYear + i);
            ChartData(i,0) = std::stod(dataMap["AvgWeight"][i]) / WtConversion;
        }

    } else if (selectedVariable == "Size at Age") {  // Size at age across time for a particular age class
        fields = {"Year","AvgSize"};
        if (theModelName == "MSVPA") {
            firstYear = FirstYear;
            nYears = LastYear-FirstYear+1;
            nmfUtils::initialize(ChartData, nYears, 1);
            queryStr = "SELECT Year, SeasSize FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND SpeName = '"  + selectedSpecies + "'" +
                    ageStr + seasonStr +
                    " Order By Year";
        } else if (theModelName == "Forecast") {
            firstYear = Forecast_FirstYear;
            nYears = Forecast_NYears;
            nmfUtils::initialize(ChartData, nYears, 1);
            queryStr = "SELECT Year, AvgSize FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                    " AND ForeName = '" + ForecastName + "'" +
                    " AND Scenario = '" + ScenarioName + "'" +
                    " AND SpeName = '"  + selectedSpecies + "'" +
                    ageStr + seasonStr +
                    " Order By Year";
        }
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int i = 0; i < nYears; ++i) {
            categories << QString::number(firstYear + i);
            ChartData(i,0) = std::stod(dataMap["AvgSize"][i]) / SizeConversion;
        }

    }


} // end getChartData

void nmfOutputChartBar::copyChartDataToGridData(
    boost::numeric::ublas::matrix<double> &ChartData,
    boost::numeric::ublas::matrix<double> &GridData)
{
    int size1 = ChartData.size1();
    int size2 = ChartData.size2();
    nmfUtils::initialize(GridData, size1, size2);
    GridData = ChartData;
}


void nmfOutputChartBar::getChartDataOfPredationMortalityByPredator(
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
      boost::numeric::ublas::matrix<double> &ChartData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    boost::numeric::ublas::vector<double> TotConsumed;
    boost::numeric::ublas::vector<std::string> PdList;
    std::string forecastNameStr = "";
    std::string scenarioNameStr = "";
    int NumRecords = 0;

    nmfUtils::initialize(TotConsumed, NYears);
    nmfUtils::initialize(PdList, nmfConstants::MaxNumberSpecies);
    int NumPredators; // NPd

    if (type == "Forecast") {
        forecastNameStr = " AND ForeName = '" + ForecastName + "'";
        scenarioNameStr = " AND Scenario = '" + ScenarioName + "'";
    }

    // First select total amount of prey type/age consumed by all predators
    fields = {"Year","TotCons"};
    queryStr = "SELECT Year, Sum(BMConsumed) as TotCons from " + SuitPreyBiomassTable + " WHERE MSVPAname='" + MSVPAName + "' "
               " and PreyName='" + species + "' " +
               " and PreyAge = " + std::to_string(speciesAge) +
               seasonStr + " GROUP BY YEAR ";
    dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
    if (dataMap["TotCons"].size() == 0) {
        std::cout << "No predation on Age = " << speciesAge << " " << species << std::endl;
        nmfUtils::initialize(ChartData, 0, 0);
        // RSK make this a popup!!
        return;
    }
    for (int i=0; i<NYears; ++i) {
        TotConsumed(i) = std::stod(dataMap["TotCons"][i]);
    }

    // Next get the list of unique predators for your prey type and age class
    fields = {"PredName"};
    queryStr = "SELECT DISTINCT(PredName) from " + SuitPreyBiomassTable + " WHERE MSVPAname='" + MSVPAName + "' " +
               forecastNameStr +
               scenarioNameStr +
               " and PreyName='" + species + "' " +
               " and PreyAge = " + std::to_string(speciesAge) +
               seasonStr + " ORDER BY PredName ";
    dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
    NumPredators = dataMap["PredName"].size();
    LegendNames.clear();
    for (int i=0; i<NumPredators; ++i) {
        PdList(i) = dataMap["PredName"][i];
        LegendNames << QString::fromStdString(PdList(i));
    }

    nmfUtils::initialize(ChartData, NYears, NumPredators);

    // Then get BM consumed by predator and calculate proportaion of mortality = bm pred / tot bm
    for (int i=0; i<NumPredators; ++i) {
        fields = {"Year","TotCons"};
        queryStr = "SELECT Year, Sum(BMConsumed) as TotCons from " + SuitPreyBiomassTable + " WHERE MSVPAname='" + MSVPAName + "' "
                   " and PreyName='" + species + "' " +
                   " and PreyAge = " + std::to_string(speciesAge) +
                   " and PredName = '" + PdList(i) + "' " +
                   seasonStr + " GROUP BY YEAR ";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        NumRecords = dataMap["TotCons"].size();
        if (NYears != int(dataMap["TotCons"].size())) {
            std::cout << queryStr << std::endl;
            std::cout << "Warning: NYears (" << NYears << ") not equal to number of records from above query ("
                      << dataMap["TotCons"].size() << ").  Re-run MSVPA configuration." << std::endl;
        }
        for (int j=0; j<NumRecords; ++j) {
            ChartData(j,i) = std::stod(dataMap["TotCons"][j])/TotConsumed(j);
        }
    } // end for i

    // Last get annual M2...then multiply
    fields = {"Year","M2"};
    queryStr = "SELECT Year, Sum(SeasM2) as M2 from  " + outputTable + " WHERE MSVPAname='" + MSVPAName + "' " +
               forecastNameStr +
               scenarioNameStr +
               " and SpeName = '" + species + "' " +
               ageStr + seasonStr + " GROUP BY YEAR ";
    dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

    for (int i=0; i<NYears; ++i) {
        categories << QString::number(FirstYear + i);
        for (int j=0; j<NumPredators; ++j) {
            ChartData(i,j) *= std::stod(dataMap["M2"][i]);
        }
    }
} // end getChartDataOfPredationMortalityByPredator





void nmfOutputChartBar::queryDataAndLoadChart(
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
        const int &selectedSpeciesNum,
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
        QStringList &LegendNames)
{
    int ageVal=0;
    QBarSeries *series = nullptr;
    QStringList categories;
    std::string newAgeStr = ageStr;
    std::string newYLabel="";
    std::string SizeUnitString;
    std::string WtUnitString;

    bool scaleFactorBool = isOfTypeAbundance(selectedVariable);

    if (newAgeStr != "") {
        ageVal = std::stoi(newAgeStr.erase(0,newAgeStr.find("=")+1));
    }

    getChartData(databasePtr, MSVPAName, theModelName,
            ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
            NYears, Nage, FirstYear, LastYear, SpeAge, fieldToSum,
            selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
            ageStr, seasonStr, selectedPreyName,
            scaleFactor, ChartData, GridData, categories, LegendNames,
            SizeUnitString, WtUnitString);

    rowLabels = categories;

    if (ChartData.size1() == 0)
        return;

    if (caller == "season") {
        if ((selectedVariable == "Abundance by Age"))
        {
            loadChartWithData(chart,series,ChartData,LegendNames,
                              true,   YMax, // RSK change this
                              scaleFactorBool);
        }
        else if ((selectedVariable == "Biomass by Age")  ||
                 (selectedVariable == "Recruit Biomass") ||
                 (selectedVariable == "Recruit Abundance")) {
            loadChartWithData(chart,series,ChartData, LegendNames,
                              true,    YMax, // RSK change this
                              scaleFactorBool);
        }   else {

            loadChartWithData(chart,series,ChartData, LegendNames,
                              true,    YMax, // RSK change this
                              scaleFactorBool);
        }

    } else if (caller == "speciesAgeSizeClass") {
            loadChartWithData(chart,series,ChartData, LegendNames,
                              true,    YMax, // RSK change this
                              scaleFactorBool);
    } else {
        loadChartWithData(chart,series,ChartData, LegendNames,
                          false,0,scaleFactorBool);
    }

    if (newAgeStr != "") {
        newAgeStr = "Age " + std::to_string(ageVal) + recruitChar;
    }

    newYLabel = yLabel;
    if (yLabel == "SizeUnitString") {
        newYLabel = SizeUnitString;
    } else if (yLabel == "WtUnitString") {
        newYLabel = WtUnitString;
    }

    setTitles(chart, series, categories,
              newAgeStr, selectedSpecies, title, titleSuffix,
              "Year",newYLabel,rearrangeTitle);

} // end queryDataAndLoadChart




void nmfOutputChartBar::querySpawningStockBiomassDataAndLoadChart(
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
        const bool useMaxValue,
        const double YMax,
        QStringList &rowLabels)
{
    int m = 0;
    int SSBNumYears=0;
    int SSBFirstYear=0;
    int SSBLastYear=0;
    double maxValue = 0.0;
    double TmpBM = 0.0;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    std::string field;
    QBarSeries *series = nullptr;
    std::size_t foundSpawning;
    QStringList categories;
    std::string newAgeStr = ageStr;
    boost::numeric::ublas::matrix<double> Maturity;
    boost::numeric::ublas::matrix<double> tempData;
    QStringList LegendNames = {""};
    //boost::numeric::ublas::matrix<double> ChartData;

    if (theModelName == "MSVPA") {

        // RSK  - make lstrip function in nmfUtils
        field = fieldToSum;
        foundSpawning = field.find("Spawning");
        if (foundSpawning != std::string::npos) { // then Spawning was found...
            field.erase(0,field.find("_")+1);
        }

        nmfUtils::initialize(ChartData, NYears, 1);
        nmfUtils::initialize(GridData,  NYears, 1);

        fields = {"Year", "Age", "Biomass"};
        queryStr = "SELECT Year, Age, Sum(" + field + ") as Biomass from MSVPASeasBiomass WHERE MSVPAname='" + MSVPAName +
                "' and SpeName='" + selectedSpecies +
                "' and Season=" + std::to_string(season) +
                "  GROUP BY Age,Year ";

        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        SSBNumYears  = NYears;
        SSBFirstYear = FirstYear;
        SSBLastYear  = LastYear;
    } else if (theModelName == "Forecast") {

        nmfUtils::initialize(ChartData, Forecast_NYears, 1);
        nmfUtils::initialize(GridData,  Forecast_NYears, 1);

        // Get annual biomass by age class
        fields = {"Year", "Age", "Biomass"};
        queryStr = "SELECT Year, Age, Sum(InitBiomass) AS Biomass FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                        " AND ForeName = '" + ForecastName + "'" +
                        " AND Scenario = '" + ScenarioName + "'" +
                        " AND SpeName = '" + selectedSpecies + "'" +
                        " AND Season = " + std::to_string(season) +
                        " GROUP By Age,Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        SSBNumYears  = Forecast_NYears;
        SSBFirstYear = Forecast_FirstYear;
        SSBLastYear  = Forecast_FirstYear + Forecast_NYears - 1;
        std::cout << "SSB1 vals: " << SSBNumYears << ", " <<
                     SSBFirstYear << ", " << SSBLastYear << ", " << Nage << ", " << selectedSpecies << std::endl;
    } // end else if Forecast...


    m = 0;
    nmfUtils::initialize(tempData, Nage, SSBNumYears);
    for (int i=0; i<Nage; ++i) {
        for (int j=0; j<SSBNumYears; ++j) {
            tempData(i,j) = std::stod(dataMap["Biomass"][m++]);
        }
    }

    nmfUtils::initialize(Maturity, Nage, SSBNumYears);
    getMaturityData(databasePtr, theModelName, Nage, SSBNumYears, SSBFirstYear, SSBLastYear,
                    selectedSpecies, Maturity);

    for (int i=0; i<SSBNumYears; ++i) {
        categories << QString::number(SSBFirstYear + i);
        TmpBM = 0.0;
        for (int j=0; j<Nage; ++j) {
            TmpBM += tempData(j,i) * Maturity(j,i);
        }
        ChartData(i,0) = (0.5 * TmpBM) / 1000.0; // 50% sex ration...but will need to include a place to enter it somewhere
    }
    rowLabels = categories;
    GridData = ChartData;

    if (caller == "season") {
        loadChartWithData(chart,series,ChartData, LegendNames, useMaxValue, YMax, false);
    }  else {
        loadChartWithData(chart,series,ChartData, LegendNames, false,0.0,false);
    }
    setTitles(chart,series,categories,"",selectedSpecies, title, titleSuffix,
              "Year",yLabel, nmfConstants::DontRearrangeTitle);

} // end querySpawningStockBiomassDataAndLoadChart


void nmfOutputChartBar::redrawChart(
        std::string caller,
        nmfDatabase *databasePtr,
        std::string theModelName,
        QChart *chart,
        std::map<std::string, QLabel*> &AllLabels,
        std::map<std::string, QComboBox*> &AllComboBoxes,
        std::map<std::string, QPushButton*> &AllButtons,
        std::map<std::string, QCheckBox*> &AllCheckBoxes,
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
        double YMaxOverride,
        bool hGridLine,
        bool vGridLine,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &ChartData,
        boost::numeric::ublas::matrix<double> &GridData)
{
    int m = 0;
    int Nage = 0;
    int seasonVal=0;
    int season=0;
    int offset = 0;
    int SpeAge = 0;
    double NConversion = 1.0;
    double scaleFactor = 1000.0; // to display on 1000's metric tons
    std::string ageStr = "";
    std::string predAgeStr = "";
    std::string seasonStr = "";
    std::string fieldToSum = "AnnBiomass";
    std::string title;
    std::string titleSuffix = "";
    std::string queryStr;
    std::string valStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    //boost::numeric::ublas::matrix<double> ChartData;
    bool useSeasonMaxValue  = AllCheckBoxes["selectSeasonCB"]->isChecked();
    bool useAgeSizeMaxValue = AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->isChecked();
    bool useMaxValue = false;
    std::string yLabel="000 Metric Tons";
    std::string yLabelConversion="";
    double CatchUnits = 0;
    int nYears;
    int firstYear;
    int NPrey;
    QStringList PreyList;

    logger->logMsg(nmfConstants::Normal,
                      theModelName + " " +
                      "nmfOutputChartBar::redrawChart "+
                      selectedDataType + ", " +
                      selectedSpecies + ", " +
                      selectedVariable + ", " +
                      selectedByVariables + ", " +
                      selectedSeason + ", " +
                      selectedSpeciesAgeSizeClass);
    setComboBoxStates(caller,AllLabels,AllComboBoxes,AllButtons,AllCheckBoxes,selectedDataType,
                      selectedSpecies,selectedVariable,selectedByVariables,
                      selectedSeason,selectedSpeciesAgeSizeClass);
    chart->removeAllSeries();

    // Find number of years and seasons
    std::map<std::string,int> initMap =
            databasePtr->nmfQueryInitFields("MSVPAlist", MSVPAName);
    //int FirstYear = initMap["FirstYear"];
    int LastYear  = initMap["LastYear"];
    int NYears    = LastYear - FirstYear + 1;
    //int NSeasons  = initMap["NSeasons"];



    // Find NConversion scale factor which is used in some cases
    fields = {"WtUnits","CatchUnits"};
    queryStr = "SELECT WtUnits,CatchUnits from Species where SpeName='" + selectedSpecies + "'";
    dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
    if (dataMap["WtUnits"].size() > 0) {
        CatchUnits = std::stod(dataMap["CatchUnits"][0]);
    }

    bool isAbundance = isOfTypeAbundance(selectedVariable);
    // CatchUnits = (isAbundance) ? CatchUnits*1000.0 : CatchUnits;
    std::vector<std::string> yLabels = {"Hundreds of Fish",
                                        "Thousands of Fish",
                                        "Millions of Fish",
                                        "Billions of Fish"};
    int i = 0;
    if (CatchUnits == 0.1) {
        NConversion = 100.0;
        yLabelConversion = yLabels[i];
    } else if (CatchUnits == 1.0) {
        NConversion = 1000.0;
        yLabelConversion = yLabels[++i];
    } else if (CatchUnits == 1000.0) {
        NConversion = 1000000.0;
        yLabelConversion = yLabels[++i];
    } else if (CatchUnits == 1000000.0) {
        NConversion = 1000000000.0;
        yLabelConversion = yLabels[++i];
    }
    yLabelConversion = (isAbundance && (i<3)) ? yLabels[i+1] : yLabels[i];

    Nage = AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->count();
    std::string newSelectedSeason = selectedSeason;
    std::string newSelectedSpeciesAgeSizeClass = selectedSpeciesAgeSizeClass;

    newSelectedSeason.erase(0,newSelectedSeason.find(" "));
    season = std::stoi(newSelectedSeason)-1;

    if (! newSelectedSpeciesAgeSizeClass.empty()) {
        offset = (newSelectedSpeciesAgeSizeClass.find("Size") != std::string::npos) ? 1 : 0;
        newSelectedSpeciesAgeSizeClass.erase(0,newSelectedSpeciesAgeSizeClass.find(" "));
        SpeAge = std::stoi(newSelectedSpeciesAgeSizeClass);
    }

    if (theModelName == "MSVPA") {
        nYears    = NYears;
        firstYear = FirstYear;
    } else {
        nYears    = Forecast_NYears;
        firstYear = Forecast_FirstYear;
    }

    AllLabels["selectSeasonLBL"]->setEnabled(false);
    AllComboBoxes["selectSeasonCMB"]->setEnabled(false);
    AllCheckBoxes["selectSeasonCB"]->setEnabled(false);
    AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(false);
    AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(false);
    AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(false);
    AllButtons["flipLeftPB"]->setEnabled(false);
    AllButtons["flipRightPB"]->setEnabled(false);


    if (selectedVariable == "Total Biomass") {                          // Population Size

        colLabels << "Biomass";
        nmfUtils::initialize(ChartData,NYears,1);

        if (selectedByVariables == "Annual") {
            title = "Total Biomass (Jan 1.)";
            useSeasonMaxValue = false;
        } else if (selectedByVariables == "Seasonal") {
            fieldToSum = "Biomass";
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            title = "Total Mid-Season Biomass: ";
            titleSuffix = selectedSeason;
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              yLabel, scaleFactor, YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData, nYears, 1);
        GridData = ChartData;

    } else if (selectedVariable == "Biomass by Age") {                  // Population Size

        colLabels << "Biomass";

        if (selectedByVariables == "Annual") {
            title = "Biomass (Jan 1.)";
            ageStr = " and Age=" + std::to_string(SpeAge);
        } else if (selectedByVariables == "Seasonal") {
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            fieldToSum = "Biomass";
            title = "Mid-Season Biomass: ";
            titleSuffix = selectedSeason;
            ageStr = " and Age=" + std::to_string(SpeAge);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
        AllButtons["flipLeftPB"]->setEnabled(true);
        AllButtons["flipRightPB"]->setEnabled(true);

        if (caller == "season" )
            useMaxValue =  useSeasonMaxValue;
        else if (caller == "speciesAgeSizeClass")
            useMaxValue =  useAgeSizeMaxValue;

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              yLabel, scaleFactor,
                              YMaxOverride, rowLabels, colLabels);

        // For age specific data..we want to see the whole matrix and be able to save that out...
        // so have to do a new query and everything
        m = 0;
        colLabels.clear();
        fields = {"Year", "SumField"};
        queryStr = "Select Year, Sum(" + fieldToSum + ") As SumField FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'"
                   " AND SpeName = '" + selectedSpecies + "'" +
                   seasonStr + " GROUP BY Year, Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        nmfUtils::initialize(GridData, nYears, Nage);
        for (int i=0; i<nYears; ++i) {
            rowLabels << QString::number(firstYear+i);
            for (int j=0; j<Nage; ++j) {
                if (i == 0) {
                    colLabels << QString::fromStdString("Age "+std::to_string(j));
                }
                GridData(i, j) = std::stod(dataMap["SumField"][m++]) / 1000.0;
            } // end for j
        } // end for i


    } else if (selectedVariable == "Spawning Stock Biomass") {          // Population Size

        colLabels << "Biomass";

        if (selectedByVariables == "Annual") {
            fieldToSum = "Spawning_AnnBiomass";
            title = "Spawning Stock Biomass (Jan 1.)";
        } else if (selectedByVariables == "Seasonal") {
            fieldToSum = "Spawning_Biomass";
            title = "Mid-Season Spawning Stock Biomass: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        } // end Seasonal

        querySpawningStockBiomassDataAndLoadChart(
                    caller, databasePtr, ChartData, GridData, theModelName,
                    ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                    NYears, Nage,
                    fieldToSum, MSVPAName, selectedSpecies,
                    ageStr, seasonVal, FirstYear, LastYear,
                    chart, title, titleSuffix, nmfConstants::DontRearrangeTitle,
                    yLabel, useSeasonMaxValue, YMaxOverride, rowLabels);



    } else if (selectedVariable == "Total Abundance") {                 // Population Size

        colLabels << "Abundance";

        if (selectedByVariables == "Annual") {
            fieldToSum = "AnnAbund";
            title = "Total Abundance (Jan 1.)";
        } else if (selectedByVariables == "Seasonal") {
            fieldToSum = "SeasAbund";
            title = "Total Mid-Season Abundance: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        } // end Seasonal

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              yLabelConversion, NConversion,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData, nYears, 1);
        GridData = ChartData;



    } else if (selectedVariable == "Abundance by Age") {                // Population Size

        colLabels << "Abundance";

        ageStr = " and Age = " + std::to_string(SpeAge);

        if (selectedByVariables == "Annual") {
            fieldToSum = "AnnAbund";
            title = "Abundance (Jan 1.)";
        } else if (selectedByVariables == "Seasonal") {
            fieldToSum = "SeasAbund";
            title = "Mid-Season Abundance: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        } // end Seasonal
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
        AllButtons["flipLeftPB"]->setEnabled(true);
        AllButtons["flipRightPB"]->setEnabled(true);

        if (caller == "season" )
            useMaxValue =  useSeasonMaxValue;
        else if (caller == "speciesAgeSizeClass")
            useMaxValue =  useAgeSizeMaxValue;
        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              yLabelConversion, NConversion,
                              YMaxOverride, rowLabels, colLabels);


        m = 0;
        colLabels.clear();
        fields = {"Year", "SumField"};
        queryStr = "Select Year, Sum(" + fieldToSum + ") As SumField FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'"
                   " AND SpeName = '" + selectedSpecies + "'" +
                   seasonStr + " GROUP BY Year, Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        rowLabels.clear();
        nmfUtils::initialize(GridData, nYears, Nage);
        for (int i=0; i<nYears; ++i) {
            rowLabels << QString::number(firstYear+i);
            for (int j=0; j<Nage; ++j) {
                if (i == 0) {
                    colLabels << QString::fromStdString("Age "+std::to_string(j));
                }
                GridData(i, j) = std::stod(dataMap["SumField"][m++]) / NConversion; //1000.0;
            } // end for j
        } // end for i

    } else if (selectedVariable == "Recruit Abundance") {               // Population Size
        ageStr = " and Age >= " + std::to_string(SpeAge);
        colLabels << "Abundance";

        if (selectedByVariables == "Annual") {
            fieldToSum = "Recruit_AnnAbund";
            title = "Abundance (Jan 1.)";
        } else if (selectedByVariables == "Seasonal") {
            fieldToSum = "Recruit_SeasAbund";
            title = "Mid-Season Abundance: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        } // end Seasonal
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
        AllButtons["flipLeftPB"]->setEnabled(true);
        AllButtons["flipRightPB"]->setEnabled(true);

        if (caller == "season" )
            useMaxValue =  useSeasonMaxValue;
        else if (caller == "speciesAgeSizeClass")
            useMaxValue =  useAgeSizeMaxValue;
        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "+", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              yLabelConversion, NConversion,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData, nYears, 1);
        GridData = ChartData;

    } else if (selectedVariable == "Recruit Biomass") {                 // Population Size
        colLabels << "Biomass";

        if (selectedByVariables == "Annual") {
            fieldToSum = "Recruit_AnnBiomass";
            title = "Biomass (Jan 1.)";
            ageStr = " and Age >= " + std::to_string(SpeAge);
        } else if (selectedByVariables == "Seasonal") {
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            fieldToSum = "Recruit_Biomass";
            title = "Mid-Season Biomass: ";
            titleSuffix = selectedSeason;
            ageStr = " and Age >= " + std::to_string(SpeAge);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
        AllButtons["flipLeftPB"]->setEnabled(true);
        AllButtons["flipRightPB"]->setEnabled(true);

        if (caller == "season" )
            useMaxValue =  useSeasonMaxValue;
        else if (caller == "speciesAgeSizeClass")
            useMaxValue =  useAgeSizeMaxValue;
        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "+", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              yLabel, scaleFactor,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData, nYears, 1);
        GridData = ChartData;

    } else if (selectedVariable == "Fishing Mortality") {               // Mortality Rates - MSVPA

        colLabels << "Predation Mortality";

        if (selectedByVariables == "Annual") {
            title = "Fishing Mortality Rate";
            seasonStr = "";
        } else if (selectedByVariables == "Seasonal") {
            title = "Fishing Mortality Rate: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
        AllButtons["flipLeftPB"]->setEnabled(true);
        AllButtons["flipRightPB"]->setEnabled(true);
        fieldToSum = "SeasF";
        ageStr = " and Age = " + std::to_string(SpeAge);

        if (caller == "season" )
            useMaxValue =  useSeasonMaxValue;
        else if (caller == "speciesAgeSizeClass")
            useMaxValue =  useAgeSizeMaxValue;
        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "Fishing Mortality", 0.001,
                              YMaxOverride, rowLabels, colLabels);


        // For age specific data..we want to see the whole matrix and be able to save that out...
        // so have to do a new query and everything
        nmfUtils::initialize(GridData, NYears, Nage);
        if (theModelName == "MSVPA") {
            nYears = NYears;
            firstYear = FirstYear;
        } else {
            nYears = Forecast_NYears;
            firstYear = Forecast_FirstYear;
        }
        m = 0;
        fields = {"Year", "SumField"};
        queryStr = "Select Year, Sum(" + fieldToSum + ") As SumField FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'"
                   " AND SpeName = '" + selectedSpecies + "'" +
                   seasonStr + " GROUP BY Year, Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        for (int i=0; i<nYears; ++i) {
            rowLabels << QString::number(firstYear+i);
            for (int j=0; j<Nage; ++j) {
                if (i == 0) {
                    colLabels << QString::fromStdString("Age "+std::to_string(j));
                }
                GridData(i,j) = nmfUtils::round(std::stod(dataMap["SumField"][m++]),3);
            } // end for j
        } // end for i

    } else if (selectedVariable == "Average Recruited F") {             // Mortality Rates

        colLabels << "Average F";

        if (selectedByVariables == "Annual") {
            title = "Average Fishing Mortality Rate";
            seasonStr = "";
        } else if (selectedByVariables == "Seasonal") {
            title = "Average Fishing Mortality Rate: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        ageStr = " and Age >= " + std::to_string(SpeAge);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "Fishing Mortality", 0.001,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData,nYears,1);
        GridData = ChartData;

    } else if (selectedVariable == "Predation Mortality") {             // Mortality Rates = MSVPA

        colLabels << "M2 by Age";

        if (selectedByVariables == "Annual") {
            title = "Predation Mortality Rate";
            seasonStr = "";
        } else if (selectedByVariables == "Seasonal") {
            title = "Predation Mortality Rate: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
        AllButtons["flipLeftPB"]->setEnabled(true);
        AllButtons["flipRightPB"]->setEnabled(true);

        fieldToSum = "SeasM2";
        ageStr = " and Age = " + std::to_string(SpeAge);

        if (caller == "season" )
            useMaxValue =  useSeasonMaxValue;
        else if (caller == "speciesAgeSizeClass")
            useMaxValue =  useAgeSizeMaxValue;
        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "Predation Mortality (M2)", 0.001,
                              YMaxOverride, rowLabels, colLabels);


        nmfUtils::initialize(GridData, nYears, Nage);
        m = 0;
        fields = {"Year", "SumField"};
        queryStr = "Select Year, Sum(SeasM2) As SumField FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'"
                   " AND SpeName  = '" + selectedSpecies + "'" +
                   seasonStr + " GROUP BY Year, Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        for (int i=0; i<nYears; ++i) {
            rowLabels << QString::number(firstYear+i);
            for (int j=0; j<Nage; ++j) {
                if (i == 0) {
                    colLabels << QString::fromStdString("Age "+std::to_string(j));
                }
                valStr = dataMap["SumField"][m++];
                GridData(i,j) = (valStr.empty()) ? 0 : nmfUtils::round(std::stod(valStr),3);
            } // end for j
        } // end for i

    } else if (selectedVariable == "Predation Mortality Rate") {        // Mortality Rates - Forecast

        colLabels << "M2 Rate";

        if (selectedByVariables == "Annual") {
            title = "Predation Mortality Rate";
            seasonStr = "";
        } else if (selectedByVariables == "Seasonal") {
            title = "Predation Mortality Rate: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
        AllButtons["flipLeftPB"]->setEnabled(true);
        AllButtons["flipRightPB"]->setEnabled(true);
        fieldToSum = "SeasM2";
        ageStr = " and Age = " + std::to_string(SpeAge);

        if (caller == "season" )
            useMaxValue =  useSeasonMaxValue;
        else if (caller == "speciesAgeSizeClass")
            useMaxValue =  useAgeSizeMaxValue;
        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "Predation Mortality (M2)", 0.001,
                              YMaxOverride, rowLabels, colLabels);


        nmfUtils::initialize(GridData, nYears, Nage);
        m = 0;
        fields = {"Year", "SumField"};
        queryStr = "Select Year, Sum(SeasM2) As SumField FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'"
                   " AND ForeName = '" + ForecastName + "'" +
                   " AND Scenario = '" + ScenarioName + "'" +
                   " AND SpeName  = '" + selectedSpecies + "'" +
                   seasonStr + " GROUP BY Year, Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        for (int i=0; i<nYears; ++i) {
            rowLabels << QString::number(firstYear+i);
            for (int j=0; j<Nage; ++j) {
                if (i == 0) {
                    colLabels << QString::fromStdString("Age "+std::to_string(j));
                }
                valStr = dataMap["SumField"][m++];
                GridData(i,j) = (valStr.empty()) ? 0 : nmfUtils::round(std::stod(valStr),3);
            } // end for j
        } // end for i


    } else if (selectedVariable == "Predation Mortality by Predator") { // Mortality Rates

        colLabels << "M2 by Predator";
        if (selectedByVariables == "Annual") {
            title = "M2 by Predator";
            seasonStr = "";
        } else if (selectedByVariables == "Seasonal") {
            title = "M2 by Predator: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        ageStr = " and Age = " + std::to_string(SpeAge);
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
        AllButtons["flipLeftPB"]->setEnabled(true);
        AllButtons["flipRightPB"]->setEnabled(true);

        if (caller == "season" )
            useMaxValue =  useSeasonMaxValue;
        else if (caller == "speciesAgeSizeClass")
            useMaxValue =  useAgeSizeMaxValue;
        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "Predation Mortality", 0.001,
                              YMaxOverride, rowLabels,colLabels);

        nmfUtils::initialize(GridData,ChartData.size1(),ChartData.size2());
        GridData = ChartData;

    } else if (selectedVariable == "Total Fishery Catch (Numbers)") {   // Mortality Rates - Forecast

        colLabels << "Catch (Numbers)";

        if (selectedByVariables == "Annual") {
            title = "Total Catch";
            seasonStr = "";
        } else if (selectedByVariables == "Seasonal") {
            title = "Total Catch: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "Catch in " + yLabelConversion, 1.0,
                              YMaxOverride, rowLabels, colLabels);


        nmfUtils::initialize(GridData, nYears, 1);
        GridData = ChartData;



    } else if (selectedVariable == "Fishery Catch at Age (Numbers)") {  // Mortality Rates - Forecast
        colLabels << "Average F";
        if (selectedByVariables == "Annual") {
            title = "Catch At Age";
        } else if (selectedByVariables == "Seasonal") {
            title = "Catch At Age: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            ageStr = " and Age = " + std::to_string(SpeAge);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        ageStr = " and Age = " + std::to_string(SpeAge);
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "Catch in " + yLabelConversion, 1.0,
                              YMaxOverride, rowLabels, colLabels);
        m = 0;
        fields = {"Year","Age","SumField"};
        queryStr = "SELECT Year, Age, Sum(SeasCatch) as SumField FROM ForeOutput WHERE MSVPAName = '" + MSVPAName + "'" +
                   " AND ForeName = '" + ForecastName + "'" +
                   " AND Scenario = '" + ScenarioName + "'" +
                   " AND SpeName  = '" + selectedSpecies + "'" +
                   " GROUP BY Year, Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        nmfUtils::initialize(GridData, nYears, Nage);
        colLabels.clear();
        for (int i=0; i<nYears; ++i) {
            rowLabels << QString::number(firstYear+i);
            for (int j=0; j<Nage; ++j) {
                if (i == 0) {
                    colLabels << QString::fromStdString("Age "+std::to_string(j));
                }
                GridData(i, j) = nmfUtils::round(std::stod(dataMap["SumField"][m++])/NConversion,1);
            } // end for j
        } // end for i

    } else if (selectedVariable == "Total Fishery Yield (Biomass)") {   // Mortality Rates - Forecast

        colLabels << "Yield";

        if (selectedByVariables == "Annual") {
            title = "Total Fishery Yield";
        } else if (selectedByVariables == "Seasonal") {
            title = "Total Fishery Yield: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "Yield in " + yLabelConversion, 1.0,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData, nYears, 1);
        GridData = ChartData;

    } else if (selectedVariable == "Fishery Yield at Age (Biomass)") {  // Mortality Rates - Forecast

        colLabels << "Yield";

        if (selectedByVariables == "Annual") {
            title = "Fishery Yield at Age";
        } else if (selectedByVariables == "Seasonal") {
            title = "Fishery Yield at Age: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        ageStr = " and Age = " + std::to_string(SpeAge);
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "Yield in " + yLabelConversion, 1.0,
                              YMaxOverride, rowLabels, colLabels);

        m = 0;
        colLabels.clear();
        fields = {"Year", "SumField"};
        queryStr = "Select Year, Sum(SeasYield) As SumField FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'"
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName  = '" + selectedSpecies + "'" +
                seasonStr + " GROUP BY Year, Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        rowLabels.clear();
        nmfUtils::initialize(GridData, nYears, Nage);
        for (int i=0; i<nYears; ++i) {
            rowLabels << QString::number(firstYear+i);
            for (int j=0; j<Nage; ++j) {
                if (i == 0) {
                    colLabels << QString::fromStdString("Age "+std::to_string(j));
                }
                GridData(i, j) = std::stod(dataMap["SumField"][m++]) / 1000.0;
            } // end for j
        } // end for i

    } else if (selectedVariable == "Fishery Mortality Rate at Age") {   // Mortality Rates - Forecast

        colLabels << "F at Age";

        if (selectedByVariables == "Annual") {
            title = "Fishery Mortality Rate";
        } else if (selectedByVariables == "Seasonal") {
            title = "Fishery Mortality Rate: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        ageStr = " and Age >= " + std::to_string(SpeAge);
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "Fishery Mortality (F)", 1.0,
                              YMaxOverride, rowLabels, colLabels);

        m = 0;
        colLabels.clear();
        fields = {"Year", "SumField"};
        queryStr = "Select Year, Sum(SeasF) As SumField FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'"
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName  = '" + selectedSpecies + "'" +
                seasonStr + " GROUP BY Year, Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        rowLabels.clear();
        nmfUtils::initialize(GridData, nYears, Nage);
        for (int i=0; i<nYears; ++i) {
            rowLabels << QString::number(firstYear+i);
            for (int j=0; j<Nage; ++j) {
                if (i == 0) {
                    colLabels << QString::fromStdString("Age "+std::to_string(j));
                }
                valStr = dataMap["SumField"][m];
                GridData(i,j) = (valStr.empty()) ? 0 : nmfUtils::round(std::stod(valStr),3);
            } // end for j
        } // end for i

    } else if (selectedVariable == "Total Biomass Consumed") {          // Consumption Rates

        colLabels << "BM Consumed";

        if (selectedByVariables == "Annual") {
            title = "Total Prey Biomass Consumed by";
        } else if (selectedByVariables == "Seasonal") {
            title = "Total Prey Biomass Consumed by";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::RearrangeTitle,
                              "Biomass (000 Metric Tons)", 1.0,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData,ChartData.size1(),ChartData.size2());
        GridData = ChartData;

    } else if (selectedVariable == "Consumption by Predator Age") {     // Consumption Rates

        colLabels << "Biomass";

        if (selectedByVariables == "Annual") {
            title = "Total Prey Biomass Consumed by";
        } else if (selectedByVariables == "Seasonal") {
            title = "Total Prey Biomass Consumed by";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        ageStr = " and Age = " + std::to_string(SpeAge-offset);
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge-offset, chart,
                              title, titleSuffix, nmfConstants::RearrangeTitle,
                              "Biomass (000 Metric Tons)", 1.0,
                              YMaxOverride, rowLabels, colLabels);


        nmfUtils::initialize(GridData, nYears, Nage);

        for (int i=0; i<nYears; ++i) {
            rowLabels << QString::number(firstYear+i);
        }
        for (int j=0; j<Nage; ++j) {
            colLabels << "Age "+QString::number(j);
        }
        m = 0;
        fields = {"Year", "TotCons"};
        if (theModelName == "MSVPA") {
            queryStr = "Select Year, Sum(BMConsumed) As TotCons FROM MSVPASuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'"
                       " AND PredName = '" + selectedSpecies + "'" +
                       seasonStr + " GROUP BY Year,PredAge";
        } else if (theModelName == "Forecast") {
            queryStr = "Select Year, Sum(TotalBMConsumed) As TotCons FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'"
                       " AND ForeName = '" + ForecastName + "'" +
                       " AND Scenario = '" + ScenarioName + "'" +
                       " AND SpeName = '" + selectedSpecies + "'" +
                       seasonStr + " GROUP BY Year,Age";
        }
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        for (int i=0; i<nYears; ++i) {
            for (int j=0; j<Nage; ++j) {
                GridData(i,j) = nmfUtils::round(std::stod(dataMap["TotCons"][m++]),1)/1000.0;
            } // end for j
        } // end for i

    } else if (selectedVariable == "Consumption by Prey Type") {        // Consumption Rates

        if (selectedByVariables == "Annual") {
            title = "Total Prey Biomass Consumed by";
        } else if (selectedByVariables == "Seasonal") {
            title = "Total Prey Biomass Consumed by";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        ageStr = " and PredAge = " + std::to_string(SpeAge-offset);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::RearrangeTitle,
                              "Biomass (000 Metric Tons)", 1.0,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData,ChartData.size1(),ChartData.size2());
        GridData = ChartData;


    } else if (selectedVariable == "Cons. by Pred. Age and Prey Type") {// Consumption Rates

        if (selectedByVariables == "Annual") {
            title = "Total Prey Biomass Consumed by";
        } else if (selectedByVariables == "Seasonal") {
            title = "Total Prey Biomass Consumed by";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        ageStr = " and PredAge = " + std::to_string(SpeAge-offset);
        AllLabels["selectSpeciesAgeSizeClassLBL"]->setEnabled(true);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::RearrangeTitle,
                              "Biomass (000 Metric Tons)", 1.0,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData,ChartData.size1(),ChartData.size2());
        GridData = ChartData;

    } else if (selectedVariable == "Consumption by Prey Age") {         // Consumption Rates

        if (selectedByVariables == "Annual") {
            title = selectedPreyName + " Biomass Consumed by";
        } else if (selectedByVariables == "Seasonal") {
            title = selectedPreyName + " Biomass Consumed by";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        ageStr = " and PredAge = " + std::to_string(SpeAge-offset);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);
        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, selectedPreyName,
                              FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::RearrangeTitle,
                              "Biomass (000 Metric Tons)", 1.0,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData,ChartData.size1(),ChartData.size2());
        GridData = ChartData;

    } else if (selectedVariable == "Wt. Specific Consumption") {        // Consumption Rates

        title = "Weight specific daily consumption by";
        titleSuffix = selectedSeason;
        seasonVal = season;
        seasonStr = " and Season = " + std::to_string(seasonVal);
        AllLabels["selectSeasonLBL"]->setEnabled(true);
        AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
        AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        ageStr = " and Age = " + std::to_string(SpeAge-offset);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              ageStr, "", seasonStr, seasonVal, "", FirstYear, LastYear, SpeAge-offset, chart,
                              title, titleSuffix, nmfConstants::RearrangeTitle,
                              "gPrey per gBW per Day", 1.0,
                              YMaxOverride, rowLabels, colLabels);

        // Use the GridData that's returned from queryDataAndLoadChart(...)

    } else if (selectedVariable == "Relative Food Availability") {      // Food Availability

        colLabels << "Rel. FA";

        if (selectedByVariables == "Annual") {
            title = "Relative Food Availability for";
        } else if (selectedByVariables == "Seasonal") {
            title = "Relative Food Availability for";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        predAgeStr = " and PredAge = " + std::to_string(SpeAge-offset);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr,
                              ChartData, GridData,
                              theModelName,
                              ForecastName,
                              ScenarioName,
                              Forecast_FirstYear,
                              Forecast_NYears,
                              NYears, Nage,
                              fieldToSum,
                              MSVPAName,
                              selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              predAgeStr, "", seasonStr, seasonVal, selectedPreyName,
                              FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::RearrangeTitle,
                              "Relative Food Availability", 1.0,
                              YMaxOverride, rowLabels, colLabels);

    } else if (selectedVariable == "Per Capita Food Availability") {    // Food Availability
        colLabels << "Rel. FA";

        if (selectedByVariables == "Annual") {
            title = "Per Capita Relative Food Availability for";
        } else if (selectedByVariables == "Seasonal") {
            title = "Per Capita Relative Food Availability for";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        predAgeStr = " and PredAge = " + std::to_string(SpeAge-offset);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              predAgeStr, "", seasonStr, seasonVal, selectedPreyName,
                              FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::RearrangeTitle,
                              "Relative Food Availability", 1.0,
                              YMaxOverride, rowLabels, colLabels);


    } else if (selectedVariable == "Food Availability by Prey Age") {   // Food Availability

        if (selectedByVariables == "Annual") {
            title = "Availability of";
        } else if (selectedByVariables == "Seasonal") {
            title = "Availability of: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        predAgeStr = " and PredAge = " + std::to_string(SpeAge-offset);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              predAgeStr, "", seasonStr, seasonVal, selectedPreyName,
                              FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::RearrangeTitle,
                              "Relative Food Availability", 1.0,
                              YMaxOverride, rowLabels, colLabels);

    } else if (selectedVariable == "Stomach Content Weight") {          // Food Availability
        // Average stomach contants at age
        colLabels << "Energy Density";

        if (selectedByVariables == "Annual") {
            title = "Average Stomach Contents";
        } else if (selectedByVariables == "Seasonal") {
            title = "Average Stomach Contents for";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        predAgeStr = " and Age = " + std::to_string(SpeAge-offset);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              predAgeStr, "", seasonStr, seasonVal, selectedPreyName,
                              FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::RearrangeTitle,
                              "Stomach Content", 1.0,
                              YMaxOverride, rowLabels, colLabels);

    } else if (selectedVariable == "Average Weight") {                  // Gowth

        colLabels << "Weight";

        if (selectedByVariables == "Annual") {
            title = "Average Weight at Age";
        } else if (selectedByVariables == "Seasonal") {
            title = "Average Mid-Season Weight at Age: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              "", "", seasonStr, seasonVal, selectedPreyName,
                              FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "WtUnitString", 1.0,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData,ChartData.size1(),ChartData.size2());
        GridData = ChartData;

    } else if (selectedVariable == "Average Size") {                    // Gowth

        colLabels << "Size";

        if (selectedByVariables == "Annual") {
            title = "Average Size at Age";
        } else if (selectedByVariables == "Seasonal") {
            title = "Average Mid-Season Size at Age: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              "", "", seasonStr, seasonVal, selectedPreyName,
                              FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "SizeUnitString", 1.0,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData,ChartData.size1(),ChartData.size2());
        GridData = ChartData;

    } else if (selectedVariable == "Weight at Age") {                   // Gowth

        colLabels << "Weight";

        if (selectedByVariables == "Annual") {
            title = "Weight at Age";
        } else if (selectedByVariables == "Seasonal") {
            title = "Mid-Season Weight at Age: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        predAgeStr = " and Age = " + std::to_string(SpeAge-offset);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              predAgeStr, "", seasonStr, seasonVal, selectedPreyName,
                              FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "WtUnitString", 1.0,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData,ChartData.size1(),ChartData.size2());
        GridData = ChartData;

    } else if (selectedVariable == "Size at Age") {                     // Growth

        colLabels << "Size";

        if (selectedByVariables == "Annual") {
            title = "Size at Age";
        } else if (selectedByVariables == "Seasonal") {
            title = "Mid-Season Size at Age: ";
            titleSuffix = selectedSeason;
            seasonVal = season;
            seasonStr = " and Season = " + std::to_string(seasonVal);
            AllLabels["selectSeasonLBL"]->setEnabled(true);
            AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
            AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
        }
        predAgeStr = " and Age = " + std::to_string(SpeAge-offset);
        AllComboBoxes["selectSpeciesAgeSizeClassCMB"]->setEnabled(true);
        AllCheckBoxes["selectSpeciesAgeSizeClassCB"]->setEnabled(true);

        queryDataAndLoadChart(caller, databasePtr, ChartData, GridData, theModelName,
                              ForecastName, ScenarioName, Forecast_FirstYear, Forecast_NYears,
                              NYears, Nage,
                              fieldToSum, MSVPAName, selectedSpecies, selectedSpeciesNum, selectedVariable, selectedByVariables,
                              predAgeStr, "", seasonStr, seasonVal, selectedPreyName,
                              FirstYear, LastYear, SpeAge, chart,
                              title, titleSuffix, nmfConstants::DontRearrangeTitle,
                              "SizeUnitString", 1.0,
                              YMaxOverride, rowLabels, colLabels);

        nmfUtils::initialize(GridData,ChartData.size1(),ChartData.size2());
        GridData = ChartData;
    }

    // Set grid line visibility
    chart->axes(Qt::Horizontal).back()->setGridLineVisible(vGridLine);
    chart->axes(Qt::Vertical).back()->setGridLineVisible(hGridLine);

} // end redrawChart


//void
//nmfOutputChartBar::Forecast_Growth_AverageWeight(
//        std::string caller,
//        nmfDatabase* databasePtr,
//        QChart* chart,
//        std::string MSVPAName,
//        std::string ForecastName,
//        std::string ScenarioName,
//        int Forecast_NYears,
//        int Forecast_FirstYear,
//        int FirstYear,
//        int NumYears, // These are years selected from the gui
//        boost::numeric::ublas::vector<int> Years,
//        boost::numeric::ublas::vector<std::string> LegendNames,
//        int Nage,
//        int RecAge,
//        std::string seasonStr,
//        std::string selectedSpecies,
//        double YMaxOverride)
//{

//} // end Forecast_Growth_AverageWeight
