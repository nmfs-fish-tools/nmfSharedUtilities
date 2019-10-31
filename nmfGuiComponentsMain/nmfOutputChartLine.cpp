
#include "nmfConstants.h"
#include "nmfOutputChartLine.h"


nmfOutputChartLine::nmfOutputChartLine(nmfLogger *theLogger):
    nmfOutputChartBase()
{
    logger = theLogger;

    FunctionMap["MSVPA_Multispecies Populations_Total Biomass"]             = MSVPA_MultispeciesPopulations_TotalBiomass;
    FunctionMap["MSVPA_Multispecies Populations_Total Abundance"]           = MSVPA_MultispeciesPopulations_TotalAbundance;
    FunctionMap["MSVPA_Multispecies Populations_Age 1+ Biomass"]            = MSVPA_MultispeciesPopulations_Age1PlusBiomass;
    FunctionMap["MSVPA_Multispecies Populations_Age 1+ Abundance"]          = MSVPA_MultispeciesPopulations_Age1PlusAbundance;
    FunctionMap["MSVPA_Multispecies Populations_Spawning Stock Biomass"]    = MSVPA_MultispeciesPopulations_SpawningStockBiomass;
    FunctionMap["MSVPA_Yield Per Recruit_YPR vs. F"]                        = MSVPA_YieldPerRecruit_YPRvsF;
    FunctionMap["MSVPA_Yield Per Recruit_SSB vs. F"]                        = MSVPA_YieldPerRecruit_SSBvsF;
    FunctionMap["MSVPA_Yield Per Recruit_Historical YPR"]                   = MSVPA_YieldPerRecruit_HistoricalYPR;
    FunctionMap["MSVPA_Yield Per Recruit_Historical F Benchmarks"]          = MSVPA_YieldPerRecruit_HistoricalFBenchmarks;
    FunctionMap["MSVPA_Yield Per Recruit_Historical SSB Benchmarks"]        = MSVPA_YieldPerRecruit_HistoricalSSBBenchmarks;

    FunctionMap["Forecast_Multispecies Populations_Total Biomass"]          = Forecast_MultispeciesPopulations_TotalBiomass;
    FunctionMap["Forecast_Multispecies Populations_Total Abundance"]        = Forecast_MultispeciesPopulations_TotalAbundance;
    FunctionMap["Forecast_Multispecies Populations_Age 1+ Biomass"]         = Forecast_MultispeciesPopulations_Age1PlusBiomass;
    FunctionMap["Forecast_Multispecies Populations_Age 1+ Abundance"]       = Forecast_MultispeciesPopulations_Age1PlusAbundance;
    FunctionMap["Forecast_Multispecies Populations_Spawning Stock Biomass"] = Forecast_MultispeciesPopulations_SpawningStockBiomass;
    FunctionMap["Forecast_Yield Per Recruit_YPR vs. F"]                     = Forecast_YieldPerRecruit_YPRvsF;
    FunctionMap["Forecast_Yield Per Recruit_SSB vs. F"]                     = Forecast_YieldPerRecruit_SSBvsF;
    FunctionMap["Forecast_Yield Per Recruit_Projected YPR"]                 = Forecast_YieldPerRecruit_ProjectedYPR;
    FunctionMap["Forecast_Yield Per Recruit_Projected F Benchmarks"]        = Forecast_YieldPerRecruit_ProjectedFBenchmarks;
    FunctionMap["Forecast_Yield Per Recruit_Projected SSB Benchmarks"]      = Forecast_YieldPerRecruit_ProjectedSSBBenchmarks;
}


void
nmfOutputChartLine::loadChartWithData(
        QChart *chart,
        double YMaxOverride,
        std::string species,
        boost::numeric::ublas::matrix<double> &ChartData,
        boost::numeric::ublas::vector<std::string> &LegendNames,
        std::string mainTitle,
        std::string xTitle,
        std::string yTitle,
        std::vector<std::string> xLabels,
        bool rotateLabels)
{
    QLineSeries *series;

    chart->removeAxis(chart->axisY());
    chart->removeAxis(chart->axisX());

    // Draw main chart title
    std::stringstream ss;
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    std::string delim = (species.empty()) ? "" : " - ";
    ss << species << delim << mainTitle;
    chart->setTitle(ss.str().c_str());

    // Draw legend on right
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);


    // Create a new X axis, add labels and the title
    QCategoryAxis *axisX = new QCategoryAxis();
    int NumXLabels = xLabels.size();
    for (int i=0; i<NumXLabels; ++i) {
        axisX->append(xLabels[i].c_str(), i+1);
    }
   // axisX->setRange(0, NumXLabels);
    if ((NumXLabels > nmfConstants::NumCategoriesForVerticalNotation) && (rotateLabels))
        axisX->setLabelsAngle(-90);
    else
        axisX->setLabelsAngle(0);
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    // Create, scale, and label Y axis.
    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%.2f");
    axisY->setMin(0.0);
    for (unsigned int line=0; line<ChartData.size2(); ++line) {
        series = new QLineSeries;
        for (unsigned int val=0; val< ChartData.size1(); ++val) {
            series->append(val+1,ChartData(val,line));
        }
        chart->addSeries(series);
        series->setName(QString::fromStdString(LegendNames(line)));
        chart->setAxisY(axisY,series);
    }
    axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    if (YMaxOverride > 0.0) {
        axisY->setMax(YMaxOverride); // YMax); // *1.2);
        axisY->setMin(0.0);
    } else {
        axisY->applyNiceNumbers(); // Adjusting yaxis to be a rounder number, i.e. 838 should be 1000
        //axisY->setMax(ymax);
        axisY->setMin(0.0);
    }

    // Set title on X axis
    chart->setAxisX(axisX, series);
    QFont titleFont = axisX->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    axisX->setTitleFont(titleFont);
    axisX->setTitleText(xTitle.c_str());
    axisY->setTitleFont(titleFont);
    axisY->setTitleText(yTitle.c_str());

} // end loadChartWithData


void nmfOutputChartLine::setTitles(
        QChart *chart,
        QLineSeries *series, QStringList categories,
        std::string species, std::string mainTitle,
        std::vector<std::string> &xLabels, std::vector<std::string> &yLabels,
        std::string xTitle, std::string yTitle,
        double min, double max, double inc,
        std::string suffix)
{
    // Draw main chart title
    std::stringstream ss;
    QFont mainTitleFont = chart->titleFont();
    mainTitleFont.setPointSize(14);
    mainTitleFont.setWeight(QFont::Bold);
    chart->setTitleFont(mainTitleFont);
    ss << species << " - " << mainTitle;
    chart->setTitle(ss.str().c_str());

    // Draw legend on right
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    // Remove the previous X axis
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());


    // Create a new X axis, add labels and the title
    QCategoryAxis *axisX = new QCategoryAxis();
    int NumXLabels = xLabels.size();
    for (int i=0; i<NumXLabels; ++i) {
        axisX->append(xLabels[i].c_str(), i+1);
    }
    axisX->setRange(0, NumXLabels);
    if (NumXLabels > nmfConstants::NumCategoriesForVerticalNotation)
        axisX->setLabelsAngle(-90);
    else
        axisX->setLabelsAngle(0);
    chart->setAxisX(axisX, series);
    QFont titleFont = axisX->titleFont();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    axisX->setTitleFont(titleFont);
    axisX->setTitleText(xTitle.c_str());


    // Create a new Y axis, add labels and the title
    QCategoryAxis *axisY = new QCategoryAxis();
    int NumYLabels = yLabels.size();
    for (int i=0; i<NumYLabels; ++i) {
        axisY->append(yLabels[i].c_str(),(i+1));
    }
    axisY->setRange(0, NumYLabels);
    chart->setAxisY(axisY, series);
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::Bold);
    axisY->setTitleFont(titleFont);
    axisY->setTitleText(xTitle.c_str());

}




// Public function to calculate YPR given an array of weights at age, M at age, and F at Age
double nmfOutputChartLine::YPR(boost::numeric::ublas::vector<double> &WeightAtAge,
        boost::numeric::ublas::vector<double> &FatAge,
        boost::numeric::ublas::vector<double> &M2atAge,
        double FullF,
        boost::numeric::ublas::vector<double> &M1,
        int Nage)
{
    double Death;   // for death rate
    double PropF;   // for F/Z
    double TempVal; // hold sum through iterations
    double NAlive;  // holds the product for number alive at age t
    double Z;       // mortality as sum of M1 + M2 + F
    double M2;

    //FatAge holds PRF values

    TempVal = 0.0;
    NAlive = 1.0;
//std::cout << "-> -------------------------------------" << std::endl;
    for (int i = 0; i < Nage; ++i) {
        M2 = (M2atAge(i) < 0) ? 0 : M2atAge(i);
//std::cout << "-> M2: " << M2 << std::endl;
        Z = (FatAge(i) * FullF) + M2 + M1(i);        // total mortality
//std::cout << "-> FatAge(i): " << FatAge(i) << std::endl;
//std::cout << "-> FullF: " << FullF << std::endl;
//std::cout << "-> M1(i): " << M1(i) << std::endl;
//std::cout << "-> Z: " << Z << std::endl;
        Death = NAlive * (1 - std::exp(-Z));         // death rate
//std::cout << "-> Death: " << Death << std::endl;
        NAlive *= std::exp(-Z);                      // number alive at age - accumulating product
//std::cout << "-> NAlive: " << NAlive << std::endl;

        PropF = (FatAge(i) * FullF) / Z;             // proportion of Z due to fishing
        TempVal += (Death * PropF * WeightAtAge(i)); // accumulates yield at age
    }

    return TempVal;
} // end YPR


double nmfOutputChartLine::F01(boost::numeric::ublas::vector<double> &WeightAtAge,
        boost::numeric::ublas::vector<double> &FatAge,
        boost::numeric::ublas::vector<double> &M2atAge,
        boost::numeric::ublas::vector<double> &M1,
        double Nage)
{

    double YPR0;
    double YPR1;
    double Slope;
    double SlopeRef;
    double F0;
    double Diff;

    // Find F0.1
    // First calculate slope at origin - Slope0
    YPR1 = YPR(WeightAtAge, FatAge, M2atAge, 0.001, M1, Nage);
    YPR0 = YPR(WeightAtAge, FatAge, M2atAge, 0.0, M1, Nage);

    // Ref slope is 10% of slope at origin
    SlopeRef = ((YPR1 - YPR0) / 0.001) * 0.1;
    Slope = (YPR1 - YPR0) / 0.001; // this is the slope at the origin
    Diff = Slope - SlopeRef; // this is the difference to minimize
    F0 = 0.001;
    while (Diff > 0.001) {

        YPR1 = YPR(WeightAtAge, FatAge, M2atAge, F0 + 0.001, M1, Nage);
        YPR0 = YPR(WeightAtAge, FatAge, M2atAge, F0, M1, Nage);

        Slope = (YPR1 - YPR0) / 0.001;
        Diff = Slope - SlopeRef; // this is the difference to minimize

        F0 += 0.001;
    }

    return (F0-0.001);

} // end F01


double nmfOutputChartLine::FMax(boost::numeric::ublas::vector<double> &WeightAtAge,
        boost::numeric::ublas::vector<double> &FatAge,
        boost::numeric::ublas::vector<double> &M2atAge,
        boost::numeric::ublas::vector<double> &M1,
        double Nage)
{
    double YPR0;
    double YPR1;
    double Slope;
    double F0;

    // Calculate Fmax
    YPR1 = YPR(WeightAtAge, FatAge, M2atAge, 0.001, M1, Nage);
    YPR0 = YPR(WeightAtAge, FatAge, M2atAge, 0.0, M1, Nage);

    Slope = (YPR1 - YPR0) / 0.001; // this is the slope at the origin
    F0 = 0.001;
    bool done=false;
    while ((Slope > 0) and (! done)) {

        YPR1 = YPR(WeightAtAge, FatAge, M2atAge, F0 + 0.001, M1, Nage);
        YPR0 = YPR(WeightAtAge, FatAge, M2atAge, F0, M1, Nage);

        Slope = (YPR1 - YPR0) / 0.001;
        F0 += 0.001;

        // Put in a break to bounce it out if F gets bigger than 4....max Fmax undefined
        // this will at least prevent infinite loops...
        done = (F0 > 4);
    }

    return (F0 < 4) ? (F0-0.001) : -9;
} // end FMax


double nmfOutputChartLine::SSB(boost::numeric::ublas::vector<double> &WeightAtAge,
        boost::numeric::ublas::vector<double> &FatAge,
        boost::numeric::ublas::vector<double> &M2atAge,
        double FullF,
        boost::numeric::ublas::vector<double> &M1,
        int &Nage,
        boost::numeric::ublas::vector<double> &Pmature)
{
    double Z;  // mortality as sum of M + F
    double M2;
    double SSBtemp = 0.0; // hold sum through iterations
    double NAlive = 1.0;  // holds the product for number alive at age t

    for (int i = 0; i < Nage; ++i) {
        M2 = (M2atAge(i) < 0) ? 0 : M2atAge(i);
        Z = M1(i) + M2 + (FullF * FatAge(i));  // total mortality for that age
        NAlive *= std::exp(-Z);  // number alive at age - accumulating product
        SSBtemp += (NAlive * WeightAtAge(i) * Pmature(i)); //  accumulates SSB at age
    } // end for i

    return SSBtemp;
} // end SSB


