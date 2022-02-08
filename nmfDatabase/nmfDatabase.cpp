/**
 *
 * @file nmfDatabase.h
 * @date Nov 21, 2016
 * @author Ronald Klasky
 *
 * Public Domain Notice
 * National Oceanic And Atmospheric Administration
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
 * for any particular purpose.
 *
 * Please cite the author(s) in any work or product based on this material.
 *
 * @brief This file contains the database API implementation (currently implemented for MySQL).
 */


//#define DEBUG 1
#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl;} while (false)
#else
#define DEBUG_MSG(str) do {} while (false)
#endif

#include <set>

#include "nmfDatabase.h"
#include "nmfConstants.h"
#include "nmfUtils.h"
#include "nmfUtilsQt.h"
#include <stdio.h>

nmfDatabase::nmfDatabase() {

    m_dbName.clear();
//  db = QSqlDatabase::addDatabase("QMYSQL");
//  db = QSqlDatabase::addDatabase("QSQLITE");

    m_FunctionMap[nmfConstantsMSVPA::TableApplication]          = createApplication;          //  0 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableForeEnergyDens]       = createForeEnergyDens;       //  1 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableForeOutput]           = createForeOutput;           //  2 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableForePredGrowth]       = createForePredGrowth;       //  3 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableForePredVonB]         = createForePredVonB;         //  4 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableForeSRQ]              = createForeSRQ;              //  5 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableForeSRR]              = createForeSRR;              //  6 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableForeSuitPreyBiomass]  = createForeSuitPreyBiomass;  //  7 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableForecasts]            = createForecasts;            //  8 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPAEnergyDens]      = createMSVPAEnergyDens;      //  9 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPAOthPrey]         = createMSVPAOthPrey;         // 10 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPAOthPreyAnn]      = createMSVPAOthPreyAnn;      // 11 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPASeasBiomass]     = createMSVPASeasBiomass;     // 12 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPASeasInfo]        = createMSVPASeasInfo;        // 13 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPASizePref]        = createMSVPASizePref;        // 14 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPASpaceO]          = createMSVPASpaceO;          // 15 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPAStomCont]        = createMSVPAStomCont;        // 16 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPASuitPreyBiomass] = createMSVPASuitPreyBiomass; // 17 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPAlist]            = createMSVPAlist;            // 18 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPAprefs]           = createMSVPAprefs;           // 19 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableMSVPAspecies]         = createMSVPAspecies;         // 20 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableOthPredSizeData]      = createOthPredSizeData;      // 21 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableOtherPredBM]          = createOtherPredBM;          // 22 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableOtherPredSpecies]     = createOtherPredSpecies;     // 23 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSSVPAAgeM]            = createSSVPAAgeM;            // 24 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableScenarioF]            = createScenarioF;            // 25 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableScenarioOthPred]      = createScenarioOthPred;      // 26 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableScenarioOthPrey]      = createScenarioOthPrey;      // 27 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableScenarioRec]          = createScenarioRec;          // 28 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableScenarios]            = createScenarios;            // 29 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSpeCatch]             = createSpeCatch;             // 30 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSpeMaturity]          = createSpeMaturity;          // 31 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSpeSSVPA]             = createSpeSSVPA;             // 32 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSpeSize]              = createSpeSize;              // 33 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSpeTuneCatch]         = createSpeTuneCatch;         // 34 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSpeTuneEffort]        = createSpeTuneEffort;        // 35 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSpeWeight]            = createSpeWeight;            // 36 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSpeXSAData]           = createSpeXSAData;           // 37 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSpeXSAIndices]        = createSpeXSAIndices;        // 38 of 39
    m_FunctionMap[nmfConstantsMSVPA::TableSpecies]              = createSpecies;              // 39 of 39
}

void
nmfDatabase::nmfSetDatabase(std::string newDatabaseName)
{
    QSqlDatabase db = QSqlDatabase::database();

    if (databaseExists(newDatabaseName)) {
        db.setDatabaseName(newDatabaseName.c_str());
        bool dbOpenOK = db.open();
        if (! dbOpenOK) {
            std::cout << "Error: Couldn't open database: " << newDatabaseName << std::endl;
        }
    }
}

//std::string
//nmfDatabase::nmfLastError()
//{
//    QSqlDatabase db = QSqlDatabase::database();
//    return db.lastError().text().toStdString();
//}


bool
nmfDatabase::nmfOpenDatabase(
        const std::string& hostName,
        const std::string& userName,
        const std::string& password,
        std::string&       errorMsg)
{
    QSqlDatabase db = QSqlDatabase::database();

    errorMsg.clear();
    db.setHostName(hostName.c_str());
    db.setUserName(userName.c_str());
    db.setPassword(password.c_str());

    bool dbOpenOK = db.open();
    if (! dbOpenOK) {
        errorMsg = db.lastError().text().toStdString();
        return false;
    }
    return true;
}

std::string
nmfDatabase::nmfGetCurrentDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    std::string qry = "SELECT database();";
    std::string databaseName;
    QSqlQuery query;
    if (query.exec(qry.c_str())) {
        while (query.next()) {
            databaseName = query.value(0).toString().toStdString();
        }
    } else {
        qDebug() << "Error: " << db.lastError().text();
    }

    return databaseName;
}

std::vector<std::string>
nmfDatabase::nmfGetDatabaseNames()
{
    QSqlDatabase db = QSqlDatabase::database();
    std::vector<std::string> databaseNames;
    std::string qry = "SHOW databases";
    std::set<std::string> exclusionSet = { "information_schema", "mysql", "performance_schema", "sys" };
    std::string databaseName;
    QSqlQuery query;
    if (query.exec(qry.c_str())) {
        while (query.next()) {
            databaseName = query.value(0).toString().toStdString();
            if (exclusionSet.find(databaseName) == exclusionSet.end()) { // if item isn't in exclusion set then keep it
                databaseNames.push_back(databaseName);
                //std::cout << databaseName << std::endl;
            }
        }
    } else {
        qDebug() << "Error: " << db.lastError().text();
    }

    return databaseNames;
}


std::vector<std::string>
nmfDatabase::nmfGetTableNames()
{
    QSqlDatabase db = QSqlDatabase::database();
    std::vector<std::string> tableNames;
    std::string qry = "SHOW tables";

    std::set<std::string> exclusionSet = { "information_schema", "mysql", "performance_schema", "sys" };
    std::string tableName;
    QSqlQuery query;
    if (query.exec(qry.c_str())) {
        while (query.next()) {
            tableName = query.value(0).toString().toStdString();
            if (exclusionSet.find(tableName) == exclusionSet.end()) { // if item isn't in exclusion set then keep it
                tableNames.push_back(tableName);
                //std::cout << tableName << std::endl;
            }
        }
    } else {
        qDebug() << "Error: " << db.lastError().text();
    }

    return tableNames;
}


void
nmfDatabase::nmfCloseDatabase()
{
    //db.rollback();
    //db.commit();

    QSqlDatabase db = QSqlDatabase::database();
    QString dbToClose = db.databaseName();
std::cout << "Trying to close: " << dbToClose.toStdString() << std::endl;
    db.close();
    QSqlDatabase::removeDatabase(dbToClose);
}

//void
//nmfDatabase::nmfSaveDatabase()
//{
//    QSqlDatabase db = QSqlDatabase::database();
//    db.commit();
//}

//void
//nmfDatabase::nmfStartTransaction()
//{
//    QSqlDatabase db = QSqlDatabase::database();
//    bool supportsTransactions = db.transaction();
//    if (!supportsTransactions) {
//        std::cout <<
//            "\nWarning: Database doesn't support transactions. " <<
//            "All modifications will happen as you modify the database. " <<
//            "\n         Change database format to InnoDB. Inspect with MySql command: show table status;\n"
//            << std::endl;
//    }
//}



std::string
nmfDatabase::nmfUpdateDatabase(std::string qry)
{
    //std::cout << "Updating database..." << std::endl;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery retv = db.exec(QString::fromStdString(qry));

    return retv.lastError().text().toStdString();
}


bool
nmfDatabase::nmfDeleteRecordsFromTable(std::string table, std::string MSVPAName)
{
    std::cout << "Deleting records from table: " << table << std::endl;

    std::string queryStr;
    std::string errorMsg;
    bool ok=true;

    queryStr = "DELETE FROM " + table + " WHERE MSVPAname=\""+MSVPAName+"\";";
    errorMsg = nmfUpdateDatabase(queryStr);
    if (nmfUtilsQt::isAnError(errorMsg)) {
        nmfUtils::printError(queryStr,errorMsg);
        ok = false;
    }

    return ok;
}

bool
nmfDatabase::nmfGetInitialData(
        const std::string& MSVPAName,
            int &NPreds, int &NPrey, int &NOtherFood, int &NOtherPreds,
            int &FirstYear, int &LastYear, int &NSeas, int &NMSVPASpe, int &NYears,
            std::string &FirstYearStr, std::string &LastYearStr)
{
    bool retv=true;
    std::vector<std::string> fields;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;

    fields = {"MSVPAName","NSpe","NPreyOnly","NOther","NOtherPred","FirstYear","LastYear","NSeasons"};
    queryStr = "SELECT MSVPAName,NSpe,NPreyOnly,NOther,NOtherPred,FirstYear,LastYear,NSeasons FROM " +
                nmfConstantsMSVPA::TableMSVPAlist + " WHERE MSVPAName = '" + MSVPAName + "'";
    dataMap = nmfQueryDatabase(queryStr, fields, nmfConstants::nullAsZero);
    NPreds       = std::stoi(dataMap["NSpe"][0]);
    NPrey        = std::stoi(dataMap["NPreyOnly"][0]);
    NOtherFood   = std::stoi(dataMap["NOther"][0]);
    NOtherPreds  = std::stoi(dataMap["NOtherPred"][0]);
    FirstYear    = std::stoi(dataMap["FirstYear"][0]);
    LastYear     = std::stoi(dataMap["LastYear"][0]);
    NSeas        = std::stoi(dataMap["NSeasons"][0]);
    NMSVPASpe    = NPreds + NPrey;
    NYears       = LastYear - FirstYear + 1;
    FirstYearStr = std::to_string(FirstYear);
    LastYearStr  = std::to_string(LastYear);

    return retv;
} // end nmfGetInitialData


void
nmfDatabase::nmfGetMSVPAInitData(
        const std::string& MSVPAName,
        int& FirstYear,
        int& LastYear,
        int& NYears,
        int& NSeasons)
{
    std::map<std::string,int> initMap = nmfQueryInitFields(nmfConstantsMSVPA::TableMSVPAlist, MSVPAName);
    FirstYear = initMap["FirstYear"];
    LastYear  = initMap["LastYear"];
    NYears    = LastYear - FirstYear + 1;
    NSeasons  = initMap["NSeasons"];
} // end getMSVPAInitData


void
nmfDatabase::getSpeciesToIndexMap(std::map<std::string,int> &SpeciesToIndexMap)
{
    std::vector<std::string> fields;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;

    // Get list of all Species
    fields   = {"SpeName","SpeIndex"};
    queryStr = "SELECT Spename,SpeIndex FROM " + nmfConstantsMSVPA::TableSpecies;
    dataMap  = nmfQueryDatabase(queryStr, fields);
    for (unsigned int i=0; i< dataMap["SpeName"].size(); ++i) {
        SpeciesToIndexMap[dataMap["SpeName"][i]] = std::stoi(dataMap["SpeIndex"][i]);
    }

} // end getSpeciesToIndexMap


bool
nmfDatabase::nmfDeleteRecordsFromTable(std::string table,
                                       std::string MSVPAName,
                                       std::string ForecastName,
                                       std::string Scenario)
{
    std::cout << "Deleting records from table: " << table << std::endl;

    std::string queryStr;
    std::string errorMsg;
    bool ok=true;

    //nmfStartTransaction();

    queryStr = "DELETE FROM " + table + " WHERE MSVPAname='"+MSVPAName+"' " +
               " AND ForeName = '" + ForecastName + "' " +
               " AND Scenario = '" + Scenario + "' ";
    errorMsg = nmfUpdateDatabase(queryStr);
    if (nmfUtilsQt::isAnError(errorMsg)) {
        nmfUtils::printError(queryStr,errorMsg);
        ok = false;
    }

    return ok;
}


void
nmfDatabase::nmfQueryAlphaBetaFields(
        const std::string &table,
        const int &speciesIndex,
        const std::string &SSVPAName,
        const int &NIndex,
        boost::numeric::ublas::vector<double> &Alpha,
        boost::numeric::ublas::vector<double> &Beta)
{
    std::vector<std::string> fields = { "Alpha", "Beta" };
    std::string queryStr = "SELECT Alpha,Beta FROM " + table
            + " WHERE SpeIndex = " + std::to_string(speciesIndex)
            + " AND SSVPAName = '" + SSVPAName + "'";
    std::map<std::string, std::vector<std::string> > dataMap;

    dataMap = nmfQueryDatabase(queryStr, fields, nmfConstants::nullAsZero);
    if (dataMap["Alpha"].size() == 0) {
        nmfUtils::printError("nmfQueryAlphaBetaFields: Table Read Error: ", "Table " + table + " is empty.");
        return;
    }

    for (int i = 0; i < NIndex; ++i) {
        Alpha(i) = std::stod(dataMap[fields[0]][i]);
        Beta(i)  = std::stod(dataMap[fields[1]][i]);
    }
}


void
nmfDatabase::nmfQueryCatchFields(
        const std::string &table,
        const int &speciesIndex,
        const std::string& FirstCatchAgeStr,
        const std::string& LastCatchAgeStr,
        const std::string& OrderByStr,
        boost::numeric::ublas::matrix<double> &Catch)
{
    int k=0;
    std::vector<std::string> fields = {"Catch"};
    std::string queryStr = "SELECT Catch FROM " + table +
            " WHERE SpeIndex = " + std::to_string(speciesIndex) +
            " AND Age >= " + FirstCatchAgeStr +
            " and Age <= " + LastCatchAgeStr + OrderByStr;
    std::map< std::string, std::vector<std::string> > dataMap;
    dataMap = nmfQueryDatabase(queryStr, fields, nmfConstants::nullAsZero);

    for (unsigned int i = 0; i < Catch.size1(); ++i) {
        for (unsigned int j = 0; j < Catch.size2(); ++j) {
            Catch(i,j) = std::stod(dataMap[fields[0]][k++]);
        }
    }
}


int
nmfDatabase::getSpeciesIndex(std::string SpeciesName)
{
    int retv = -1;

    std::vector<std::string> fields = {"SpeIndex", "SpeName"};
    std::string queryStr = "SELECT SpeIndex,SpeName FROM " +
                            nmfConstantsMSVPA::TableSpecies +
                           " WHERE SpeName = '" + SpeciesName + "'";
    std::map< std::string, std::vector<std::string> > dataMap;
    dataMap = nmfQueryDatabase(queryStr, fields, nmfConstants::nullAsZero);
    if (dataMap.size() > 0) {
        retv = std::stoi(dataMap[fields[0]][0]);
    }
    return retv;
}


std::tuple<int, int, int, int>
nmfDatabase::nmfQueryAgeFields(const std::string &table, const int &speciesIndex)
{
    std::vector<std::string> fields = {"MaxAge", "MinCatAge", "MaxCatAge", "PlusClass"};
    std::string queryStr = "SELECT MaxAge,MinCatAge,MaxCatAge,PlusClass FROM " + table + " WHERE SpeIndex=" +
                           std::to_string(speciesIndex);
    std::map< std::string, std::vector<std::string> > dataMap;

    dataMap = nmfQueryDatabase(queryStr, fields, nmfConstants::nullAsZero);
    int MaxAge        = std::stoi(dataMap[fields[0]][0]);
    int FirstCatchAge = std::stoi(dataMap[fields[1]][0]);
    int LastCatchAge  = std::stoi(dataMap[fields[2]][0]);
    int isPlusClass   = std::stoi(dataMap[fields[3]][0]);

    return std::make_tuple(MaxAge, FirstCatchAge, LastCatchAge, isPlusClass);
}

