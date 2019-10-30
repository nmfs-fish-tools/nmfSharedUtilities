// new code
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


/**
 * @mainpage Overview
 *
 * @section intro Introduction
 *
 * This is the introduction.
 *
 * @section Background
 *
 * This is some background.
 *
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


nmfDatabase::nmfDatabase() {

    db = QSqlDatabase::addDatabase("QMYSQL");

    FunctionMap["ForeEnergyDens"]       = createForeEnergyDens;       //  1 of 39
    FunctionMap["ForeOutput"]           = createForeOutput;           //  2 of 39
    FunctionMap["ForePredGrowth"]       = createForePredGrowth;       //  3 of 39
    FunctionMap["ForePredVonB"]         = createForePredVonB;         //  4 of 39
    FunctionMap["ForeSRQ"]              = createForeSRQ;              //  5 of 39
    FunctionMap["ForeSRR"]              = createForeSRR;              //  6 of 39
    FunctionMap["ForeSuitPreyBiomass"]  = createForeSuitPreyBiomass;  //  7 of 39
    FunctionMap["Forecasts"]            = createForecasts;            //  8 of 39
    FunctionMap["MSVPAEnergyDens"]      = createMSVPAEnergyDens;      //  9 of 39
    FunctionMap["MSVPAOthPrey"]         = createMSVPAOthPrey;         // 10 of 39
    FunctionMap["MSVPAOthPreyAnn"]      = createMSVPAOthPreyAnn;      // 11 of 39
    FunctionMap["MSVPASeasBiomass"]     = createMSVPASeasBiomass;     // 12 of 39
    FunctionMap["MSVPASeasInfo"]        = createMSVPASeasInfo;        // 13 of 39
    FunctionMap["MSVPASizePref"]        = createMSVPASizePref;        // 14 of 39
    FunctionMap["MSVPASpaceO"]          = createMSVPASpaceO;          // 15 of 39
    FunctionMap["MSVPAStomCont"]        = createMSVPAStomCont;        // 16 of 39
    FunctionMap["MSVPASuitPreyBiomass"] = createMSVPASuitPreyBiomass; // 17 of 39
    FunctionMap["MSVPAlist"]            = createMSVPAlist;            // 18 of 39
    FunctionMap["MSVPAprefs"]           = createMSVPAprefs;           // 19 of 39
    FunctionMap["MSVPAspecies"]         = createMSVPAspecies;         // 20 of 39
    FunctionMap["OthPredSizeData"]      = createOthPredSizeData;      // 21 of 39
    FunctionMap["OtherPredBM"]          = createOtherPredBM;          // 22 of 39
    FunctionMap["OtherPredSpecies"]     = createOtherPredSpecies;     // 23 of 39
    FunctionMap["SSVPAAgeM"]            = createSSVPAAgeM;            // 24 of 39
    FunctionMap["ScenarioF"]            = createScenarioF;            // 25 of 39
    FunctionMap["ScenarioOthPred"]      = createScenarioOthPred;      // 26 of 39
    FunctionMap["ScenarioOthPrey"]      = createScenarioOthPrey;      // 27 of 39
    FunctionMap["ScenarioRec"]          = createScenarioRec;          // 28 of 39
    FunctionMap["Scenarios"]            = createScenarios;            // 29 of 39
    FunctionMap["SpeCatch"]             = createSpeCatch;             // 30 of 39
    FunctionMap["SpeMaturity"]          = createSpeMaturity;          // 31 of 39
    FunctionMap["SpeSSVPA"]             = createSpeSSVPA;             // 32 of 39
    FunctionMap["SpeSize"]              = createSpeSize;              // 33 of 39
    FunctionMap["SpeTuneCatch"]         = createSpeTuneCatch;         // 34 of 39
    FunctionMap["SpeTuneEffort"]        = createSpeTuneEffort;        // 35 of 39
    FunctionMap["SpeWeight"]            = createSpeWeight;            // 36 of 39
    FunctionMap["SpeXSAData"]           = createSpeXSAData;           // 37 of 39
    FunctionMap["SpeXSAIndices"]        = createSpeXSAIndices;        // 38 of 39
    FunctionMap["Species"]              = createSpecies;              // 39 of 39
}

void
nmfDatabase::nmfSetDatabase(std::string newDatabaseName)
{
    db.setDatabaseName(newDatabaseName.c_str());

    bool dbOpenOK = db.open();
    if (! dbOpenOK) {
       std::cout << "Error: Couldn't open database: " << newDatabaseName << std::endl;
    }
}

std::string
nmfDatabase::nmfLastError()
{
    return db.lastError().text().toStdString();
}


bool
nmfDatabase::nmfOpenDatabase(
        const std::string& hostName,
        const std::string& userName,
        const std::string& password,
        std::string&       errorMsg)
{
    errorMsg.clear();
    db.setHostName(hostName.c_str());
    db.setUserName(userName.c_str());
    db.setPassword(password.c_str());

    bool dbOpenOK = db.open();
    if (! dbOpenOK) {
        errorMsg = db.lastError().text().toStdString();
       //std::cout << "Error: Couldn't open database." << std::endl;
       return false;
    }
    return true;
    //nmfStartTransaction();
}

std::vector<std::string>
nmfDatabase::nmfGetDatabaseNames()
{
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
nmfDatabase::nmfCloseDatabase() {
    //db.rollback();
    //db.commit();

    QString dbToClose = db.databaseName();
std::cout << "Trying to close: " << dbToClose.toStdString() << std::endl;
    db.close();
    QSqlDatabase::removeDatabase(dbToClose);
}

void
nmfDatabase::nmfSaveDatabase() {
    db.commit();
}

void
nmfDatabase::nmfStartTransaction() {
    bool supportsTransactions = db.transaction();
    if (!supportsTransactions) {
        std::cout <<
            "\nWarning: Database doesn't support transactions. " <<
            "All modifications will happen as you modify the database. " <<
            "\n         Change database format to InnoDB. Inspect with MySql command: show table status;\n"
            << std::endl;
    }
}


// Very slow
std::string
nmfDatabase::nmfUpdateDatabase(std::string qry)
{
//std::cout << "Updating database..." << std::endl;

    //nmfStartTransaction();

    QSqlQuery retv = db.exec(QString::fromStdString(qry));

    //nmfSaveDatabase();

    return retv.lastError().text().toStdString();
}


bool
nmfDatabase::nmfDeleteRecordsFromTable(std::string table, std::string MSVPAName)
{
    std::cout << "Deleting records from table: " << table << std::endl;

    std::string queryStr;
    std::string errorMsg;
    bool ok=true;

    //nmfStartTransaction();

    queryStr = "DELETE FROM " + table + " WHERE MSVPAname=\""+MSVPAName+"\";";
    errorMsg = nmfUpdateDatabase(queryStr);
    if ( errorMsg != " " ) {
        nmfUtils::printError(queryStr,errorMsg);
        ok = false;
    }

    //nmfSaveDatabase();

    return ok;
}

bool
nmfDatabase::nmfGetInitialData(std::string MSVPAName,
            int &NPreds, int &NPrey, int &NOtherFood, int &NOtherPreds,
            int &FirstYear, int &LastYear, int &NSeas, int &NMSVPASpe, int &NYears,
            std::string &FirstYearStr, std::string &LastYearStr)
{
    bool retv=true;
    std::vector<std::string> fields;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;

    fields = {"MSVPAName","NSpe","NPreyOnly","NOther","NOtherPred","FirstYear","LastYear","NSeasons"};
    queryStr = "SELECT MSVPAName,NSpe,NPreyOnly,NOther,NOtherPred,FirstYear,LastYear,NSeasons FROM MSVPAlist WHERE MSVPAName = '"
                + MSVPAName + "'";
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
nmfDatabase::nmfGetMSVPAInitData(std::string &MSVPAName,
   int &MSVPA_FirstYear,
   int &MSVPA_LastYear,
   int &MSVPA_NYears,
   int &MSVPA_NSeasons)
{
    std::map<std::string,int> initMap = nmfQueryInitFields("MSVPAlist", MSVPAName);
    MSVPA_FirstYear = initMap["FirstYear"];
    MSVPA_LastYear  = initMap["LastYear"];
    MSVPA_NYears    = MSVPA_LastYear - MSVPA_FirstYear + 1;
    MSVPA_NSeasons  = initMap["NSeasons"];
} // end getMSVPAInitData


void
nmfDatabase::getSpeciesToIndexMap(std::map<std::string,int> &SpeciesToIndexMap)
{
    std::vector<std::string> fields;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;

    // Get list of all Species
    fields   = {"SpeName","SpeIndex"};
    queryStr = "SELECT Spename,SpeIndex FROM Species";
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
    if ( errorMsg != " " ) {
        nmfUtils::printError(queryStr,errorMsg);
        ok = false;
    }

    //nmfSaveDatabase();

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
        const std::string FirstCatchAgeStr,
        const std::string LastCatchAgeStr,
        const std::string OrderByStr,
        boost::numeric::ublas::matrix<double> &Catch)
{
    int k=0;
    std::vector<std::string> fields = {"Catch"};
    std::string queryStr = "SELECT Catch FROM " + table + " WHERE SpeIndex = " + std::to_string(speciesIndex) +
                           " AND Age >= " + FirstCatchAgeStr + " and Age <= " + LastCatchAgeStr + OrderByStr;
    std::map< std::string, std::vector<std::string> > dataMap;
    dataMap = nmfQueryDatabase(queryStr, fields, nmfConstants::nullAsZero);

    for (unsigned int i = 0; i < Catch.size1(); ++i) {
        for (unsigned int j = 0; j < Catch.size2(); ++j) {
            Catch(i,j) = std::stod(dataMap[fields[0]][k++]);
        }
    }
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
    std::string queryStr = "SELECT FirstYear,LastYear,NSeasons,GrowthModel FROM " + table + " WHERE MSVPAName='" +
                           MSVPAName + "'";
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
      errorMsg = db.lastError().text().toStdString();
      if (errorMsg.empty())
          std::cout << "Error: Check for loaded database." << std::endl;
      else if (errorMsg != " ")
          std::cout << "Error: " << errorMsg << std::endl;
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
        const int &i,
        boost::numeric::ublas::matrix<double> &WtAtAge)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    // Get Wt At Age...wts at beginning of year
    fields = {"Year","Age","InitWt"};
    queryStr = "SELECT Year, Age, InitWt FROM ForeOutput WHERE MSVPAname = '" + MSVPAName +
            "' AND ForeName = '" + ForecastName +
            "' AND Scenario = '" + ScenarioName +
            "' AND SpeName = '" + species +
            "' AND Year = " + std::to_string(Year) +
            "  AND Season = 0 ORDER By Age";
    dataMap = nmfQueryDatabase(queryStr, fields);
    for (int j=0; j<Nage; ++j) {
        WtAtAge(i,j) = std::stod(dataMap["InitWt"][j]);
    }

} // end nmfQueryForecastWeightAtAgeData



void
nmfDatabase::nmfQueryWeightAtAgeData(
        const int &index,
        const int &Year,
        const int &Nage,
        const std::string &species,
        boost::numeric::ublas::matrix<double> &WtAtAge)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    // Get Wt at Age
    fields = {"Value"};
    queryStr = "SELECT Value FROM SpeWeight WHERE SpeName = '" + species + "' " +
            " AND Year = " + std::to_string(Year) +
            " AND Variable = 'Weight' " +
            " ORDER BY Age";
    dataMap = nmfQueryDatabase(queryStr, fields);
    for (int j=0; j<Nage; ++j) {
        WtAtAge(index,j) = std::stod(dataMap["Value"][j]);
    }

} // end nmfQueryWeightAtAgeData

void
nmfDatabase::nmfQueryForecastMortalityData(
        const std::string &MSVPAName,
        const std::string &ForecastName,
        const std::string &ScenarioName,
        const std::string &species,
        const int &Year,
        const int &Nage,
        const int &i,
        boost::numeric::ublas::matrix<double> &FatAge,
        boost::numeric::ublas::matrix<double> &M1atAge,
        boost::numeric::ublas::matrix<double> &M2atAge)
{
    std::string M2Str;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    fields = {"Year","Age","F","M2","M1"};
    queryStr = "SELECT Year, Age, Sum(SeasF) as F, Sum(SeasM2) as M2, Sum(SeasM1) as M1  FROM ForeOutput WHERE MSVPAname = '" + MSVPAName +
            "' AND ForeName = '" + ForecastName +
            "' AND Scenario = '" + ScenarioName +
            "' AND SpeName = '" + species +
            "' AND Year = " + std::to_string(Year) +
            "  GROUP BY Year, Age";
    dataMap = nmfQueryDatabase(queryStr, fields);
    for (int j=0; j<Nage; ++j) {
        FatAge(i,j)  = std::stod(dataMap["F"][j]);
        M1atAge(i,j) = std::stod(dataMap["M1"][j]);
        M2Str = dataMap["M2"][j];
        M2atAge(i,j) = M2Str.empty() ? 0 : std::stod(M2Str);
    }

} // end nmfQueryForecastMortalityData


void
nmfDatabase::nmfQueryMortalityData(
        const std::string &MSVPAName,
        const int &i,
        const int &Year,
        const int &Nage,
        const std::string &species,
        boost::numeric::ublas::matrix<double> &FatAge,
        boost::numeric::ublas::matrix<double> &M1atAge,
        boost::numeric::ublas::matrix<double> &M2atAge)
{
    std::string M2Str;
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    fields = {"Age","F","M2","M1"};
    queryStr = "SELECT Age, Sum(SeasF) as F, Sum(SeasM2) as M2, Sum(SeasM1) as M1 FROM MSVPASeasBiomass WHERE MSVPAName = '" + MSVPAName +
            "' AND SpeName = '" + species +
            "' AND Year = " + std::to_string(Year) +
            " GROUP BY Age";
//std::cout << queryStr << std::endl;

    dataMap = nmfQueryDatabase(queryStr, fields);
    for (int j=0; j<Nage; ++j) {
        FatAge(i,j)  = std::stod(dataMap["F"][j]);
        M1atAge(i,j) = std::stod(dataMap["M1"][j]);
        M2Str = dataMap["M2"][j];
        M2atAge(i,j) = M2Str.empty() ? 0 : std::stod(M2Str);
    }

} // end nmfQueryWeightAtAgeData


bool
nmfDatabase::getMortalityData(
        nmfLogger*   logger,
        const std::string& ProjectSettingsConfig,
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

    fields     = {"SystemName","SpeName","Segment","ColName","Value"};
    queryStr   = "SELECT SystemName,SpeName,Segment,ColName,Value FROM " + TableName;
    queryStr  += " WHERE SystemName = '" + ProjectSettingsConfig + "'";
    queryStr  += " AND SpeName = '" + Species + "'";
    queryStr  += " ORDER BY Segment";
    dataMap    = nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        msg = "getMortalityData: No records found in table: " + TableName;
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
    queryStr = "SELECT Year, Age, Sum(SeasCatch) as Catch FROM ForeOutput WHERE MSVPAname = '" + MSVPAName +
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
    queryStr = "SELECT Age,Catch FROM SpeCatch WHERE SpeName = '" + species + "' " +
            " AND Year = " + std::to_string(Year) +
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
    queryStr = "SELECT AgeM1,AgeM2 FROM SSVPAAgeM WHERE SpeIndex = " + std::to_string(SpeciesIndex) +
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
        const int &i,
        const int &Year,
        const int &Nage,
        const std::string &species,
        boost::numeric::ublas::matrix<double> &Pmature)
{
    std::string queryStr;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::vector<std::string> fields;

    fields = {"PMature"};
    queryStr = "SELECT PMature FROM SpeMaturity WHERE SpeName = '" + species + "' " +
            " AND Year = " + std::to_string(Year) +
            " ORDER BY Age";
//std::cout << queryStr << std::endl;
    dataMap = nmfQueryDatabase(queryStr, fields);
    for (int j=0; j<Nage; ++j) {
        Pmature(i,j) = std::stod(dataMap["PMature"][j]);
    }

} // end nmfQueryMaturityData


void
nmfDatabase::nmfQueryForecastPreyList(
        const std::string &MSVPAName,
        const std::string &ForecastName,
        const std::string &ScenarioName,
        const std::string &Species,
        const bool includePredAge,
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
    queryStr = "SELECT DISTINCT PreyName FROM ForeSuitPreyBiomass WHERE MSVPAname = '" + MSVPAName + "'" +
            " AND ForeName = '" + ForecastName + "'" +
            " AND Scenario = '" + ScenarioName + "'" +
            " AND PredName = '" + Species + "'" +
            predAgeClause + seasonStr +
            " ORDER BY PreyName";
    dataMap = nmfQueryDatabase(queryStr, fields);

    for (unsigned int i = 0; i < dataMap["PreyName"].size(); ++i) {
        PreyList << QString::fromStdString(dataMap["PreyName"][i]);
    }
} // end nmfQueryForecastPreyList


void
nmfDatabase::nmfQueryMsvpaPreyList(
        std::string selectedSpeciesAgeSizeClass,
        std::string selectedSpecies,
        std::string MSVPAName,
        std::string &PredAge,
        int &NPrey,
        QStringList &PreyList,
        bool includedPredAge)
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

    queryStr = "SELECT DISTINCT PreyName FROM MSVPASuitPreyBiomass WHERE PredName='" + selectedSpecies +
            "' and MSVPAName='" + MSVPAName + "'" +
            predAgeClause + " ORDER by PreyName";

    dataMap = nmfQueryDatabase(queryStr, fields);
    NPrey = dataMap["PreyName"].size();
    for (int i=0; i<NPrey; ++i) {
        PreyList << QString::fromStdString(dataMap["PreyName"][i]);
    }

} // end nmfQueryMsvpaPreyList


// -------------------------- MSSPM -----------------------

/*
 *
 * Create a map of Scenario names and Forecast Labels that belong to each Scenario.
 *
 */