double nmfOutputChartLine::SSBBench(boost::numeric::ublas::vector<double> &WeightAtAge,
        boost::numeric::ublas::vector<double> &FatAge,
        boost::numeric::ublas::vector<double> &M2atAge,
        boost::numeric::ublas::vector<double> &M1,
        int &Nage,
        boost::numeric::ublas::vector<double> &Pmature,
        double BenchVal)
{
    double SSBtemp;
    double SSBMax;
    double F0 = 0.001;
    double Ratio;

    // Find F giving SSB/R = benchval of max
    SSBMax  = SSB(WeightAtAge, FatAge, M2atAge, 0, M1, Nage, Pmature);
    SSBtemp = SSBMax;
    Ratio   = SSBtemp / SSBMax;

    // RSK check this logic for potential of infinite loop
    while (Ratio > BenchVal) {
        SSBtemp = SSB(WeightAtAge, FatAge, M2atAge, F0, M1, Nage, Pmature);
        Ratio = SSBtemp / SSBMax;
        F0 += 0.001;
    }

    return (F0 - 0.001);

} // end SSBBench


void nmfOutputChartLine::calculateWeightAveFAndAssignPRFs(
            const int &NumYears,
            const int &RecAge,
            const int &Nage,
            boost::numeric::ublas::matrix<double> &FatAge,
            boost::numeric::ublas::matrix<double> &Catch,
            boost::numeric::ublas::vector<double> &FullF,
            boost::numeric::ublas::matrix<double> &PRF)
{
    double tmpSum;
    double tmpTotCat;

    for (int i=0; i<NumYears; ++i) {
        tmpSum = 0.0;
        tmpTotCat = 0.0;
        for (int j=RecAge; j<Nage; ++j) {
            tmpSum    += (FatAge(i,j) * Catch(i,j));
            tmpTotCat += Catch(i,j);
        }
        FullF(i) = tmpSum / tmpTotCat;
    } // end i

    // Assign PRFs to unrecruited age classes
    for (int i = 0; i < NumYears; ++i) {
        for (int j = 0; j < RecAge; ++j) {
            PRF(i, j) = FatAge(i, j) / FullF(i);
        }
        for (int j = RecAge; j < Nage; ++j) {
            PRF(i, j) = 1;
        }
    }

} // end calculateWeightAveFAndAssignPRFs







void nmfOutputChartLine::redrawChart(
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
    int m = 0;
    int Nage;
    int NumYears=0;
    int RecAge=0;
    std::string str1,str2;
    std::string seasonStr;
    std::string newSelectedFullyRecruitedAge = selectedFullyRecruitedAge;
    boost::numeric::ublas::vector<int> Years;
    boost::numeric::ublas::vector<std::string> LegendNames;
    std::string functionName = "";
    int MSVPA_FirstYear=0;
    int MSVPA_LastYear=0;
    int MSVPA_NYears=0;
    int MSVPA_NSeasons=0;

    if (MSVPAName == "")
        return; // Means that user hasn't yet chosen an MSVPA configuration

    if (selectedVariable.empty()) {
        logger->logMsg(nmfConstants::Normal,
                          theModelName + " " +
                          "nmfOutputChartLine::redrawChart: "+
                          selectedDataType + ", " +
                          selectedYPRAnalysisType);
    } else {
        logger->logMsg(nmfConstants::Normal,
                          theModelName + " " +
                          "nmfOutputChartLine::redrawChart: "+
                          selectedDataType + ", " +
                          selectedVariable + ", " +
                          selectedByVariables);
    }

    // Get initial MSVPA and Forecast data
    databasePtr->nmfGetMSVPAInitData(MSVPAName,
        MSVPA_FirstYear, MSVPA_LastYear, MSVPA_NYears, MSVPA_NSeasons);

    nmfUtils::split(selectedSeason," ",str1,str2);
    seasonStr = " AND Season = " + std::to_string(std::stoi(str2)-1);

    // Set widget states
    AllComboBoxes["selectSeasonCMB"]->setEnabled(false);
    AllCheckBoxes["selectSeasonCB"]->setEnabled(false);
    if (selectedByVariables == "Seasonal") {
        AllComboBoxes["selectSeasonCMB"]->setEnabled(true);
        AllCheckBoxes["selectSeasonCB"]->setEnabled(true);
    }
    Nage = AllComboBoxes["selectFullyRecruitedAgeCMB"]->count();

    chart->removeAllSeries();

    //
    // Call the appropriate function by concatenating theModelName and the MenuItem
    // i.e. MSVPA_MultispeciesPopulations or Forecast_Yield Per Recruit_Projected SSB Benchmarks

    bool isYPRAnalysisTypeCMBHidden = AllComboBoxes["selectYPRAnalysisTypeCMB"]->isHidden();
    functionName = (isYPRAnalysisTypeCMBHidden) ?
        theModelName + "_" + selectedDataType + "_" + selectedVariable:
        theModelName + "_" + selectedDataType + "_" + selectedYPRAnalysisType;

    // Find recruit age (not used in all called functions).
    if (! newSelectedFullyRecruitedAge.empty() ) {
        nmfUtils::split(newSelectedFullyRecruitedAge," ",str1,str2);
        RecAge = std::stoi(str2);
    }

    // Find selected years and make legend names (also not used in all called functions).
    if ((theModelName == "Forecast") || (! isYPRAnalysisTypeCMBHidden)){
        NumYears = selectedYears.size();
        nmfUtils::initialize(Years, NumYears);
        nmfUtils::initialize(LegendNames, NumYears);
        for (int i=0; i<NumYears; ++i) {
            Years(i) = std::stoi(selectedYears[i]);
            LegendNames(i) = std::to_string(Years(i));
        }
    } else if (theModelName == "MSVPA") {
        NumYears = MSVPA_NYears;
        nmfUtils::initialize(Years, NumYears);
        nmfUtils::initialize(LegendNames, NumYears);
        for (int i=0; i<NumYears; ++i) {
            Years(i) = FirstYear + i;
            LegendNames(i) = std::to_string(Years(i));
        }
    }

    if (FunctionMap.find(functionName) == FunctionMap.end()) {

        std::cout << "Error: \"" << functionName << "\" not found in function map." << std::endl;

    } else {

        FunctionMap[functionName](
                databasePtr,
                chart,
                theModelName,
                MSVPAName,
                ForecastName,
                ScenarioName,
                Forecast_NYears,
                Forecast_FirstYear,
                FirstYear,
                NumYears,
                Years,
                LegendNames,
                Nage,
                RecAge,
                seasonStr,
                selectedSpecies,
                YMaxForAllAges,
                rowLabels,
                colLabels,
                GridData);

    }

    // Set grid line visibility
    chart->axisX()->setGridLineVisible(vGridLine);
    chart->axisY()->setGridLineVisible(hGridLine);

} // end redrawChart



void
nmfOutputChartLine::getMSVPASpecies(
        nmfDatabase* databasePtr,
        const std::string &MSVPAName,
        int &NMSVPASpecies,
        boost::numeric::ublas::vector<std::string> &MSVPASpeList)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    // Load all msvpa species and ages
    fields = {"SpeName"};
    queryStr = "SELECT SpeName FROM MSVPAspecies WHERE MSVPAname = '" + MSVPAName + "'" +
               " AND (Type = 0 or Type = 1)";
    dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
    NMSVPASpecies = dataMap["SpeName"].size();
    nmfUtils::initialize(MSVPASpeList,NMSVPASpecies);

    for (int i=0;i<NMSVPASpecies; ++i) {
        MSVPASpeList(i) = dataMap["SpeName"][i];
    }
}


void
nmfOutputChartLine::getMSVPASpeciesNamesAges(
        nmfDatabase* databasePtr,
        const int &NMSVPASpe,
        boost::numeric::ublas::vector<std::string> &MSVPASpeList,
        std::vector<int> &MSVPASpeAge)

{
    std::string queryStr,queryStr2;
    std::map<std::string, std::vector<std::string> > dataMap,dataMap2;
    std::vector<std::string> fields,fields2;

    for (int i=0; i<NMSVPASpe; ++i) {
        fields2 = {"MaxAge"};
        queryStr2 = "SELECT MaxAge FROM Species WHERE SpeName = '" + MSVPASpeList(i) + "'";
        dataMap2 = databasePtr->nmfQueryDatabase(queryStr2, fields2);
        MSVPASpeAge.push_back( std::stoi(dataMap2["MaxAge"][0]) );
    }
}


void
nmfOutputChartLine::getMaturity(
        nmfDatabase* databasePtr,
        const int &Forecast_NYears,
        const int &FirstYear,
        const int &NMSVPASpe,
        const boost::numeric::ublas::vector<std::string> &MSVPASpeList,
        const std::vector<int> &MSVPASpeAge,
        Boost3DArrayDouble &Maturity)
{
    int m = 0;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    // Get Maturity data....RSK possibly make this an nmfDatabase function
    for (int i = 0; i < NMSVPASpe; ++i) {
        fields = {"PMature"};
        queryStr = "SELECT PMature FROM SpeMaturity WHERE SpeName = '" + MSVPASpeList(i) + "'" +
                   " AND Year = " + std::to_string(FirstYear) +
                   " ORDER By Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        m = 0;
        for (int j = 0; j <= MSVPASpeAge[i]; ++j) {
            for (int k = 0; k < Forecast_NYears; ++k) {
                Maturity[i][j][k] = std::stod(dataMap["PMature"][m]);
            }
            ++m;
        }
    }
} // end getMaturity



void
nmfOutputChartLine::Forecast_MultispeciesPopulations_TotalBiomass(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int NMSVPASpe=0;

    double YMax = 0.0; // RSK remove this????
    std::vector<std::string> XLabelNames;
    boost::numeric::ublas::matrix<double> ChartData;
    boost::numeric::ublas::vector<std::string> MSVPASpeList;
    std::vector<int> MSVPASpeAge;
    Boost3DArrayDouble Maturity(
        boost::extents[nmfConstants::MaxTotalSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    getMSVPASpecies(databasePtr,MSVPAName,NMSVPASpe,MSVPASpeList);
    getMSVPASpeciesNamesAges(databasePtr,NMSVPASpe,MSVPASpeList,MSVPASpeAge);
    getMaturity(databasePtr,Forecast_NYears,FirstYear,NMSVPASpe,MSVPASpeList,MSVPASpeAge,Maturity);

    nmfUtils::initialize(ChartData, Forecast_NYears, NMSVPASpe);
    nmfUtils::initialize(GridData,  Forecast_NYears, NMSVPASpe);

    for (int j = 0; j < Forecast_NYears; ++j) {
        rowLabels << QString::number(Forecast_FirstYear+j);
    }

    for (int i = 0; i < NMSVPASpe; ++i) {
        colLabels << QString::fromStdString(MSVPASpeList(i));
        fields = {"Year","Biomass"};
        queryStr = "SELECT Year, Sum(InitBiomass) AS Biomass FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '" + MSVPASpeList(i) + "'" +
                  seasonStr +
                " GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int j = 0; j < Forecast_NYears; ++j) {
            XLabelNames.push_back(std::to_string(Forecast_FirstYear + j));
            ChartData(j,i) = std::stod(dataMap["Biomass"][j]) / 1000.0; // RSK replace with constant
        }
    }
    GridData = ChartData;

    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      ChartData,
                      MSVPASpeList,
                      "Total Biomass (Jan 1.) for MSVPA Species",
                      "Year",              // x title
                      "000 Metric Tons",   // y title
                      XLabelNames,
                      (Forecast_NYears > nmfConstants::NumCategoriesForVerticalNotation));

} // end Forecast_MultispeciesPopulations_TotalBiomass


void nmfOutputChartLine::Forecast_MultispeciesPopulations_TotalAbundance(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int NMSVPASpe=0;

    double YMax = 0.0; // RSK remove this????
    std::vector<std::string> XLabelNames;
    boost::numeric::ublas::matrix<double> ChartData;
    boost::numeric::ublas::vector<std::string> MSVPASpeList;
    std::vector<int> MSVPASpeAge;
    Boost3DArrayDouble Maturity(
        boost::extents[nmfConstants::MaxTotalSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    getMSVPASpecies(databasePtr,MSVPAName,NMSVPASpe,MSVPASpeList);
    getMSVPASpeciesNamesAges(databasePtr,NMSVPASpe,MSVPASpeList,MSVPASpeAge);
    getMaturity(databasePtr,Forecast_NYears,FirstYear,NMSVPASpe,MSVPASpeList,MSVPASpeAge,Maturity);

    nmfUtils::initialize(ChartData, Forecast_NYears, NMSVPASpe);
    nmfUtils::initialize(GridData,  Forecast_NYears, NMSVPASpe);

    for (int j = 0; j <  Forecast_NYears; ++j) {
        rowLabels << QString::number(Forecast_FirstYear+j);
    }

    for (int i = 0; i < NMSVPASpe; ++i) {
        colLabels << QString::fromStdString(MSVPASpeList(i));
        fields = {"Year","Abundance"};
        queryStr = "SELECT Year, Sum(InitAbund) AS Abundance FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '" + MSVPASpeList(i) + "'" +
                  seasonStr +
                " GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int j = 0; j <  Forecast_NYears; ++j) {
            XLabelNames.push_back(std::to_string(Forecast_FirstYear + j));
            ChartData(j,i) = std::stod(dataMap["Abundance"][j]) / 1000.0; // RSK replace with constant
        }
     }

    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      ChartData,
                      MSVPASpeList,
                      "Total Abundance (Jan 1.) of MSVPA Species",
                      "Year",               // x title
                      "Millions of Fish",   // y title
                      XLabelNames,
                      (Forecast_NYears > nmfConstants::NumCategoriesForVerticalNotation));

    GridData = ChartData;

} // end Forecast_MultispeciesPopulations_TotalAbundance