std::map<std::string, int>
nmfDatabase::nmfQueryInitFields(const std::string &table, const std::string &MSVPAName)
{
    std::vector<std::string> fields = {"FirstYear", "LastYear", "NSeasons", "GrowthModel"};
    std::string queryStr = "SELECT FirstYear,LastYear,NSeasons,GrowthModel FROM " +
            table + " WHERE MSVPAName='" + MSVPAName + "'";
    std::map< std::string, std::vector<std::string> > dataMap;
    std::map< std::string, int> retMap;
    std::string field;

    dataMap = nmfQueryDatabase(queryStr, fields, nmfConstants::nullAsZero);
    if (dataMap["FirstYear"].size() > 0) {
        for (unsigned int i=0; i<fields.size(); ++i) {
            field = fields[i];
            retMap[field] = std::stoi(dataMap[field][0]);
        }
    } else {
        std::cout << "Warning: nmfDatabase::nmfQueryInitFields query returned no records: " << queryStr << std::endl;
    }

    return retMap;
}

std::map<std::string, std::vector<std::string> >
nmfDatabase::nmfQueryPredatorPreyFields(const std::string &table, const std::string &MSVPAName)
{
    std::vector<std::string> fields = {"SpeName", "Type"};
    std::string queryStr = "SELECT SpeName,Type FROM " + table + " WHERE MSVPAName='" +
            MSVPAName + "'";
    std::map< std::string, std::vector<std::string> > dataMap;
    std::map< std::string, std::vector<std::string> > retMap;
    std::string field;

    dataMap = nmfQueryDatabase(queryStr, fields, nmfConstants::nullAsZero);

    for (unsigned int i=0; i<dataMap["SpeName"].size(); ++i) {
        switch (std::stoi(dataMap["Type"][i])) {
            case nmfConstants::TYPE_PREDATOR:
                retMap["predators"].push_back(dataMap["SpeName"][i]);
                break;
            case nmfConstants::TYPE_PREY:
                retMap["prey"].push_back(dataMap["SpeName"][i]);
                break;
            case nmfConstants::TYPE_OTHER_PREDATOR:
                retMap["otherPredators"].push_back(dataMap["SpeName"][i]);
                break;
            default:
                break;
        }
    }
    return retMap;
}

std::map< std::string, std::vector< std::string> >
nmfDatabase::nmfQueryDatabase(
        const std::string &qry,
        const std::vector<std::string> &fields,
        const bool setNullToBlank)
{

    std::map< std::string, std::vector<std::string> > dataMap;
    std::string errorMsg;

    QSqlQuery query;
    if (query.exec(qry.c_str()))
    {
        while (query.next())
        {
            int i=0;
            for (std::string field : fields) {
                if (setNullToBlank && query.value(i).isNull()) {
                    dataMap[field].push_back("");
                } else {
                    // RSK
                    //qDebug() << field.c_str() << "," << query.value(0).toString() << "," << query.value(1).toString();
                    dataMap[field].push_back(query.value(i).toString().toStdString());
                }
                ++i;
            }
        }
    } else {
        QSqlDatabase db = QSqlDatabase::database();
        errorMsg = db.lastError().text().toStdString();
        if (errorMsg.empty()) {
            // This prints out when not always desired. Consider adding verbose flag to call.
            //std::cout << "Error: Check for loaded database." << std::endl;
        } else if (nmfUtilsQt::isAnError(errorMsg)) {
            // This prints an errorMsg on startup of:
            // Error: Access denied for user ... (using password: NO) QMYSQL: Unable to connect
            // which came from:
            //  nmfSetup_Tab2::loadDatabaseNames ->
            //    nmfDatabase::getListOfAuthenticatedDatabaseNames
            // std::cout << "Error: " << errorMsg << std::endl;
        }
    }
    //qDebug() << "Query complete.";

    return dataMap;
}

void
nmfDatabase::RestoreCSVFile(QString &TableName,
                            std::string &ProjectDir,
                            std::vector<std::string> &fields)
{
    unsigned int NumRecords;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    QString firstLine;
    QString msg;
    std::string totStr;
    std::string fieldsStr;

    // Read header line from template file
    QString path = QDir(QString::fromStdString(ProjectDir)).filePath("inputData");
    QString fileNameWithPath = QDir(path).filePath(TableName+".csv");
    QFile fin(fileNameWithPath);
    if (! fin.open(QIODevice::ReadOnly)) {
        std::cout << "RestoreCSVFile: Couldn't open for reading: " << fileNameWithPath.toStdString() << std::endl;
        return;
    }
    QTextStream inStream(&fin);
    firstLine = inStream.readLine();
    fin.close();

    // Read table data
    fieldsStr  = boost::algorithm::join(fields,",");
    queryStr   = "SELECT " + fieldsStr + " FROM " + TableName.toStdString();
    dataMap    = nmfQueryDatabase(queryStr,fields,true);
    NumRecords = dataMap[fields[0]].size();
//    if (NumRecords == 0) {
//        std::cout << "RestoreCSVFile: " << TableName.toStdString()+".csv (Database table empty)" << std::endl;
//        std::cout << queryStr << std::endl;
//        return;
//    }

    // OK if NumRecords == 0. Means that all rows except header should be deleted.

    // Open output file
    QString tmpFileNameWithPath = QDir(path).filePath("."+TableName+".csv");
    QFile fout(tmpFileNameWithPath);
    if (! fout.open(QIODevice::WriteOnly)) {
        std::cout << "RestoreCSVFile: Couldn't open for writing: " << fileNameWithPath.toStdString() << std::endl;
        return;
    }
    QTextStream outStream(&fout);

    // Output table data to CSV file
    outStream << firstLine << "\n";
    for (unsigned int i=0; i<NumRecords; ++i) {
        totStr.clear();
        for (std::string field : fields) {
            totStr += std::string(dataMap[field][i]) + ", ";
        }
        totStr = totStr.substr(0,totStr.size()-2);
        outStream << QString::fromStdString(totStr) << "\n";
    }
    fout.close();

    // Now, mv temp file to actual file.
    int retv = nmfUtilsQt::rename(tmpFileNameWithPath, fileNameWithPath);
    if (retv < 0) {
        std::cout << "Error: Couldn't rename " <<
                     tmpFileNameWithPath.toStdString() << " to " <<
                     fileNameWithPath.toStdString() <<
                     ". Restore aborted." << std::endl;
        return;
    }

    std::cout << "Restored: " << TableName.toStdString()+".csv" << std::endl;

} // end RestoreCSVFile

void
nmfDatabase::nmfQueryForecastWeightAtAgeData(
        const std::string &MSVPAName,
        const std::string &ForecastName,
        const std::string &ScenarioName,
        const std::string &species,
        const int &Year,
        const int &Nage,
        const int &index,
        boost::numeric::ublas::matrix<double> &WtAtAge)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    // Get Wt At Age...wts at beginning of year
    fields = {"Year","Age","InitWt"};
    queryStr = "SELECT Year, Age, InitWt FROM " +
                nmfConstantsMSVPA::TableForeOutput +
               " WHERE MSVPAname = '" + MSVPAName +
               "' AND ForeName = '" + ForecastName +
               "' AND Scenario = '" + ScenarioName +
               "' AND SpeName = '" + species +
               "' AND Year = " + std::to_string(Year) +
               "  AND Season = 0 ORDER By Age";
    dataMap = nmfQueryDatabase(queryStr, fields);
    for (int j=0; j<Nage; ++j) {
        WtAtAge(index,j) = std::stod(dataMap["InitWt"][j]);
    }

} // end nmfQueryForecastWeightAtAgeData



bool
nmfDatabase::nmfQueryWeightAtAgeData(
        const int &index,
        const int &Year,
        const int &Nage,
        const std::string &species,
        boost::numeric::ublas::matrix<double> &WtAtAge)
{
    bool retv = false;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    // Get Wt at Age
    fields = {"Value"};
    queryStr = "SELECT Value FROM " +
                nmfConstantsMSVPA::TableSpeWeight +
               " WHERE SpeName = '" + species + "' " +
               " AND Year = " + std::to_string(Year) +
               " AND Variable = 'Weight' " +
               " ORDER BY Age";
    dataMap = nmfQueryDatabase(queryStr, fields);
    if (dataMap.size() > 0) {
        retv = true;
        for (int j=0; j<Nage; ++j) {
            WtAtAge(index,j) = std::stod(dataMap["Value"][j]);
        }
    }

    return retv;

} // end nmfQueryWeightAtAgeData

void
nmfDatabase::nmfQueryForecastMortalityData(
        const std::string& MSVPAName,
        const std::string& ForecastName,
        const std::string& ScenarioName,
        const std::string& Species,
        const int& Year,
        const int& Nage,
        const int& index,
        boost::numeric::ublas::matrix<double>& FatAge,
        boost::numeric::ublas::matrix<double>& M1atAge,
        boost::numeric::ublas::matrix<double>& M2atAge)
{
    std::string M2Str;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    fields = {"Year","Age","F","M2","M1"};
    queryStr = "SELECT Year, Age, Sum(SeasF) as F, Sum(SeasM2) as M2, Sum(SeasM1) as M1 FROM " +
                nmfConstantsMSVPA::TableForeOutput +
               " WHERE MSVPAname = '" + MSVPAName +
               "' AND ForeName = '" + ForecastName +
               "' AND Scenario = '" + ScenarioName +
               "' AND SpeName = '"  + Species +
               "' AND Year = " + std::to_string(Year) +
               "  GROUP BY Year, Age";
    dataMap = nmfQueryDatabase(queryStr, fields);
    for (int j=0; j<Nage; ++j) {
        FatAge(index,j)  = std::stod(dataMap["F"][j]);
        M1atAge(index,j) = std::stod(dataMap["M1"][j]);
        M2Str = dataMap["M2"][j];
        M2atAge(index,j) = M2Str.empty() ? 0 : std::stod(M2Str);
    }

} // end nmfQueryForecastMortalityData


void
nmfDatabase::nmfQueryMortalityData(
        const std::string& MSVPAName,
        const int& index,
        const int& Year,
        const int& Nage,
        const std::string& species,
        boost::numeric::ublas::matrix<double>& FatAge,
        boost::numeric::ublas::matrix<double>& M1atAge,
        boost::numeric::ublas::matrix<double>& M2atAge)
{
    std::string M2Str;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    fields = {"Age","F","M2","M1"};
    queryStr = "SELECT Age, Sum(SeasF) as F, Sum(SeasM2) as M2, Sum(SeasM1) as M1 FROM " +
                nmfConstantsMSVPA::TableMSVPASeasBiomass +
               " WHERE MSVPAName = '" + MSVPAName +
               "' AND SpeName = '" + species +
               "' AND Year = " + std::to_string(Year) +
               " GROUP BY Age";
    dataMap = nmfQueryDatabase(queryStr, fields);
    for (int j=0; j<Nage; ++j) {
        FatAge(index,j)  = std::stod(dataMap["F"][j]);
        M1atAge(index,j) = std::stod(dataMap["M1"][j]);
        M2Str = dataMap["M2"][j];
        M2atAge(index,j) = M2Str.empty() ? 0 : std::stod(M2Str);
    }

} // end nmfQueryWeightAtAgeData


bool
nmfDatabase::getMortalityData(
        nmfLogger*   logger,
        const std::string& ModelName,
        const std::string &Species,
        const int &NumYears,
        const int &NumAges,
        const std::string TableName,
        boost::numeric::ublas::matrix<double> &TableData)
{
    int m;
    int NumRecords;
    int FirstYear,LastYear;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string msg;

    fields     = {"ModelName","SpeName","Segment","ColName","Value"};
    queryStr   = "SELECT ModelName,SpeName,Segment,ColName,Value FROM " + TableName;
    queryStr  += " WHERE ModelName = '" + ModelName + "'";
    queryStr  += " AND SpeName = '" + Species + "'";
    queryStr  += " ORDER BY Segment";
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        msg = "nmfDatabase::getMortalityData: No records found in table: " + TableName;
        logger->logMsg(nmfConstants::Error,msg);
        logger->logMsg(nmfConstants::Error,queryStr);
        return false;
    }

    nmfUtils::initialize(TableData,NumYears,NumAges);

    m = 0;
    int VeryFirstYear = std::stoi(dataMap["Value"][0]);
    while (m < NumRecords) {
        FirstYear = std::stoi(dataMap["Value"][m++]);
        LastYear  = std::stoi(dataMap["Value"][m++]);
        for (int year=FirstYear; year<=LastYear; ++year) {
            if (year == FirstYear) {
                for (int age=0; age<NumAges; ++age) {
                    TableData(year-VeryFirstYear,age) =
                            std::stod(dataMap["Value"][m++]);
                }
            } else {
                for (int age=0; age<NumAges; ++age) {
                    TableData(year-VeryFirstYear,age) =
                            TableData(year-VeryFirstYear-1,age);
                }
            }
        }
    }

    return true;
}




void
nmfDatabase::nmfQueryForecastAgeCatchData(
        const std::string &MSVPAName,
        const std::string &ForecastName,
        const std::string &ScenarioName,
        const std::string &species,
        const int &Year,
        const int &Nage,
        const int &i,
        boost::numeric::ublas::matrix<double> &Catch)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    // Need to get catches to calculate a weighted average FullF
    fields = {"Year","Age","Catch"};
    queryStr = "SELECT Year, Age, Sum(SeasCatch) as Catch FROM " +
                nmfConstantsMSVPA::TableForeOutput +
               " WHERE MSVPAname = '" + MSVPAName +
               "' AND ForeName = '" + ForecastName +
               "' AND Scenario = '" + ScenarioName +
               "' AND SpeName = '" + species +
               "' AND Year = " + std::to_string(Year) +
               "  GROUP BY Year, Age";
    dataMap = nmfQueryDatabase(queryStr, fields);
    for (unsigned int j=0; j<=(unsigned int)Nage; ++j) {
        if (j < dataMap["Year"].size()) {
            Catch(i,j) = (std::stoi(dataMap["Age"][j]) == (int)j) ? std::stod(dataMap["Catch"][j]) : 0.0;
        }
    }

} // end nmfQueryForecastAgeCatchData



void
nmfDatabase::nmfQueryAgeCatchData(
        const int &i,
        const int &Year,
        const int &Nage,
        const std::string &species,
        boost::numeric::ublas::matrix<double> &Catch)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    // I need to get catches to calculate a weighted average FullF
    fields = {"Age","Catch"};
    queryStr = "SELECT Age,Catch FROM " + nmfConstantsMSVPA::TableSpeCatch +
               " WHERE SpeName = '" + species +
               "' AND Year = " + std::to_string(Year) +
               " ORDER BY Age";
    dataMap = nmfQueryDatabase(queryStr, fields);
    size_t NumRecords = dataMap["Age"].size();
    unsigned int m = 0;
    for (int j=0; j<Nage; ++j) {
        if (m < NumRecords) {
            Catch(i,j) = 0;
            if (std::stoi(dataMap["Age"][m]) == j) {
                Catch(i,j) = std::stod(dataMap["Catch"][m++]);
            }
        } else {
            break;
        }
    }

} // end nmfQueryWeightAtAgeData


void
nmfDatabase::nmfQueryNaturalMortalityData(
        const int &SpeciesIndex,
        const int &SSVPAIndex,
        const int &MaxCatchAge,
        const int &NCatchYears,
        boost::numeric::ublas::matrix<double> &M)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    int m;

    fields   = {"AgeM1","AgeM2"};
    queryStr = "SELECT AgeM1,AgeM2 FROM " + nmfConstantsMSVPA::TableSSVPAAgeM +
               " WHERE SpeIndex = " + std::to_string(SpeciesIndex) +
               " AND SSVPAIndex = " + std::to_string(SSVPAIndex);
    dataMap  = nmfQueryDatabase(queryStr, fields);
    m = 0;
    for (int i = 0; i <= MaxCatchAge; ++i) {
        for (int j = 0; j < NCatchYears; ++j) {
            M(j,i) = std::stod(dataMap["AgeM1"][m]) + std::stod(dataMap["AgeM2"][m]);
        } // end for j
        ++m;
    } // end for i

} // end nmfQueryNaturalMortalityData


void
nmfDatabase::nmfQueryMaturityData(
        const int &index,
        const int &Year,
        const int &Nage,
        const std::string &Species,
        boost::numeric::ublas::matrix<double> &Pmature)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    fields = {"PMature"};
    queryStr = "SELECT PMature FROM " + nmfConstantsMSVPA::TableSpeMaturity +
               " WHERE SpeName = '" + Species + "' " +
               " AND Year = " + std::to_string(Year) +
               " ORDER BY Age";
    dataMap = nmfQueryDatabase(queryStr, fields);
    for (int j=0; j<Nage; ++j) {
        Pmature(index,j) = std::stod(dataMap["PMature"][j]);
    }

} // end nmfQueryMaturityData


