/**
 @file nmfDatabase.cpp
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This file contains the NMFS DatabaseAPI implementation.
 @date Nov 21, 2016
 */

/**
 * @mainpage Overview
 *
 * @section MySQL Database API
 *
 * The files in this project contain MySQL database methods. These routines
 * allow the user to access the appropriate MySQL database and extract the
 * desired data.
 *
 * @section Background
 *
 * This API assumes the user is interacting with a MySQL database. If the database
 * type should change, the user would just need to re-write the methods in this
 * project. The calling code shouldn't need to change.
 *
 * The MySQL relational database was chosen as it's open source and cross platform. There
 * are, however, slight differences with running MySQL on Windows vs Linux. One such difference
 * is that the database table name are case insensitive on Window and hence appear as all
 * lowercase on Windows. Database tables may use mixed case on Linux.
 *
 * N.B. If you're exporting a database from Linux to Windows, that will work fine. However, if you're
 * exporting a database from Windows to Linux, the table names will import into Linux as all lowercase
 * and if the code is expecting mixed case table names, you'll get errors. I suggest always to use
 * lowercase table names.
 *
 */
//#define DEBUG 1
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl;} while (false)
#else
#define DEBUG_MSG(str) do {} while (false)
#endif


#include <QDebug>
#include <QSqlError>
#include <set>
#include "nmfDatabase.h"
#include "nmfConstants.h"
#include "nmfUtils.h"


void nmfDatabase::checkForTableAndCreate(QString table)
{
    if (m_FunctionMap.find(table.toStdString()) == m_FunctionMap.end()) {

        std::cout << "Warning: \"" << table.toStdString() << "\" not found in nmfDatabase::FunctionMap" << std::endl;

    } else {
        std::string errorMsg;
        QString qcmd;

        m_FunctionMap[table.toStdString()](table.toStdString(),qcmd);

        errorMsg = nmfUpdateDatabase(qcmd.toStdString());
        if (nmfUtilsQt::isAnError(errorMsg)) {
            nmfUtils::printError("Create table error: ", errorMsg);
        }

    }
} // checkForTableAndCreate

void
nmfDatabase::createApplication(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(Name  VARCHAR(255) NOT NULL,";
    qcmd += " PRIMARY KEY (Name))";

} // end createApplication

void
nmfDatabase::createForeEnergyDens(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName  VARCHAR(255) NOT NULL,";
    qcmd += " ForeName   VARCHAR(255) NOT NULL,";
    qcmd += " SpeName    VARCHAR(255),";
    qcmd += " SpeType    int(11),";
    qcmd += " SpeIndex   int(11)      NOT NULL,";
    qcmd += " Age        int(11)      NOT NULL,";
    qcmd += " Season     int(11)      NOT NULL,";
    qcmd += " EnergyDens double,";
    qcmd += " AvgDietE   double,";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,SpeIndex,Age,Season))";

} // end createForeEnergyDens


void
nmfDatabase::createForeOutput(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName       VARCHAR(255) NOT NULL,";
    qcmd += " ForeName        VARCHAR(255) NOT NULL,";
    qcmd += " Scenario        VARCHAR(255) NOT NULL,";
    qcmd += " Year            int(11)      NOT NULL,";
    qcmd += " Season          int(11)      NOT NULL,";
    qcmd += " SpeName         VARCHAR(100) NOT NULL,";
    qcmd += " Age             int(11)      NOT NULL,";
    qcmd += " SpeType         int(11),";
    qcmd += " InitAbund       double,";
    qcmd += " EndAbund        double,";
    qcmd += " InitBiomass     double,";
    qcmd += " EndBiomass      double,";
    qcmd += " SeasM2          double,";
    qcmd += " SeasF           double,";
    qcmd += " SeasM1          double,";
    qcmd += " AvgSize         double,";
    qcmd += " AvgWeight       double,";
    qcmd += " TotalSBM        double,";
    qcmd += " TotalBMConsumed double,";
    qcmd += " StomCont        double,";
    qcmd += " InitWt          double,";
    qcmd += " EndWt           double,";
    qcmd += " SeasCatch       double,";
    qcmd += " SeasYield       double,";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,Scenario,Year,Season,SpeName,Age))";

} // end createForeOutput