void nmfOutputChartLine::Forecast_MultispeciesPopulations_Age1PlusBiomass(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int NMSVPASpe=0;

    double YMax = 0.0; // RSK remove this????
    std::vector<std::string> XLabelNames;
    boost::numeric::ublas::matrix<double> ChartData;
    boost::numeric::ublas::vector<std::string> MSVPASpeList;
    std::vector<int> MSVPASpeAge;
    Boost3DArrayDouble Maturity(
        boost::extents[nmfConstants::MaxTotalSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    getMSVPASpecies(databasePtr,MSVPAName,NMSVPASpe,MSVPASpeList);
    getMSVPASpeciesNamesAges(databasePtr,NMSVPASpe,MSVPASpeList,MSVPASpeAge);
    getMaturity(databasePtr,Forecast_NYears,FirstYear,NMSVPASpe,MSVPASpeList,MSVPASpeAge,Maturity);

    nmfUtils::initialize(ChartData, Forecast_NYears, NMSVPASpe);
    nmfUtils::initialize(GridData,  Forecast_NYears, NMSVPASpe);

    for (int j = 0; j <  Forecast_NYears; ++j) {
        rowLabels << QString::number(Forecast_FirstYear+j);
    }

    for (int i = 0; i < NMSVPASpe; ++i) {
        colLabels << QString::fromStdString(MSVPASpeList(i));

        fields = {"Year","Biomass"};
        queryStr = "SELECT Year, Sum(InitBiomass) AS Biomass FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '" + MSVPASpeList(i) + "'" +
                  seasonStr +
                " AND Age >= 1 GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int j = 0; j <  Forecast_NYears; ++j) {
            XLabelNames.push_back(std::to_string(Forecast_FirstYear + j));
            ChartData(j,i) = std::stod(dataMap["Biomass"][j]) / 1000.0; // RSK replace with constant
        }
    }

    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      ChartData,
                      MSVPASpeList,
                      "Age 1+ Biomass (Jan 1.) for MSVPA Species",
                      "Year",              // x title
                      "000 Metric Tons",   // y title
                      XLabelNames,
                      (Forecast_NYears > nmfConstants::NumCategoriesForVerticalNotation));

    GridData = ChartData;

} // end Forecast_MultispeciesPopulations_Age1PlusBiomass


void nmfOutputChartLine::Forecast_MultispeciesPopulations_Age1PlusAbundance(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int NMSVPASpe=0;

    double YMax = 0.0; // RSK remove this????
    std::vector<std::string> XLabelNames;
    boost::numeric::ublas::matrix<double> ChartData;
    boost::numeric::ublas::vector<std::string> MSVPASpeList;
    std::vector<int> MSVPASpeAge;
    Boost3DArrayDouble Maturity(
        boost::extents[nmfConstants::MaxTotalSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    getMSVPASpecies(databasePtr,MSVPAName,NMSVPASpe,MSVPASpeList);
    getMSVPASpeciesNamesAges(databasePtr,NMSVPASpe,MSVPASpeList,MSVPASpeAge);
    getMaturity(databasePtr,Forecast_NYears,FirstYear,NMSVPASpe,MSVPASpeList,MSVPASpeAge,Maturity);

    nmfUtils::initialize(ChartData, Forecast_NYears, NMSVPASpe);
    nmfUtils::initialize(GridData,  Forecast_NYears, NMSVPASpe);

    for (int j = 0; j <  Forecast_NYears; ++j) {
        rowLabels << QString::number(Forecast_FirstYear+j);
    }

    for (int i = 0; i < NMSVPASpe; ++i) {
        colLabels << QString::fromStdString(MSVPASpeList(i));

        fields = {"Year","Abundance"};
        queryStr = "SELECT Year, Sum(InitAbund) AS Abundance FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '" + MSVPASpeList(i) + "'" +
                  seasonStr +
                " AND Age >= 1 GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int j = 0; j <  Forecast_NYears; ++j) {
            XLabelNames.push_back(std::to_string(Forecast_FirstYear + j));
            ChartData(j,i) = std::stod(dataMap["Abundance"][j]) / 1000.0; // RSK replace with constant
        }
    }

    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      ChartData,
                      MSVPASpeList,
                      "Age 1+ Abundance (Jan 1.) of MSVPA Species",
                      "Year",              // x title
                      "Millions of Fish",   // y title
                      XLabelNames,
                      (Forecast_NYears > nmfConstants::NumCategoriesForVerticalNotation));

    GridData = ChartData;


} // end Forecast_MultispeciesPopulations_Age1PlusAbundance


void nmfOutputChartLine::Forecast_MultispeciesPopulations_SpawningStockBiomass(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int NMSVPASpe=0;
    int m=0;

    double YMax = 0.0; // RSK remove this????
    std::vector<std::string> XLabelNames;
    boost::numeric::ublas::matrix<double> ChartData;
    boost::numeric::ublas::vector<std::string> MSVPASpeList;
    std::vector<int> MSVPASpeAge;
    Boost3DArrayDouble Maturity(
        boost::extents[nmfConstants::MaxTotalSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    getMSVPASpecies(databasePtr,MSVPAName,NMSVPASpe,MSVPASpeList);
    getMSVPASpeciesNamesAges(databasePtr,NMSVPASpe,MSVPASpeList,MSVPASpeAge);
    getMaturity(databasePtr,Forecast_NYears,FirstYear,NMSVPASpe,MSVPASpeList,MSVPASpeAge,Maturity);

    nmfUtils::initialize(ChartData, Forecast_NYears, NMSVPASpe);
    nmfUtils::initialize(GridData,  Forecast_NYears, NMSVPASpe);

    for (int j = 0; j <  Forecast_NYears; ++j) {
        rowLabels << QString::number(Forecast_FirstYear+j);
    }

    for (int i = 0; i < NMSVPASpe; ++i) {
        colLabels << QString::fromStdString(MSVPASpeList(i));

        fields = {"Year","Age","Biomass"};

        queryStr = "SELECT Year, Age, Sum(InitBiomass) AS Biomass FROM ForeOutput WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND ForeName = '" + ForecastName + "'" +
                " AND Scenario = '" + ScenarioName + "'" +
                " AND SpeName = '"  + MSVPASpeList(i) + "'" +
                seasonStr +
                " GROUP By Year, Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        m = 0;
        for (int j = 0; j < Forecast_NYears; ++j) {

            XLabelNames.push_back(std::to_string(Forecast_FirstYear + j));
            for (int k = 0; k < MSVPASpeAge[i]; ++k) {
                ChartData(j,i) += (Maturity[i][k][j] * std::stod(dataMap["Biomass"][m++]) / 1000.0); // RSK replace with constant
            }
            ChartData(j,i) *= 0.5;  // sex ratio again !
        }
    }

    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      ChartData,
                      MSVPASpeList,
                      "Spawning Stock Biomass (Jan 1.) for MSVPA Species",
                      "Year",              // x title
                      "000 Metric Tons",   // y title
                      XLabelNames,
                      (Forecast_NYears > nmfConstants::NumCategoriesForVerticalNotation));

    GridData = ChartData;

} // end Forecast_MultispeciesPopulations_SpawningStockBiomass


std::string
nmfOutputChartLine::getYAxisUnits(std::string selectedSpecies)
{
    nmfDatabase* databasePtr;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    std::string yAxisUnits="";

    // Find units
    fields = {"WtUnits"};
    queryStr = "SELECT WtUnits FROM Species WHERE SpeName = '" + selectedSpecies + "' ";
    dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

    double units = std::stod(dataMap["WtUnits"][0]);
    if (units == 1.0)
        yAxisUnits = "(kg)";
    else if (units == 0.001)
        yAxisUnits = "(g)";
    else if (units == (1/2.2)) // In original code. Not sure about this. -RSK
        yAxisUnits = "(lbs.)";

    return yAxisUnits;

} // end getYAxisUnits



void nmfOutputChartLine::Forecast_YieldPerRecruit_YPRvsF(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    int LoopCount = 0;
    char buf[1000];
    std::vector<std::string> XLabelNames;
    double YMax=0.0;
    std::string yAxisUnits = getYAxisUnits(selectedSpecies);
    boost::numeric::ublas::matrix<double> Pmature;
    boost::numeric::ublas::matrix<double> FatAge;
    boost::numeric::ublas::matrix<double> WtAtAge;
    boost::numeric::ublas::matrix<double> M1atAge;
    boost::numeric::ublas::matrix<double> M2atAge;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> YPROut;
    boost::numeric::ublas::matrix<double> PRF;
    boost::numeric::ublas::vector<double> FullF;
    boost::numeric::ublas::vector<double> YPRObs;
    boost::numeric::ublas::vector<double> F01Val;
    boost::numeric::ublas::vector<double> FMaxVal;
    boost::numeric::ublas::vector<double> tmpPRF;
    boost::numeric::ublas::vector<double> tmpM2;
    boost::numeric::ublas::vector<double> tmpM1;
    boost::numeric::ublas::vector<double> tmpWt;
    boost::numeric::ublas::vector<double> tmpMat;

    nmfUtils::initialize(Pmature, NumYears, Nage);
    nmfUtils::initialize(FatAge,  NumYears, Nage);
    nmfUtils::initialize(WtAtAge, NumYears, Nage);
    nmfUtils::initialize(M1atAge, NumYears, Nage);
    nmfUtils::initialize(M2atAge, NumYears, Nage);
    nmfUtils::initialize(Catch,   NumYears, Nage);
    nmfUtils::initialize(FullF,   NumYears);
    nmfUtils::initialize(PRF,     NumYears, Nage);
    nmfUtils::initialize(YPROut,  26, NumYears);  // RSK replace the hardcoded 26 with a constant
    nmfUtils::initialize(GridData,26, NumYears+1);  // RSK replace the hardcoded 26 with a constant
    nmfUtils::initialize(YPRObs,  NumYears);
    nmfUtils::initialize(F01Val,  NumYears);
    nmfUtils::initialize(FMaxVal, NumYears);

    // OK...then will have to select appropriate data for your species and such
    // Get prop mature and wt at age
    for (int i=0; i<NumYears; ++i) {

        // Get prop mature and wt at age
        databasePtr->nmfQueryMaturityData(
            i,FirstYear,Nage,selectedSpecies,Pmature);

        // Get Wt At Age...wts at beginning of year
        databasePtr->nmfQueryForecastWeightAtAgeData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,WtAtAge);

        // Get mortalities at age F, M2, M1
        databasePtr->nmfQueryForecastMortalityData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,FatAge,M1atAge,M2atAge);

        // Need to get catches to calculate a weighted average FullF
        databasePtr->nmfQueryForecastAgeCatchData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,Catch);

    } // end for i

    // Calculate weighted average F..flat top above RecAge..do we want an option though..probably
    calculateWeightAveFAndAssignPRFs(NumYears,RecAge,Nage,FatAge,Catch,FullF,PRF);

    // Ok..that's all we need to send to the SSB calc.
    nmfUtils::initialize(tmpPRF, Nage);
    nmfUtils::initialize(tmpM2,  Nage);
    nmfUtils::initialize(tmpM1,  Nage);
    nmfUtils::initialize(tmpWt,  Nage);
    nmfUtils::initialize(tmpMat, Nage);
    double yprOut;
    colLabels << "Avg F";

    for (int i = 0; i < NumYears; ++ i) {

        colLabels << QString::number(Forecast_FirstYear+i);

        for (int j = 0; j < Nage; ++j) {
            tmpPRF(j) = PRF(i,j);
            tmpM2(j)  = M2atAge(i,j);
            tmpM1(j)  = M1atAge(i,j);
            tmpWt(j)  = WtAtAge(i,j);
            tmpMat(j) = Pmature(i,j);
        }

        YPRObs(i) = YPR(tmpWt, tmpPRF, tmpM2, FullF(i), tmpM1, Nage);

        // Generate YPR across a range of Fs..25 values in all
        LoopCount = 0;
        for (double XX = 0.0; XX <= 2.6; XX += 0.1) {
            if (i == 0) {
                rowLabels << QString::number(XX);
                GridData(LoopCount,0) = XX;
            }
            sprintf(buf,"%0.1f",XX);
            XLabelNames.push_back(buf);
            yprOut = YPR(tmpWt, tmpPRF, tmpM2, XX, tmpM1, Nage);   // RSK - tbd - error here, -nan with Menhaden
            YPROut(LoopCount,i) = yprOut;
            if (yprOut > YMax)
                YMax = yprOut;
            GridData(LoopCount,i+1) = yprOut;
            ++LoopCount;
        } // end XX
        F01Val(i) = F01(tmpWt, tmpPRF, tmpM2, tmpM1, Nage);
        // Calculate Fmax
        FMaxVal(i) = FMax(tmpWt, tmpPRF, tmpM2, tmpM1, Nage);
    } // end for i

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      YPROut, LegendNames,
                      "Yield Per Recruit vs. Fishing Mortality Rate",
                      "Fully Recruited F", "Yield Per Recruit "+yAxisUnits,
                      XLabelNames,false);

} // end Forecast_YieldPerRecruit_YPRvsF