void
nmfDatabase::nmfQueryForecastPreyList(
        const std::string &MSVPAName,
        const std::string &ForecastName,
        const std::string &ScenarioName,
        const std::string &PredatorSpecies,
        const bool& includePredAge,
        const std::string &ageStr,
        const std::string &seasonStr,
        QStringList &PreyList)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    std::string predAgeClause = "";
    if (includePredAge) {
        //predAgeClause = " AND PredAge = " + PredAge;
        predAgeClause = ageStr;
    }

    fields = {"PreyName"};
    queryStr = "SELECT DISTINCT PreyName FROM " +
                nmfConstantsMSVPA::TableForeSuitPreyBiomass +
               " WHERE MSVPAname = '" + MSVPAName + "'" +
               " AND ForeName = '" + ForecastName + "'" +
               " AND Scenario = '" + ScenarioName + "'" +
               " AND PredName = '" + PredatorSpecies + "'" +
                predAgeClause + seasonStr +
               " ORDER BY PreyName";
    dataMap = nmfQueryDatabase(queryStr, fields);

    for (unsigned int i = 0; i < dataMap["PreyName"].size(); ++i) {
        PreyList << QString::fromStdString(dataMap["PreyName"][i]);
    }
} // end nmfQueryForecastPreyList


void
nmfDatabase::nmfQueryMsvpaPreyList(
        const std::string& selectedPredator,
        const std::string& MSVPAName,
        const bool& includedPredAge,
        std::string& selectedSpeciesAgeSizeClass,
        std::string& PredAge,
        int& NPrey,
        QStringList& PreyList)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;
    std::string predAgeClause = PredAge;

    fields = {"PreyName"};
    // selectedSpeciesAgeSizeClass = something like "Age 0" or "Size 1"
    // Need to strip off either "Age " or "Size " and leave only the number
    if (includedPredAge) {
        bool isSize = (selectedSpeciesAgeSizeClass[0] == 'S');
        PredAge = selectedSpeciesAgeSizeClass.erase(0,selectedSpeciesAgeSizeClass.find(" "));
        if (isSize) {
            PredAge = std::to_string(std::stoi(PredAge)-1);
        }
        predAgeClause = " and PredAge = " + PredAge;
    }

    queryStr = "SELECT DISTINCT PreyName FROM " +
                nmfConstantsMSVPA::TableMSVPASuitPreyBiomass +
               " WHERE PredName='" + selectedPredator +
               "' and MSVPAName='" + MSVPAName + "'" +
                predAgeClause + " ORDER by PreyName";
    dataMap = nmfQueryDatabase(queryStr, fields);
    NPrey = dataMap["PreyName"].size();
    for (int i=0; i<NPrey; ++i) {
        PreyList << QString::fromStdString(dataMap["PreyName"][i]);
    }

} // end nmfQueryMsvpaPreyList


//void
//nmfDatabase::nmfDeleteConnection()
//{
//    QSqlDatabase db = QSqlDatabase::database();
//    db.removeDatabase(db.connectionName());
//}

void
nmfDatabase::nmfSetConnectionByName(QString name)
{
    m_dbName = name;
}


// -------------------------- MSSPM -----------------------


void
nmfDatabase::createScenarioMap(
        const std::string& ProjectName,
        const std::string& ModelName,
        std::map<QString,QStringList>& ScenarioForecastMap)
{
    std::string queryStr;
    std::string ScenarioName;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMapScenario;
    std::map<std::string, std::vector<std::string> > dataMapForecast;
    QStringList tmpList;

    // Clear the map
    ScenarioForecastMap.clear();

    // Get Scenario Names
    fields   = {"ScenarioName"};
    queryStr = "SELECT DISTINCT ScenarioName FROM " +
                nmfConstantsMSSPM::TableForecastBiomassMultiScenario +
               " WHERE ProjectName = '" + ProjectName +
               "' AND ModelName = '"    + ModelName   + "'";
    dataMapScenario = nmfQueryDatabase(queryStr, fields);
    for (unsigned i=0; i<dataMapScenario["ScenarioName"].size(); ++i) {
        ScenarioName = dataMapScenario["ScenarioName"][i];
        fields    = {"SortOrder","ForecastLabel"};
        queryStr  = "SELECT DISTINCT SortOrder,ForecastLabel FROM " +
                     nmfConstantsMSSPM::TableForecastBiomassMultiScenario +
                     " WHERE ProjectName = '" + ProjectName  +
                     "' AND ModelName = '"    + ModelName    +
                     "' AND ScenarioName = '" + ScenarioName +
                     "' ORDER BY SortOrder,ForecastLabel";
        dataMapForecast = nmfQueryDatabase(queryStr, fields);
        tmpList.clear();
        for (unsigned j=0; j<dataMapForecast["ForecastLabel"].size(); ++j) {
            tmpList << QString::fromStdString(dataMapForecast["ForecastLabel"][j]);
        }
        ScenarioForecastMap[QString::fromStdString(ScenarioName)] = tmpList;
    }
}

void
nmfDatabase::createUnitsMap(
        const std::string& ProjectName,
        const std::string& ModelName,
        std::map<QString,QString>& previousUnits)
{
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    fields   = {"TableName","Units"};
    queryStr = "SELECT TableName,Units FROM " +
                nmfConstantsMSSPM::TableUnits +
               " WHERE ProjectName = '" + ProjectName +
               "' AND  ModelName   = '" + ModelName   + "'";
    dataMap  = nmfQueryDatabase(queryStr, fields);
    for (int i=0; i<(int)dataMap["Units"].size(); ++i) {
        previousUnits[QString::fromStdString(dataMap["TableName"][i])] =
                QString::fromStdString(dataMap["Units"][i]);
    }
}

bool
nmfDatabase::getRunLengthAndStartYear(
        nmfLogger* logger,
        const std::string& ProjectName,
        const std::string& ModelName,
        int &RunLength,
        int &StartYear)
{
    int NumRecords;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    // Find Models data
    fields    = {"RunLength","StartYear"};
    queryStr  = "SELECT RunLength,StartYear FROM " +
                 nmfConstantsMSSPM::TableModels +
                " WHERE ProjectName = '" + ProjectName +
                "' AND ModelName = '"    + ModelName + "'";
    dataMap   = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["RunLength"].size();
    if (NumRecords == 0){
        logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::getRunLengthAndStartYear: No records found in table " +
                       nmfConstantsMSSPM::TableModels);
        logger->logMsg(nmfConstants::Error,queryStr);
        return false;
    }
    RunLength = std::stoi(dataMap["RunLength"][0]);
    StartYear = std::stoi(dataMap["StartYear"][0]);
    return true;
}

bool
nmfDatabase::updateAllModelsInProject(
        QWidget* parent,
        const std::string& type,
        const std::string& projectName,
        const std::string& currentModel,
        std::vector<std::string>& modelsInProject)
{
    int NumRecords;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string HarvestForm;
    std::string ObsBiomassType;
    std::string whereClause = "";
    QString msg;

    // Get information about the current model
    modelsInProject.clear();
    fields    = {"ProjectName","ModelName","HarvestForm","ObsBiomassType"};
    queryStr  = "SELECT ProjectName,ModelName,HarvestForm,ObsBiomassType FROM " +
                nmfConstantsMSSPM::TableModels +
                " WHERE ProjectName = '" + projectName +
                "' AND ModelName = '"    + currentModel + "'";
    dataMap   = nmfQueryDatabase(queryStr, fields);
    NumRecords = (int)dataMap["ModelName"].size();
    if (NumRecords == 0) {
        return true;
    }
    if (type == "Harvest") {
        whereClause += " AND HarvestForm = '" + dataMap["HarvestForm"][0] + "'";
    } else if (type == "ObservedBiomass") {
        whereClause += " AND ObsBiomassType = '" + dataMap["ObsBiomassType"][0] + "'";
    }

    // Get data from other models in current project
    fields    = {"ProjectName","ModelName","HarvestForm","ObsBiomassType"};
    queryStr  = "SELECT ProjectName,ModelName,HarvestForm,ObsBiomassType FROM " +
                 nmfConstantsMSSPM::TableModels +
                " WHERE ProjectName = '" + projectName +
                "' AND ModelName = '"    + currentModel + "'";
    queryStr += whereClause;
    dataMap   = nmfQueryDatabase(queryStr, fields);
    NumRecords = (int)dataMap["ModelName"].size();
    if (NumRecords == 0) {
        return true;
    }
    for (int i=0;i<NumRecords;++i) {
        modelsInProject.push_back(dataMap["ModelName"][i]);
    }
    int numModelsInProject = (int)modelsInProject.size();

    // Query user
    if (numModelsInProject > 1) {
        msg  = "\nFound " + QString::number(numModelsInProject) + " models in the current project. ";
        msg += "Saving this data will cause all of this project's models to be updated. " ;
        msg += "\n\nOK to proceed?\n";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Save"),
                                                                  QObject::tr(msg.toLatin1()),
                                                                  QMessageBox::No|QMessageBox::Yes,
                                                                  QMessageBox::Yes);
        if (reply == QMessageBox::No) {
            return false;
        }
    }

    return true;
}

bool
nmfDatabase::cleanTables(
        nmfLogger* logger,
        const std::string& projectName,
        const std::string& modelName)
{
    std::string cmd;
    std::string errorMsg;

    for (std::string tableToClean : nmfConstantsMSSPM::TablesToClean) {
        cmd  = "DELETE FROM " + tableToClean  +
               " WHERE ProjectName = '" + projectName +
               "' AND ModelName = '"    + modelName + "'";
        errorMsg = nmfUpdateDatabase(cmd);
        if (nmfUtilsQt::isAnError(errorMsg)) {
            logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::cleanTables: Delete error: " + errorMsg);
            logger->logMsg(nmfConstants::Error,"cmd: " + cmd);
            return false;
        }
    }
    return true;
}

bool
nmfDatabase::getAlgorithmIdentifiers(
        QWidget*     widget,
        nmfLogger*   logger,
        const std::string& ProjectName,
        const std::string& ModelName,
        std::string& Algorithm,
        std::string& Minimizer,
        std::string& ObjectiveCriterion,
        std::string& Scaling,
        std::string& CompetitionForm,
        const bool&  showMsg)
{
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    // Get current algorithm and run its estimation routine
    fields     = {"Algorithm","Minimizer","ObjectiveCriterion","WithinGuildCompetitionForm","Scaling"};
    queryStr   = "SELECT Algorithm,Minimizer,ObjectiveCriterion,WithinGuildCompetitionForm,Scaling FROM " +
                  nmfConstantsMSSPM::TableModels +
                 " WHERE ProjectName = '" + ProjectName +
                 "' AND ModelName = '"    + ModelName   + "'";
    dataMap    = nmfQueryDatabase(queryStr, fields);
    if (dataMap["Algorithm"].size() == 0) {
        logger->logMsg(nmfConstants::Warning,"[Warning] nmfDatabase::AlgorithmIdentifiers: No Models found. Please create a Model and click Save on Setup Tab 3.");
        logger->logMsg(nmfConstants::Warning,queryStr);
        if (showMsg && (widget != nullptr)) {
            QString msg = "\nNo Models set. Please confirm the following:\n\n";
            msg += "1. In Setup Page 2: Create/Load a Project\n\n";
            msg += "2. In Setup Page 3: Create Guilds/Species\n\n";
            msg += "3. In Setup Page 4: Save a Model\n";
            QMessageBox::warning(widget, "Warning", msg, QMessageBox::Ok);
        }
        return false;
    }
    Algorithm          = dataMap["Algorithm"][0];
    Minimizer          = dataMap["Minimizer"][0];
    ObjectiveCriterion = dataMap["ObjectiveCriterion"][0];
    Scaling            = dataMap["Scaling"][0];
    CompetitionForm    = dataMap["WithinGuildCompetitionForm"][0];

    return true;
}

// -------------------------- General -----------------------

bool
nmfDatabase::databaseExists(std::string dbName)
{
    bool retv = false;
    std::string existingDatabase;
    std::vector<std::string> fields = { "Database" };
    std::string queryStr = "SHOW databases";
    std::map<std::string, std::vector<std::string> > dataMap;

    dataMap = nmfQueryDatabase(queryStr, fields);
    for (unsigned int i = 0; i < dataMap["Database"].size(); ++i) {
        existingDatabase = dataMap["Database"][i];
        if (QString::fromStdString(dbName).toLower() == QString::fromStdString(existingDatabase).toLower()) {
            retv = true;
            break;
        }
    }
    return retv;
}

QString
nmfDatabase::importDatabase(QWidget*     widget,
                            nmfLogger*   logger,
                            std::string& ProjectDir,
                            std::string& Username,
                            std::string& Password)
{
    QMessageBox::StandardButton reply;
    QString databaseDir;
    QString msg;
    QString cmd;
    QString existingDatabase;
    QStringList nullArgs = {};
    QProcess process;
    QStringList args;
    std::string errorMsg="";

    databaseDir = QDir(ProjectDir.c_str()).filePath("databases");
    if (! QDir(databaseDir).exists()) {
        databaseDir = QString::fromStdString(ProjectDir);
    }

    QString InputFileName = QFileDialog::getOpenFileName(widget,
        "Import Database",
        databaseDir.toLatin1(),
        "*.sql");
    QString fileDatabaseName = QFileInfo(InputFileName).baseName();
    if (fileDatabaseName.isEmpty() || fileDatabaseName.contains(" ")) {
        msg  = "Error: nmfDatabase::importDatabase: Illegal filename found (" + fileDatabaseName + "). ";
        msg += "Filename may not contain spaces.";
        logger->logMsg(nmfConstants::Error,msg.toStdString());
        return "";
    }

    // Does database already exist?
    bool databaseAlreadyExists = databaseExists(fileDatabaseName.toStdString());
    //QApplication::flush();
    QApplication::processEvents();

    // If database exists, ask user if they want it overwritten.
    // If database does not exists, create it first prior to importing.
    if (databaseAlreadyExists) {
        // check if user wants to overwrite
        msg  = "\n\nDatabase exists:  " + fileDatabaseName + "\n\nOK to Overwrite?\n\n";
        msg += "N.B. A database import may take a minute or so to complete.\n";
        reply = QMessageBox::question(widget,
                                      "Database Exists",
                                      msg.toLatin1(),
                                      QMessageBox::No|QMessageBox::Yes,
                                      QMessageBox::Yes);
        if (reply == QMessageBox::No)
            return "";
    } else {
        // create the database
        cmd = "CREATE DATABASE " + fileDatabaseName;
        errorMsg = nmfUpdateDatabase(cmd.toStdString());
        if (nmfUtilsQt::isAnError(errorMsg)) {
            logger->logMsg(nmfConstants::Error,"Error: nmfDatabase::importDatabase: "+errorMsg);
            return "";
        }

        cmd = "USE " + fileDatabaseName;
        errorMsg = nmfUpdateDatabase(cmd.toStdString());
        if (nmfUtilsQt::isAnError(errorMsg)) {
            logger->logMsg(nmfConstants::Error,"Error: nmfDatabase::importDatabase: "+errorMsg);
            return "";
        }

    }

    // Hiro -update logic for Mac OS
    if (nmfUtils::isOSWindows())
    {
        // This seems to work for Windows.
        // Create MySQL import command to be placed into a batch file
        boost::replace_all(Password,"!","\!");
        std::string mysqlCmd = "mysql -u" + Username + " -p" + Password + " " +
                fileDatabaseName.toStdString() + " < \"" +
                InputFileName.toStdString() + "\"";
        std::string importBatchFile = "mysql_import.bat";

        // Delete any existing import batch file
        std::remove(importBatchFile.c_str());

        // Write import batch file
        std::ofstream fout(importBatchFile);
        fout << mysqlCmd;
        fout.close();

        QMessageBox::information(widget,"Import Database",
                                 "\nThis may take a minute or two. Please be patient.\n");

        QApplication::setOverrideCursor(Qt::WaitCursor);

        // Execute import batch file
        QProcess::execute(importBatchFile.c_str(),nullArgs);

        // Remove import batch file
        std::remove(importBatchFile.c_str());

        QApplication::restoreOverrideCursor();
    } else {
        // This seems to work for Linux.
        // Done with all checks so go ahead with the mysql import.
        args << "-u" + QString::fromStdString(Username)
             << "-p" + QString::fromStdString(Password)
             << fileDatabaseName;
        QApplication::setOverrideCursor(Qt::WaitCursor);
        process.setStandardInputFile(InputFileName);
        process.start("mysql", args);
        if (! process.waitForFinished(-1)) { // -1 so it won't timeout
            process.kill();
        }
        QApplication::restoreOverrideCursor();
    }
    return fileDatabaseName;
}