void
nmfDatabase::createForePredGrowth(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " ForeName  VARCHAR(255) NOT NULL,";
    qcmd += " PredName  VARCHAR(255),";
    qcmd += " PredIndex int(11)      NOT NULL,";
    qcmd += " PredType  int(11),";
    qcmd += " Age       int(11)      NOT NULL,";
    qcmd += " RAlpha    double,";
    qcmd += " RBeta     double,";
    qcmd += " RQ        double,";
    qcmd += " ACT       double,";
    qcmd += " E         double,";
    qcmd += " U         double,";
    qcmd += " SDA       double,";
    qcmd += " LWAlpha   double,";
    qcmd += " LWBeta    double,";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,PredIndex,Age))";

} // end createForePredGrowth


void
nmfDatabase::createForePredVonB(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " ForeName  VARCHAR(255) NOT NULL,";
    qcmd += " PredName  VARCHAR(255) NOT NULL,";
    qcmd += " PredIndex int(11),";
    qcmd += " PredType  int(11),";
    qcmd += " Linf      double,";
    qcmd += " GrowthK   double,";
    qcmd += " TZero     double,";
    qcmd += " LWAlpha   double,";
    qcmd += " LWBeta    double,";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,PredName))";

} // end createForePredVonB


void
nmfDatabase::createForeSRQ(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " ForeName  VARCHAR(255) NOT NULL,";
    qcmd += " SpeName   VARCHAR(255),";
    qcmd += " SpeIndex  int(11)      NOT NULL,";
    qcmd += " Quartile  int(11)      NOT NULL,";
    qcmd += " MinSSB    double,";
    qcmd += " MaxSSB    double,";
    qcmd += " MinRec    double,";
    qcmd += " MaxRec    double,";
    qcmd += " MeanRec   double,";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,SpeIndex,Quartile))";

} // end createForeSRQ


void
nmfDatabase::createForeSRR(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName   VARCHAR(255) NOT NULL,";
    qcmd += " ForeName    VARCHAR(255) NOT NULL,";
    qcmd += " SpeName     VARCHAR(255),";
    qcmd += " SpeIndex    int(11)      NOT NULL,";
    qcmd += " SRRType     int(11),";
    qcmd += " SRRA        double,";
    qcmd += " SRRB        double,";
    qcmd += " SRRK        double,";
    qcmd += " Userdefined tinyint(1),";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,SpeIndex))";

} // end createForeSRR


void
nmfDatabase::createForeSuitPreyBiomass(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName       VARCHAR(255) NOT NULL,";
    qcmd += " ForeName        VARCHAR(100) NOT NULL,";
    qcmd += " Scenario        VARCHAR(100) NOT NULL,";
    qcmd += " PredName        VARCHAR(100) NOT NULL,";
    qcmd += " PredAge         int(11)      NOT NULL,";
    qcmd += " PreyName        VARCHAR(100) NOT NULL,";
    qcmd += " PreyAge         int(11)      NOT NULL,";
    qcmd += " Year            int(11)      NOT NULL,";
    qcmd += " Season          int(11)      NOT NULL,";
    qcmd += " SuitPreyBiomass double,";
    qcmd += " PropDiet        double,";
    qcmd += " EDens           double,";
    qcmd += " BMConsumed      double,";
    qcmd += " PredType        int(11),";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,Scenario,PredName,PredAge,PreyName,PreyAge,Year,Season))";

} // end createForeSuitPreyBiomass