void
nmfDatabase::createScenarioMap(std::map<QString,QStringList>& ScenarioForecastMap)
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
    queryStr = "SELECT DISTINCT ScenarioName FROM ForecastBiomassMultiScenario";
    dataMapScenario = nmfQueryDatabase(queryStr, fields);
    for (unsigned i=0; i<dataMapScenario["ScenarioName"].size(); ++i) {
        ScenarioName = dataMapScenario["ScenarioName"][i];
        fields    = {"SortOrder","ForecastLabel"};
        queryStr  = "SELECT DISTINCT SortOrder,ForecastLabel FROM ForecastBiomassMultiScenario";
        queryStr += " WHERE ScenarioName = '" + ScenarioName + "' ORDER BY SortOrder,ForecastLabel";
        dataMapForecast = nmfQueryDatabase(queryStr, fields);
        tmpList.clear();
        for (unsigned j=0; j<dataMapForecast["ForecastLabel"].size(); ++j) {
            tmpList << QString::fromStdString(dataMapForecast["ForecastLabel"][j]);
        }
        ScenarioForecastMap[QString::fromStdString(ScenarioName)] = tmpList;
    }
}

// -------------------------- General -----------------------


bool
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
    bool okToRead=true;
    QProcess *myProcess;
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
    if (InputFileName.isEmpty())
        return false;
    QString fileDatabaseName = QFileInfo(InputFileName).baseName();

    // Does database already exist?
    bool databaseAlreadyExists = false;
    std::vector<std::string> fields = { "Database" };
    std::string queryStr = "SHOW databases";
    std::map<std::string, std::vector<std::string> > dataMap;
    dataMap = nmfQueryDatabase(queryStr, fields);
    for (unsigned int i = 0; i < dataMap["Database"].size(); ++i) {
        existingDatabase = QString::fromStdString(dataMap["Database"][i]);
        if (fileDatabaseName == existingDatabase) {
            databaseAlreadyExists = true;
            break;
        }
    }
    QApplication::flush();

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
            return false;
    } else {
        // create the database
        cmd = "CREATE DATABASE " + fileDatabaseName;
        errorMsg = nmfUpdateDatabase(cmd.toStdString());
        if (errorMsg != " ") {
            logger->logMsg(nmfConstants::Error,"Error: nmfUtilsQt::importDatabase: "+errorMsg);
            return false;
        }
    }

    // Done with all checks so go ahead with the mysql import.
    if (okToRead) {
        args << "-u" + QString::fromStdString(Username)
             << "-p" + QString::fromStdString(Password)
             << fileDatabaseName;
        widget->setCursor(Qt::WaitCursor);
        myProcess = new QProcess(0);
        myProcess->setStandardInputFile(InputFileName);
        myProcess->start("mysql", args);
        if (! myProcess->waitForFinished(-1)) { // -1 so it won't timeout
            myProcess->kill();
            delete myProcess;
        }
        widget->setCursor(Qt::ArrowCursor);
    }

    return true;
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
    QProcess *myProcess;
    bool okToWrite = false;

    // Get the databases full path. Create it if it doesn't yet exist.
    QString databaseDir = QDir(ProjectDir.c_str()).filePath("databases");
    if (! QDir(databaseDir).exists()) {
        QDir().mkpath(databaseDir);
    }

    // Show file dialog and have user enter in the output .sql file name.
    QString selFilter = "Database Files (*.sql)";
    QString OutputFileName = QFileDialog::getSaveFileName(widget,
        "Export Database",
        databaseDir.toLatin1(),
        "*.sql",
        &selFilter,
        QFileDialog::DontConfirmOverwrite);
    if (OutputFileName.isEmpty())
        return;
    QApplication::flush();

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
        widget->setCursor(Qt::WaitCursor);
        args << "-u" + QString::fromStdString(Username)
             << "-p" + QString::fromStdString(Password)
             << QString::fromStdString(ProjectDatabase);
        myProcess = new QProcess(0);
        myProcess->setStandardOutputFile(OutputFileName);
        myProcess->start("mysqldump", args);
        if (! myProcess->waitForFinished(-1)) { // -1 so it won't timeout
            myProcess->kill();
            delete myProcess;
        }
        widget->setCursor(Qt::ArrowCursor);
    }

}