void
nmfDatabase::exportDatabase(QWidget*     widget,
                            std::string& ProjectDir,
                            std::string& Username,
                            std::string& Password,
                            std::string& ProjectDatabase)
{
    QString msg;
    QStringList args;
    QMessageBox::StandardButton reply;
    bool okToWrite = false;

    // Get the databases full path. Create it if it doesn't yet exist.
    QString databaseDir = QDir(ProjectDir.c_str()).filePath("databases");

    if (! QDir(databaseDir).exists()) {
        QDir().mkpath(databaseDir);
    }

    // Show file dialog and have user enter in the output .sql file name.
    QString selFilter = "Database Files (*.sql)";
    QString filePath = databaseDir.append(QString::fromStdString("/" + ProjectDatabase));
    QString OutputFileName = QFileDialog::getSaveFileName(widget,
        "Export Database",
        filePath.toLatin1(),
        "*.sql",
        &selFilter,
        QFileDialog::DontConfirmOverwrite);
    if (OutputFileName.isEmpty())
        return;
    //QApplication::flush();
    QApplication::processEvents();

    // Check for correct file extension and add one if it's not there or is incorrect.
    QFileInfo fi(OutputFileName);
    if (fi.suffix().isEmpty() || (fi.suffix() != "sql"))
        OutputFileName += ".sql";

    // If the file already exists, check with user if they want to overwrite it.
    if (QFileInfo(OutputFileName).exists()) {
        msg = "\nFile exists: " + OutputFileName + "\n\nOK to Overwrite?";
        reply = QMessageBox::question(widget,
                                      "File Exists",
                                      msg.toLatin1(),
                                      QMessageBox::No|QMessageBox::Yes,
                                      QMessageBox::Yes);
        okToWrite = (reply == QMessageBox::Yes);
    } else {
        okToWrite = true;
    }

    // Done with all checks so go ahead with the mysqldump.
    if (okToWrite) {

        QApplication::setOverrideCursor(Qt::WaitCursor);

        QProcess dumpProcess;
        QStringList args;
        args << "-u" + QString::fromStdString(Username)
             << "-p" + QString::fromStdString(Password)
             << QString::fromStdString(ProjectDatabase);
        dumpProcess.setStandardOutputFile(OutputFileName);
        dumpProcess.start("mysqldump", args);
        if (! dumpProcess.waitForFinished(-1)) { // -1 so it won't timeout
            dumpProcess.kill();
        }

        QApplication::restoreOverrideCursor();

    }

    QMessageBox::information(widget, "Export Database",
                             QString::fromStdString("\nDatabase " + ProjectDatabase + " has been successfully exported.\n"));
}

bool
nmfDatabase::getSpeciesData(
        nmfLogger*   logger,
        const std::string&  species,
        int&         MinAge,
        int&         MaxAge,
        int&         FirstYear,
        int&         LastYear,
        float&       MinLength,
        float&       MaxLength,
        int&         NumLengthBins)
{
    std::string msg;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    // Get species data
    fields     = {"SpeName","MinAge","MaxAge","FirstYear","LastYear","MinLength","MaxLength","NumLengthBins"};
    queryStr   = "SELECT SpeName,MinAge,MaxAge,FirstYear,LastYear,MinLength,MaxLength,NumLengthBins FROM " +
                  nmfConstantsMSCAA::TableSpecies +
                 " WHERE SpeName = '" + species + "'";
    dataMap    = nmfQueryDatabase(queryStr, fields);
    if (dataMap["SpeName"].size() == 0) {
        msg = "nmfDatabase::getSpeciesData: No records found in Species for: " +species;
        logger->logMsg(nmfConstants::Error,msg);
        return false;
    }

    MinAge        = std::stoi(dataMap["MinAge"][0]);
    MaxAge        = std::stoi(dataMap["MaxAge"][0]);
    FirstYear     = std::stoi(dataMap["FirstYear"][0]);
    LastYear      = std::stoi(dataMap["LastYear"][0]);
    MinLength     = std::stof(dataMap["MinLength"][0]);
    MaxLength     = std::stof(dataMap["MaxLength"][0]);
    NumLengthBins = std::stoi(dataMap["NumLengthBins"][0]);

    return true;
}

bool nmfDatabase::getGuilds(
        nmfLogger*  logger,
        std::vector<std::string>& guilds)
{
    int NumRecords;
    std::string msg;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    guilds.clear();

    // Get species data
    fields     = {"GuildName"};
    queryStr   = "SELECT GuildName FROM " + nmfConstantsMSSPM::TableGuilds;
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["GuildName"].size();
    if (NumRecords == 0) {
        msg = "nmfDatabase::getAllGuilds: No records found";
        logger->logMsg(nmfConstants::Error,msg);
        return false;
    }
    for (int i=0; i<NumRecords; ++i) {
        guilds.push_back(dataMap["GuildName"][i]);
    }
    return true;
}

bool
nmfDatabase::getGuilds(
        nmfLogger* logger,
        int &NumGuilds,
        QStringList &GuildList)
{
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    GuildList.clear();

    fields   = {"GuildName"};
    queryStr = "SELECT GuildName FROM " +
                nmfConstantsMSSPM::TableGuilds +
               " ORDER BY GuildName";
    dataMap  = nmfQueryDatabase(queryStr, fields);
    NumGuilds = dataMap["GuildName"].size();
    if (NumGuilds == 0) {
        logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::getGuilds: No guilds found in table Guilds");
        return false;
    }

    for (int guild=0; guild<NumGuilds; ++guild) {
        GuildList << QString::fromStdString(dataMap["GuildName"][guild]);
    }

    return true;
}

bool nmfDatabase::getSpecies(
        nmfLogger*  logger,
        std::vector<std::string>& species)
{
    int NumRecords;
    std::string msg;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    species.clear();

    // Get species data
    fields     = {"SpeName"};
    queryStr   = "SELECT SpeName FROM " + nmfConstantsMSSPM::TableSpecies;
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        msg = "nmfDatabase::getAllSpecies: No records found";
        logger->logMsg(nmfConstants::Error,msg);
        return false;
    }
    for (int i=0; i<NumRecords; ++i) {
        species.push_back(dataMap["SpeName"][i]);
    }
    return true;
}

bool
nmfDatabase::getSpecies(
        nmfLogger* logger,
        int &NumSpecies,
        QStringList &SpeciesList)
{
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    SpeciesList.clear();

    fields   = {"SpeName"};
    queryStr = "SELECT SpeName FROM " +
                nmfConstantsMSSPM::TableSpecies +
               " ORDER BY SpeName";
    dataMap  = nmfQueryDatabase(queryStr, fields);
    NumSpecies = dataMap["SpeName"].size();
    if (NumSpecies == 0) {
        logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::getSpecies: No species found in table Species");
        return false;
    }

    for (int species=0; species<NumSpecies; ++species) {
        SpeciesList << QString::fromStdString(dataMap["SpeName"][species]);
    }

    return true;
}

bool
nmfDatabase::getListOfAuthenticatedDatabaseNames(
        QList<QString>& authenticatedDatabases)
{
    int numDatabases;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string name;
    std::string currentDatabase = nmfGetCurrentDatabase();

    fields   = { "Database" };
    queryStr = "SHOW databases";
    dataMap  = nmfQueryDatabase(queryStr,fields);
    numDatabases = dataMap["Database"].size();
    if (numDatabases <= 0) {
        return false;
    }

    for (int i = 0; i < numDatabases; ++i) {
        name = dataMap["Database"][i];
        if (authenticateDatabase(name)) {
            authenticatedDatabases.push_back(QString::fromStdString(name));
        }
    }

    // Must reset database as authenticateDatabase has to set to each database
    // to query one of its tables
    nmfSetDatabase(currentDatabase);

    return true;
}




void
nmfDatabase::getSpeciesGuildMap(std::map<std::string,std::string>& SpeciesGuildMap)
{
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    fields   = {"SpeName","GuildName"};
    queryStr = "SELECT SpeName,GuildName FROM " +
                nmfConstantsMSSPM::TableSpecies +
               " ORDER BY SpeName";
    dataMap  = nmfQueryDatabase(queryStr, fields);

    for (unsigned i=0; i<dataMap["SpeName"].size(); ++i) {
        SpeciesGuildMap[dataMap["SpeName"][i]] = dataMap["GuildName"][i];
    }
}



bool
nmfDatabase::getTimeSeriesDataByGuild(
        nmfLogger* logger,
        const std::string& ProjectName,
        const std::string& ModelName,
        const std::string& ForecastName,
        const std::string& TableName,
        const int &NumGuilds,
        const int &RunLength,
        const QString& OutputChartType,
        boost::numeric::ublas::matrix<double> &OutputBiomass,
        boost::numeric::ublas::matrix<double> &TableData)
{
    int m=0;
    int NumRecords;
    int NumSpecies;
    int NumGuilds2;
    int GuildNum;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string errorMsg;
    std::string ModifiedTableName = "";
    std::string GuildName;
    std::string SpeciesName;
    QStringList SpeciesList;
    QStringList GuildList;
    std::map<std::string,std::string> SpeciesToGuildMap;
    std::map<std::string,int> GuildNameToNumMap;
    std::string CompetitionForm;
    std::string Algorithm;
    std::string Minimizer;
    std::string ObjectiveCriterion;
    std::string Scaling;
    std::string isAggProdStr = "0";
    std::vector<double> EstCatchability = {};
//  bool isCatch  = (TableName == nmfConstantsMSSPM::TableHarvestCatch);
    bool isEffort = (TableName == nmfConstantsMSSPM::TableHarvestEffort);
//  bool isChartBiomass      = (OutputChartType == nmfConstantsMSSPM::OutputChartBiomass);
    bool isChartHarvest      = (OutputChartType == nmfConstantsMSSPM::OutputChartHarvest);
    bool isChartExploitation = (OutputChartType == nmfConstantsMSSPM::OutputChartExploitation);
//std::cout << "TableName: " << TableName << std::endl;

    boost::numeric::ublas::matrix<double> DenominatorData;

    nmfUtils::initialize(TableData,      RunLength+1,NumGuilds); // +1 because there's a 0 year
    nmfUtils::initialize(DenominatorData,RunLength+1,NumGuilds); // +1 because there's a 0 year

    if (! getGuilds(logger,NumGuilds2,GuildList)) {
        return false;
    }

    if (isEffort) { // multiplyByCatchability) {
        getAlgorithmIdentifiers(
                    nullptr,logger,ProjectName,ModelName,
                    Algorithm,Minimizer,ObjectiveCriterion,
                    Scaling,CompetitionForm,nmfConstantsMSSPM::DontShowPopupError);
        getEstimatedParameter(nmfConstantsMSSPM::TableOutputCatchability,
                              ProjectName,ModelName,
                              Algorithm,Minimizer,ObjectiveCriterion,
                              Scaling,isAggProdStr,EstCatchability);
    }

    // Get Species names
    if (! getSpecies(logger,NumSpecies,SpeciesList))
        return false;

    // Load data
    if (ForecastName == "") {
        ModifiedTableName = TableName;
        fields   = {"ProjectName","ModelName","SpeName","Year","Value"};
        queryStr = "SELECT ProjectName,ModelName,SpeName,Year,Value FROM " +
                    ModifiedTableName +
                   " WHERE ProjectName = '" + ProjectName +
                   "' AND ModelName = '"    + ModelName +
                   "' ORDER BY SpeName,Year";
    } else {
        ModifiedTableName = "forecast" + TableName;
        fields   = {"ForecastName","SpeName","Year","Value"};
        queryStr = "SELECT ForecastName,SpeName,Year,Value FROM " +
                    ModifiedTableName +
                   " WHERE ProjectName = '" + ProjectName +
                   "' AND ModelName = '"    + ModelName +
                   "' AND ForecastName = '" + ForecastName +
                   "' ORDER BY SpeName,Year";
    }
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        logger->logMsg(nmfConstants::Error,"[Error 1] getTimeSeriesDataByGuild: No records found in table "+TableName);
        return false;
    }
    if (NumRecords != NumSpecies*(RunLength+1)) {
        errorMsg  = "[Error 2] getTimeSeriesDataByGuild: Number of records found (" + std::to_string(NumRecords) + ") in ";
        errorMsg += "table " + ModifiedTableName + " does not equal number of Species*(RunLength+1) (";
        errorMsg += std::to_string(NumSpecies) + "*" + std::to_string((RunLength+1)) + "=";
        errorMsg += std::to_string(NumSpecies*(RunLength+1)) + ") records";
        errorMsg += "\n" + queryStr;
        logger->logMsg(nmfConstants::Error,errorMsg);
    }

    m = 0;
    int num=0;
    for (QString guildName : GuildList) {
        GuildNameToNumMap[guildName.toStdString()] = num++;
    }
    getSpeciesGuildMap(SpeciesToGuildMap);
    for (int i=0; i<NumSpecies; ++i) {
        SpeciesName = dataMap["SpeName"][m];
        GuildName   = SpeciesToGuildMap[SpeciesName];
        GuildNum    = GuildNameToNumMap[GuildName];
        for (int time=0; time<=RunLength; ++time) {
            if (isEffort) {
                if (isChartHarvest) {
                    TableData(time,GuildNum)       += EstCatchability[i]*std::stod(dataMap["Value"][m++])*OutputBiomass(time,i); // q·E·B(c)
                } else if (isChartExploitation) {
                    TableData(time,GuildNum)       += EstCatchability[i]*std::stod(dataMap["Value"][m++])*OutputBiomass(time,i); // q·E·B(c)
                    DenominatorData(time,GuildNum) += OutputBiomass(time,i);
                } else {
                    TableData(time,GuildNum)       += std::stod(dataMap["Value"][m++]);
                }
            } else {
                if (isChartHarvest) {
                    TableData(time,GuildNum)       += std::stod(dataMap["Value"][m++]);
                } else if (isChartExploitation) {
                    TableData(time,GuildNum)       += std::stod(dataMap["Value"][m++]); // C
                    DenominatorData(time,GuildNum) += OutputBiomass(time,i);            //  /B(c)
                } else {
                    TableData(time,GuildNum)       += std::stod(dataMap["Value"][m++]);
                }
            }
        }
    }

    if (isChartExploitation) {
        for (int guild=0; guild<(int)TableData.size2(); ++guild) {
            for (int time=0; time<=RunLength; ++time) {
                TableData(time,guild) /= DenominatorData(time,guild);
            }
        }
    }

    return true;
}