void
nmfDatabase::createForecasts(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " ForeName  VARCHAR(255) NOT NULL,";
    qcmd += " InitYear  int(11),";
    qcmd += " NYears    VARCHAR(255),";
    qcmd += " Growth    int(11),";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName))";

} // end createForecasts
void
nmfDatabase::createMSVPAEnergyDens(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName  VARCHAR(255) NOT NULL,";
    qcmd += " SpeName    VARCHAR(255),";
    qcmd += " SpeType    int(11),";
    qcmd += " SpeIndex   int(11)      NOT NULL,";
    qcmd += " Age        int(11)      NOT NULL,";
    qcmd += " Season     int(11)      NOT NULL,";
    qcmd += " EnergyDens double,";
    qcmd += " AvgDietE   double,";
    qcmd += " PRIMARY KEY (MSVPAName,SpeIndex,Age,Season))";

} // end createMSVPAEnergyDens


void
nmfDatabase::createMSVPAOthPrey(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName   VARCHAR(255) NOT NULL,";
    qcmd += " OthPreyName VARCHAR(255) NOT NULL,";
    qcmd += " MinSize     double,";
    qcmd += " MaxSize     double,";
    qcmd += " SizeAlpha   double,";
    qcmd += " SizeBeta    double,";
    qcmd += " PRIMARY KEY (MSVPAName,OthPreyName))";

} // end createMSVPAOthPrey


void
nmfDatabase::createMSVPAOthPreyAnn(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName   VARCHAR(255) NOT NULL,";
    qcmd += " OthPreyName VARCHAR(255) NOT NULL,";
    qcmd += " Year        int(11)      NOT NULL,";
    qcmd += " Season      int(11)      NOT NULL,";
    qcmd += " Biomass     double,";
    qcmd += " PRIMARY KEY (MSVPAName,OthPreyName,Year,Season))";

} // end createMSVPAOthPreyAnn


void
nmfDatabase::createMSVPASeasBiomass(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName     VARCHAR(255) NOT NULL,";
    qcmd += " SpeName       VARCHAR(255) NOT NULL,";
    qcmd += " Season        int(11)      NOT NULL,";
    qcmd += " SpeType       int(11),";
    qcmd += " Year          int(11)      NOT NULL,";
    qcmd += " Age           int(11)      NOT NULL,";
    qcmd += " Biomass       double,";
    qcmd += " TotalPredCons double,";
    qcmd += " SeasM2        double,";
    qcmd += " AnnAbund      double,";
    qcmd += " SeasAbund     double,";
    qcmd += " SeasF         double,";
    qcmd += " SeasM1        double,";
    qcmd += " SeasWt        double,";
    qcmd += " SeasSize      double,";
    qcmd += " AnnBiomass    double,";
    qcmd += " StomCont      double,";
    qcmd += " PRIMARY KEY (MSVPAName,SpeName,Season,Year,Age))";

} // end createMSVPASeasBiomass


void
nmfDatabase::createMSVPASeasInfo(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " Year      int(11)      NOT NULL,";
    qcmd += " Season    int(11)      NOT NULL,";
    qcmd += " Variable  VARCHAR(255) NOT NULL,";
    qcmd += " Value     double,";
    qcmd += " PRIMARY KEY (MSVPAName,Year,Season,Variable))";

} // end createMSVPASeasInfo


void
nmfDatabase::createMSVPASizePref(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " SpeIndex  int(11)      NOT NULL,";
    qcmd += " SpeName   VARCHAR(255),";
    qcmd += " Age       int(11)      NOT NULL,";
    qcmd += " EvacAlpha double,";
    qcmd += " EvacBeta  double,";
    qcmd += " SizeAlpha double,";
    qcmd += " SizeBeta  double,";
    qcmd += " PRIMARY KEY (MSVPAName,SpeIndex,Age))";

} // end createMSVPASizePref