void
nmfOutputChartLine::Forecast_YieldPerRecruit_SSBvsF(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    int LoopCount = 0;
    char buf[100];
    std::vector<std::string> XLabelNames;
    double YMax=0.0;
    std::string yAxisUnits = getYAxisUnits(selectedSpecies);
    boost::numeric::ublas::matrix<double> Pmature;
    boost::numeric::ublas::matrix<double> FatAge;
    boost::numeric::ublas::matrix<double> WtAtAge;
    boost::numeric::ublas::matrix<double> M1atAge;
    boost::numeric::ublas::matrix<double> M2atAge;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> YPROut;
    boost::numeric::ublas::matrix<double> PRF;
    boost::numeric::ublas::vector<double> FullF;
    boost::numeric::ublas::vector<double> YPRObs;
    boost::numeric::ublas::vector<double> tmpPRF;
    boost::numeric::ublas::vector<double> tmpM2;
    boost::numeric::ublas::vector<double> tmpM1;
    boost::numeric::ublas::vector<double> tmpWt;
    boost::numeric::ublas::vector<double> tmpMat;
    boost::numeric::ublas::vector<double> SSB03;
    boost::numeric::ublas::vector<double> SSB10;
    boost::numeric::ublas::vector<double> SSBMax;

    nmfUtils::initialize(Pmature, NumYears, Nage);
    nmfUtils::initialize(FatAge,  NumYears, Nage);
    nmfUtils::initialize(WtAtAge, NumYears, Nage);
    nmfUtils::initialize(M1atAge, NumYears, Nage);
    nmfUtils::initialize(M2atAge, NumYears, Nage);
    nmfUtils::initialize(Catch,   NumYears, Nage);
    nmfUtils::initialize(FullF,   NumYears);
    nmfUtils::initialize(PRF,     NumYears, Nage);
    nmfUtils::initialize(YPROut,  26, NumYears);  // RSK replace the hardcoded 25 with a constant
    nmfUtils::initialize(GridData,26, NumYears+1);
    nmfUtils::initialize(YPRObs,  NumYears);
    nmfUtils::initialize(SSB03,   NumYears);
    nmfUtils::initialize(SSB10,   NumYears);
    nmfUtils::initialize(SSBMax,  NumYears);
    double yprOut;
    colLabels << "Avg F";

    // OK...then will have to select appropriate data for your species and such
    // Get prop mature and wt at age

    for (int i=0; i<NumYears; ++i) {
        colLabels << QString::number(Forecast_FirstYear+i);

        // Get prop mature and wt at age
        databasePtr->nmfQueryMaturityData(
                    i,FirstYear,Nage,selectedSpecies,Pmature);

        // Get Wt At Age...wts at beginning of year
        databasePtr->nmfQueryForecastWeightAtAgeData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,WtAtAge);

        // Get mortalities at age F, M2, M1
        databasePtr->nmfQueryForecastMortalityData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,FatAge,M1atAge,M2atAge);

        // Need to get catches to calculate a weighted average FullF
        databasePtr->nmfQueryForecastAgeCatchData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,Catch);

    } // end for i

    // Calculate weighted average F..flat top above RecAge..do we want an option though..probably
    calculateWeightAveFAndAssignPRFs(NumYears,RecAge,Nage,FatAge,Catch,FullF,PRF);

    // Ok..that's all we need to send to the SSB calc.
    nmfUtils::initialize(tmpPRF, Nage);
    nmfUtils::initialize(tmpM2,  Nage);
    nmfUtils::initialize(tmpM1,  Nage);
    nmfUtils::initialize(tmpWt,  Nage);
    nmfUtils::initialize(tmpMat, Nage);

    for (int i = 0; i < NumYears; ++ i) {

        for (int j = 0; j < Nage; ++j) {
            tmpPRF(j) = PRF(i,j);
            tmpM2(j)  = M2atAge(i,j);
            tmpM1(j)  = M1atAge(i,j);
            tmpWt(j)  = WtAtAge(i,j);
            tmpMat(j) = Pmature(i,j);
        }
        YPRObs(i) = SSB(tmpWt, tmpPRF, tmpM2, FullF(i), tmpM1, Nage, tmpMat);

        LoopCount = 0;
        // Generate YPR across a range of Fs..25 values in all
        for (double XX = 0.0; XX <= 2.6; XX += 0.1) {
            if (i == 0) {
                rowLabels << QString::number(XX);
                GridData(LoopCount,0) = XX;
            }
            sprintf(buf,"%0.1f",XX);
            XLabelNames.push_back(buf);
            yprOut = SSB(tmpWt, tmpPRF, tmpM2, XX, tmpM1, Nage, tmpMat);
            YPROut(LoopCount,i) = yprOut;
            if (yprOut > YMax)
                YMax = yprOut;
            GridData(LoopCount,i+1) = yprOut;
            ++LoopCount;
        } // end XX

        SSBMax(i) = SSB(tmpWt, tmpPRF, tmpM2, 0, tmpM1, Nage, tmpMat);
        SSB03(i)  = SSBBench(tmpWt, tmpPRF, tmpM2, tmpM1, Nage, tmpMat, 0.03);
        SSB10(i)  = SSBBench(tmpWt, tmpPRF, tmpM2, tmpM1, Nage, tmpMat, 0.1);
    } // end for i

//std::cout << "YMax: " << YMax << std::endl;
    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      YPROut, LegendNames,
                      "SSB Per Recruit vs. Fishing Mortality Rate",
                      "Fully Recruited F", "SSB Per Recruit "+yAxisUnits,
                      XLabelNames,false);

} // end Forecast_YieldPerRecruit_SSBvsF


void
nmfOutputChartLine::Forecast_YieldPerRecruit_ProjectedYPR(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::vector<std::string> XLabelNames;
    double YMax=0.0;
    std::string yAxisUnits = getYAxisUnits(selectedSpecies);
    boost::numeric::ublas::matrix<double> Pmature;
    boost::numeric::ublas::matrix<double> FatAge;
    boost::numeric::ublas::matrix<double> WtAtAge;
    boost::numeric::ublas::matrix<double> M1atAge;
    boost::numeric::ublas::matrix<double> M2atAge;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> YPROut;
    boost::numeric::ublas::matrix<double> PRF;
    boost::numeric::ublas::vector<double> FullF;
    boost::numeric::ublas::vector<double> YPRObs;
    boost::numeric::ublas::vector<double> tmpPRF;
    boost::numeric::ublas::vector<double> tmpM2;
    boost::numeric::ublas::vector<double> tmpM1;
    boost::numeric::ublas::vector<double> tmpWt;
    boost::numeric::ublas::vector<double> tmpMat;
    boost::numeric::ublas::vector<double> FMaxVal;
    boost::numeric::ublas::matrix<double> CombinedYPROut;

    nmfUtils::initialize(Pmature, NumYears, Nage);
    nmfUtils::initialize(FatAge,  NumYears, Nage);
    nmfUtils::initialize(WtAtAge, NumYears, Nage);
    nmfUtils::initialize(M1atAge, NumYears, Nage);
    nmfUtils::initialize(M2atAge, NumYears, Nage);
    nmfUtils::initialize(Catch,   NumYears, Nage);
    nmfUtils::initialize(FullF,   NumYears);
    nmfUtils::initialize(PRF,     NumYears, Nage);
    nmfUtils::initialize(YPROut,  26, NumYears);  // RSK replace the hardcoded 25 with a constant
    nmfUtils::initialize(YPRObs,  NumYears);
    nmfUtils::initialize(FMaxVal, NumYears);
    nmfUtils::initialize(CombinedYPROut,  NumYears, 2);
    nmfUtils::initialize(GridData,        NumYears, 2);
    nmfUtils::initialize(LegendNames, 2);

    // OK...then will have to select appropriate data for your species and such
    // Get prop mature and wt at age

    for (int i=0; i<NumYears; ++i) {

        XLabelNames.push_back(std::to_string(Years(i)));

        // Get prop mature and wt at age
        databasePtr->nmfQueryMaturityData(
                    i,FirstYear,Nage,selectedSpecies,Pmature);

        // Get Wt At Age...wts at beginning of year
        databasePtr->nmfQueryForecastWeightAtAgeData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,WtAtAge);

        // Get mortalities at age F, M2, M1
        databasePtr->nmfQueryForecastMortalityData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,FatAge,M1atAge,M2atAge);

        // Need to get catches to calculate a weighted average FullF
        databasePtr->nmfQueryForecastAgeCatchData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,Catch);

    } // end for i

    // Calculate weighted average F..flat top above RecAge..do we want an option though..probably
    calculateWeightAveFAndAssignPRFs(NumYears,RecAge,Nage,FatAge,Catch,FullF,PRF);

    // Ok..that's all we need to send to the YPR calc.
    nmfUtils::initialize(tmpPRF, Nage);
    nmfUtils::initialize(tmpM2,  Nage);
    nmfUtils::initialize(tmpM1,  Nage);
    nmfUtils::initialize(tmpWt,  Nage);
    nmfUtils::initialize(tmpMat, Nage);

    colLabels << "YPR Obs." << "YPR Max.";
    for (int i = 0; i < NumYears; ++ i) {
        rowLabels << QString::number(Forecast_FirstYear+i);
        for (int j = 0; j < Nage; ++j) {
            tmpPRF(j) = PRF(i,j);
            tmpM2(j)  = M2atAge(i,j);
            tmpM1(j)  = M1atAge(i,j);
            tmpWt(j)  = WtAtAge(i,j);
            tmpMat(j) = Pmature(i,j);
        }
        YPRObs(i) = YPR( tmpWt, tmpPRF, tmpM2, FullF(i), tmpM1, Nage);;
        FMaxVal(i) = FMax(tmpWt, tmpPRF, tmpM2, tmpM1, Nage);

        // Get YPR at Fmax
        YPROut(i,0) = (FMaxVal(i) > 0) ?
            YPR(tmpWt, tmpPRF, tmpM2, FMaxVal(i), tmpM1, Nage) :
            YPR(tmpWt, tmpPRF, tmpM2, 3.0, tmpM1, Nage);

        CombinedYPROut(i,0) = YPRObs(i);
        CombinedYPROut(i,1) = YPROut(i,0);

        YMax = std::max({YMax,
                         CombinedYPROut(i,0),
                         CombinedYPROut(i,1)});
    } // end for i

    LegendNames(0) = "Observed YPR";
    LegendNames(1) = "Maximum YPR";

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      CombinedYPROut, LegendNames,
                      "Projected Yield Per Recruit",
                      "Year", "Yield Per Recruit "+yAxisUnits,
                      XLabelNames,true);

    GridData = CombinedYPROut;

} // end Forecast_YieldPerRecruit_ProjectedYPR