bool
nmfDatabase::getTimeSeriesData(
        QWidget*           Widget,
        nmfLogger*         Logger,
        const std::string& ProjectName,
        const std::string& ModelName,
        const std::string  ForecastName,
        const std::string& TableName,
        const int&         NumSpecies,
        const int&         RunLength,
        boost::numeric::ublas::matrix<double>& TableData)
{
    int m=0;
    int NumRecords;
    int NumYears = RunLength+1; // +1 because there's a 0 year
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string errorMsg;
    std::string ModifiedTableName = "";
    QString msg;

    nmfUtils::initialize(TableData,NumYears,NumSpecies);

    // Load data
    if (ForecastName == "") {
        ModifiedTableName = TableName;
        fields   = {"ProjectName","ModelName","SpeName","Year","Value"};
        queryStr = "SELECT ProjectName,ModelName,SpeName,Year,Value FROM " +
                    ModifiedTableName +
                   " WHERE ProjectName = '" + ProjectName +
                   "' AND ModelName = '"    + ModelName +
                   "' ORDER BY SpeName,Year";
    } else {
        ModifiedTableName = "forecast" + TableName;;
        fields   = {"ProjectName","ModelName","ForecastName","SpeName","Year","Value"};
        queryStr = "SELECT ProjectName,ModelName,ForecastName,SpeName,Year,Value FROM " +
                    ModifiedTableName +
                   " WHERE ProjectName = '" + ProjectName +
                   "' AND ModelName = '"    + ModelName +
                   "' AND ForecastName = '" + ForecastName +
                   "' ORDER BY SpeName,Year";
    }
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        Logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::getTimeSeriesData: No records found in table "+TableName);
        Logger->logMsg(nmfConstants::Error,queryStr);
        msg = "\nMissing or unsaved data. Please populate and resave table: " + QString::fromStdString(TableName);
        QMessageBox::critical(Widget, "Error", msg, QMessageBox::Ok);
        return false;
    }
    if (NumRecords != NumSpecies*(RunLength+1)) {
        errorMsg  = "[Error 2] nmfDatabase::getTimeSeriesData: Number of records found (" + std::to_string(NumRecords) + ") in ";
        errorMsg += "table " + ModifiedTableName + " does not equal number of Species*(RunLength+1) (";
        errorMsg += std::to_string(NumSpecies) + "*" + std::to_string((RunLength+1)) + "=";
        errorMsg += std::to_string(NumSpecies*(RunLength+1)) + ") records";
        Logger->logMsg(nmfConstants::Error,errorMsg);
        errorMsg  = queryStr;
        Logger->logMsg(nmfConstants::Error,errorMsg);
        msg = "\nMissing or unsaved data.\n\nPlease populate and resave table: " + QString::fromStdString(ModifiedTableName) + "\n";
        QMessageBox::critical(Widget, "Error", msg, QMessageBox::Ok);
        return false;
    }

    for (int species=0; species<NumSpecies; ++species) {
        for (int time=0; time<=RunLength; ++time) {
            //TableData(time,species) = std::stod(dataMap["Value"][m++]);
            TableData(time,species) = QString::fromStdString(dataMap["Value"][m++]).toDouble();
        }
    }

    return true;
}

bool
nmfDatabase::getForecastInfo(
        const std::string& TableName,
        const std::string& ProjectName,
        const std::string& ModelName,
        const std::string& ForecastName,
        int&               RunLength,
        int&               StartForecastYear,
        std::string&       Algorithm,
        std::string&       Minimizer,
        std::string&       ObjectiveCriterion,
        std::string&       Scaling,
        int&               NumRuns)
{
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    // Find Forecast info
    fields    = {"ProjectName","ModelName","ForecastName","Algorithm","Minimizer","ObjectiveCriterion","Scaling","GrowthForm","HarvestForm","WithinGuildCompetitionForm","PredationForm","RunLength","StartYear","EndYear","NumRuns"};
    queryStr  = "SELECT ProjectName,ModelName,ForecastName,Algorithm,Minimizer,ObjectiveCriterion,Scaling,GrowthForm,HarvestForm,WithinGuildCompetitionForm,PredationForm,RunLength,StartYear,EndYear,NumRuns FROM " +
                 TableName +
                " WHERE ProjectName = '" + ProjectName +
                "' AND ModelName = '"    + ModelName +
                "' AND ForecastName = '" + ForecastName + "'";
    dataMap   = nmfQueryDatabase(queryStr, fields);
    if (dataMap["ForecastName"].size() != 0) {
        RunLength          = std::stoi(dataMap["RunLength"][0]);
        StartForecastYear  = std::stoi(dataMap["StartYear"][0]);
     // EndYear            = std::stoi(dataMap["EndYear"][0]);
        Algorithm          = dataMap["Algorithm"][0];
        Minimizer          = dataMap["Minimizer"][0];
        ObjectiveCriterion = dataMap["ObjectiveCriterion"][0];
        Scaling            = dataMap["Scaling"][0];
        NumRuns            = std::stoi(dataMap["NumRuns"][0]);
        return true;
    }
    return false;
}


bool
nmfDatabase::getForecastBiomass(
        QWidget*           Widget,
        nmfLogger*         Logger,
        const std::string& ProjectName,
        const std::string& ModelName,
        const std::string& ForecastName,
        const int&         NumSpecies,
        const int&         RunLength,
        std::string&       Algorithm,
        std::string&       Minimizer,
        std::string&       ObjectiveCriterion,
        std::string&       Scaling,
        std::vector<boost::numeric::ublas::matrix<double> >& ForecastBiomass)
{
    int m=0;
    int NumRecords;
    std::vector<std::string> fields;
    std::string queryStr;
    std::string errorMsg;
    QString msg;
    std::map<std::string, std::vector<std::string> > dataMapForecastBiomass;

    ForecastBiomass.clear();

    // Load Forecast Biomass data (ie, calculated from estimated parameters r and alpha)
    fields    = {"ProjectName","ModelName","ForecastName","Algorithm","Minimizer","ObjectiveCriterion","Scaling","SpeName","Year","Value"};
    queryStr  = "SELECT ProjectName,ModelName,ForecastName,Algorithm,Minimizer,ObjectiveCriterion,Scaling,SpeName,Year,Value FROM " +
                 nmfConstantsMSSPM::TableForecastBiomass +
                " WHERE ProjectName = '"       + ProjectName +
                "' AND ModelName = '"          + ModelName +
                "' AND ForecastName = '"       + ForecastName +
                "' AND Algorithm = '"          + Algorithm +
                "' AND Minimizer = '"          + Minimizer +
                "' AND ObjectiveCriterion = '" + ObjectiveCriterion +
                "' AND Scaling = '"            + Scaling +
                "' ORDER BY SpeName,Year";
    dataMapForecastBiomass = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMapForecastBiomass["SpeName"].size();
    if (NumRecords == 0) {
        errorMsg  = "[Warning] nmfDatabase::getForecastBiomass: No records found in table ForecastBiomass";
        Logger->logMsg(nmfConstants::Warning,errorMsg);
        msg = "\nNo ForecastBiomass records found.\n\nPlease make sure a Forecast has been run.\n";
        QMessageBox::warning(Widget, "Warning", msg, QMessageBox::Ok);
        return false;
    }
    if (NumRecords != NumSpecies*(RunLength+1)) {
        errorMsg  = "[Error 2] nmfDatabase::getForecastBiomass: Number of records found (" + std::to_string(NumRecords) + ") in ";
        errorMsg += "table ForecastBiomass does not equal number of NumSpecies*(RunLength+1) (";
        errorMsg += std::to_string(NumSpecies) + "*" + std::to_string((RunLength+1)) + "=";
        errorMsg += std::to_string(NumSpecies*(RunLength+1)) + ") records";
        errorMsg += "\n" + queryStr;
        Logger->logMsg(nmfConstants::Error,errorMsg);
        return false;
    }

    boost::numeric::ublas::matrix<double> TmpMatrix;
    nmfUtils::initialize(TmpMatrix,RunLength+1,NumSpecies);

    for (int species=0; species<NumSpecies; ++species) {
        for (int time=0; time<=RunLength; ++time) {
            TmpMatrix(time,species) = std::stod(dataMapForecastBiomass["Value"][m++]);
        }
    }
    ForecastBiomass.push_back(TmpMatrix);

    return true;
}

bool
nmfDatabase::getForecastBiomassMonteCarlo(
        QWidget*           Widget,
        nmfLogger*         Logger,
        const std::string& ProjectName,
        const std::string& ModelName,
        const std::string& ForecastName,
        const int&         NumSpecies,
        const int&         RunLength,
        const int&         NumRuns,
        std::string&       Algorithm,
        std::string&       Minimizer,
        std::string&       ObjectiveCriterion,
        std::string&       Scaling,
        std::vector<boost::numeric::ublas::matrix<double> >& ForecastBiomassMonteCarlo)
{
    int m=0;
    int NumRecords;
    std::vector<std::string> fields;
    std::string queryStr;
    std::string errorMsg;
    std::map<std::string, std::vector<std::string> > dataMapForecastBiomassMonteCarlo;
    boost::numeric::ublas::matrix<double> TmpMatrix;
    QString msg;

    ForecastBiomassMonteCarlo.clear();

    // Load Forecast Biomass data (ie, calculated from estimated parameters r and alpha)
    fields    = {"ProjectName","ModelName","ForecastName","RunNum","Algorithm","Minimizer","ObjectiveCriterion","Scaling","SpeName","Year","Value"};
    queryStr  = "SELECT ProjectName,ModelName,ForecastName,RunNum,Algorithm,Minimizer,ObjectiveCriterion,Scaling,SpeName,Year,Value FROM " +
                 nmfConstantsMSSPM::TableForecastBiomassMonteCarlo +
                " WHERE ProjectName = '"       + ProjectName +
                "' AND ModelName = '"          + ModelName +
                "' AND ForecastName = '"       + ForecastName +
                "' AND Algorithm = '"          + Algorithm +
                "' AND Minimizer = '"          + Minimizer +
                "' AND ObjectiveCriterion = '" + ObjectiveCriterion +
                "' AND Scaling = '"            + Scaling +
                "' ORDER BY RunNum,SpeName,Year";
    dataMapForecastBiomassMonteCarlo = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMapForecastBiomassMonteCarlo["SpeName"].size();
    if (NumRecords == 0) {
        //m_ChartView2d->hide();
        errorMsg  = "[Error 1] nmfDatabase::getForecastBiomassMonteCarlo: No records found in table ForecastBiomassMonteCarlo";
        Logger->logMsg(nmfConstants::Error,errorMsg);
        Logger->logMsg(nmfConstants::Error,queryStr);
        msg = "\nNo forecastbiomassmontecarlo records found.\n\nPlease make sure a Forecast has been run.\n";
        QMessageBox::warning(Widget, "Warning", msg, QMessageBox::Ok);
        return false;
    }
    if (NumRecords != NumRuns*NumSpecies*(RunLength+1)) {
        errorMsg  = "[Error 2] nmfDatabase::getForecastBiomassMonteCarlo: Number of records found (" + std::to_string(NumRecords) + ") in ";
        errorMsg += "table forecastbiomassmontecarlo does not equal number of NumRuns*NumSpecies*(RunLength+1) (";
        errorMsg += std::to_string(NumRuns) + "*";
        errorMsg += std::to_string(NumSpecies) + "*" + std::to_string((RunLength+1)) + "=";
        errorMsg += std::to_string(NumRuns*NumSpecies*(RunLength+1)) + ") records";
        errorMsg += "\n" + queryStr;
        Logger->logMsg(nmfConstants::Error,errorMsg);
        return false;
    }

    // Load data into data structure
    for (int runNum=0; runNum<NumRuns; ++runNum) {
        nmfUtils::initialize(TmpMatrix,RunLength+1,NumSpecies);
        for (int species=0; species<NumSpecies; ++species) {
            for (int time=0; time<=RunLength; ++time) {
                TmpMatrix(time,species) = std::stod(dataMapForecastBiomassMonteCarlo["Value"][m++]);
            }
        }
        ForecastBiomassMonteCarlo.push_back(TmpMatrix);
    }

    return true;
}



bool
nmfDatabase::getForecastMonteCarloHoverData(
        QWidget*             widget,
        nmfLogger*           logger,
        const std::string&   Species,
        const std::string&   ProjectName,
        const std::string&   ModelName,
        const std::string&   ForecastName,
        const std::string&   Algorithm,
        const std::string&   Minimizer,
        const std::string&   ObjectiveCriterion,
        const std::string&   Scaling,
        QStringList&         HoverData)
{
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string TableName = nmfConstantsMSSPM::TableForecastMonteCarloParameters;
    double  val1,val2,val3;
    QString str1,str2,str3;

    fields     = {"ProjectName","ModelName","ForecastName","RunNum","Algorithm","Minimizer","ObjectiveCriterion","Scaling",
                  "SpeName","GrowthRate","CarryingCapacity","Harvest"};
    queryStr   = "SELECT ProjectName,ModelName,ForecastName,RunNum,Algorithm,Minimizer,ObjectiveCriterion,Scaling,";
    queryStr  += "SpeName,GrowthRate,CarryingCapacity,Harvest FROM " +
                  TableName +
                 " WHERE ProjectName = '"       + ProjectName +
                 "' AND ModelName = '"          + ModelName +
                 "' AND ForecastName = '"       + ForecastName +
                 "' AND Algorithm = '"          + Algorithm +
                 "' AND Minimizer = '"          + Minimizer +
                 "' AND ObjectiveCriterion = '" + ObjectiveCriterion +
                 "' AND Scaling = '"            + Scaling +
                 "' AND SpeName = '"            + Species +
                 "' ORDER by RunNum,SpeName";
    dataMap = nmfQueryDatabase(queryStr, fields);
    int NumRecords = dataMap["GrowthRate"].size();
    if (NumRecords == 0) {
        logger->logMsg(nmfConstants::Error, queryStr);
        return false;
    }
    for (int i=0;i<NumRecords;++i) {
        val1 = std::stod(dataMap["GrowthRate"][i])*100.0;
        val2 = std::stod(dataMap["CarryingCapacity"][i])*100.0;
        val3 = std::stod(dataMap["Harvest"][i])*100.0;
        str1 = QString{"%1%"}.arg(val1,4,'f',1); // Ex. 12.3%
        str2 = QString{"%1%"}.arg(val2,4,'f',1);
        str3 = QString{"%1%"}.arg(val3,4,'f',1);
        HoverData << "( " + str1 + ", " + str2 + ", " + str3 + " )"; // Ex
    }
    return true;
}

bool
nmfDatabase::isARelativeBiomassModel(
        const std::string& ProjectName,
        const std::string& ModelName)
{
    bool retv=false;
    std::vector<std::string> fields;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;

    fields    = {"ModelName","ObsBiomassType"};
    queryStr  = "SELECT ModelName,ObsBiomassType FROM " +
                 nmfConstantsMSSPM::TableModels +
                " WHERE ProjectName = '" + ProjectName +
                "' AND ModelName = '"    + ModelName   + "'";
    dataMap   = nmfQueryDatabase(queryStr,fields);
    int NumRecords = dataMap["ModelName"].size();
    if (NumRecords == 1) {
        retv = (dataMap["ObsBiomassType"][0] == "Relative");
    }
    return retv;
}

bool
nmfDatabase::isSurveyQ(
        const std::string& ProjectName,
        const std::string& ModelName)
{
    bool retv=false;
    std::vector<std::string> fields;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;

    fields    = {"ObsBiomassType"};
    queryStr  = "SELECT ObsBiomassType FROM " +
                 nmfConstantsMSSPM::TableModels +
                " WHERE ProjectName = '" + ProjectName +
                "' AND ModelName = '"    + ModelName   + "'";
    dataMap   = nmfQueryDatabase(queryStr,fields);
    int NumRecords = dataMap["ObsBiomassType"].size();
    if (NumRecords == 1) {
        retv = (dataMap["ObsBiomassType"][0] == "Relative");
    }

    return retv;
}