void
nmfDatabase::createMSVPASpaceO(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " Season    int(11)      NOT NULL,";
    qcmd += " SpeIndex  int(11),";
    qcmd += " SpeType   int(11),";
    qcmd += " SpeName   VARCHAR(255) NOT NULL,";
    qcmd += " Age       int(11)      NOT NULL,";
    qcmd += " PreyIndex int(11)      NOT NULL,";
    qcmd += " PreyName  VARCHAR(255),";
    qcmd += " SpOverlap double,";
    qcmd += " PRIMARY KEY (MSVPAName,Season,SpeName,Age,PreyIndex))";

} // end createMSVPASpaceO


void
nmfDatabase::createMSVPAStomCont(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName   VARCHAR(255) NOT NULL,";
    qcmd += " SpeIndex    int(11)      NOT NULL,";
    qcmd += " SpeType     int(11)      NOT NULL,";
    qcmd += " SpeName     VARCHAR(255),";
    qcmd += " Age         int(11)      NOT NULL,";
    qcmd += " Season      int(11)      NOT NULL,";
    qcmd += " MeanGutFull double,";
    qcmd += " PRIMARY KEY (MSVPAName,SpeIndex,SpeType,Age,Season))";

} // end createMSVPAStomCont


void
nmfDatabase::createMSVPASuitPreyBiomass(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName       VARCHAR(255) NOT NULL,";
    qcmd += " PredName        VARCHAR(255) NOT NULL,";
    qcmd += " PredAge         int(11)      NOT NULL,";
    qcmd += " PreyName        VARCHAR(255) NOT NULL,";
    qcmd += " PreyAge         int(11)      NOT NULL,";
    qcmd += " Year            int(11)      NOT NULL,";
    qcmd += " Season          int(11)      NOT NULL,";
    qcmd += " SuitPreyBiomass double,";
    qcmd += " PropDiet        double,";
    qcmd += " BMConsumed      double,";
    qcmd += " PreyEDens       double,";
    qcmd += " PRIMARY KEY (MSVPAName,PredName,PredAge,PreyName,PreyAge,Year,Season))";
} // end createMSVPASuitPreyBiomass


void
nmfDatabase::createMSVPAlist(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName   VARCHAR(255) NOT NULL,";
    qcmd += " NSpe        int(11),";
    qcmd += " NPreyOnly   int(11),";
    qcmd += " NOther      int(11),";
    qcmd += " NOtherPred  int(11),";
    qcmd += " FirstYear   int(11),";
    qcmd += " LastYear    int(11),";
    qcmd += " NSeasons    int(11),";
    qcmd += " AnnTemps    int(11),";
    qcmd += " SeasSpaceO  int(11),";
    qcmd += " GrowthModel int(11),";
    qcmd += " Complete    int(11),";
    qcmd += " PRIMARY KEY (MSVPAName))";
} // end createMSVPAlist


void
nmfDatabase::createMSVPAprefs(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " SpeIndex  int(11),";
    qcmd += " SpeType   int(11),";
    qcmd += " SpeName   VARCHAR(255) NOT NULL,";
    qcmd += " Age       int(11)      NOT NULL,";
    qcmd += " PreyIndex int(11)      NOT NULL,";
    qcmd += " PreyName  VARCHAR(255),";
    qcmd += " PrefRank  double,";
    qcmd += " PrefVal   double,";
    qcmd += " PRIMARY KEY (MSVPAName,SpeName,Age,PreyIndex))";
} // end createMSVPAprefs


void
nmfDatabase::createMSVPAspecies(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName  VARCHAR(255) NOT NULL,";
    qcmd += " SpeName    VARCHAR(255) NOT NULL,";
    qcmd += " SpeIndex   int(11),";
    qcmd += " Type       int(11),";
    qcmd += " TimeRec    double,";
    qcmd += " LenRec     double,";
    qcmd += " WtRec      double,";
    qcmd += " SSVPAname  VARCHAR(255),";
    qcmd += " SSVPAindex int(11),";
    qcmd += " PRIMARY KEY (MSVPAName,SpeName))";
} // end createMSVPAspecies