void
nmfOutputChartLine::Forecast_YieldPerRecruit_ProjectedFBenchmarks(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::vector<std::string> XLabelNames;
    double YMax=0.0;
    std::string yAxisUnits = getYAxisUnits(selectedSpecies);
    boost::numeric::ublas::matrix<double> Pmature;
    boost::numeric::ublas::matrix<double> FatAge;
    boost::numeric::ublas::matrix<double> WtAtAge;
    boost::numeric::ublas::matrix<double> M1atAge;
    boost::numeric::ublas::matrix<double> M2atAge;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> YPROut;
    boost::numeric::ublas::matrix<double> PRF;
    boost::numeric::ublas::vector<double> FullF;
    boost::numeric::ublas::vector<double> YPRObs;
    boost::numeric::ublas::vector<double> tmpPRF;
    boost::numeric::ublas::vector<double> tmpM2;
    boost::numeric::ublas::vector<double> tmpM1;
    boost::numeric::ublas::vector<double> tmpWt;
    boost::numeric::ublas::vector<double> tmpMat;
    boost::numeric::ublas::vector<double> FMaxVal;
    boost::numeric::ublas::vector<double> SSB03;
    boost::numeric::ublas::vector<double> SSB10;
    boost::numeric::ublas::vector<double> F01Val;
    boost::numeric::ublas::matrix<double> CombinedYPROut;

    nmfUtils::initialize(Pmature, NumYears, Nage);
    nmfUtils::initialize(FatAge,  NumYears, Nage);
    nmfUtils::initialize(WtAtAge, NumYears, Nage);
    nmfUtils::initialize(M1atAge, NumYears, Nage);
    nmfUtils::initialize(M2atAge, NumYears, Nage);
    nmfUtils::initialize(Catch,   NumYears, Nage);
    nmfUtils::initialize(FullF,   NumYears);
    nmfUtils::initialize(PRF,     NumYears, Nage);
    nmfUtils::initialize(YPROut,  26, NumYears);  // RSK replace the hardcoded 25 with a constant
    nmfUtils::initialize(YPRObs,  NumYears);
    nmfUtils::initialize(FMaxVal, NumYears);
    nmfUtils::initialize(F01Val,  NumYears);
    nmfUtils::initialize(SSB03,   NumYears);
    nmfUtils::initialize(SSB10,   NumYears);
    nmfUtils::initialize(CombinedYPROut,  NumYears, 5); // RSK replace with constant
    nmfUtils::initialize(GridData,        NumYears, 5);
    nmfUtils::initialize(LegendNames, 5);

    // OK...then will have to select appropriate data for your species and such
    // Get prop mature and wt at age

    for (int i=0; i<NumYears; ++i) {

        XLabelNames.push_back(std::to_string(Years(i)));

        // Get prop mature and wt at age
        databasePtr->nmfQueryMaturityData(
                    i,FirstYear,Nage,selectedSpecies,Pmature);

        // Get Wt At Age...wts at beginning of year
        databasePtr->nmfQueryForecastWeightAtAgeData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,WtAtAge);

        // Get mortalities at age F, M2, M1
        databasePtr->nmfQueryForecastMortalityData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,FatAge,M1atAge,M2atAge);

        // Need to get catches to calculate a weighted average FullF
        databasePtr->nmfQueryForecastAgeCatchData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,Catch);

    } // end for i


    // Calculate weighted average F..flat top above RecAge..do we want an option though..probably
    calculateWeightAveFAndAssignPRFs(NumYears,RecAge,Nage,FatAge,Catch,FullF,PRF);

    // Ok..that's all we need to send to the YPR calc.
    nmfUtils::initialize(tmpPRF, Nage);
    nmfUtils::initialize(tmpM2,  Nage);
    nmfUtils::initialize(tmpM1,  Nage);
    nmfUtils::initialize(tmpWt,  Nage);
    nmfUtils::initialize(tmpMat, Nage);
    colLabels <<  "F Obs." << "F Max." << "F 0.1" <<
                  "F at 3% Max SSB/R"  << "F at 10% Max SSB/R ";
    for (int i = 0; i < NumYears; ++ i) {
        rowLabels << QString::number(Forecast_FirstYear+i);
        for (int j = 0; j < Nage; ++j) {
            tmpPRF(j) = PRF(i,j);
            tmpM2(j)  = M2atAge(i,j);
            tmpM1(j)  = M1atAge(i,j);
            tmpWt(j)  = WtAtAge(i,j);
            tmpMat(j) = Pmature(i,j);
        }

        // Calculate F0.1
        F01Val(i) = F01(tmpWt, tmpPRF, tmpM2, tmpM1, Nage);
        // Calculate Fmax
        FMaxVal(i) = FMax(tmpWt, tmpPRF, tmpM2, tmpM1, Nage);
        // Calculate FSSB03
        SSB03(i) = SSBBench(tmpWt, tmpPRF, tmpM2, tmpM1, Nage, tmpMat, 0.03);
        // Calculate FSSB10
        SSB10(i) = SSBBench(tmpWt, tmpPRF, tmpM2, tmpM1, Nage, tmpMat, 0.1);

        CombinedYPROut(i,0) = FullF(i);
        CombinedYPROut(i,1) = (FMaxVal(i) > 0) ? FMaxVal(i) : 2.5;
        CombinedYPROut(i,2) = F01Val(i);
        CombinedYPROut(i,3) = SSB03(i);
        CombinedYPROut(i,4) = SSB10(i);

        YMax = std::max({YMax,
                         CombinedYPROut(i,0),
                         CombinedYPROut(i,1),
                         CombinedYPROut(i,2),
                         CombinedYPROut(i,3),
                         CombinedYPROut(i,4)});

    } // end for i

    LegendNames(0) = "Observed F";
    LegendNames(1) = "Fmax";
    LegendNames(2) = "F 0.1";
    LegendNames(3) = "F at 3% Max SSB/R";
    LegendNames(4) = "F at 10% Max SSB/R";

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;

    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      CombinedYPROut, LegendNames,
                      "Projected F Benchmarks",
                      "Year", "Fishing Mortality Rate "+yAxisUnits,
                      XLabelNames,true);

    GridData = CombinedYPROut;

} // end Forecast_YieldPerRecruit_ProjectedFBenchmarks


void
nmfOutputChartLine::Forecast_YieldPerRecruit_ProjectedSSBBenchmarks(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::vector<std::string> XLabelNames;
    double YMax=0.0;
    double yprObs;
    double ssbMax03;
    double ssbMax10;
    std::string yAxisUnits = getYAxisUnits(selectedSpecies);
    boost::numeric::ublas::matrix<double> Pmature;
    boost::numeric::ublas::matrix<double> FatAge;
    boost::numeric::ublas::matrix<double> WtAtAge;
    boost::numeric::ublas::matrix<double> M1atAge;
    boost::numeric::ublas::matrix<double> M2atAge;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> YPROut;
    boost::numeric::ublas::matrix<double> PRF;
    boost::numeric::ublas::vector<double> FullF;
    boost::numeric::ublas::vector<double> YPRObs;
    boost::numeric::ublas::vector<double> tmpPRF;
    boost::numeric::ublas::vector<double> tmpM2;
    boost::numeric::ublas::vector<double> tmpM1;
    boost::numeric::ublas::vector<double> tmpWt;
    boost::numeric::ublas::vector<double> tmpMat;
    boost::numeric::ublas::vector<double> SSBMax;
    boost::numeric::ublas::matrix<double> CombinedYPROut;

    nmfUtils::initialize(Pmature, NumYears, Nage);
    nmfUtils::initialize(FatAge,  NumYears, Nage);
    nmfUtils::initialize(WtAtAge, NumYears, Nage);
    nmfUtils::initialize(M1atAge, NumYears, Nage);
    nmfUtils::initialize(M2atAge, NumYears, Nage);
    nmfUtils::initialize(Catch,   NumYears, Nage);
    nmfUtils::initialize(FullF,   NumYears);
    nmfUtils::initialize(PRF,     NumYears, Nage);
    nmfUtils::initialize(YPROut,  26, NumYears);  // RSK replace the hardcoded 25 with a constant
    nmfUtils::initialize(YPRObs,  NumYears);
    nmfUtils::initialize(SSBMax, NumYears);
    nmfUtils::initialize(CombinedYPROut, NumYears, 3); // RSK replace with constant
    nmfUtils::initialize(GridData,       NumYears, 4);
    nmfUtils::initialize(LegendNames, 3);

    // OK...then will have to select appropriate data for your species and such
    // Get prop mature and wt at age

    for (int i=0; i<NumYears; ++i) {

        XLabelNames.push_back(std::to_string(Years(i)));

        // Get prop mature and wt at age
        databasePtr->nmfQueryMaturityData(
                    i,FirstYear,Nage,selectedSpecies,Pmature);

        // Get Wt At Age...wts at beginning of year
        databasePtr->nmfQueryForecastWeightAtAgeData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,WtAtAge);

        // Get mortalities at age F, M2, M1
        databasePtr->nmfQueryForecastMortalityData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,FatAge,M1atAge,M2atAge);

        // Need to get catches to calculate a weighted average FullF
        databasePtr->nmfQueryForecastAgeCatchData(
                    MSVPAName,ForecastName,ScenarioName,selectedSpecies,
                    Years(i)-Forecast_FirstYear,Nage,i,Catch);

    } // end for i


    // Calculate weighted average F..flat top above RecAge..do we want an option though..probably
    calculateWeightAveFAndAssignPRFs(NumYears,RecAge,Nage,FatAge,Catch,FullF,PRF);

    // Ok..that's all we need to send to the YPR calc.
    nmfUtils::initialize(tmpPRF, Nage);
    nmfUtils::initialize(tmpM2,  Nage);
    nmfUtils::initialize(tmpM1,  Nage);
    nmfUtils::initialize(tmpWt,  Nage);
    nmfUtils::initialize(tmpMat, Nage);
    colLabels << "Obs. SSB/R" << "SSB/R Max" <<
                 "3% SSB/R Max" << "10% SSB/R Max";
    for (int i = 0; i < NumYears; ++ i) {
        rowLabels << QString::number(Forecast_FirstYear+i);
        for (int j = 0; j < Nage; ++j) {
            tmpPRF(j) = PRF(i,j);
            tmpM2(j)  = M2atAge(i,j);
            tmpM1(j)  = M1atAge(i,j);
            tmpWt(j)  = WtAtAge(i,j);
            tmpMat(j) = Pmature(i,j);
        }

        // Calculate Observed SSB
        YPRObs(i) = SSB(tmpWt, tmpPRF, tmpM2, FullF(i), tmpM1, Nage, tmpMat);
        // Calculate SSBMax
        SSBMax(i) = SSB(tmpWt, tmpPRF, tmpM2, 0, tmpM1, Nage, tmpMat);

        yprObs = YPRObs(i);
        ssbMax03 = SSBMax(i) * 0.03;
        ssbMax10 = SSBMax(i) * 0.10;

        CombinedYPROut(i,0) = yprObs;
        CombinedYPROut(i,1) = ssbMax03;
        CombinedYPROut(i,2) = ssbMax10;

        GridData(i,0) = yprObs;
        GridData(i,1) = SSBMax(i);
        GridData(i,2) = ssbMax03;
        GridData(i,3) = ssbMax10;

        YMax = std::max({YMax,
                         CombinedYPROut(i,0),
                         CombinedYPROut(i,1),
                         CombinedYPROut(i,2)});

    } // end for i

    LegendNames(0) = "Observed SSB/R";
    LegendNames(1) = "3% of Max SSB/R";
    LegendNames(2) = "10% of Max SSB/R";

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;

    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      CombinedYPROut, LegendNames,
                      "Projected SSB/R Benchmarks",
                      "Year", "Fishing Mortality Rate "+yAxisUnits,
                      XLabelNames,true);

} // end Forecast_YieldPerRecruit_ProjectedSSBBenchmarks


void
nmfOutputChartLine::MSVPA_YieldPerRecruit_YPRvsF (
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    int LoopCount = 0;
    char buf[100];
    std::vector<std::string> XLabelNames;
    double YMax=0.0;
    std::string yAxisUnits = getYAxisUnits(selectedSpecies);
    boost::numeric::ublas::matrix<double> Pmature;
    boost::numeric::ublas::matrix<double> FatAge;
    boost::numeric::ublas::matrix<double> WtAtAge;
    boost::numeric::ublas::matrix<double> M1atAge;
    boost::numeric::ublas::matrix<double> M2atAge;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> YPROut;
    boost::numeric::ublas::matrix<double> PRF;
    boost::numeric::ublas::vector<double> FullF;
    boost::numeric::ublas::vector<double> YPRObs;
    boost::numeric::ublas::vector<double> F01Val;
    boost::numeric::ublas::vector<double> FMaxVal;
    boost::numeric::ublas::vector<double> tmpPRF;
    boost::numeric::ublas::vector<double> tmpM2;
    boost::numeric::ublas::vector<double> tmpM1;
    boost::numeric::ublas::vector<double> tmpWt;
    boost::numeric::ublas::vector<double> tmpMat;

    nmfUtils::initialize(Pmature, NumYears, Nage);
    nmfUtils::initialize(FatAge,  NumYears, Nage);
    nmfUtils::initialize(WtAtAge, NumYears, Nage);
    nmfUtils::initialize(M1atAge, NumYears, Nage);
    nmfUtils::initialize(M2atAge, NumYears, Nage);
    nmfUtils::initialize(Catch,   NumYears, Nage);
    nmfUtils::initialize(FullF,   NumYears);
    nmfUtils::initialize(PRF,     NumYears, Nage);
    nmfUtils::initialize(YPROut,  26, NumYears);  // RSK replace the hardcoded 26 with a constant
    nmfUtils::initialize(GridData,26, NumYears+1);  // RSK replace the hardcoded 26 with a constant, +1 because stored XX values in 1st column
    nmfUtils::initialize(YPRObs,  NumYears);
    nmfUtils::initialize(F01Val,  NumYears);
    nmfUtils::initialize(FMaxVal, NumYears);

    // OK...then will have to select appropriate data for your species and such
    // Get prop mature and wt at age
    for (int i=0; i<NumYears; ++i) {

        // Get prop mature and wt at age
        databasePtr->nmfQueryMaturityData(
            i,Years(i),Nage,selectedSpecies,Pmature);

        // Get Wt At Age...wts at beginning of year
        databasePtr->nmfQueryWeightAtAgeData(
                    i,Years(i),Nage,selectedSpecies,WtAtAge);

        // Get mortalities at age F, M2, M1
        databasePtr->nmfQueryMortalityData(
                    MSVPAName,i,Years(i)-FirstYear,Nage,
                    selectedSpecies,FatAge,M1atAge,M2atAge);

        // Need to get catches to calculate a weighted average FullF
        databasePtr->nmfQueryAgeCatchData(
                    i,Years(i),Nage,selectedSpecies,Catch);

    } // end for i

    // Calculate weighted average F..flat top above RecAge..do we want an option though..probably
    calculateWeightAveFAndAssignPRFs(NumYears,RecAge,Nage,FatAge,Catch,FullF,PRF);
    rowLabels.clear();
    colLabels.clear();

    // Ok..that's all we need to send to the SSB calc.
    nmfUtils::initialize(tmpPRF, Nage);
    nmfUtils::initialize(tmpM2,  Nage);
    nmfUtils::initialize(tmpM1,  Nage);
    nmfUtils::initialize(tmpWt,  Nage);
    nmfUtils::initialize(tmpMat, Nage);
    int k;
    colLabels << "Avg. F";
    int firstYear = (theModelName == "MSVPA") ? FirstYear : Forecast_FirstYear;

    for (int i = 0; i < NumYears; ++ i) {
        colLabels << QString::number(firstYear+i);
        for (int j = 0; j < Nage; ++j) {
            tmpPRF(j) = PRF(i,j);
            tmpM2(j)  = M2atAge(i,j);
            tmpM1(j)  = M1atAge(i,j);
            tmpWt(j)  = WtAtAge(i,j);
            tmpMat(j) = Pmature(i,j);
        }

        YPRObs(i) = YPR(tmpWt, tmpPRF, tmpM2, FullF(i), tmpM1, Nage);
        LoopCount = 0;
        // Generate YPR across a range of Fs..25 values in all
        k = 0;
        for (double XX = 0.0; XX <= 2.6; XX += 0.1) {
            if (i == 0) {
                sprintf(buf,"%0.1f",XX);
                XLabelNames.push_back(buf);
            }
            YPROut(LoopCount,i) = YPR(tmpWt, tmpPRF, tmpM2, XX, tmpM1, Nage);
            if (YPROut(LoopCount,i) > YMax)
                YMax = YPROut(LoopCount,i);
            if (i == 0) {
                rowLabels << "";
                GridData(k,0) = XX;
            }
            GridData(k,i+1) = YPROut(LoopCount,i);

            ++LoopCount;

            ++k;
        } // end XX
        F01Val(i) = F01(tmpWt, tmpPRF, tmpM2, tmpM1, Nage);
        // Calculate Fmax
        FMaxVal(i) = FMax(tmpWt, tmpPRF, tmpM2, tmpM1, Nage);
    } // end for i

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      YPROut, LegendNames,
                      "Yield Per Recruit vs. Fishing Mortality Rate",
                      "Fully Recruited F", "Yield Per Recruit "+yAxisUnits,
                      XLabelNames,false);

} // end MSVPA_YieldPerRecruit_YPRvsF