bool
nmfDatabase::getForecastHarvest(
        QWidget*           Widget,
        nmfLogger*         Logger,
        const std::string& ProjectName,
        const std::string& ModelName,
        const std::string& ForecastName,
        const int&         NumSpecies,
        const int&         RunLength,
        std::string&       Algorithm,
        std::string&       Minimizer,
        std::string&       ObjectiveCriterion,
        std::string&       Scaling,
        const std::string& HarvestForm,
        std::vector<boost::numeric::ublas::matrix<double> >& ForecastHarvest)
{
    int m=0;
    int NumRecords;
    std::vector<std::string> fields;
    std::string queryStr;
    std::string errorMsg;
    QString msg;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string ForecastTable = nmfConstantsMSSPM::TableForecastHarvestCatch;
    ForecastHarvest.clear();

    if (HarvestForm == "Effort (qE)") {
        ForecastTable = nmfConstantsMSSPM::TableForecastHarvestEffort;
    } else if (HarvestForm == "Exploitation (F)") {
        ForecastTable = nmfConstantsMSSPM::TableForecastHarvestExploitation;
    }

    // Load Forecast Harvest data
    fields    = {"ProjectName","ModelName","ForecastName","Algorithm","Minimizer","ObjectiveCriterion","Scaling","SpeName","Year","Value"};
    queryStr  = "SELECT ProjectName,ModelName,ForecastName,Algorithm,Minimizer,ObjectiveCriterion,Scaling,SpeName,Year,Value FROM " +
                 ForecastTable +
                " WHERE ProjectName = '"       + ProjectName +
                "' AND ModelName = '"          + ModelName +
                "' AND ForecastName = '"       + ForecastName +
                "' AND Algorithm = '"          + Algorithm +
                "' AND Minimizer = '"          + Minimizer +
                "' AND ObjectiveCriterion = '" + ObjectiveCriterion +
                "' AND Scaling = '"            + Scaling +
                "' ORDER BY SpeName,Year";
    dataMap = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        errorMsg  = "[Warning] nmfDatabase::getForecastCatch: No records found in table " + ForecastTable;
        Logger->logMsg(nmfConstants::Warning,errorMsg);
        msg = "\nNo " + QString::fromStdString(ForecastTable) + " records found.\n\nPlease make sure a Forecast has been run.\n";
        QMessageBox::warning(Widget, "Warning", msg, QMessageBox::Ok);
        return false;
    }
    if (NumRecords != NumSpecies*(RunLength+1)) {
        errorMsg  = "[Error 2] nmfDatabase::getForecastCatch: Number of records found (" + std::to_string(NumRecords) + ") in ";
        errorMsg += "table " + ForecastTable + " does not equal number of NumSpecies*(RunLength+1) (";
        errorMsg += std::to_string(NumSpecies) + "*" + std::to_string((RunLength+1)) + "=";
        errorMsg += std::to_string(NumSpecies*(RunLength+1)) + ") records";
        errorMsg += "\n" + queryStr;
        Logger->logMsg(nmfConstants::Error,errorMsg);
        return false;
    }

    boost::numeric::ublas::matrix<double> TmpMatrix;
    nmfUtils::initialize(TmpMatrix,RunLength+1,NumSpecies);

    for (int species=0; species<NumSpecies; ++species) {
        for (int time=0; time<=RunLength; ++time) {
            TmpMatrix(time,species) = std::stod(dataMap["Value"][m++]);
        }
    }
    ForecastHarvest.push_back(TmpMatrix);
    return true;
}

bool
nmfDatabase::updateForecastMonteCarloParameters(
        QWidget*                   widget,
        nmfLogger*                 logger,
        const std::string&         ProjectName,
        const std::string&         ModelName,
        const std::string&         ForecastName,
        const std::string&         Algorithm,
        const std::string&         Minimizer,
        const std::string&         ObjectiveCriterion,
        const std::string&         Scaling,
        const QStringList&         Species,
        const int&                 RunNumber,
        const std::vector<double>& GrowthRandomValues,
        const std::vector<double>& CarryingCapacityRandomValues,
        const std::vector<double>& CatchabilityRandomValues,
        const std::vector<double>& ExponentRandomValues,
        const std::vector<double>& CompetitionAlphaRandomValues,
        const std::vector<double>& CompetitionBetaSpeciesRandomValues,
        const std::vector<double>& CompetitionBetaGuildsRandomValues,
        const std::vector<double>& CompetitionBetaGuildsGuildsRandomValues,
        const std::vector<double>& PredationRandomValues,
        const std::vector<double>& HandlingRandomValues,
        const std::vector<double>& HarvestRandomValues)
{
    std::string saveCmd;
    std::string deleteCmd;
    std::string errorMsg;
    std::string tableName = nmfConstantsMSSPM::TableForecastMonteCarloParameters;
    QString msg;
    double catchability;
    double exponent;
    double alpha;
    double betaSpecies;
    double betaGuilds;
    double betaGuildsGuilds;
    double predation;
    double handling;
    double harvest;
    double carryingCapacity;

    // Delete the current entry here
    deleteCmd  = "DELETE FROM " + tableName;
    deleteCmd += " WHERE ProjectName = '"       + ProjectName +
                 "' AND ModelName = '"          + ModelName +
                 "' AND ForecastName = '"       + ForecastName +
                 "' AND RunNum = "              + std::to_string(RunNumber) +
                 "  AND Algorithm = '"          + Algorithm +
                 "' AND Minimizer = '"          + Minimizer +
                 "' AND ObjectiveCriterion = '" + ObjectiveCriterion +
                 "' AND Scaling = '"            + Scaling + "'";
    errorMsg = nmfUpdateDatabase(deleteCmd);
    if (nmfUtilsQt::isAnError(errorMsg)) {
        msg = "\nError in ForecastMonteCarloParameters command. Couldn't delete all records from " +
                QString::fromStdString(tableName) + " table";
        logger->logMsg(nmfConstants::Error,"nmfDatabase::updateForecastMonteCarloParameters: DELETE error: " + errorMsg);
        logger->logMsg(nmfConstants::Error,"cmd: " + deleteCmd);
        QMessageBox::warning(widget, "Error", msg, QMessageBox::Ok);
        return false;
    }

    saveCmd  = "INSERT INTO " +
                tableName +
               " (ProjectName,ModelName,ForecastName,RunNum,Algorithm,Minimizer,ObjectiveCriterion,Scaling," +
               "SpeName,GrowthRate,CarryingCapacity,Catchability," +
               "Exponent,CompetitionAlpha,CompetitionBetaSpecies,CompetitionBetaGuilds," +
               "CompetitionBetaGuildsGuilds,Predation,Handling," +
               "Harvest) VALUES ";
    int NumValues = GrowthRandomValues.size();

    for (int j=0; j<NumValues; ++j) {
        carryingCapacity = checkForValues(j,NumValues,CarryingCapacityRandomValues);
        catchability     = checkForValues(j,NumValues,CatchabilityRandomValues);
        exponent         = checkForValues(j,NumValues,ExponentRandomValues);
        alpha            = checkForValues(j,NumValues,CompetitionAlphaRandomValues);
        betaSpecies      = checkForValues(j,NumValues,CompetitionBetaSpeciesRandomValues);
        betaGuilds       = checkForValues(j,NumValues,CompetitionBetaGuildsRandomValues);
        betaGuildsGuilds = checkForValues(j,NumValues,CompetitionBetaGuildsGuildsRandomValues);
        predation        = checkForValues(j,NumValues,PredationRandomValues);
        handling         = checkForValues(j,NumValues,HandlingRandomValues);
        harvest          = checkForValues(j,NumValues,HarvestRandomValues);
        saveCmd += "('"   + ProjectName +
                    "','" + ModelName +
                    "','" + ForecastName +
                    "',"  + std::to_string(RunNumber) +
                    ",'"  + Algorithm +
                    "','" + Minimizer +
                    "','" + ObjectiveCriterion +
                    "','" + Scaling +
                    "','" + Species[j].toStdString() +
                    "',"  + std::to_string(GrowthRandomValues[j]) +
                    ","   + std::to_string(carryingCapacity) +
                    ","   + std::to_string(catchability) +
                    ","   + std::to_string(exponent) +
                    ","   + std::to_string(alpha) +
                    ","   + std::to_string(betaSpecies) +
                    ","   + std::to_string(betaGuilds) +
                    ","   + std::to_string(betaGuildsGuilds) +
                    ","   + std::to_string(predation) +
                    ","   + std::to_string(handling) +
                    ","   + std::to_string(harvest) + "),";
    }

    saveCmd = saveCmd.substr(0,saveCmd.size()-1);

    errorMsg = nmfUpdateDatabase(saveCmd);
    if (nmfUtilsQt::isAnError(errorMsg)) {
        logger->logMsg(nmfConstants::Error,"[Error] nmfDatabase::updateForecastMonteCarloParameters: Write table error: " + errorMsg);
        logger->logMsg(nmfConstants::Error,"saveCmd: " + saveCmd);
        return false;
    }

    return true;
}


bool
nmfDatabase::getAllTables(std::vector<std::string>& databaseTables)
{
    int numTables;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string currentDatabase = nmfGetCurrentDatabase();
    std::string columnName = "Tables_in_"+currentDatabase;

    databaseTables.clear();
    fields    = { columnName };
    queryStr  = "SHOW tables";
    dataMap   = nmfQueryDatabase(queryStr,fields);
    numTables = dataMap[columnName].size();

    for (int i = 0; i < numTables; ++i) {
        databaseTables.push_back(dataMap[columnName][i]);
    }

    return (numTables > 0);
}


bool
nmfDatabase::authenticateDatabase(const std::string& databaseName)
{
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    nmfSetDatabase(databaseName);

    fields   = {"Name"};
    queryStr = "SELECT Name FROM " + nmfConstantsMSSPM::TableApplication;
    dataMap  = nmfQueryDatabase(queryStr, fields);
    if (dataMap["Name"].size() != 1) {
        return false;
    }

    return (QApplication::applicationName().toStdString() == dataMap["Name"][0]);
}



void
nmfDatabase::saveApplicationTable(
        QWidget*       widget,
        nmfLogger*     logger,
        std::string&   tableName)
{
    std::string saveCmd;
    std::string deleteCmd;
    std::string errorMsg;
    std::string appName = QApplication::applicationName().toStdString();
    QString msg;

    // Delete the current entry here
    deleteCmd = "DELETE FROM " + tableName; // delete all rows
    errorMsg = nmfUpdateDatabase(deleteCmd);
    if (nmfUtilsQt::isAnError(errorMsg)) {
        msg = "\nError  in saveApplicationTable command. Couldn't delete all records from " +
                QString::fromStdString(tableName) + " table";
        logger->logMsg(nmfConstants::Error,"nmfDatabase::saveApplicationTable: DELETE error: " + errorMsg);
        logger->logMsg(nmfConstants::Error,"cmd: " + deleteCmd);
        QMessageBox::warning(widget, "Error", msg, QMessageBox::Ok);
        return;
    }

    // Save the new data
    saveCmd = "INSERT INTO " + tableName +" (Name) VALUES ('" + appName + "')";
    errorMsg = nmfUpdateDatabase(saveCmd);
    if (nmfUtilsQt::isAnError(errorMsg)) {
        logger->logMsg(nmfConstants::Error,"nmfDatabase::saveApplicationTable: Write table error: " + errorMsg);
        logger->logMsg(nmfConstants::Error,"cmd: " + saveCmd);
        QMessageBox::warning(widget, "Error",
                             "\nError in saveApplicationTable command\n",
                             QMessageBox::Ok);
    }

}

bool
nmfDatabase::loadVector(
        nmfLogger* Logger,
        const std::vector<std::map<std::string, std::vector<std::string> > >& dataMaps,
        const int& NumRows,
        std::vector<double>& vec)
{
    int m=0;
    bool retv = true;
    std::string msg;
    std::map<std::string, std::vector<std::string> > dataMap0 = dataMaps[0];
    std::map<std::string, std::vector<std::string> > dataMap1 = dataMaps[1];

    vec.clear();

    int NumRecords = int(dataMap0["SpeName"].size());
    if (NumRows != NumRecords) {
        msg = "\nnmfDatabase::loadVector: NumSpecies (" + std::to_string(NumRows) +
              ") != NumRecords (" +
                std::to_string(NumRecords) + ")";
        Logger->logMsg(nmfConstants::Error,msg);
        retv = false;
    } else {
        m = 0;
        for (int species=0; species<NumRows; ++species) {
            vec.push_back((std::stod(dataMap1["Value"][m]) +
                           std::stod(dataMap0["Value"][m]))/2.0);
            ++m;
        }
    }
    return retv;
}

bool
nmfDatabase::loadMatrix(
        nmfLogger* Logger,
        const std::vector<std::map<std::string, std::vector<std::string> > >& dataMaps,
        const int& NumRows,
        const int& NumCols,
        boost::numeric::ublas::matrix<double>& matrix)
{
    int m=0;
    bool retv = true;
    std::string msg;
    std::map<std::string, std::vector<std::string> > dataMap0 = dataMaps[0];
    std::map<std::string, std::vector<std::string> > dataMap1 = dataMaps[1];

    nmfUtils::initialize(matrix,NumRows,NumCols);

    int NumRecords = int(dataMap0["SpeciesA"].size());
    if (NumRows*NumCols != NumRecords) {
        msg = "\nnmfDatabase::loadMatrix: NumSpecies*NumSpecies (" + std::to_string(NumRows) +
                "x" + std::to_string(NumCols) + ") != NumRecords (" +
                std::to_string(NumRecords) + ")";
        Logger->logMsg(nmfConstants::Error,msg);
        retv = false;
    } else {
        m = 0;
        for (int speciesA=0; speciesA<NumRows; ++speciesA) {
            for (int speciesB=0; speciesB<NumCols; ++speciesB) {
                matrix(speciesA,speciesB) = (std::stod(dataMap1["Value"][m]) +
                                             std::stod(dataMap0["Value"][m]))/2.0;
                ++m;
            }
        }
    }
    return retv;
}


bool
nmfDatabase::getGuildData(nmfLogger* Logger,
                          const int& NumGuilds,
                          const int& RunLength,
                          const QStringList& GuildList,
                          std::map<int,std::vector<int> >& GuildSpecies,
                          std::vector<int>& GuildNum,
                          boost::numeric::ublas::matrix<double>& ObservedBiomassByGuilds)
{
    int NumSpecies;
    int guildNum;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string guildName;
    std::map<std::string,double> InitialGuildBiomass;
    std::map<std::string,int> GuildMap;
    std::string msg;

    GuildSpecies.clear();
    GuildNum.clear();
    InitialGuildBiomass.clear();
    ObservedBiomassByGuilds.clear();

    fields    = {"GuildName","GuildK"};
    queryStr  = "SELECT GuildName,GuildK FROM " +
                 nmfConstantsMSSPM::TableGuilds +
                " ORDER by GuildName";
    dataMap   = nmfQueryDatabase(queryStr, fields);
    for (int i=0; i<NumGuilds; ++i) {
        guildName = dataMap["GuildName"][i];
        GuildMap[guildName] = i;
    }

    // Load Growth Rate Min and Max
    fields     = {"SpeName","GuildName","InitBiomass"};
    queryStr   = "SELECT SpeName,GuildName,InitBiomass FROM " +
                  nmfConstantsMSSPM::TableSpecies +
                 " ORDER BY SpeName";
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumSpecies = dataMap["SpeName"].size();
    if (NumSpecies == 0) {
        msg = "\nnmfDatabase::getGuildData: NumSpecies = 0)";
        Logger->logMsg(nmfConstants::Error,msg);
        return false;
    }

    for (int species=0; species<NumSpecies; ++species) {
        guildName = dataMap["GuildName"][species];
        guildNum  = GuildMap[guildName];
        InitialGuildBiomass[guildName] += std::stod(dataMap["InitBiomass"][species]);
        GuildSpecies[guildNum].push_back(species);
        GuildNum.push_back(guildNum);
    }

    nmfUtils::initialize(ObservedBiomassByGuilds,RunLength+1,NumGuilds);
    for (int i=0; i<NumGuilds; ++i) {
       ObservedBiomassByGuilds(0,i) = InitialGuildBiomass[GuildList[i].toStdString()];
    }

    return true;
}