void
nmfDatabase::createOthPredSizeData(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex     int(11) NOT NULL,";
    qcmd += " SpeName      VARCHAR(255),";
    qcmd += " SizeCat      int(11) NOT NULL,";
    qcmd += " MinLen       int(11),";
    qcmd += " MaxLen       int(11),";
    qcmd += " PropBM       double,";
    qcmd += " ConsAlpha    double,";
    qcmd += " ConsBeta     double,";
    qcmd += " SizeSelAlpha double,";
    qcmd += " SizeSelBeta  double,";
    qcmd += " PRIMARY KEY (SpeIndex,SizeCat))";
} // end createOthPredSizeData


void
nmfDatabase::createOtherPredBM(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex int(11) NOT NULL,";
    qcmd += " SpeName VARCHAR(255),";
    qcmd += " Year int(11) NOT NULL,";
    qcmd += " Biomass double,";
    qcmd += " PRIMARY KEY (SpeIndex,Year))";

} // end createOtherPredBM


void
nmfDatabase::createOtherPredSpecies(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex      int(11) NOT NULL,";
    qcmd += " SpeName       VARCHAR(255),";
    qcmd += " FirstYear     int(11),";
    qcmd += " LastYear      int(11),";
    qcmd += " BMUnitIndex   int(11),";
    qcmd += " BMUnits       double,";
    qcmd += " WtUnitIndex   int(11),";
    qcmd += " WtUnits       double,";
    qcmd += " SizeUnitIndex int(11),";
    qcmd += " SizeUnits     double,";
    qcmd += " MinSize       int(11),";
    qcmd += " MaxSize       int(11),";
    qcmd += " SizeStruc     int(11),";
    qcmd += " NumSizeCats   int(11),";
    qcmd += " PRIMARY KEY (SpeIndex))";
} // end createOtherPredSpecies


void
nmfDatabase::createSSVPAAgeM(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex   int(11) NOT NULL,";
    qcmd += " SSVPAIndex int(11) NOT NULL,";
    qcmd += " SSVPAName  VARCHAR(255),";
    qcmd += " Age        int(11) NOT NULL,";
    qcmd += " AgeM1      double,";
    qcmd += " AgeM2      double,";
    qcmd += " PRF        double,";
    qcmd += " PRIMARY KEY (SpeIndex,SSVPAIndex,Age))";
} // end createSSVPAAgeM


void
nmfDatabase::createScenarioF(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " ForeName  VARCHAR(75)  NOT NULL,";
    qcmd += " Scenario  VARCHAR(75)  NOT NULL,";
    qcmd += " VarType   VARCHAR(75),";
    qcmd += " SpeName   VARCHAR(75),";
    qcmd += " SpeIndex  int(11)      NOT NULL,";
    qcmd += " Age       int(11)      NOT NULL,";
    qcmd += " Year      int(11)      NOT NULL,";
    qcmd += " F         double,";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,Scenario,SpeIndex,Age,Year))";

} // end createScenarioF


void
nmfDatabase::createScenarioOthPred(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " ForeName  VARCHAR(100) NOT NULL,";
    qcmd += " Scenario  VARCHAR(100) NOT NULL,";
    qcmd += " SpeName   VARCHAR(100) NOT NULL,";
    qcmd += " SpeIndex  int(11),";
    qcmd += " SizeClass int(11)      NOT NULL,";
    qcmd += " Year      int(11)      NOT NULL,";
    qcmd += " Biomass   double,";
    qcmd += " PropBM    double,";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,Scenario,SpeName,SizeClass,Year))";

} // end createScenarioOthPred