void
nmfOutputChartLine::MSVPA_YieldPerRecruit_SSBvsF(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    int LoopCount = 0;
    int k;
    char buf[100];
    std::vector<std::string> XLabelNames;
    double YMax=0.0;
    std::string yAxisUnits = getYAxisUnits(selectedSpecies);
    boost::numeric::ublas::matrix<double> Pmature;
    boost::numeric::ublas::matrix<double> FatAge;
    boost::numeric::ublas::matrix<double> WtAtAge;
    boost::numeric::ublas::matrix<double> M1atAge;
    boost::numeric::ublas::matrix<double> M2atAge;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> YPROut;
    boost::numeric::ublas::matrix<double> PRF;
    boost::numeric::ublas::vector<double> FullF;
    boost::numeric::ublas::vector<double> YPRObs;
    boost::numeric::ublas::vector<double> tmpPRF;
    boost::numeric::ublas::vector<double> tmpM2;
    boost::numeric::ublas::vector<double> tmpM1;
    boost::numeric::ublas::vector<double> tmpWt;
    boost::numeric::ublas::vector<double> tmpMat;
    boost::numeric::ublas::vector<double> SSB03;
    boost::numeric::ublas::vector<double> SSB10;
    boost::numeric::ublas::vector<double> SSBMax;

    nmfUtils::initialize(Pmature, NumYears, Nage);
    nmfUtils::initialize(FatAge,  NumYears, Nage);
    nmfUtils::initialize(WtAtAge, NumYears, Nage);
    nmfUtils::initialize(M1atAge, NumYears, Nage);
    nmfUtils::initialize(M2atAge, NumYears, Nage);
    nmfUtils::initialize(Catch,   NumYears, Nage);
    nmfUtils::initialize(FullF,   NumYears);
    nmfUtils::initialize(PRF,     NumYears, Nage);
    nmfUtils::initialize(YPROut,  26, NumYears);  // RSK replace the hardcoded 25 with a constant
    nmfUtils::initialize(GridData,26, NumYears+1);  // RSK replace the hardcoded 26 with a constant, +1 because stored XX values in 1st column
    nmfUtils::initialize(YPRObs,  NumYears);
    nmfUtils::initialize(SSB03,   NumYears);
    nmfUtils::initialize(SSB10,   NumYears);
    nmfUtils::initialize(SSBMax,  NumYears);
    rowLabels.clear();
    colLabels.clear();
    colLabels << "Avg. F";

    // OK...then will have to select appropriate data for your species and such
    // Get prop mature and wt at age
    for (int i=0; i<NumYears; ++i) {

        // Get prop mature and wt at age
        databasePtr->nmfQueryMaturityData(
            i,Years(i),Nage,selectedSpecies,Pmature);

        // Get Wt At Age...wts at beginning of year
        databasePtr->nmfQueryWeightAtAgeData(
                    i,Years(i),Nage,selectedSpecies,WtAtAge);

        // Get mortalities at age F, M2, M1
        databasePtr->nmfQueryMortalityData(
                    MSVPAName,i,Years(i)-FirstYear,Nage,
                    selectedSpecies,FatAge,M1atAge,M2atAge);

        // Need to get catches to calculate a weighted average FullF
        databasePtr->nmfQueryAgeCatchData(
                    i,Years(i),Nage,selectedSpecies,Catch);

    } // end for i

    // Calculate weighted average F..flat top above RecAge..do we want an option though..probably
    calculateWeightAveFAndAssignPRFs(NumYears,RecAge,Nage,FatAge,Catch,FullF,PRF);

    // Ok..that's all we need to send to the SSB calc.
    nmfUtils::initialize(tmpPRF, Nage);
    nmfUtils::initialize(tmpM2,  Nage);
    nmfUtils::initialize(tmpM1,  Nage);
    nmfUtils::initialize(tmpWt,  Nage);
    nmfUtils::initialize(tmpMat, Nage);
    int firstYear = (theModelName == "MSVPA") ? FirstYear : Forecast_FirstYear;

    for (int i = 0; i < NumYears; ++ i) {
        colLabels << QString::number(firstYear+i);

        for (int j = 0; j < Nage; ++j) {
            tmpPRF(j) = PRF(i,j);
            tmpM2(j)  = M2atAge(i,j);
            tmpM1(j)  = M1atAge(i,j);
            tmpWt(j)  = WtAtAge(i,j);
            tmpMat(j) = Pmature(i,j);
        }
        YPRObs(i) = SSB(tmpWt, tmpPRF, tmpM2, FullF(i), tmpM1, Nage, tmpMat);

        LoopCount = 0;
        // Generate YPR across a range of Fs..25 values in all
        k = 0;
        for (double XX = 0.0; XX <= 2.6; XX += 0.1) {
            if (i == 0) {
                rowLabels << "";
                GridData(k,0) = XX;
                sprintf(buf,"%0.1f",XX);
                XLabelNames.push_back(buf);
            }
            YPROut(LoopCount,i) = SSB(tmpWt, tmpPRF, tmpM2, XX, tmpM1, Nage, tmpMat);
            if (YPROut(LoopCount,i) > YMax)
                YMax = YPROut(LoopCount,i);
            GridData(k,i+1) = YPROut(LoopCount,i);

            ++LoopCount;
            ++k;
        } // end XX

        SSBMax(i) = SSB(tmpWt, tmpPRF, tmpM2, 0, tmpM1, Nage, tmpMat);
        SSB03(i)  = SSBBench(tmpWt, tmpPRF, tmpM2, tmpM1, Nage, tmpMat, 0.03);
        SSB10(i)  = SSBBench(tmpWt, tmpPRF, tmpM2, tmpM1, Nage, tmpMat, 0.1);
    } // end for i

//std::cout << "YMax: " << YMax << std::endl;
    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      YPROut, LegendNames,
                      "SSB Per Recruit vs. Fishing Mortality Rate",
                      "Fully Recruited F", "SSB Per Recruit "+yAxisUnits,
                      XLabelNames,false);

} // end MSVPA_YieldPerRecruit_SSBvsF


void
nmfOutputChartLine::MSVPA_YieldPerRecruit_HistoricalYPR(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::vector<std::string> XLabelNames;
    double YMax=0.0;
    std::string yAxisUnits = getYAxisUnits(selectedSpecies);
    boost::numeric::ublas::matrix<double> Pmature;
    boost::numeric::ublas::matrix<double> FatAge;
    boost::numeric::ublas::matrix<double> WtAtAge;
    boost::numeric::ublas::matrix<double> M1atAge;
    boost::numeric::ublas::matrix<double> M2atAge;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> YPROut;
    boost::numeric::ublas::matrix<double> PRF;
    boost::numeric::ublas::vector<double> FullF;
    boost::numeric::ublas::vector<double> YPRObs;
    boost::numeric::ublas::vector<double> tmpPRF;
    boost::numeric::ublas::vector<double> tmpM2;
    boost::numeric::ublas::vector<double> tmpM1;
    boost::numeric::ublas::vector<double> tmpWt;
    boost::numeric::ublas::vector<double> tmpMat;
    boost::numeric::ublas::vector<double> FMaxVal;
    boost::numeric::ublas::matrix<double> CombinedYPROut;

    nmfUtils::initialize(Pmature, NumYears, Nage);
    nmfUtils::initialize(FatAge,  NumYears, Nage);
    nmfUtils::initialize(WtAtAge, NumYears, Nage);
    nmfUtils::initialize(M1atAge, NumYears, Nage);
    nmfUtils::initialize(M2atAge, NumYears, Nage);
    nmfUtils::initialize(Catch,   NumYears, Nage);
    nmfUtils::initialize(FullF,   NumYears);
    nmfUtils::initialize(PRF,     NumYears, Nage);
    nmfUtils::initialize(YPROut,  NumYears, 1);  // RSK replace the hardcoded 25 with a constant
    nmfUtils::initialize(GridData,NumYears, 2);
    nmfUtils::initialize(YPRObs,  NumYears);
    nmfUtils::initialize(FMaxVal, NumYears);
    nmfUtils::initialize(CombinedYPROut,  NumYears, 2);
    nmfUtils::initialize(LegendNames, 2);
    rowLabels.clear();
    colLabels.clear();
    colLabels << "YPR Obs." << "YPR Max.";

    int firstYear = (theModelName == "MSVPA") ? FirstYear : Forecast_FirstYear;

    // OK...then will have to select appropriate data for your species and such
    // Get prop mature and wt at age
    for (int i=0; i<NumYears; ++i) {
        rowLabels << QString::number(firstYear+i);
        XLabelNames.push_back(std::to_string(Years(i)));

        // Get prop mature and wt at age
        databasePtr->nmfQueryMaturityData(
            i,Years(i),Nage,selectedSpecies,Pmature);

        // Get Wt At Age...wts at beginning of year
        databasePtr->nmfQueryWeightAtAgeData(
                    i,Years(i),Nage,selectedSpecies,WtAtAge);

        // Get mortalities at age F, M2, M1
        databasePtr->nmfQueryMortalityData(
                    MSVPAName,i,Years(i)-FirstYear,Nage,
                    selectedSpecies,FatAge,M1atAge,M2atAge);

        // Need to get catches to calculate a weighted average FullF
        databasePtr->nmfQueryAgeCatchData(
                    i,Years(i),Nage,selectedSpecies,Catch);

    } // end for i

    // Calculate weighted average F..flat top above RecAge..do we want an option though..probably
    calculateWeightAveFAndAssignPRFs(NumYears,RecAge,Nage,FatAge,Catch,FullF,PRF);

    // Ok..that's all we need to send to the YPR calc.
    nmfUtils::initialize(tmpPRF, Nage);
    nmfUtils::initialize(tmpM2,  Nage);
    nmfUtils::initialize(tmpM1,  Nage);
    nmfUtils::initialize(tmpWt,  Nage);
    nmfUtils::initialize(tmpMat, Nage);

    for (int i = 0; i < NumYears; ++ i) {

        for (int j = 0; j < Nage; ++j) {
            tmpPRF(j) = PRF(i,j);
            tmpM2(j)  = M2atAge(i,j);
            tmpM1(j)  = M1atAge(i,j);
            tmpWt(j)  = WtAtAge(i,j);
            tmpMat(j) = Pmature(i,j);
        }

        YPRObs(i)  = YPR( tmpWt, tmpPRF, tmpM2, FullF(i), tmpM1, Nage);
        FMaxVal(i) = FMax(tmpWt, tmpPRF, tmpM2, tmpM1, Nage);

        // Get YPR at Fmax
        YPROut(i,0) = (FMaxVal(i) > 0) ?
            YPR(tmpWt, tmpPRF, tmpM2, FMaxVal(i), tmpM1, Nage) :
            YPR(tmpWt, tmpPRF, tmpM2, 3.0, tmpM1, Nage);
        CombinedYPROut(i,0) = YPRObs(i);
        CombinedYPROut(i,1) = YPROut(i,0);
        YMax = std::max({YMax,
                         CombinedYPROut(i,0),
                         CombinedYPROut(i,1)});
std::cout << "Loading grid data...." << std::endl;
        GridData(i,0) = YPRObs(i);
        GridData(i,1) = YPROut(i,0);

    } // end for i

    LegendNames(0) = "Observed YPR";
    LegendNames(1) = "Maximum YPR";
    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      CombinedYPROut, LegendNames,
                      "Historical Yield Per Recruit",
                      "Year", "Yield Per Recruit "+yAxisUnits,
                      XLabelNames,true);

} // end MSVPA_YieldPerRecruit_HistoricalYPR