bool
nmfDatabase::getPredationData(const std::string& PredationForm,
                              nmfLogger* Logger,
                              const std::string& ProjectName,
                              const std::string& ModelName,
                              const int& NumSpecies,
                              boost::numeric::ublas::matrix<double>& Rho,
                              boost::numeric::ublas::matrix<double>& Handling,
                              std::vector<double>& Exponent)
{
    bool ok;
    bool retv = true;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMapMin;
    std::map<std::string, std::vector<std::string> > dataMapMax;
    std::string queryStr;
    std::vector<std::map<std::string, std::vector<std::string> > > dataMaps = {dataMapMin,dataMapMax};
    std::vector<std::string> rhoFilenames      = {nmfConstantsMSSPM::TablePredationRhoMin,      nmfConstantsMSSPM::TablePredationRhoMax};
    std::vector<std::string> handlingFilenames = {nmfConstantsMSSPM::TablePredationHandlingMin, nmfConstantsMSSPM::TablePredationHandlingMax};
    std::vector<std::string> exponentFilenames = {nmfConstantsMSSPM::TablePredationExponentMin, nmfConstantsMSSPM::TablePredationExponentMax};

    if (PredationForm == "Null") {
        return true;
    }

    fields   = {"ProjectName","ModelName","SpeciesA","SpeciesB","Value"};

    // Calculate the Rho regardless of the Predation type
    for (int i=0; i<int(rhoFilenames.size()); ++i) {
        queryStr = "SELECT ProjectName,ModelName,SpeciesA,SpeciesB,Value FROM " +
                    rhoFilenames[i]  +
                   " WHERE ProjectName = '" + ProjectName +
                   "' AND  ModelName = '"   + ModelName   +
                   "' ORDER BY SpeciesA,SpeciesB ";
        dataMaps[i] = nmfQueryDatabase(queryStr, fields);
    }
    ok = loadMatrix(Logger,dataMaps,NumSpecies,NumSpecies,Rho);
    if (! ok) {
        return false;
    }

    // Calculate Handling if need be
    if ((PredationForm == "Type II") || (PredationForm == "Type III")) {
        for (int i=0; i<int(handlingFilenames.size()); ++i) {
            queryStr = "SELECT ModelName,SpeciesA,SpeciesB,Value FROM " +
                        handlingFilenames[i]  +
                       " WHERE ProjectName = '" + ProjectName +
                       "' AND ModelName = '"    + ModelName +
                       "' ORDER BY SpeciesA,SpeciesB ";
            dataMaps[i] = nmfQueryDatabase(queryStr, fields);
        }
        ok = loadMatrix(Logger,dataMaps,NumSpecies,NumSpecies,Handling);
        if (! ok) {
            return false;
        }
    }

    // Calculate the Exponent if need be
    if (PredationForm == "Type III") {
        for (int i=0; i<int(exponentFilenames.size()); ++i) {
            fields   = {"ModelName","SpeName","Value"};
            queryStr = "SELECT ModelName,SpeName,Value FROM " +
                        exponentFilenames[i]  +
                       " WHERE ProjectName = '" + ProjectName +
                       "' AND ModelName = '"    + ModelName +
                       "' ORDER BY SpeName ";
            dataMaps[i] = nmfQueryDatabase(queryStr, fields);
        }
        ok = loadVector(Logger,dataMaps,NumSpecies,Exponent);
        if (! ok) {
            return false;
        }
    }

    return retv;
}

bool
nmfDatabase::getCompetitionData(const std::string& CompetitionType,
                                nmfLogger* Logger,
                                const std::string& ProjectName,
                                const std::string& ModelName,
                                const int& NumSpecies,
                                const int& NumGuilds,
                                boost::numeric::ublas::matrix<double>& CompetitionAlpha,
                                boost::numeric::ublas::matrix<double>& CompetitionBetaSpecies,
                                boost::numeric::ublas::matrix<double>& CompetitionBetaGuild,
                                boost::numeric::ublas::matrix<double>& CompetitionBetaGuildGuild)
{
    bool ok;
    bool retv = true;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMapMin;
    std::map<std::string, std::vector<std::string> > dataMapMax;
    std::string queryStr;
    std::vector<std::map<std::string, std::vector<std::string> > > dataMaps = {dataMapMin,dataMapMax};
    std::vector<std::string> AlphaFilenames            = {nmfConstantsMSSPM::TableCompetitionAlphaMin,           nmfConstantsMSSPM::TableCompetitionAlphaMax};
    std::vector<std::string> BetaSpeciesFilenames      = {nmfConstantsMSSPM::TableCompetitionBetaSpeciesMin,     nmfConstantsMSSPM::TableCompetitionBetaSpeciesMax};
    std::vector<std::string> BetaGuildsFilenames       = {nmfConstantsMSSPM::TableCompetitionBetaGuildsMin,      nmfConstantsMSSPM::TableCompetitionBetaGuildsMin};
    std::vector<std::string> BetaGuildsGuildsFilenames = {nmfConstantsMSSPM::TableCompetitionBetaGuildsGuildsMin,nmfConstantsMSSPM::TableCompetitionBetaGuildsGuildsMax};

    if (CompetitionType == "Null") {
        return true;
    }

    if (CompetitionType == "NO_K") {

        // AlphaFilenames
        fields   = {"ProjectName","ModelName","SpeciesA","SpeciesB","Value"};
        for (int i=0; i<int(AlphaFilenames.size()); ++i) {
            queryStr = "SELECT ProjectName,ModelName,SpeciesA,SpeciesB,Value FROM " +
                        AlphaFilenames[i]  +
                       " WHERE ProjectName = '" + ProjectName +
                       "' AND ModelName = '"    + ModelName +
                       "' ORDER BY SpeciesA,SpeciesB ";
            dataMaps[i] = nmfQueryDatabase(queryStr, fields);
        }
        ok = loadMatrix(Logger,dataMaps,NumSpecies,NumSpecies,CompetitionAlpha);
        if (! ok) {
            retv = false;
        }

    } else if (CompetitionType == "MS-PROD") {

        // BetaSpeciesFilenames
        fields   = {"ProjectName","ModelName","SpeciesA","SpeciesB","Value"};
        for (int i=0; i<int(BetaSpeciesFilenames.size()); ++i) {
            queryStr = "SELECT ProjectName,ModelName,SpeciesA,SpeciesB,Value FROM " +
                        BetaSpeciesFilenames[i]  +
                       " WHERE ProjectName = '" + ProjectName +
                       "' AND ModelName = '"    + ModelName +
                       "' ORDER BY SpeciesA,SpeciesB ";
            dataMaps[i] = nmfQueryDatabase(queryStr, fields);
        }
        ok = loadMatrix(Logger,dataMaps,NumSpecies,NumSpecies,CompetitionBetaSpecies);
        if (! ok) {
            retv = false;
        }

        // BetaGuildsFilenames
        fields   = {"ProjectName","ModelName","Guild","SpeName","Value"};
        for (int i=0; i<int(BetaGuildsFilenames.size()); ++i) {
            queryStr = "SELECT ProjectName,ModelName,Guild,SpeName,Value FROM " +
                        BetaGuildsFilenames[i]  +
                       " WHERE ProjectName = '" + ProjectName +
                       "' AND ModelName = '"    + ModelName +
                       "' ORDER BY Guild,SpeName ";
            dataMaps[i] = nmfQueryDatabase(queryStr, fields);
        }
        ok = loadMatrix(Logger,dataMaps,NumSpecies,NumGuilds,CompetitionBetaGuild);
        if (! ok) {
            retv = false;
        }

    } else if (CompetitionType == "AGG-PROD") {

        // BetaGuildsGuildsFilenames
        fields   = {"ProjectName","ModelName","GuildA","GuildB","Value"};
        for (int i=0; i<int(BetaGuildsGuildsFilenames.size()); ++i) {
            queryStr = "SELECT ProjectName,ModelName,GuildA,GuildB,Value FROM " +
                        BetaGuildsGuildsFilenames[i]  +
                       " WHERE ProjectName = '" + ProjectName +
                       "' AND ModelName = '"    + ModelName +
                       "' ORDER BY GuildA,GuildB ";
            dataMaps[i] = nmfQueryDatabase(queryStr, fields);
        }
        ok = loadMatrix(Logger,dataMaps,NumGuilds,NumGuilds,CompetitionBetaGuildGuild);
        if (! ok) {
            retv = false;
        }
    }

    return retv;
}

bool
nmfDatabase::getSpeciesInitialCovariateData(nmfLogger* Logger,
                                            std::string& ProjectName,
                                            std::string& ModelName,
                                            std::vector<double>& GrowthRateCovariateCoeffs,
                                            std::vector<double>& CarryingCapacityCovariateCoeffs,
                                            std::vector<double>& CatchabilityCovariateCoeffs)
{
    int NumRecords;
    int NumSpecies;
    double covariateInitialValue = 0.0;
    std::string CoeffName;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string tableName = nmfConstantsMSSPM::TableCovariateInitialValuesAndRanges;
    std::vector<nmfStructsQt::CovariateStruct> covariateRangeVector;
    QStringList SpeciesList;

    // Get data from database table
    fields     = {"ProjectName","ModelName","SpeName",
                  "CoeffName","CoeffMinName","CoeffMaxName",
                  "CoeffValue","CoeffMinValue","CoeffMaxValue"};
    queryStr   = "SELECT ProjectName,ModelName,SpeName,CoeffName,CoeffMinName,CoeffMaxName,CoeffValue,CoeffMinValue,CoeffMaxValue FROM " +
                  tableName +
                 " WHERE ProjectName = '" + ProjectName +
                 "' AND ModelName = '"    + ModelName +
                 "' ORDER BY SpeName ";
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["CoeffName"].size();
    if (NumRecords == 0) {  // No covariate coefficient data so just fill with 0's
       getSpecies(Logger,NumSpecies,SpeciesList);
       for (int i=0; i<NumSpecies; ++i) {
           GrowthRateCovariateCoeffs.push_back(0);
           CarryingCapacityCovariateCoeffs.push_back(0);
           CatchabilityCovariateCoeffs.push_back(0);
       }
       return true;
    }

    GrowthRateCovariateCoeffs.clear();
    CarryingCapacityCovariateCoeffs.clear();
    for (int i=0; i<NumRecords; ++i) {
        CoeffName = dataMap["CoeffName"][i];
        if (CoeffName == "GrowthRate") {
            covariateInitialValue = (dataMap["CoeffValue"][i].empty()) ?
                 0 : QString::fromStdString(dataMap["CoeffValue"][i]).toDouble();
            GrowthRateCovariateCoeffs.push_back(covariateInitialValue);
        } else if (CoeffName == "CarryingCapacity") {
            covariateInitialValue = (dataMap["CoeffValue"][i].empty()) ?
                 0 : QString::fromStdString(dataMap["CoeffValue"][i]).toDouble();
            CarryingCapacityCovariateCoeffs.push_back(covariateInitialValue);
        } else if (CoeffName == "Catchability") {
            covariateInitialValue = (dataMap["CoeffValue"][i].empty()) ?
                 0 : QString::fromStdString(dataMap["CoeffValue"][i]).toDouble();
            CatchabilityCovariateCoeffs.push_back(covariateInitialValue);
        }
    }

    return true;
}

bool
nmfDatabase::getSpeciesInitialData(nmfLogger* Logger,
                                   int& NumSpecies,
                                   QStringList& SpeciesList,
                                   std::vector<double>& InitBiomass,
                                   std::vector<double>& GrowthRate,
                                   std::vector<double>& SpeciesK)
{
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    SpeciesList.clear();
    InitBiomass.clear();
    GrowthRate.clear();
    SpeciesK.clear();

    fields   = {"SpeName","InitBiomass","GrowthRate","SpeciesK"};
    queryStr = "SELECT SpeName,InitBiomass,GrowthRate,SpeciesK from " +
                nmfConstantsMSSPM::TableSpecies +
               " ORDER BY SpeName";
    dataMap  = nmfQueryDatabase(queryStr, fields);
    NumSpecies = dataMap["SpeName"].size();
    if (NumSpecies == 0) {
        Logger->logMsg(nmfConstants::Error,"[Error 1] getSpeciesInitialData: No species found in table Species");
        return false;
    }

    for (int species=0; species<NumSpecies; ++species) {
        SpeciesList << QString::fromStdString(dataMap["SpeName"][species]);
        InitBiomass.push_back(std::stod(dataMap["InitBiomass"][species]));
        GrowthRate.push_back(std::stod(dataMap["GrowthRate"][species]));
        SpeciesK.push_back(std::stod(dataMap["SpeciesK"][species]));
    }

    return true;
}

bool
nmfDatabase::clearTable(nmfLogger* Logger,
                        const std::string& TableName)
{
    std::string cmd      = "truncate table " + TableName;
    std::string errorMsg = nmfUpdateDatabase(cmd);
    if (nmfUtilsQt::isAnError(errorMsg)) {
        Logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::clearTable: truncate error: " + errorMsg);
        Logger->logMsg(nmfConstants::Error,"cmd: " + cmd);
        return false;
    }
    return true;
}

bool
nmfDatabase::clearTable(nmfLogger* Logger,
                        const std::string& TableName,
                        const std::string& ProjectName)
{
    std::string cmd = "DELETE FROM " + TableName +
                      " WHERE ProjectName = '" + ProjectName + "'";
    std::string errorMsg = nmfUpdateDatabase(cmd);
    if (nmfUtilsQt::isAnError(errorMsg)) {
        Logger->logMsg(nmfConstants::Error,"[Error 2] nmfDatabase::clearTable: truncate error: " + errorMsg);
        Logger->logMsg(nmfConstants::Error,"cmd: " + cmd);
        return false;
    }
    return true;
}

bool
nmfDatabase::getModelFormData(nmfLogger*   Logger,
                              const std::string& ProjectName,
                              const std::string& ModelName,
                              std::string& GrowthForm,
                              std::string& HarvestForm,
                              std::string& CompetitionForm,
                              std::string& PredationForm,
                              int&         RunLength,
                              int&         InitialYear,
                              bool&        isBiomassAbsolute)
{
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap,dataMapMin,dataMapMax;
    std::string queryStr;

    // Find model forms
    fields     = {"GrowthForm","HarvestForm","WithinGuildCompetitionForm","PredationForm","RunLength","StartYear","ObsBiomassType"};
    queryStr   = "SELECT GrowthForm,HarvestForm,WithinGuildCompetitionForm,PredationForm,RunLength,StartYear,ObsBiomassType FROM " +
                  nmfConstantsMSSPM::TableModels +
                 " WHERE ProjectName = '" + ProjectName +
                 "' AND ModelName = '"    + ModelName   + "'";
    dataMap    = nmfQueryDatabase(queryStr, fields);
    if (dataMap["GrowthForm"].size() == 0) {
        std::string msg = "[Error 1] getModelFormData: No Form data found in Models table for ModelName = " + ModelName;
        Logger->logMsg(nmfConstants::Error,msg);
        return false;
    }
    GrowthForm        = dataMap["GrowthForm"][0];
    HarvestForm       = dataMap["HarvestForm"][0];
    CompetitionForm   = dataMap["WithinGuildCompetitionForm"][0];
    PredationForm     = dataMap["PredationForm"][0];
    RunLength         = std::stoi(dataMap["RunLength"][0]);
    InitialYear       = std::stoi(dataMap["StartYear"][0]);
    isBiomassAbsolute = (dataMap["ObsBiomassType"][0] == "Absolute");

    return true;
}

QStringList
nmfDatabase::getVectorParameterNames(
        nmfLogger*   logger,
        const std::string& ProjectName,
        const std::string& ModelName)
{
    int NumRecords;
    std::vector<std::string> fields;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;

    QStringList parameterNames = nmfConstantsMSSPM::VectorParameterNames;

    // Get Model structure data
    fields     = {"ProjectName","ModelName","ObsBiomassType","GrowthForm","HarvestForm","WithinGuildCompetitionForm","PredationForm"};
    queryStr   = "SELECT ProjectName,ModelName,ObsBiomassType,GrowthForm,HarvestForm,WithinGuildCompetitionForm,PredationForm FROM " +
                  nmfConstantsMSSPM::TableModels +
                 " WHERE ProjectName = '" + ProjectName +
                 "' AND ModelName = '"    + ModelName   + "'";
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["ModelName"].size();
    if (NumRecords == 0) {
        logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::callback_UpdateDiagnosticParameterChoices: No records found in table Models for ModelName = "+ModelName);
        return {};
    }

    // Check for appropriate items in parameter combo boxes.
    if (dataMap["ObsBiomassType"][0] != "Relative") {
        parameterNames.removeAll("SurveyQ");
    }
    if (dataMap["GrowthForm"][0] != "Logistic") {
        parameterNames.removeAll("Carrying Capacity (K)");
    }
    if (dataMap["GrowthForm"][0] == "Null") {
        parameterNames.removeAll("Growth Rate (r)");
    }
    if (dataMap["HarvestForm"][0] != "Effort (qE)") {
        parameterNames.removeAll("Catchability (q)");
    }

    return parameterNames;
}