void
nmfDatabase::createScenarioOthPrey(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " ForeName  VARCHAR(255) NOT NULL,";
    qcmd += " Scenario  VARCHAR(255) NOT NULL,";
    qcmd += " SpeName   VARCHAR(255),";
    qcmd += " SpeIndex  int(11)      NOT NULL,";
    qcmd += " Year      int(11)      NOT NULL,";
    qcmd += " Season    int(11)      NOT NULL,";
    qcmd += " Biomass   double,";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,Scenario,SpeName,Year,Season))";

} // end createScenarioOthPrey


void
nmfDatabase::createScenarioRec(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName   VARCHAR(255) NOT NULL,";
    qcmd += " ForeName    VARCHAR(100) NOT NULL,";
    qcmd += " Scenario    VARCHAR(100) NOT NULL,";
    qcmd += " SpeName     VARCHAR(100) NOT NULL,";
    qcmd += " SpeIndex    int(11),";
    qcmd += " Year        int(11)      NOT NULL,";
    qcmd += " RecAdjust   double,";
    qcmd += " AbsRecruits int(11),";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,Scenario,SpeName,Year))";

} // end createScenarioRec


void
nmfDatabase::createScenarios(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(MSVPAName  VARCHAR(255) NOT NULL,";
    qcmd += " ForeName   VARCHAR(255) NOT NULL,";
    qcmd += " Scenario   VARCHAR(255) NOT NULL,";
    qcmd += " VarF       int(11),";
    qcmd += " VarOthPred int(11),";
    qcmd += " VarOthPrey int(11),";
    qcmd += " VarRec     int(11),";
    qcmd += " FishAsF    tinyint(1),";
    qcmd += " PRIMARY KEY (MSVPAName,ForeName,Scenario))";

} // end createScenarios


void
nmfDatabase::createSpeCatch(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex int(11) NOT NULL,";
    qcmd += " SpeName  VARCHAR(255),";
    qcmd += " Year     int(11) NOT NULL,";
    qcmd += " Age      int(11) NOT NULL,";
    qcmd += " Catch    float,";
    qcmd += " PRIMARY KEY (SpeIndex,Year,Age))";
} // end createSpeCatch


void
nmfDatabase::createSpeMaturity(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex int(11) NOT NULL,";
    qcmd += " SpeName  VARCHAR(255),";
    qcmd += " Year     int(11) NOT NULL,";
    qcmd += " Age      int(11) NOT NULL,";
    qcmd += " PMature  float,";
    qcmd += " PRIMARY KEY (SpeIndex,Year,Age))";
} // end createSpeMaturity


void
nmfDatabase::createSpeSSVPA(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex       int(11) NOT NULL,";
    qcmd += " SSVPAName      VARCHAR(255) NOT NULL,";
    qcmd += " SSVPAIndex     int(11) NOT NULL,";
    qcmd += " Type           int(11),";
    qcmd += " AgeM           int(11),";
    qcmd += " TermZType      int(11),";
    qcmd += " TermF          double,";
    qcmd += " RefAge         int(11),";
    qcmd += " LogCatchAge1   int(11),";
    qcmd += " LogCatchAge2   int(11),";
    qcmd += " FullRecAge     int(11),";
    qcmd += " PartSVPA       int(11),";
    qcmd += " pSVPANCatYrs   int(11),";
    qcmd += " pSVPANMortYrs  int(11),";
    qcmd += " pSVPARefAge    int(11),";
    qcmd += " NFleets        int(11),";
    qcmd += " NXSAIndex      int(11),";
    qcmd += " Downweight     int(11),";
    qcmd += " DownweightType int(11),";
    qcmd += " DownweightYear int(11),";
    qcmd += " Shrink         tinyint(1),";
    qcmd += " ShrinkCV       double,";
    qcmd += " ShrinkYears    int(11),";
    qcmd += " ShrinkAge      int(11),";
    qcmd += " PRIMARY KEY (SpeIndex,SSVPAName,SSVPAIndex))";
} // end createSSVPAAgeM


void
nmfDatabase::createSpeSize(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex int(11) NOT NULL,";
    qcmd += " SpeName VARCHAR(255),";
    qcmd += " Year int(11) NOT NULL,";
    qcmd += " Age int(11) NOT NULL,";
    qcmd += " Variable VARCHAR(255) NOT NULL,";
    qcmd += " Value double,";
    qcmd += " PRIMARY KEY (SpeIndex,Year,Age,Variable))";
} // end createSpeSize


void
nmfDatabase::createSpeTuneCatch(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeName    VARCHAR(255),";
    qcmd += " SpeIndex   int(11) NOT NULL,";
    qcmd += " SSVPAName  VARCHAR(255) NOT NULL,";
    qcmd += " Fleet      int(11) NOT NULL,";
    qcmd += " Year       int(11) NOT NULL,";
    qcmd += " Age        int(11) NOT NULL,";
    qcmd += " Catch      double,";
    qcmd += " PRIMARY KEY (SpeIndex,SSVPAName,Fleet,Year,Age))";
} // end createTuneCatch


void
nmfDatabase::createSpeTuneEffort(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeName    VARCHAR(255),";
    qcmd += " SpeIndex   int(11) NOT NULL,";
    qcmd += " SSVPAName  VARCHAR(255) NOT NULL,";
    qcmd += " Fleet      int(11) NOT NULL,";
    qcmd += " Year       int(11) NOT NULL,";
    qcmd += " Effort     double,";
    qcmd += " PRIMARY KEY (SpeIndex,SSVPAName,Fleet,Year))";
} // end createTuneEffort


void
nmfDatabase::createSpeWeight(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex int(11) NOT NULL,";
    qcmd += " SpeName VARCHAR(255),";
    qcmd += " Year int(11) NOT NULL,";
    qcmd += " Age int(11) NOT NULL,";
    qcmd += " Variable VARCHAR(255) NOT NULL,";
    qcmd += " Value double,";
    qcmd += " PRIMARY KEY (SpeIndex,Year,Age,Variable))";
} // end createSpeWeight


void
nmfDatabase::createSpeXSAData(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex  int(11) NOT NULL,";
    qcmd += " SSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " `Index`   int(11) NOT NULL,";
    qcmd += " Year      int(11),";
    qcmd += " Age       int(11),";
    qcmd += " Value     double,";
    qcmd += " PRIMARY KEY (SpeIndex,SSVPAName,`Index`,Year,Age))";
} // end createSpeXSAData


void
nmfDatabase::createSpeXSAIndices(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex  int(11) NOT NULL,";
    qcmd += " SSVPAName VARCHAR(255) NOT NULL,";
    qcmd += " `Index`   int(11) NOT NULL,";
    qcmd += " IndexName VARCHAR(255),";
    qcmd += " Alpha     double,";
    qcmd += " Beta      double,";
    qcmd += " PRIMARY KEY (SpeIndex,SSVPAName,`Index`))";
} // end createSpeXSAIndices


void
nmfDatabase::createSpecies(const std::string table, QString &qcmd)
{
    qcmd  = "CREATE TABLE IF NOT EXISTS " + QString::fromStdString(table);
    qcmd += "(SpeIndex      int(11) NOT NULL,";
    qcmd += " SpeName       VARCHAR(255),";
    qcmd += " MaxAge        int(11),";
    qcmd += " MinCatAge     int(11),";
    qcmd += " MaxCatAge     int(11),";
    qcmd += " FirstYear     int(11),";
    qcmd += " LastYear      int(11),";
    qcmd += " PlusClass     tinyint(1),";
    qcmd += " CatchUnits    double,";
    qcmd += " WtTypeIndex   int(11),";
    qcmd += " WtUnits       double,";
    qcmd += " SizeTypeIndex int(11),";
    qcmd += " SizeUnits     double,";
    qcmd += " FixedMaturity tinyint(1),";
    qcmd += " PRIMARY KEY (SpeIndex))";
} // end createSpecies