void
nmfOutputChartLine::MSVPA_YieldPerRecruit_HistoricalFBenchmarks(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::vector<std::string> XLabelNames;
    double YMax=0.0;
    std::string yAxisUnits = getYAxisUnits(selectedSpecies);
    boost::numeric::ublas::matrix<double> Pmature;
    boost::numeric::ublas::matrix<double> FatAge;
    boost::numeric::ublas::matrix<double> WtAtAge;
    boost::numeric::ublas::matrix<double> M1atAge;
    boost::numeric::ublas::matrix<double> M2atAge;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> PRF;
    boost::numeric::ublas::vector<double> FullF;
    boost::numeric::ublas::vector<double> YPRObs;
    boost::numeric::ublas::vector<double> tmpPRF;
    boost::numeric::ublas::vector<double> tmpM2;
    boost::numeric::ublas::vector<double> tmpM1;
    boost::numeric::ublas::vector<double> tmpWt;
    boost::numeric::ublas::vector<double> tmpMat;
    boost::numeric::ublas::vector<double> FMaxVal;
    boost::numeric::ublas::vector<double> SSB03;
    boost::numeric::ublas::vector<double> SSB10;
    boost::numeric::ublas::vector<double> F01Val;
    boost::numeric::ublas::matrix<double> CombinedYPROut;

    nmfUtils::initialize(Pmature, NumYears, Nage);
    nmfUtils::initialize(FatAge,  NumYears, Nage);
    nmfUtils::initialize(WtAtAge, NumYears, Nage);
    nmfUtils::initialize(M1atAge, NumYears, Nage);
    nmfUtils::initialize(M2atAge, NumYears, Nage);
    nmfUtils::initialize(Catch,   NumYears, Nage);
    nmfUtils::initialize(FullF,   NumYears);
    nmfUtils::initialize(PRF,     NumYears, Nage);
    nmfUtils::initialize(YPRObs,  NumYears);
    nmfUtils::initialize(GridData,NumYears, 5);
    nmfUtils::initialize(FMaxVal, NumYears);
    nmfUtils::initialize(F01Val,  NumYears);
    nmfUtils::initialize(SSB03,   NumYears);
    nmfUtils::initialize(SSB10,   NumYears);
    nmfUtils::initialize(CombinedYPROut,  NumYears, 5); // RSK replace with constant
    nmfUtils::initialize(LegendNames, 5);
    rowLabels.clear();
    colLabels.clear();
    colLabels << "F Obs." << "F Max." << "F 0.1"
              << "F at 3% Max SSB/R"  << "F at 10% Max SSB/R";
    int firstYear = (theModelName == "MSVPA") ? FirstYear : Forecast_FirstYear;

    // OK...then will have to select appropriate data for your species and such
    // Get prop mature and wt at age

    for (int i=0; i<NumYears; ++i) {
        rowLabels << QString::number(firstYear+i);

        XLabelNames.push_back(std::to_string(Years(i)));

        // Get prop mature and wt at age
        databasePtr->nmfQueryMaturityData(
            i,Years(i),Nage,selectedSpecies,Pmature);

        // Get Wt At Age...wts at beginning of year
        databasePtr->nmfQueryWeightAtAgeData(
                    i,Years(i),Nage,selectedSpecies,WtAtAge);

        // Get mortalities at age F, M2, M1
        databasePtr->nmfQueryMortalityData(
                    MSVPAName,i,Years(i)-FirstYear,Nage,
                    selectedSpecies,FatAge,M1atAge,M2atAge);

        // Need to get catches to calculate a weighted average FullF
        databasePtr->nmfQueryAgeCatchData(
                    i,Years(i),Nage,selectedSpecies,Catch);

    } // end for i


    // Calculate weighted average F..flat top above RecAge..do we want an option though..probably
    calculateWeightAveFAndAssignPRFs(NumYears,RecAge,Nage,FatAge,Catch,FullF,PRF);

    // Ok..that's all we need to send to the YPR calc.
    nmfUtils::initialize(tmpPRF, Nage);
    nmfUtils::initialize(tmpM2,  Nage);
    nmfUtils::initialize(tmpM1,  Nage);
    nmfUtils::initialize(tmpWt,  Nage);
    nmfUtils::initialize(tmpMat, Nage);

    for (int i = 0; i < NumYears; ++ i) {

        for (int j = 0; j < Nage; ++j) {
            tmpPRF(j) = PRF(i,j);
            tmpM2(j)  = M2atAge(i,j);
            tmpM1(j)  = M1atAge(i,j);
            tmpWt(j)  = WtAtAge(i,j);
            tmpMat(j) = Pmature(i,j);
        }

        // Calculate F0.1
        F01Val(i) = F01(tmpWt, tmpPRF, tmpM2, tmpM1, Nage);
        // Calculate Fmax
        FMaxVal(i) = FMax(tmpWt, tmpPRF, tmpM2, tmpM1, Nage);
        // Calculate FSSB03
        SSB03(i) = SSBBench(tmpWt, tmpPRF, tmpM2, tmpM1, Nage, tmpMat, 0.03);
        // Calculate FSSB10
        SSB10(i) = SSBBench(tmpWt, tmpPRF, tmpM2, tmpM1, Nage, tmpMat, 0.1);

        CombinedYPROut(i,0) = FullF(i);
        CombinedYPROut(i,1) = (FMaxVal(i) > 0) ? FMaxVal(i) : 2.5;
        CombinedYPROut(i,2) = F01Val(i);
        CombinedYPROut(i,3) = SSB03(i);
        CombinedYPROut(i,4) = SSB10(i);

        YMax = std::max({YMax,
                         CombinedYPROut(i,0),
                         CombinedYPROut(i,1),
                         CombinedYPROut(i,2),
                         CombinedYPROut(i,3),
                         CombinedYPROut(i,4)});

        GridData(i,0) = FullF(i);
        GridData(i,1) = FMaxVal(i);
        GridData(i,2) = F01Val(i);
        GridData(i,3) = SSB03(i);
        GridData(i,4) = SSB10(i);

    } // end for i

    LegendNames(0) = "Observed F";
    LegendNames(1) = "Fmax";
    LegendNames(2) = "F 0.1";
    LegendNames(3) = "F at 3% Max SSB/R";
    LegendNames(4) = "F at 10% Max SSB/R";

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;

    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      CombinedYPROut, LegendNames,
                      "HistoricaL F Benchmarks",
                      "Year", "Fishing Mortality Rate "+yAxisUnits,
                      XLabelNames,true);

} // end Forecast_YieldPerRecruit_HistoricalFBenchmarks



void
nmfOutputChartLine::MSVPA_YieldPerRecruit_HistoricalSSBBenchmarks(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::vector<std::string> XLabelNames;
    double YMax=0.0;
    std::string yAxisUnits = getYAxisUnits(selectedSpecies);
    boost::numeric::ublas::matrix<double> Pmature;
    boost::numeric::ublas::matrix<double> FatAge;
    boost::numeric::ublas::matrix<double> WtAtAge;
    boost::numeric::ublas::matrix<double> M1atAge;
    boost::numeric::ublas::matrix<double> M2atAge;
    boost::numeric::ublas::matrix<double> Catch;
    boost::numeric::ublas::matrix<double> YPROut;
    boost::numeric::ublas::matrix<double> PRF;
    boost::numeric::ublas::vector<double> FullF;
    boost::numeric::ublas::vector<double> YPRObs;
    boost::numeric::ublas::vector<double> tmpPRF;
    boost::numeric::ublas::vector<double> tmpM2;
    boost::numeric::ublas::vector<double> tmpM1;
    boost::numeric::ublas::vector<double> tmpWt;
    boost::numeric::ublas::vector<double> tmpMat;
    boost::numeric::ublas::vector<double> SSBMax;
    boost::numeric::ublas::matrix<double> CombinedYPROut;

    nmfUtils::initialize(Pmature, NumYears, Nage);
    nmfUtils::initialize(FatAge,  NumYears, Nage);
    nmfUtils::initialize(WtAtAge, NumYears, Nage);
    nmfUtils::initialize(M1atAge, NumYears, Nage);
    nmfUtils::initialize(M2atAge, NumYears, Nage);
    nmfUtils::initialize(Catch,   NumYears, Nage);
    nmfUtils::initialize(FullF,   NumYears);
    nmfUtils::initialize(PRF,     NumYears, Nage);
    nmfUtils::initialize(YPROut,  26, NumYears);  // RSK replace the hardcoded 25 with a constant
    nmfUtils::initialize(GridData,NumYears, 4);
    nmfUtils::initialize(YPRObs,  NumYears);
    nmfUtils::initialize(SSBMax,  NumYears);
    nmfUtils::initialize(CombinedYPROut,  NumYears, 3); // RSK replace with constant
    nmfUtils::initialize(LegendNames, 3);
    rowLabels.clear();
    colLabels.clear();
    colLabels << "Obs. SSB/R" << "SSB/R Max." << "3% SSB/R Max."
              << "10% SSB/R Max.";
    int firstYear = (theModelName == "MSVPA") ? FirstYear : Forecast_FirstYear;

    // OK...then will have to select appropriate data for your species and such
    // Get prop mature and wt at age

    for (int i=0; i<NumYears; ++i) {
        rowLabels << QString::number(firstYear+i);

        XLabelNames.push_back(std::to_string(Years(i)));

        // Get prop mature and wt at age
        databasePtr->nmfQueryMaturityData(
            i,Years(i),Nage,selectedSpecies,Pmature);

        // Get Wt At Age...wts at beginning of year
        databasePtr->nmfQueryWeightAtAgeData(
                    i,Years(i),Nage,selectedSpecies,WtAtAge);

        // Get mortalities at age F, M2, M1
        databasePtr->nmfQueryMortalityData(
                    MSVPAName,i,Years(i)-FirstYear,Nage,
                    selectedSpecies,FatAge,M1atAge,M2atAge);

        // Need to get catches to calculate a weighted average FullF
        databasePtr->nmfQueryAgeCatchData(
                    i,Years(i),Nage,selectedSpecies,Catch);

    } // end for i


    // Calculate weighted average F..flat top above RecAge..do we want an option though..probably
    calculateWeightAveFAndAssignPRFs(NumYears,RecAge,Nage,FatAge,Catch,FullF,PRF);

    // Ok..that's all we need to send to the YPR calc.
    nmfUtils::initialize(tmpPRF, Nage);
    nmfUtils::initialize(tmpM2,  Nage);
    nmfUtils::initialize(tmpM1,  Nage);
    nmfUtils::initialize(tmpWt,  Nage);
    nmfUtils::initialize(tmpMat, Nage);

    for (int i = 0; i < NumYears; ++ i) {

        for (int j = 0; j < Nage; ++j) {
            tmpPRF(j) = PRF(i,j);
            tmpM2(j)  = M2atAge(i,j);
            tmpM1(j)  = M1atAge(i,j);
            tmpWt(j)  = WtAtAge(i,j);
            tmpMat(j) = Pmature(i,j);
        }

        // Calculate Observed SSB
        YPRObs(i) = SSB(tmpWt, tmpPRF, tmpM2, FullF(i), tmpM1, Nage, tmpMat);
        // Calculate SSBMax
        SSBMax(i) = SSB(tmpWt, tmpPRF, tmpM2, 0, tmpM1, Nage, tmpMat);

        CombinedYPROut(i,0) = YPRObs(i);
        CombinedYPROut(i,1) = SSBMax(i) * 0.03;
        CombinedYPROut(i,2) = SSBMax(i) * 0.1;

        YMax = std::max({YMax,
                         CombinedYPROut(i,0),
                         CombinedYPROut(i,1),
                         CombinedYPROut(i,2)});

        GridData(i,0) = YPRObs(i);
        GridData(i,1) = SSBMax(i);
        GridData(i,2) = 0.03 * SSBMax(i);
        GridData(i,3) = 0.10 * SSBMax(i);

    } // end for i

    LegendNames(0) = "Observed SSB/R";
    LegendNames(1) = "3% of Max SSB/R";
    LegendNames(2) = "10% of Max SSB/R";

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      CombinedYPROut, LegendNames,
                      "Historical SSB/R Benchmarks",
                      "Year", "Fishing Mortality Rate "+yAxisUnits,
                      XLabelNames,true);

} // end MSVPA_YieldPerRecruit_HistoricalSSBBenchmarks