void
nmfDatabase::loadEstimatedVectorParameters(
        nmfLogger*   logger,
        const std::string& ProjectName,
        const std::string& ModelName,
        QComboBox*   cmbox)
{
    int index;
    int NumRecords;
    std::vector<std::string> fields;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;

    // Get Model structure data
    fields     = {"ProjectName","ModelName","ObsBiomassType","GrowthForm","HarvestForm","WithinGuildCompetitionForm","PredationForm"};
    queryStr   = "SELECT ProjectName,ModelName,ObsBiomassType,GrowthForm,HarvestForm,WithinGuildCompetitionForm,PredationForm FROM " +
                  nmfConstantsMSSPM::TableModels +
                 " WHERE ProjectName = '" + ProjectName +
                 "' AND ModelName = '"    + ModelName   + "'";
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["ModelName"].size();
    if (NumRecords == 0) {
        logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::callback_UpdateDiagnosticParameterChoices: No records found in table Models for ModelName = "+ModelName);
        return;
    }

    // Figure out which items should be in the pulldown lists based upon the Model structure
    cmbox->blockSignals(true);
    cmbox->clear();
    cmbox->addItems(nmfConstantsMSSPM::VectorParameterNames);

    // Check for appropriate items in parameter combo boxes.
    if (dataMap["ObsBiomassType"][0] != "Relative") {
        index = cmbox->findText("SurveyQ");
        cmbox->removeItem(index);
    }
    if (dataMap["GrowthForm"][0] != "Logistic") {
        index = cmbox->findText("Carrying Capacity (K)");
        cmbox->removeItem(index);
    }
    if (dataMap["GrowthForm"][0] == "Null") {
        index = cmbox->findText("Growth Rate (r)");
        cmbox->removeItem(index);
    }
    if (dataMap["HarvestForm"][0] != "Effort (qE)") {
        index = cmbox->findText("Catchability (q)");
        cmbox->removeItem(index);
    }
    cmbox->blockSignals(false);
}



bool
nmfDatabase::getHarvestDataByGuild(
        nmfLogger* logger,
        const std::string& projectName,
        const std::string& modelName,
        const QString& outputChartType,
        const int& numSpeciesOrGuilds,
        std::string& chartLabel,
        boost::numeric::ublas::matrix<double>& OutputBiomass,
        boost::numeric::ublas::matrix<double>& harvestData)
{
    int RunLength;
    std::string HarvestForm;

    getHarvestFormData(logger,projectName,modelName,RunLength,HarvestForm);
    chartLabel = HarvestForm;

    // Get appropriate Harvest data (i.e., Catch or Effort)
    if (HarvestForm == nmfConstantsMSSPM::HarvestCatch.toStdString()) {
        if (! getTimeSeriesDataByGuild(logger,projectName,modelName,"",
                                       nmfConstantsMSSPM::TableHarvestCatch,
                                       numSpeciesOrGuilds,RunLength,
                                       outputChartType,OutputBiomass,harvestData)) {
            return false;
        }
    } else if (HarvestForm == nmfConstantsMSSPM::HarvestEffort.toStdString()) {
        if (! getTimeSeriesDataByGuild(logger,projectName,modelName,"",
                                       nmfConstantsMSSPM::TableHarvestEffort,
                                       numSpeciesOrGuilds,RunLength,
                                       outputChartType,OutputBiomass,harvestData)) {
            return false;
        }

    } else if (HarvestForm == nmfConstantsMSSPM::HarvestExploitation.toStdString()) {
        if (! getTimeSeriesDataByGuild(logger,projectName,modelName,"",
                                       nmfConstantsMSSPM::TableHarvestExploitation,
                                       numSpeciesOrGuilds,RunLength,
                                       outputChartType,OutputBiomass,harvestData)) {
            return false;
        }
    } else {
        return false;
    }


    return true;
}

bool
nmfDatabase::getHarvestFormData(
        nmfLogger* logger,
        const std::string& projectName,
        const std::string& modelName,
        int& RunLength,
        std::string& HarvestForm)
{
    int NumRecords;
    std::vector<std::string> fields;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;

    // Get needed data from Models table
    fields     = {"RunLength","HarvestForm"};
    queryStr   = "SELECT RunLength,HarvestForm FROM " +
                  nmfConstantsMSSPM::TableModels +
                 " WHERE ProjectName = '" + projectName +
                 "' AND ModelName = '"    + modelName   + "'";
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["RunLength"].size();
    if (NumRecords == 0) {
        logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::getHarvestData: No records found in table Models for ModelName = "+modelName);
        return false;
    }
    RunLength   = std::stoi(dataMap["RunLength"][0]);
    HarvestForm = dataMap["HarvestForm"][0];

    return true;
}

bool
nmfDatabase::getSurveyQData(
        nmfLogger* logger,
        std::vector<double>& SurveyQ)
{
    int NumRecords;
    std::vector<std::string> fields;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;

    // Get needed data from Models table
    fields     = {"SurveyQ"};
    queryStr   = "SELECT SurveyQ FROM " +
                  nmfConstantsMSSPM::TableSpecies;
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SurveyQ"].size();
    if (NumRecords == 0) {
        logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::getSurveyQData: No records found in table: species");
        return false;
    }
    for (int i=0; i<NumRecords; ++i) {
        SurveyQ.push_back(std::stod(dataMap["SurveyQ"][i]));
    }

    return true;
}

bool
nmfDatabase::getEstimatedBiomass(
        const int&         NumSpecies,
        const int&         RunLength,
        const std::string& TableName,
        const std::string& ProjectName,
        const std::string& ModelName,
        const std::string& Algorithm,
        const std::string& Minimizer,
        const std::string& ObjectiveCriterion,
        const std::string& Scaling,
        const std::string& isAggProd,
        boost::numeric::ublas::matrix<double>& EstBiomass)
{
    int NumRecords;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    EstBiomass.clear();
    nmfUtils::initialize(EstBiomass,RunLength+1,NumSpecies);

    fields   = {"ProjectName","ModelName","Algorithm","Minimizer","ObjectiveCriterion","Scaling","isAggProd","SpeName","Year","Value"};
    queryStr = "SELECT ProjectName,ModelName,Algorithm,Minimizer,ObjectiveCriterion,Scaling,isAggProd,SpeName,Year,Value FROM " +
                TableName +
               " WHERE ProjectName = '"       + ProjectName +
               "' AND ModelName = '"          + ModelName +
               "' AND Algorithm = '"          + Algorithm   +
               "' AND Minimizer = '"          + Minimizer +
               "' AND ObjectiveCriterion = '" + ObjectiveCriterion +
               "' AND Scaling = '"            + Scaling +
               "' AND isAggProd = "           + isAggProd +
               " ORDER BY SpeName,Year ";
    dataMap  = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if ((NumRecords == 0) ||
        (NumRecords != NumSpecies*(RunLength+1))) {
        std::cout << queryStr << std::endl;
        return false;
    }

    int m = 0;
    for (int i=0; i<NumSpecies; ++i) {
        for (int j=0; j<=RunLength; ++j) {
            EstBiomass(j,i) = std::stod(dataMap["Value"][m++]);
        }
    }

    return true;
}


bool
nmfDatabase::getEstimatedParameter(
        const std::string& TableName,
        const std::string& ProjectName,
        const std::string& ModelName,
        const std::string& Algorithm,
        const std::string& Minimizer,
        const std::string& ObjectiveCriterion,
        const std::string& Scaling,
        const std::string& isAggProd,
        std::vector<double>& EstParameter)
{
    int NumRecords;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;

    EstParameter.clear();

    fields   = {"ProjectName","ModelName","Algorithm","Minimizer","ObjectiveCriterion","Scaling","isAggProd","SpeName","Value"};
    queryStr = "SELECT ProjectName,ModelName,Algorithm,Minimizer,ObjectiveCriterion,Scaling,isAggProd,SpeName,Value FROM " +
                TableName +
               " WHERE ProjectName = '"       + ProjectName +
               "' AND ModelName = '"          + ModelName +
               "' AND Algorithm = '"          + Algorithm +
               "' AND Minimizer = '"          + Minimizer +
               "' AND ObjectiveCriterion = '" + ObjectiveCriterion +
               "' AND Scaling = '"            + Scaling +
               "' AND isAggProd = "           + isAggProd +
               " ORDER BY SpeName ";

    dataMap  = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        return false;
    }

    for (int i=0; i<NumRecords; ++i) {
        EstParameter.push_back(std::stod(dataMap["Value"][i]));
    }

    return true;
}

bool
nmfDatabase::getHarvestData(const std::string& HarvestType,
                            nmfLogger* Logger,
                            const std::string& ProjectName,
                            const std::string& ModelName,
                            const int& NumSpecies,
                            const int& RunLength,
                            boost::numeric::ublas::matrix<double>& Catch,
                            boost::numeric::ublas::matrix<double>& Effort,
                            boost::numeric::ublas::matrix<double>& Exploitation,
                            std::vector<double>& Catchability)
{
    bool retv = true;
    int m;
    int NumRecords;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string msg;
    std::string HarvestTypePrefix = "harvest";
    std::string HarvestTableName;

    // Because type could be: "Effort (qE)" or "Exploitation (F)"
    HarvestTableName = HarvestTypePrefix + QString::fromStdString(HarvestType).toLower().split(" ")[0].toStdString();

    fields   = {"ProjectName","ModelName","SpeName","Year","Value"};
    queryStr = "SELECT ProjectName,ModelName,SpeName,Year,Value FROM " +
                HarvestTableName +
               " WHERE ProjectName = '" + ProjectName +
               "' AND ModelName = '"    + ModelName +
               "' ORDER BY SpeName,Year ";
    dataMap  = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumSpecies*RunLength != NumRecords) {
        msg = "\nnmfDatabase::getHarvestData: NumSpecies*RunLength (" + std::to_string(NumSpecies) +
                "x" + std::to_string(RunLength) + ") != NumRecords (" +
                std::to_string(NumRecords) + ")";
        Logger->logMsg(nmfConstants::Error,msg);
        retv = false;
    } else {
        m = 0;
        for (int species=0; species<NumSpecies; ++species) {
            for (int year=0; year<RunLength; ++year) {
                if (HarvestTableName == nmfConstantsMSSPM::TableHarvestCatch) {
                    Catch(year,species) = std::stod(dataMap["Value"][m++]);
                } else if (HarvestTableName == nmfConstantsMSSPM::TableHarvestEffort) {
                    Effort(year,species) = std::stod(dataMap["Value"][m++]);
                } else if (HarvestTableName == nmfConstantsMSSPM::TableHarvestExploitation) {
                    Exploitation(year,species) = std::stod(dataMap["Value"][m++]);
                }
            }
        }
    }

    if (HarvestTableName == nmfConstantsMSSPM::TableHarvestEffort) {
        fields   = {"SpeName","Catchability"};
        queryStr = "SELECT SpeName,Catchability FROM " +
                    nmfConstantsMSSPM::TableSpecies +
                    " ORDER BY SpeName ";
        dataMap  = nmfQueryDatabase(queryStr, fields);
        NumRecords = dataMap["SpeName"].size();
        if (NumSpecies != NumRecords) {
            msg = "\nnmfDatabase::getHarvestData: NumSpecies (" + std::to_string(NumSpecies) +
                    ") != NumRecords (" + std::to_string(NumRecords) + ")";
            Logger->logMsg(nmfConstants::Error,msg);
            retv = false;
        } else {
            m = 0;
            for (int species=0; species<NumSpecies; ++species) {
                    Catchability.push_back(std::stod(dataMap["Catchability"][m++]));
            }
        }
    }

    return retv;
}

bool
nmfDatabase::getHarvestData(
        QWidget* parent,
        nmfLogger* logger,
        const std::string& projectName,
        const std::string& modelName,
        const int& numSpeciesOrGuilds,
        const bool& divideByOutputBiomass,
        const bool& multiplyByOutputBiomass,
        const bool& multiplyByCatchability,
        const std::vector<double>& EstCatchability,
        const std::vector<boost::numeric::ublas::matrix<double> >& OutputBiomass,
        boost::numeric::ublas::matrix<double>& harvestData)
{
    int RunLength;
    std::string HarvestForm;

    getHarvestFormData(logger,projectName,modelName,RunLength,HarvestForm);

    // Get appropriate Harvest data (i.e., Catch or Effort)
    if (HarvestForm == nmfConstantsMSSPM::HarvestCatch.toStdString()) {
        if (! getTimeSeriesData(parent,logger,projectName,modelName,
                                "",nmfConstantsMSSPM::TableHarvestCatch,
                                numSpeciesOrGuilds,RunLength,harvestData)) {
            return false;
        }
        if (divideByOutputBiomass) {
            for (int species=0; species<(int)harvestData.size2(); ++species) {
                for (int time=0; time<(int)harvestData.size1(); ++time) {
                        harvestData(time,species) /= OutputBiomass[0](time,species);
                }
            }
        }
    } else if (HarvestForm == nmfConstantsMSSPM::HarvestEffort.toStdString()) {
        if (! getTimeSeriesData(parent,logger,projectName,modelName,
                                "",nmfConstantsMSSPM::TableHarvestEffort,
                                numSpeciesOrGuilds,RunLength,harvestData)) {
            return false;
        }
        boost::numeric::ublas::matrix<double> tmpHarvestData;
        nmfUtils::initialize(tmpHarvestData,harvestData.size1(),harvestData.size2());

        // Multiply the Effort data by the estimated catchability and output biomass data
        double catchability = 0;
        for (int species=0; species<(int)harvestData.size2(); ++species) {
            catchability = EstCatchability[species];
            for (int time=0; time<(int)harvestData.size1(); ++time) {
                if (multiplyByCatchability && multiplyByOutputBiomass) {
                    tmpHarvestData(time,species) += harvestData(time,species) * catchability * OutputBiomass[0](time,species);
                } else if (multiplyByCatchability && (! multiplyByOutputBiomass)) {
                    tmpHarvestData(time,species) += harvestData(time,species) * catchability;
                }
            }
        }
        harvestData = tmpHarvestData;
    } else {
        return false;
    }

    return true;
}

std::string
nmfDatabase::getCovariateAlgorithmType(
        nmfLogger* logger,
        const std::string& projectName,
        const std::string& modelName)
{
    int NumRecords;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string covariateAlgorithmType = "";
    std::string msg;

    fields   = {"CovariateAlgorithmType"};
    queryStr = "SELECT CovariateAlgorithmType FROM " +
                nmfConstantsMSSPM::TableModels +
                " WHERE ProjectName = '" + projectName +
                "' AND ModelName = '" + modelName + "'";
    dataMap  = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["CovariateAlgorithmType"].size();
    if (NumRecords == 1) {
        covariateAlgorithmType = dataMap["CovariateAlgorithmType"][0];
    } else {
        if (logger != nullptr) {
            msg = "nmfDatabase::getCovariateAlgorithmType: Found number of records = " +
                   QString::number(NumRecords).toStdString();
            logger->logMsg(nmfConstants::Error,msg);
            logger->logMsg(nmfConstants::Error,queryStr);
        }
    }

    return covariateAlgorithmType;
}

double
nmfDatabase::checkForValues(
            const int& index,
            const int& numValues,
            const std::vector<double>& vec)
{
    double retv = 0;
    if (int(vec.size()) >= numValues) {
        retv = vec[index];
    }
    return retv;
}

void
nmfDatabase::updateUnitsTable(
        QWidget* parent,
        nmfLogger* logger,
        const std::string& projectName,
        const std::string& modelName,
        const std::string& tableName,
        const std::string& units)
{
    std::string cmd;
    std::string errorMsg;

    // Delete existing units record
    cmd = "DELETE FROM " + nmfConstantsMSSPM::TableUnits +
          " WHERE ProjectName = '" + projectName +
          "' AND ModelName = '" + modelName +
          "' AND TableName = '" + tableName + "'";
    errorMsg = nmfUpdateDatabase(cmd);
    if (nmfUtilsQt::isAnError(errorMsg)) {
        logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::updateUnitsTable: DELETE error: " + errorMsg);
        logger->logMsg(nmfConstants::Error,"cmd: " + cmd);
        QMessageBox::warning(parent, "Error",
                             "\nError in Save command. Couldn't delete record from units table.\n",
                             QMessageBox::Ok);
        parent->setCursor(Qt::ArrowCursor);
        return;
    }

    // Add current units
    cmd  = "INSERT INTO " + nmfConstantsMSSPM::TableUnits +
           " (ProjectName,ModelName,TableName,Units) VALUES ('" +
            projectName + "','" +
            modelName + "','" +
            tableName + "','" +
            units + "')";
    errorMsg = nmfUpdateDatabase(cmd);
    if (nmfUtilsQt::isAnError(errorMsg)) {
        logger->logMsg(nmfConstants::Error,"[Error 1] nmfDatabase::updateUnitsTable: Write table error: " + errorMsg);
        logger->logMsg(nmfConstants::Error,"cmd: " + cmd);
        QMessageBox::warning(parent,"Warning",
                             "\nCouldn't INSERT INTO units table.\n",
                             QMessageBox::Ok);
        return;
    }
}