void
nmfOutputChartLine::MSVPA_MultispeciesPopulations_TotalBiomass(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int NMSVPASpe=0;

    double YMax = 0.0; // RSK remove this????
    std::vector<std::string> XLabelNames;
    boost::numeric::ublas::matrix<double> ChartData;
    boost::numeric::ublas::vector<std::string> MSVPASpeList;
    std::vector<int> MSVPASpeAge;
    Boost3DArrayDouble Maturity(
        boost::extents[nmfConstants::MaxTotalSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    rowLabels.clear();
    colLabels.clear();

    // Select sum database field
    std::string str1,str2;
    nmfUtils::split(seasonStr,"=",str1,str2);
    int season = stoi(str2);
    std::string abundVariable = (season == 0) ? "AnnBiomass" : "Biomass";

    getMSVPASpecies(databasePtr,MSVPAName,NMSVPASpe,MSVPASpeList);
    getMSVPASpeciesNamesAges(databasePtr,NMSVPASpe,MSVPASpeList,MSVPASpeAge);
    //getMaturity(databasePtr,Forecast_NYears,FirstYear,NMSVPASpe,MSVPASpeList,MSVPASpeAge,Maturity);

    nmfUtils::initialize(ChartData, NumYears, NMSVPASpe);
    nmfUtils::initialize(GridData,  NumYears, NMSVPASpe);

    for (int i = 0; i < NMSVPASpe; ++i) {
        colLabels << QString::fromStdString(MSVPASpeList(i));
        fields = {"Year","Biomass"};
        queryStr = "SELECT Year, Sum(" + abundVariable + ") AS Biomass FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND SpeName = '" + MSVPASpeList(i) + "'" +
                  seasonStr +
                " GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);
        for (int j = 0; j < NumYears; ++j) {
            if (i == 0) {
                rowLabels << QString::number(FirstYear+j); // RSK improve on this, both rowLabels and X... aren't necessary
                XLabelNames.push_back(std::to_string(FirstYear + j));
            }
            ChartData(j,i) = std::stod(dataMap["Biomass"][j]) / 1000.0; // RSK replace with constant
        }
    }
    GridData = ChartData;

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      ChartData,
                      MSVPASpeList,
                      "Total Biomass (Jan 1.) for MSVPA Species",
                      "Year",              // x title
                      "000 Metric Tons",   // y title
                      XLabelNames,true);

} // end MSVPA_MultispeciesPopulations_TotalBiomass



void nmfOutputChartLine::MSVPA_MultispeciesPopulations_TotalAbundance(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int NMSVPASpe=0;

    double YMax = 0.0; // RSK remove this????
    std::vector<std::string> XLabelNames;
    boost::numeric::ublas::matrix<double> ChartData;
    boost::numeric::ublas::vector<std::string> MSVPASpeList;
    std::vector<int> MSVPASpeAge;
    //Boost3DArrayDouble Maturity(
    //    boost::extents[nmfConstants::MaxTotalSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    rowLabels.clear();
    colLabels.clear();

    // Select sum database field
    std::string str1,str2;
    nmfUtils::split(seasonStr,"=",str1,str2);
    int season = stoi(str2);
    std::string abundVariable = (season == 0) ? "AnnAbund" : "SeasAbund";

    getMSVPASpecies(databasePtr,MSVPAName,NMSVPASpe,MSVPASpeList);
    getMSVPASpeciesNamesAges(databasePtr,NMSVPASpe,MSVPASpeList,MSVPASpeAge);
    //getMaturity(databasePtr,Forecast_NYears,FirstYear,NMSVPASpe,MSVPASpeList,MSVPASpeAge,Maturity);

    nmfUtils::initialize(ChartData, NumYears, NMSVPASpe);
    nmfUtils::initialize(GridData,  NumYears, NMSVPASpe);

    for (int i = 0; i < NMSVPASpe; ++i) {
        colLabels << QString::fromStdString(MSVPASpeList(i));
        fields = {"Year","Abundance"};
        queryStr = "SELECT Year, Sum(" + abundVariable + ") AS Abundance FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND SpeName = '" + MSVPASpeList(i) + "'" +
                  seasonStr +
                " GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int j = 0; j <  NumYears; ++j) {
            if (i == 0) {
                rowLabels << QString::number(FirstYear+j); // RSK improve on this, both rowLabels and X... aren't necessary
                XLabelNames.push_back(std::to_string(FirstYear + j));
            }
            //XLabelNames.push_back(std::to_string(FirstYear + j));
            ChartData(j,i) = std::stod(dataMap["Abundance"][j]) / 1000.0; // RSK replace with constant
        }
     }
    GridData = ChartData;

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      ChartData,
                      MSVPASpeList,
                      "Total Abundance (Jan 1.) of MSVPA Species",
                      "Year",               // x title
                      "Millions of Fish",   // y title
                      XLabelNames,true);

} // end MSVPA_MultispeciesPopulations_TotalAbundance


void nmfOutputChartLine::MSVPA_MultispeciesPopulations_Age1PlusBiomass(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int NMSVPASpe=0;

    double YMax = 0.0; // RSK remove this????
    std::vector<std::string> XLabelNames;
    boost::numeric::ublas::matrix<double> ChartData;
    boost::numeric::ublas::vector<std::string> MSVPASpeList;
    std::vector<int> MSVPASpeAge;
    //Boost3DArrayDouble Maturity(
    //    boost::extents[nmfConstants::MaxTotalSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    rowLabels.clear();
    colLabels.clear();

    // Select sum database field
    std::string str1,str2;
    nmfUtils::split(seasonStr,"=",str1,str2);
    int season = stoi(str2);
    std::string abundVariable = (season == 0) ? "AnnBiomass" : "Biomass";

    getMSVPASpecies(databasePtr,MSVPAName,NMSVPASpe,MSVPASpeList);
    getMSVPASpeciesNamesAges(databasePtr,NMSVPASpe,MSVPASpeList,MSVPASpeAge);
    //getMaturity(databasePtr,Forecast_NYears,FirstYear,NMSVPASpe,MSVPASpeList,MSVPASpeAge,Maturity);

    nmfUtils::initialize(ChartData, NumYears, NMSVPASpe);
    nmfUtils::initialize(GridData,  NumYears, NMSVPASpe);

    for (int i = 0; i < NMSVPASpe; ++i) {
        colLabels << QString::fromStdString(MSVPASpeList(i));

        fields = {"Year","Biomass"};
        queryStr = "SELECT Year, Sum(" + abundVariable + ") AS Biomass FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                   " AND SpeName = '" + MSVPASpeList(i) + "'" +
                     seasonStr +
                   " AND Age >= 1 GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int j = 0; j <  NumYears; ++j) {
            if (i == 0) {
                rowLabels << QString::number(FirstYear+j); // RSK improve on this, both rowLabels and X... aren't necessary
                XLabelNames.push_back(std::to_string(FirstYear + j));
            }
            //XLabelNames.push_back(std::to_string(FirstYear + j));
            ChartData(j,i) = std::stod(dataMap["Biomass"][j]) / 1000.0; // RSK replace with constant
        }
    }
    GridData = ChartData;

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      ChartData,
                      MSVPASpeList,
                      "Age 1+ Biomass (Jan 1.) for MSVPA Species",
                      "Year",              // x title
                      "000 Metric Tons",   // y title
                      XLabelNames,true);

} // end MSVPA_MultispeciesPopulations_Age1PlusBiomass



void nmfOutputChartLine::MSVPA_MultispeciesPopulations_Age1PlusAbundance(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int NMSVPASpe=0;

    double YMax = 0.0; // RSK remove this????
    std::vector<std::string> XLabelNames;
    boost::numeric::ublas::matrix<double> ChartData;
    boost::numeric::ublas::vector<std::string> MSVPASpeList;
    std::vector<int> MSVPASpeAge;
    rowLabels.clear();
    colLabels.clear();

    //Boost3DArrayDouble Maturity(
    //    boost::extents[nmfConstants::MaxTotalSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    // Select sum database field
    std::string str1,str2;
    nmfUtils::split(seasonStr,"=",str1,str2);
    int season = stoi(str2);
    std::string abundVariable = (season == 0) ? "AnnAbund" : "SeasAbund";

    getMSVPASpecies(databasePtr,MSVPAName,NMSVPASpe,MSVPASpeList);
    getMSVPASpeciesNamesAges(databasePtr,NMSVPASpe,MSVPASpeList,MSVPASpeAge);
    //getMaturity(databasePtr,NumYears,FirstYear,NMSVPASpe,MSVPASpeList,MSVPASpeAge,Maturity);

    nmfUtils::initialize(ChartData, NumYears, NMSVPASpe);
    nmfUtils::initialize(GridData,  NumYears, NMSVPASpe);

    for (int i = 0; i < NMSVPASpe; ++i) {
        colLabels << QString::fromStdString(MSVPASpeList(i));

        fields = {"Year","Abundance"};
        queryStr = "SELECT Year, Sum(" + abundVariable + ") AS Abundance FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND SpeName = '" + MSVPASpeList(i) + "'" +
                  seasonStr +
                " AND Age >= 1 GROUP By Year";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        for (int j = 0; j <  NumYears; ++j) {
            if (i == 0) {
                rowLabels << QString::number(FirstYear+j); // RSK improve on this, both rowLabels and X... aren't necessary
                XLabelNames.push_back(std::to_string(FirstYear + j));
            }
            //XLabelNames.push_back(std::to_string(FirstYear + j));
            ChartData(j,i) = std::stod(dataMap["Abundance"][j]) / 1000.0; // RSK replace with constant
        }
    }
    GridData = ChartData;

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      ChartData,
                      MSVPASpeList,
                      "Age 1+ Abundance (Jan 1.) of MSVPA Species",
                      "Year",              // x title
                      "Millions of Fish",   // y title
                      XLabelNames,true);

} // end MSVPA_MultispeciesPopulations_Age1PlusAbundance



void nmfOutputChartLine::MSVPA_MultispeciesPopulations_SpawningStockBiomass(
        nmfDatabase* databasePtr,
        QChart* chart,
        std::string theModelName,
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
        double YMaxOverride,
        QStringList &rowLabels,
        QStringList &colLabels,
        boost::numeric::ublas::matrix<double> &GridData)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int NMSVPASpe=0;
    int m=0;

    double YMax = 0.0; // RSK remove this????
    std::vector<std::string> XLabelNames;
    boost::numeric::ublas::matrix<double> ChartData;
    boost::numeric::ublas::vector<std::string> MSVPASpeList;
    std::vector<int> MSVPASpeAge;
    Boost3DArrayDouble Maturity(
        boost::extents[nmfConstants::MaxTotalSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    // Select sum database field
    std::string str1,str2;
    nmfUtils::split(seasonStr,"=",str1,str2);
    int season = stoi(str2);
    std::string abundVariable = (season == 0) ? "AnnBiomass" : "Biomass";
    rowLabels.clear();
    colLabels.clear();

    getMSVPASpecies(databasePtr,MSVPAName,NMSVPASpe,MSVPASpeList);
    getMSVPASpeciesNamesAges(databasePtr,NMSVPASpe,MSVPASpeList,MSVPASpeAge);
    getMaturity(databasePtr,NumYears,FirstYear,NMSVPASpe,MSVPASpeList,MSVPASpeAge,Maturity);

    nmfUtils::initialize(ChartData, NumYears, NMSVPASpe);
    nmfUtils::initialize(GridData,  NumYears, NMSVPASpe);

    for (int i = 0; i < NMSVPASpe; ++i) {
        colLabels << QString::fromStdString(MSVPASpeList(i));

        fields = {"Year","Age","Biomass"};
        queryStr = "SELECT Year, Age, Sum(" + abundVariable + ") AS Biomass FROM MSVPASeasBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
                " AND SpeName = '"  + MSVPASpeList(i) + "'" +
                  seasonStr +
                " GROUP By Year, Age";
        dataMap = databasePtr->nmfQueryDatabase(queryStr, fields);

        m = 0;
        for (int j = 0; j < NumYears; ++j) {
            if (i == 0) {
                rowLabels << QString::number(FirstYear+j); // RSK improve on this, both rowLabels and X... aren't necessary
                XLabelNames.push_back(std::to_string(FirstYear + j));
            }
            //XLabelNames.push_back(std::to_string(FirstYear + j));
            for (int k = 0; k < MSVPASpeAge[i]; ++k) {
                ChartData(j,i) += (Maturity[i][k][j] * std::stod(dataMap["Biomass"][m++]) / 1000.0); // RSK replace with constant
            }
            ChartData(j, i) *= 0.5;  // sex ratio again !
        }
    }
    GridData = ChartData;

    YMax = (YMaxOverride > 0.0) ? YMaxOverride : YMax;
    loadChartWithData(chart, YMax,
                      selectedSpecies,
                      ChartData,
                      MSVPASpeList,
                      "Spawning Stock Biomass (Jan 1.) for MSVPA Species",
                      "Year",              // x title
                      "000 Metric Tons",   // y title
                      XLabelNames,true);
} // end MSVPA_MultispeciesPopulations_SpawningStockBiomass



