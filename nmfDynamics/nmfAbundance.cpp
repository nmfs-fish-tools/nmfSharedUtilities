

#include "nmfAbundance.h"
#include "nmfConstantsMSCAA.h"
#include "nmfConstants.h"



nmfAbundance::nmfAbundance(
        nmfDatabase* databasePtr,
        nmfLogger*   logger,
        std::map<std::string,boost::numeric::ublas::matrix<double> >& Abundance,
        std::map<std::string,std::vector<double> >& Recruitment,
        std::map<std::string,std::vector<double> >& SpawningStockBiomass,
        std::map<std::string,boost::numeric::ublas::matrix<double> >& NaturalMortality,
        std::map<std::string,boost::numeric::ublas::matrix<double> >& FishingMortality,
        std::map<std::string,boost::numeric::ublas::matrix<double> >& PredationMortality)
{
//std::cout << "nmfAbundance::nmfAbundance" << std::endl;
    m_databasePtr = databasePtr;
    m_logger      = logger;
    m_nuOther     = 0;
    m_ProjectSettingsConfig.clear();
    m_PreferredRatioEta.clear();
    m_PreferredGTRatio.clear();
    m_PreferredLTRatio.clear();
    m_VulnerabilityRho.clear();
    m_InitialAbundance.clear();
    Abundance.clear();
    m_ConsumptionBiomassRatio.clear();
    m_Maturity.clear();
    m_NaturalMortality.clear();
    m_FishingMortality.clear();
    m_PredationMortality.clear();
    m_FisheryCatch.clear();

    ReadSettings();

    int NumYears;
    int NumAges;
    int MinAge,MaxAge,FirstYear,LastYear,NumLengthBins;
    float MinLength,MaxLength;
    std::vector<std::string> allSpecies;


    if (! getSystemData("System",m_SystemStruct)) {
        m_logger->logMsg(nmfConstants::Warning,"No System data found.");
        return;
    }

    if (! getPredatorPreyData("PredatorPreyPreferredRatio", m_PreferredRatioEta)) {
        m_logger->logMsg(nmfConstants::Warning,"No Predator Prey Preferred Ratio data found.");
        return;
    }

    if (! getPredatorPreyData("PredatorPreyVarianceLTRatio",m_PreferredLTRatio)) {
        m_logger->logMsg(nmfConstants::Warning,"No Predator Prey Variance < η data found.");
        return;
    }

    if (! getPredatorPreyData("PredatorPreyVarianceGTRatio",m_PreferredGTRatio)) {
        m_logger->logMsg(nmfConstants::Warning,"No Predator Prey Variance > η data found.");
        return;
    }

    if (! getPredatorPreyData("PredatorPreyVulnerability",m_VulnerabilityRho)) {
        m_logger->logMsg(nmfConstants::Warning,"No Predator Prey Vulnerability found.");
        return;
    }

    // Get all the species names
    m_databasePtr->getAllSpecies(m_logger,allSpecies);

    // Initialize weight, initial abundance, consumption, and maturity
    for (std::string species : allSpecies) {
        m_databasePtr->getSpeciesData(m_logger,species,
                                      MinAge,MaxAge,FirstYear,LastYear,
                                      MinLength,MaxLength,NumLengthBins);
        NumYears = LastYear - FirstYear + 1;
        NumAges  = MaxAge - MinAge + 1;

        getYearAgeData(  species,NumYears,NumAges,"Weight",          m_WeightMap);
        getYearAgeData(  species,NumYears,NumAges,"InitialAbundance",m_InitialAbundance);
        getYearAgeData(  species,NumYears,NumAges,"Consumption",     m_ConsumptionBiomassRatio);
        getYearAgeData(  species,NumYears,NumAges,"Maturity",        m_Maturity);
        getMortalityData(species,NumYears,NumAges,"MortalityNatural",m_NaturalMortality);
        getMortalityData(species,NumYears,NumAges,"MortalityFishing",m_FishingMortality);
        getFisheryCatch( species,NumYears,NumAges,"CatchFishery",    m_FisheryCatch);

        // Store initial abundance into Abundance as well as mortalities
        Abundance[species]        = m_InitialAbundance[species];
        NaturalMortality[species] = m_NaturalMortality[species];
        FishingMortality[species] = m_FishingMortality[species];
    }
}

void
nmfAbundance::getFisheryCatch(const std::string &Species,
                              const int &NumYears,
                              const int &NumAges,
                              const std::string TableName,
                              std::map<std::string,boost::numeric::ublas::matrix<double> >& TableData)
{
    if (! getFleetCatchTotals(Species,NumYears,NumAges,TableName,TableData)) {
        m_logger->logMsg(nmfConstants::Warning,"No " + TableName + " Year-Age data found.");
        return;
    }
}

bool
nmfAbundance::getFleetCatchTotals(const std::string &Species,
                                  const int &NumYears,
                                  const int &NumAges,
                                  const std::string &TableName,
                                  std::map<std::string,boost::numeric::ublas::matrix<double> >& m_FisheryCatch)
{
    int m;
    int NumRecords;
    int NumFleets;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string msg;
    std::string units;
    double sf = 1.0;
    boost::numeric::ublas::matrix<double> tempMatrix;

    // Find number of fleets for the species
    fields = {"NumFleets"};
    queryStr  = "SELECT COUNT(DISTINCT(Fleet)) AS NumFleets FROM " + TableName;
    queryStr += " WHERE SystemName = '" + m_ProjectSettingsConfig + "'";
    queryStr += " AND SpeName = '" + Species + "'";
    dataMap   = m_databasePtr->nmfQueryDatabase(queryStr, fields);
    NumFleets = std::stoi(dataMap["NumFleets"][0]);

    // Sum up yearly totals per fleet per age
    fields     = {"SystemName","SpeName","Fleet","Year","Age","Value","Units"};
    queryStr   = "SELECT SystemName,SpeName,Fleet,Year,Age,Value,Units FROM " + TableName;
    queryStr  += " WHERE SystemName = '" + m_ProjectSettingsConfig + "'";
    queryStr  += " AND SpeName = '" + Species + "'";
    queryStr  += " ORDER BY Fleet,Year,Age";
    dataMap    = m_databasePtr->nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();

    if (NumFleets == 0) {
        msg = "ByYear: No fleets found in table: " + TableName;
        m_logger->logMsg(nmfConstants::Error,msg);
        return false;
    }
    if (NumRecords == 0) {
        msg = "ByYear: No records found in table: " + TableName;
        m_logger->logMsg(nmfConstants::Error,msg);
        return false;
    }
    if (NumRecords != NumFleets*NumYears*NumAges) {
        msg  = "ByYear: Incorrect number of records found in table: " + TableName + "\n";
        msg += "Found " + std::to_string(NumRecords) + " records.\n";
        msg += "Expecting NumFleets*NumYears*NumAges: " + std::to_string(NumFleets*NumYears*NumAges);
        m_logger->logMsg(nmfConstants::Error,msg);
        return false;
    }
    nmfUtils::initialize(tempMatrix,NumYears,NumAges);

    // Get scalefactor from units.  Put values into units of fish.
    units = dataMap["Units"][0];
    if (units == "000 Fish")
        sf = 0.001;
    else if (units == "000 000 Fish")
        sf = 0.000001;

    m = 0;
    for (int Fleet = 0; Fleet < NumFleets; ++Fleet) {
        for (int Year = 0; Year < NumYears; ++Year) {
            for (int Age = 0; Age < NumAges; ++Age) {
                tempMatrix(Year,Age) += sf*std::stod(dataMap["Value"][m++]);
            }
        }
    }
    m_FisheryCatch[Species] = tempMatrix;

    return true;
}

bool
nmfAbundance::getSpeciesParameters(
        const std::string TableName,
        std::string RecruitmentType,
        std::map<std::string,double>& Alpha,
        std::map<std::string,double>& Beta,
        std::map<std::string,double>& Gamma)
{
    int m;
    int NumRecords;
    int NumSpecies;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string msg;
    std::string species;
    std::vector<std::string> ParameterNames = {"alpha","beta","gamma"};
    int NumParameters = ParameterNames.size();

    fields     = {"SystemName","Algorithm","SpeName","ParameterName","Value"};
    queryStr   = "SELECT SystemName,Algorithm,SpeName,ParameterName,Value FROM " + TableName;
    queryStr  += " WHERE SystemName = '" + m_ProjectSettingsConfig + "'";
//    queryStr  += " AND SpeName = '" + Species + "'";
    queryStr  += " AND Algorithm = '" + RecruitmentType + "'";
    queryStr  += " ORDER BY ParameterName";
    dataMap    = m_databasePtr->nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        msg = "getSpeciesParameters: No records found in table: " + TableName;
        m_logger->logMsg(nmfConstants::Error,msg);
        return false;
    }
    NumSpecies = NumRecords/NumParameters;

    m = 0;
    for (int i=0; i<NumSpecies; ++i) {
        species = dataMap["SpeName"][m];
        Alpha[species] = std::stod(dataMap["Value"][m++]);
    }
    for (int i=0; i<NumSpecies; ++i) {
        species = dataMap["SpeName"][m];
        Beta[species] = std::stod(dataMap["Value"][m++]);
    }
    for (int i=0; i<NumSpecies; ++i) {
        species = dataMap["SpeName"][m];
        Gamma[species] = std::stod(dataMap["Value"][m++]);
    }

    return true;
}

void
nmfAbundance::getMortalityData(const std::string& species,
                               const int& numYears,
                               const int& numAges,
                               const std::string tableName,
                               std::map<std::string,boost::numeric::ublas::matrix<double> >& tableMap)
{
    boost::numeric::ublas::matrix<double> tempMatrix;

    if (! m_databasePtr->getMortalityData(
                m_logger,m_ProjectSettingsConfig,species,
                numYears,numAges,tableName,tempMatrix)) {
        m_logger->logMsg(nmfConstants::Warning,"No " + tableName + " data found.");
        return;
    }
    tableMap[species] = tempMatrix;
}

void
nmfAbundance::getYearAgeData(const std::string& species,
                             const int& numYears,
                             const int& numAges,
                             const std::string tableName,
                             std::map<std::string,boost::numeric::ublas::matrix<double> >& tableMap)
{
    boost::numeric::ublas::matrix<double> tempMatrix;

    if (! getDatabaseData(species,numYears,numAges,tableName, tempMatrix)) {
        m_logger->logMsg(nmfConstants::Warning,"No " + tableName + " Year-Age data found for Species: "+species);
        return;
    }
    tableMap[species] = tempMatrix;
//    if (tableName == "InitialAbundance") {
//        m_Abundance[species] = tempMatrix;
//qDebug() << "Loaded abundance for :" << QString::fromStdString(species)
//         << ", size1 and 2: " << m_Abundance[species].size1() <<
//            m_Abundance[species].size2();
//    }
}

void
nmfAbundance::ReadSettings()
{
    QSettings settings("NOAA", "MSCAA");

    settings.beginGroup("Settings");
    m_ProjectSettingsConfig = settings.value("Name","").toString().toStdString();
    settings.endGroup();
}

/*
bool
nmfAbundance::getTheMortalityData(
        const std::string &Species,
        const int &NumYears,
        const int &NumAges,
        const std::string &TableName,
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
    queryStr  += " WHERE SystemName = '" + m_ProjectSettingsConfig + "'";
    queryStr  += " AND SpeName = '" + Species + "'";
    queryStr  += " ORDER BY Segment";
    dataMap    = m_databasePtr->nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        msg = "getMortalityData: No records found in table: " + TableName;
        m_logger->logMsg(nmfConstants::Error,msg);
        m_logger->logMsg(nmfConstants::Error,queryStr);
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
*/


bool
nmfAbundance::getPredatorPreyData(
        std::string TableName,
        boost::numeric::ublas::matrix<double>& TableData)
{
    int m;
    int NumRecords;
    int NumSpecies;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string msg;

    fields     = {"SystemName","PredatorName","PreyName","Value"};
    queryStr   = "SELECT SystemName,PredatorName,PreyName,Value FROM " + TableName;
    queryStr  += " WHERE SystemName = '" + m_ProjectSettingsConfig + "'";
    queryStr  += " ORDER BY PredatorName,PreyName";
    dataMap    = m_databasePtr->nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["PredatorName"].size();
    if (NumRecords == 0) {
        msg = "getPredatorPreyData: No records found in table: " + TableName;
        m_logger->logMsg(nmfConstants::Error,msg);
        return false;
    }
    NumSpecies = std::sqrt(NumRecords);
    nmfUtils::initialize(TableData,NumSpecies,NumSpecies);

    m = 0;
    while (m < NumRecords)
    {
        for (int row = 0; row < NumSpecies; ++row)
        {
            for (int col = 0; col < NumSpecies; ++col) {
                TableData(row,col) = std::stod(dataMap["Value"][m++]);
std::cout << "rho: " << row << "," << col << ": " << TableData(row,col) << std::endl;
            }
        }
    }

    return true;
}

bool
nmfAbundance::getSystemData(
        const std::string&  TableName,
              AbundanceSystemStruct& SystemData)
{
    int NumRecords;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string msg;

    fields     = {"SystemName","TotalBiomass","FH_FirstYear","FH_LastYear","NumSpInter","AbundanceDriver"};
    queryStr   = "SELECT SystemName,TotalBiomass,FH_FirstYear,FH_LastYear,NumSpInter,AbundanceDriver FROM " + TableName;
    queryStr  += " WHERE SystemName = '" + m_ProjectSettingsConfig + "'";
    dataMap    = m_databasePtr->nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["TotalBiomass"].size();
    if (NumRecords == 0) {
        msg = "nmfAbundance::getSystemData: No records found in table: " + TableName;
        m_logger->logMsg(nmfConstants::Error,msg);
        return false;
    }

    QString totalBiomass = QString::fromStdString(dataMap["TotalBiomass"][0]);
    SystemData.SystemName      = dataMap["SystemName"][0];
    SystemData.FH_FirstYear    = std::stoi(dataMap["FH_FirstYear"][0]);
    SystemData.FH_LastYear     = std::stoi(dataMap["FH_LastYear"][0]);
    SystemData.NumSpInter      = std::stoi(dataMap["NumSpInter"][0]);
    SystemData.AbundanceDriver = dataMap["AbundanceDriver"][0];

    // Convert total system biomass to metric tons
    QStringList parts = totalBiomass.split('_');
    SystemData.TotalBiomass = parts[0].toDouble();
    if (parts[1] == "000 Metric Tons") {
        SystemData.TotalBiomass *= 1000;
    } else if (parts[1] == "000 000 Metric Tons") {
        SystemData.TotalBiomass *= 1000000;
    }

}




bool
nmfAbundance::getDatabaseData(
        const std::string &Species,
        const int &NumYears,
        const int &NumAges,
        const std::string &TableName,
        boost::numeric::ublas::matrix<double> &TableData)
{
    int m;
    int NumYears2;
    int NumRecords;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string msg;
    double sf = 1.0;

    nmfUtils::initialize(TableData,NumYears,NumAges);
    NumYears2 = (TableName == "InitialAbundance") ? 1 : NumYears;

    fields     = {"SystemName","SpeName","Year","Age","Value","Units"};
    queryStr   = "SELECT SystemName,SpeName,Year,Age,Value,Units FROM " + TableName;
    queryStr  += " WHERE SpeName = '"  + Species + "'";
    queryStr  += " AND SystemName = '" + m_ProjectSettingsConfig + "'";
    queryStr  += " ORDER BY Year,Age";
    dataMap    = m_databasePtr->nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        msg = "Error: nmfAbundance::getDatabaseData: No records found in table: " + TableName;
        m_logger->logMsg(nmfConstants::Error,msg);
        return false;
    }
    if (NumRecords != NumYears2*NumAges) {
        msg = "Error: nmfAbundance::getDatabaseData: Incorrect number of records found in table: " + TableName + "\n";
        msg += "Found " + std::to_string(NumRecords) + " records.\n";
        msg += "Calculated NumYears2*NumAges (" +
                std::to_string(NumYears2) + "*" +
                std::to_string(NumAges) + "=" +
                std::to_string(NumYears2*NumAges) + ") records.";
        m_logger->logMsg(nmfConstants::Error,msg);
        return false;
    }

    // Scale to kilograms if Weight table
    if (TableName == "Weight") {
        if (dataMap["Units"][0] == "Grams") {
            sf = 1000.0;
        }
    }

    m = 0;
    for (int year = 0; year < NumYears2; ++year) {
        for (int age = 0; age < NumAges; ++age) {
            TableData(year,age) = sf*std::stod(dataMap["Value"][m++]);
        }
    }
    return true;
}


void
nmfAbundance::calculateSizePreference(
        const std::vector<std::string>& AllSpecies,
        const boost::numeric::ublas::matrix<double>& PreferredRatioEta,
        const boost::numeric::ublas::matrix<double>& PreferredLTRatio,
        const boost::numeric::ublas::matrix<double>& PreferredGTRatio,
        Boost5DArrayDouble& SizePreferenceG)
{
    int predNum = -1;
    int preyNum = -1;
    double weightFactor;
    double variance;
    double weightRatio;
    double eta;

    // RSK - This assumes all species have same num years!!
    for (int year = 0; year < m_WeightMap[AllSpecies[0]].size1(); ++year) {
        preyNum = -1;
        for (std::string preySpecies : AllSpecies) {
            ++preyNum;
            for (int preyAge = 0; preyAge < m_WeightMap[preySpecies].size2(); ++preyAge) {
                predNum = -1;
                for (std::string predSpecies : AllSpecies) {
                    ++predNum;
                    for (int predAge = 0; predAge < m_WeightMap[predSpecies].size2(); ++predAge) {
                        weightRatio  = m_WeightMap[predSpecies](year,predAge) /
                                       m_WeightMap[preySpecies](year,preyAge);
                        eta = PreferredRatioEta(predNum,preyNum);
                        weightFactor = std::log(weightRatio) - eta;
                        variance = (weightRatio < eta) ? PreferredLTRatio(predNum,preyNum) :
                                                         PreferredGTRatio(predNum,preyNum);

                        SizePreferenceG[preyNum][preyAge][predNum][predAge][year] =
                                (variance == 0) ? 0 : std::exp((-1.0/(2.0*variance*variance)) *
                                                      weightFactor*weightFactor);
                    }
                }
            }
        }
    }

}

/*
 *
 * Eq'n (5) of Curtis' Dissertation:
 *
 * ρ = general vulnerability
 * g = size preference
 *
 * ν(i,a,j,b,t) = ρ(i,j)⋅g(i,a,j,b,t)
 *
 */
void
nmfAbundance::calculateSuitability(
        const std::vector<std::string>& AllSpecies,
        const boost::numeric::ublas::matrix<double>& VulnerabilityRho,
        Boost5DArrayDouble& SizePreferenceG,
        Boost5DArrayDouble& VulnerabilityNu)
{
    int predNum;
    int preyNum;

    for (int year = 0; year < m_WeightMap[AllSpecies[0]].size1(); ++year) {
        preyNum = -1;
        for (std::string preySpecies : AllSpecies) {
            ++preyNum;
            for (int preyAge = 0; preyAge < m_WeightMap[preySpecies].size2(); ++preyAge) {
                predNum = -1;
                for (std::string predSpecies : AllSpecies) {
                    ++predNum;
                    for (int predAge = 0; predAge < m_WeightMap[predSpecies].size2(); ++predAge) {
                        VulnerabilityNu[preyNum][preyAge][predNum][predAge][year] =
                                VulnerabilityRho(preyNum,predNum) *
                                SizePreferenceG[preyNum][preyAge][predNum][predAge][year];
//std::cout << "*** *** *** VulRho: " << preyNum << "," << predNum << ": " << VulnerabilityRho(preyNum,predNum) << std::endl;
//std::cout << "valu: " << preyNum << "," << preyAge << "," << predNum << "," << predAge << "," << year << ","
//          << VulnerabilityRho(preyNum,predNum)
//          << " * " << SizePreferenceG[preyNum][preyAge][predNum][predAge][year] << std::endl;
//std::cout << "valu: " << preyNum << "," << preyAge << "," << predNum << "," << predAge << "," << year << ","
//          << VulnerabilityNu[preyNum][preyAge][predNum][predAge][year] << std::endl;
                    }
                }
            }
        }
    }

}

void
nmfAbundance::calculateSuitabilityNuOther(
            const std::vector<std::string>& AllSpecies,
            const Boost5DArrayDouble& VulnerabilityNu,
            double& nuOther)
{
    int predNum;
    int NumYears = m_WeightMap[AllSpecies[0]].size1();
    double numValues = 0;

    // Hack just to find a reasonable nu other value...may need to modify this
    //std::cout << "Using an average nu value for nuOther." << std::endl;
    nuOther = 0;
    for (int year = 0; year < NumYears; ++year) {
        predNum = -1;
        for (std::string predSpecies : AllSpecies) {
            ++predNum;
            for (int predAge = 0; predAge < m_WeightMap[predSpecies].size2(); ++predAge) {
                nuOther += VulnerabilityNu[0][0][predNum][predAge][year];
                ++numValues;
            }
        }
    }
    nuOther = (numValues == 0) ? 0 : nuOther/= numValues;
}

double
nmfAbundance::getNuOther()
{
    return m_nuOther;
}


void
nmfAbundance::setNuOther(double nuOther)
{
    m_nuOther = nuOther;
}


void
nmfAbundance::calculateScaledSuitability(
    const NuOther& nuOtherUser,
    const std::vector<std::string>& AllSpecies,
    Boost5DArrayDouble& VulnerabilityNu,
    Boost5DArrayDouble& ScaledSuitabilityNuTilde,
    double& nuOther)
{
    int predNum,preyNum,preyNum2;
    double nuSum = 0;
    double den;

    calculateSuitabilityNuOther(AllSpecies,VulnerabilityNu,nuOther);
    nuOther = (nuOtherUser.useUserNuOther) ?
                 nuOtherUser.nuOther : nuOther;
    setNuOther(nuOther);
//std::cout << "*** *** Using nu other value of: " << nuOther << std::endl;

    for (int year = 0; year < m_WeightMap[AllSpecies[0]].size1(); ++year) {
        preyNum = -1;
        for (std::string preySpecies : AllSpecies) {
            ++preyNum;
            for (int preyAge = 0; preyAge < m_WeightMap[preySpecies].size2(); ++preyAge) {
                predNum = -1;
                for (std::string predSpecies : AllSpecies) {
                    ++predNum;

                    for (int predAge = 0; predAge < m_WeightMap[predSpecies].size2(); ++predAge) {

                        nuSum = 0;
                        preyNum2 = -1;
                        for (std::string preySpecies2 : AllSpecies) {
                            ++preyNum2;
                            for (int preyAge2 = 0; preyAge2 < m_WeightMap[preySpecies2].size2(); ++preyAge2) {
                                nuSum +=  VulnerabilityNu[preyNum2][preyAge2][predNum][predAge][year];
                            }
                        }

                        den = nuSum + nuOther;
                        ScaledSuitabilityNuTilde[preyNum][preyAge][predNum][predAge][year] =
                             (den == 0) ? 0 :
                              VulnerabilityNu[preyNum][preyAge][predNum][predAge][year] / den;

//std::cout << "valss: " << preyNum << "," << preyAge << "," << predNum << "," << predAge << "," << year << ","
//          << VulnerabilityNu[preyNum][preyAge][predNum][predAge][year]
//          << " / " << nuSum << "+" << nuOther << std::endl;
                    }
                }
            }
        }
    }

}

bool
nmfAbundance::getYearlyParameters(
        const std::string &Species,
        const int &NumYears,
        const std::string TableName,
        std::map<std::string,std::vector<double> > &SigmaMap,
        std::map<std::string,std::vector<double> > &ZetaMap)
{
    int m;
    int NumRecords;
    std::vector<std::string> fields;
    std::map<std::string, std::vector<std::string> > dataMap;
    std::string queryStr;
    std::string msg;
    std::vector<std::string> ParameterNames = {"sigma","zeta"};
    int NumParameters = ParameterNames.size();
    std::vector<double> Sigma;
    std::vector<double> Zeta;

    fields     = {"SystemName","SpeName","Year","ParameterName","Value"};
    queryStr   = "SELECT SystemName,SpeName,Year,ParameterName,Value FROM " + TableName;
    queryStr  += " WHERE SystemName = '" + m_ProjectSettingsConfig + "'";
    queryStr  += " AND SpeName = '" + Species + "'";
    queryStr  += " ORDER BY ParameterName,Year";
    dataMap    = m_databasePtr->nmfQueryDatabase(queryStr, fields);
    NumRecords = dataMap["SpeName"].size();
    if (NumRecords == 0) {
        msg = "nmfAbundance::getYearlyParameters: No records found in table: " + TableName;
        m_logger->logMsg(nmfConstants::Error,msg);
        return false;
    }
    if (NumRecords != NumParameters*NumYears) {
        msg = "nmfAbundance::getYearlyParameters: Incorrect number of records found in table: " + TableName + "\n";
        msg += "Found " + std::to_string(NumRecords) + " records.\n";
        msg += "Calculated NumParameters*NumYears (" + std::to_string(NumParameters*NumYears) + ") records.";
        m_logger->logMsg(nmfConstants::Error,msg);
        return false;
    }

    m = 0;
    for (int i=0; i<NumYears; ++i) {
        Sigma.push_back(std::stod(dataMap["Value"][m++]));
    }
    for (int i=0; i<NumYears; ++i) {
        Zeta.push_back(std::stod(dataMap["Value"][m++]));
    }

    SigmaMap[Species] = Sigma;
    ZetaMap[Species]  = Zeta;

    return true;
}

void
nmfAbundance::calculatePredation(
        int year,
        std::vector<std::string>& AllSpecies,
        Boost3DArrayDouble& Biomass,
        Boost5DArrayDouble& BiomassAvailablePreyPhi,
        Boost3DArrayDouble& BiomassTotalPreyPhi,
        std::map<std::string,boost::numeric::ublas::matrix<double> >& PredationMortality,
        std::map<std::string,boost::numeric::ublas::matrix<double> >& AbundanceTable)
{
    int NumYears;
    int preyNum = -1;
    int NumPreyAges;
    int predNum;
    double doubleSum;
    double value;
    double den;

    for (std::string preySpecies : AllSpecies) {
//std::cout << preySpecies << std::endl;

        ++preyNum;
        NumYears    = m_WeightMap[preySpecies].size1();
        NumPreyAges = m_WeightMap[preySpecies].size2();
        for (int preyAge = 0; preyAge < NumPreyAges; ++preyAge) {
//std::cout << "preyage: " << preyAge << std::endl;
            doubleSum =  0;
            predNum   = -1;
            for (std::string predSpecies : AllSpecies) {
//std::cout << "  " << predSpecies << std::endl;
                ++predNum;
                for (int predAge = 0; predAge < m_WeightMap[predSpecies].size2(); ++predAge) {
                    if (year >= m_ConsumptionBiomassRatio[predSpecies].size1()) {
                        m_logger->logMsg(nmfConstants::Error,"nmfAbundance::calculatePredation: Found species with different number of years.");
                        return;
                    }
//std::cout << "year:    " << year << std::endl;
//std::cout << "predage: " << predAge << std::endl;
//std::cout << "sizes: " << m_ConsumptionBiomassRatio[predSpecies].size1() << "," << m_ConsumptionBiomassRatio[predSpecies].size2() << std::endl;
//std::cout << "m_ConsumptionBiomassRatio: " << predSpecies << "," << year << "," << predAge << ": " << m_ConsumptionBiomassRatio[predSpecies](year,predAge) << std::endl;
//std::cout << "Biomass: " << predNum << "," << predAge << "," << year << ": " << Biomass[predNum][predAge][year] << std::endl;
//std::cout << "BiomassAvailablePreyPhi: " << BiomassAvailablePreyPhi[preyNum][preyAge][predNum][predAge][year] << std::endl;
//std::cout << "  BiomassTotalPreyPhi: " << year << ": " << BiomassTotalPreyPhi[predNum][predAge][year] << std::endl;
                    if (BiomassTotalPreyPhi[predNum][predAge][year] == 0) {
                        m_logger->logMsg(nmfConstants::Error,"Found phi divide by 0 error.");
                        return;
                    }
//std::cout << BiomassTotalPreyPhi[predNum][predAge][year] << std::endl; // This is negative...why?
                    den = BiomassTotalPreyPhi[predNum][predAge][year];
                    if (den != 0) {
                        doubleSum += m_ConsumptionBiomassRatio[predSpecies](year,predAge) * Biomass[predNum][predAge][year] *
                                (BiomassAvailablePreyPhi[preyNum][preyAge][predNum][predAge][year] / den);
                    }

                }
            }
            den = AbundanceTable[preySpecies](year,preyAge) *
                  m_WeightMap[preySpecies](year,preyAge);
            value = (den == 0) ? 0 : (1.0/den) * doubleSum;
            m_PredationMortality[preySpecies](year,preyAge) = value;
            PredationMortality[preySpecies](year,preyAge)   = value;
        }
//std::cout << "preyAge loop done" << std::endl;
    }
}

void
nmfAbundance::calculateBiomassForCurrentYear(
            int year,
            std::vector<std::string>& AllSpecies,
            std::map<std::string,boost::numeric::ublas::matrix<double> >& WeightMap,
            std::map<std::string,boost::numeric::ublas::matrix<double> >& Abundance,
            Boost3DArrayDouble& Biomass,
            double& sumBiomass)
{
    int speciesNum = -1;
    int MinAge,MaxAge,FirstYear,LastYear,NumLengthBins;
    float MinLength,MaxLength;
    double value;

    sumBiomass = 0;
    for (std::string species : AllSpecies) {
        ++speciesNum;
        m_databasePtr->getSpeciesData(m_logger,
                                      species,MinAge,MaxAge,FirstYear,LastYear,
                                      MinLength,MaxLength,NumLengthBins);
        for (int age = 0; age < MaxAge-MinAge+1; ++age) {
            value = WeightMap[species](year,age) *
                    Abundance[species](year,age);
            if (value < 0) {
                m_logger->logMsg(nmfConstants::Error,"Got Biomass < 0. Setting to 0.");
                value = 0;
            }
            Biomass[speciesNum][age][year] = value;
//std::cout << "=========> Biomass " << species << "," << age << "," << year << ": " << value << ", weight: " <<
//          WeightMap[species](year,age) << std::endl;
            sumBiomass += value;
        }
    }
}

void
nmfAbundance::calculateBiomassAvailablePreyPhi(
        int year,
        std::vector<std::string>& AllSpecies,
        Boost5DArrayDouble& ScaledSuitabilityNuTilde,
        Boost3DArrayDouble& Biomass,
        Boost5DArrayDouble& BiomassAvailablePreyPhi)
{
    // Calculate phi(i,a,j,b,t)
    int predNum;
    int preyNum = -1;
    for (std::string preySpecies : AllSpecies) {
        ++preyNum;
        for (int preyAge = 0; preyAge < m_WeightMap[preySpecies].size2(); ++preyAge) {
            predNum = -1;
            for (std::string predSpecies : AllSpecies) {
                ++predNum;
                for (int predAge = 0; predAge < m_WeightMap[predSpecies].size2(); ++predAge) {
                    BiomassAvailablePreyPhi[preyNum][preyAge][predNum][predAge][year] =
                            ScaledSuitabilityNuTilde[preyNum][preyAge][predNum][predAge][year] *
                            Biomass[preyNum][preyAge][year];
//std::cout << "vals: " << preyNum << "," << preyAge << "," << predNum << "," << predAge << "," << year << ","
//          << ScaledSuitabilityNuTilde[preyNum][preyAge][predNum][predAge][year]
//          << "," << Biomass[preyNum][preyAge][year] << std::endl;
                }
            }
        }
    }
}

void
nmfAbundance::calculateBiomassTotalPreyPhi(
            int year,
            double nuOther,
            std::vector<std::string>& AllSpecies,
            std::vector<double>& Bother,
            Boost5DArrayDouble& BiomassAvailablePreyPhi,
            Boost3DArrayDouble& BiomassTotalPreyPhi)
{
    // Calculate phi(j,b,t)
    int predNum;
    int preyNum2;
    int preyNum = -1;
    double PhiOther;
    double sumPhi;

    for (std::string preySpecies : AllSpecies) {
       ++preyNum;
       for (int preyAge = 0; preyAge < m_WeightMap[preySpecies].size2(); ++preyAge) {
           predNum = -1;
           for (std::string predSpecies : AllSpecies) {
               ++predNum;
               for (int predAge = 0; predAge < m_WeightMap[predSpecies].size2(); ++predAge) {
                   sumPhi   = 0;
                   preyNum2 = -1;
                   for (std::string preySpecies : AllSpecies) {
                       ++preyNum2;
                       for (int preyAge2 = 0; preyAge2 < m_WeightMap[preySpecies].size2(); ++preyAge2) {
                           sumPhi += BiomassAvailablePreyPhi[preyNum2][preyAge2][predNum][predAge][year];
                       }
                   }
                   PhiOther = nuOther*Bother[year];
//std::cout << "=> " << nuOther << "," << Bother[year] << std::endl; // Bother is negative, why?
                   BiomassTotalPreyPhi[predNum][predAge][year] = PhiOther + sumPhi;
               }
           }
       }
   }
}

void
nmfAbundance::getData(NuOther& nuOtherUser,
                      QString RecruitmentType,
                      std::map<std::string,boost::numeric::ublas::matrix<double> >& Abundance,
                      std::map<std::string,std::vector<double> >& Recruitment,
                      std::map<std::string,std::vector<double> >& SpawningStockBiomass,
                      std::map<std::string,boost::numeric::ublas::matrix<double> >& PredationMortality)
{
std::cout << "nmfAbundance::getData start" << std::endl;
    int NumPreyAges;
    int NumSpecies,NumYears,NumAges;
    double sumBiomass=0;
    double nuOther;
    double lastAge;
    double penultAge;
    double Mortality;
    double LastMortality;
    double PenultMortality;
    double eFactorLastAge;
    double eFactorPenultAge;
    std::vector<double> Bother;
    std::vector<double> BiomassAvailablePreyPhiOther;
    std::vector<std::string> AllSpecies;
    std::map<std::string,std::vector<double> > SpawningBiomassValue;
    std::map<std::string,double> alpha;
    std::map<std::string,double> beta;
    std::map<std::string,double> gamma;
    std::map<std::string,std::vector<double> > sigma;
    std::map<std::string,std::vector<double> > zeta;
    boost::numeric::ublas::matrix<double> predationMatrix;

    m_databasePtr->getAllSpecies(m_logger,AllSpecies);
    NumSpecies = AllSpecies.size();

    Boost3DArrayDouble Biomass(boost::extents[NumSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);
    Boost3DArrayDouble BiomassTotalPreyPhi(boost::extents[NumSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);
    Boost3DArrayDouble Predation(boost::extents[NumSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);
    Boost5DArrayDouble SizePreferenceG(boost::extents[NumSpecies][nmfConstants::MaxNumberAges][NumSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);
    Boost5DArrayDouble VulnerabilityNu(boost::extents[NumSpecies][nmfConstants::MaxNumberAges][NumSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);
    Boost5DArrayDouble ScaledSuitabilityNuTilde(boost::extents[NumSpecies][nmfConstants::MaxNumberAges][NumSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);
    Boost5DArrayDouble BiomassAvailablePreyPhi(boost::extents[NumSpecies][nmfConstants::MaxNumberAges][NumSpecies][nmfConstants::MaxNumberAges][nmfConstants::MaxNumberYears]);

    nmfUtils::initialize(SizePreferenceG);
    nmfUtils::initialize(VulnerabilityNu);
    nmfUtils::initialize(ScaledSuitabilityNuTilde);
    nmfUtils::initialize(Biomass);
    nmfUtils::initialize(BiomassTotalPreyPhi);
    nmfUtils::initialize(Predation);

    // Initialize Predation data structures
    for (std::string species : AllSpecies) {
        NumYears    = m_WeightMap[species].size1();
        NumPreyAges = m_WeightMap[species].size2();
        nmfUtils::initialize(predationMatrix,NumYears,NumPreyAges);
        PredationMortality[species]   = predationMatrix;
        m_PredationMortality[species] = predationMatrix;
    }

    if (! getSpeciesParameters("SimulationParametersSpecies",
                               RecruitmentType.toStdString(),alpha,beta,gamma)) {
        m_logger->logMsg(nmfConstants::Warning,"nmfAbundance::getAbundance: No alpha,beta,gamma parameter data found.");
        return;
    }

    for (std::string Species : AllSpecies) {
        NumYears = m_WeightMap[Species].size1();
        if (! getYearlyParameters(Species,NumYears,"SimulationParametersYearly",sigma,zeta)) {
            m_logger->logMsg(nmfConstants::Warning,"nmfAbundance::getAbundance: No sigma,zeta parameter data found.");
            return;
        }
    }

    // Calculate the size preference: g(i,a,j,b,t)
    calculateSizePreference(AllSpecies,m_PreferredRatioEta,
                            m_PreferredLTRatio,m_PreferredGTRatio,
                            SizePreferenceG);

    // Next calculate the suitability: ν(i,a,j,b,t)
    calculateSuitability(AllSpecies,m_VulnerabilityRho,
                         SizePreferenceG, VulnerabilityNu);

    // Next calculate the scaled suitability: ν~(i,a,j,b,t)
    calculateScaledSuitability(nuOtherUser,AllSpecies,VulnerabilityNu,
                               ScaledSuitabilityNuTilde,nuOther);

    // Calculate initial Biomass and Predation matrices
    calculateBiomassForCurrentYear(0,AllSpecies,m_WeightMap,m_InitialAbundance,Biomass,sumBiomass);
    Bother.push_back(m_SystemStruct.TotalBiomass-sumBiomass);
    calculateBiomassAvailablePreyPhi(0,AllSpecies,ScaledSuitabilityNuTilde,Biomass,BiomassAvailablePreyPhi);
    BiomassAvailablePreyPhiOther.push_back(nuOther*Bother[0]);
    calculateBiomassTotalPreyPhi(0,nuOther,AllSpecies,Bother,BiomassAvailablePreyPhi,BiomassTotalPreyPhi);
    calculatePredation(0,AllSpecies,Biomass,
                       BiomassAvailablePreyPhi,BiomassTotalPreyPhi,
                       PredationMortality,m_InitialAbundance);

    // Find first year's spawning biomass
    for (std::string species : AllSpecies) {
        NumAges = m_WeightMap[species].size2();
        SpawningBiomassValue[species].push_back(getSpawningBiomass(species,0,NumAges,m_InitialAbundance));
    }

    double MortalityExponential;
    double RecruitmentValue;
    for (std::string species : AllSpecies) {
        NumYears = m_WeightMap[species].size1();
        NumAges  = m_WeightMap[species].size2();
        for (int year = 0; year < NumYears-1; ++year) {
//std::cout << "-----------Year: " << std::to_string(year)  << " of " << NumYears-1 << std::endl;
           // First get abundance for first age group
           RecruitmentValue = getRecruitment(species,RecruitmentType,year+1,
                                             alpha,beta,gamma,sigma,zeta,
                                             SpawningBiomassValue[species][year]);
           Abundance[species](year+1,0) = RecruitmentValue;
           Recruitment[species].push_back(RecruitmentValue);
           SpawningStockBiomass[species].push_back(SpawningBiomassValue[species][year]);

           // Get Abundance for all but first and last age groups
           for (int age=0; age<NumAges-2; ++age) {
               if (m_SystemStruct.AbundanceDriver == "Fishery Catch") {
                   Mortality = m_NaturalMortality[species](year,age) +
                               m_PredationMortality[species](year,age);
                   MortalityExponential = std::exp(-Mortality/2.0);
                   Abundance[species](year+1,age+1) =
                           (Abundance[species](year,age)*MortalityExponential - m_FisheryCatch[species](year,age)) *
                            MortalityExponential;
               } else {
                   Mortality = m_NaturalMortality[species](year,age) +
                               m_FishingMortality[species](year,age) +
                               m_PredationMortality[species](year,age);
                   MortalityExponential = std::exp(-Mortality/2.0);
                   Abundance[species](year+1,age+1) =
                           (Abundance[species](year,age)*MortalityExponential) * MortalityExponential;
               }
           }

           // Get Abundance for last age group
           lastAge   = NumAges-1; // m_maxAge-m_minAge;
           penultAge = lastAge-1;
           if (m_SystemStruct.AbundanceDriver == "Fishery Catch") {
               PenultMortality = m_NaturalMortality[species](year,penultAge) +
                                 m_PredationMortality[species](year,penultAge);
               LastMortality   = m_NaturalMortality[species](year,lastAge) +
                                 m_PredationMortality[species](year,lastAge);
               eFactorPenultAge = std::exp(-PenultMortality/2.0);
               eFactorLastAge   = std::exp(-LastMortality/2.0);
               Abundance[species](year+1,lastAge) =
                       (Abundance[species](year,penultAge)*eFactorPenultAge - m_FisheryCatch[species](year,penultAge))*eFactorPenultAge +
                       (Abundance[species](year,lastAge)  *eFactorLastAge   - m_FisheryCatch[species](year,lastAge))  *eFactorLastAge;
           } else {
               PenultMortality = m_NaturalMortality[species](year,penultAge) +
                                 m_FishingMortality[species](year,penultAge) +
                                 m_PredationMortality[species](year,penultAge);
               LastMortality   = m_NaturalMortality[species](year,lastAge) +
                                 m_FishingMortality[species](year,lastAge) +
                                 m_PredationMortality[species](year,lastAge);
               eFactorPenultAge = std::exp(-PenultMortality/2.0);
               eFactorLastAge   = std::exp(-LastMortality/2.0);
               Abundance[species](year+1,lastAge) =
                       (Abundance[species](year,penultAge)*eFactorPenultAge)*eFactorPenultAge +
                       (Abundance[species](year,lastAge)  *eFactorLastAge)  *eFactorLastAge;
           }
           SpawningBiomassValue[species].push_back(getSpawningBiomass(species,year,NumAges,Abundance));


           // Update the Biomass and Predation tables for the next year
           calculateBiomassForCurrentYear(year+1,AllSpecies,m_WeightMap,Abundance,Biomass,sumBiomass);
           Bother.push_back(m_SystemStruct.TotalBiomass-sumBiomass);
//std::cout << "~~~~> " << m_SystemStruct.TotalBiomass << "," << sumBiomass << std::endl;
           calculateBiomassAvailablePreyPhi(year+1,AllSpecies,ScaledSuitabilityNuTilde,
                                            Biomass,BiomassAvailablePreyPhi);
           BiomassAvailablePreyPhiOther.push_back(nuOther*Bother[year+1]);
           calculateBiomassTotalPreyPhi(year+1,nuOther,AllSpecies,Bother,
                                        BiomassAvailablePreyPhi,BiomassTotalPreyPhi);
           calculatePredation(year+1,AllSpecies,Biomass,
                              BiomassAvailablePreyPhi,BiomassTotalPreyPhi,
                              PredationMortality,Abundance);

       } // end for year

   } // end for species

// Write out predation matrix
std::cout << "Predation Matrix" << std::endl;
for (std::string species : AllSpecies) {
   std::cout << species << std::endl;
   for (int i = 0; i < PredationMortality[species].size1(); ++i) {
       for (int j = 0; j < PredationMortality[species].size2(); ++j) {
           std::cout << PredationMortality[species](i,j) << " ";
       }
       std::cout << std::endl;
   }
}


}

double
nmfAbundance::getRecruitment(const std::string species,
                             const QString RecruitmentType,
                             const int year,
                             std::map<std::string,double> alpha,
                             std::map<std::string,double> beta,
                             std::map<std::string,double> gamma,
                             std::map<std::string,std::vector<double> > sigma,
                             std::map<std::string,std::vector<double> > zeta,
                             const double SpawningBiomass)
{
    double retv = 0;

    if (RecruitmentType == "Ricker") {
        // R(y) = alpha * B(sp,y-1) exp[-beta(B(sp,y-1)^gamma]e^(zeta(y)-(sigma(R)^2/2)
        // double sigma = sigma[year];
        retv = alpha[species] * SpawningBiomass *
               std::exp(-beta[species] * std::pow(SpawningBiomass,gamma[species]));
               // * std::exp(m_zeta[year]-(sigma*sigma)/2.0);
    } else if (RecruitmentType == "Beverton-Holt") {
        // double sigma = m_sigma[year];
        retv = ((alpha[species] * SpawningBiomass) /
                (beta[species] + SpawningBiomass));
                // * std::exp(zeta[year]-(sigma*sigma)/2.0);
    }

    return retv;
}

double
nmfAbundance::getSpawningBiomass(std::string species,
                                 int year, int numAges,
                                 std::map<std::string,boost::numeric::ublas::matrix<double> >& Abundance)
{
    double sum = 0;
    double totalMortality;
    int Ts = 6; // T^s hard coded to 6 (from Butterworth paper)

    for (int age = 0; age < numAges; ++age) {
        totalMortality = -(m_NaturalMortality[species](year,age) +
                           m_FishingMortality[species](year,age) +
                           m_PredationMortality[species](year,age));
        sum += m_Maturity[species](year,age) *
               m_WeightMap[species](year,age)   *
               Abundance[species](year,age)  *
               std::exp((totalMortality) * (Ts/12.0));
//             std::exp(-m_naturalMortality(yearIdx,age) * (Ts/12.0));
    }
std::cout << "WARNING: Check summation for catch vs fishing mortality correctness" << std::endl;

    return sum * nmfConstantsMSCAA::Kg2Mt;
}


/*
void
nmfAbundance::getAbundanceOLD(boost::numeric::ublas::matrix<double> &Abundance,
                        std::vector<double> &Recruitment,
                        std::vector<double> &SpawningBiomass)
{
    double RecruitmentValue = 0;
    double SpawningBiomassValue = 0;
    double eFactorAge       = 0;
    double eFactorPenultAge = 0;
    double eFactorLastAge   = 0;
    double initialAbundance;
    int    NumYears = m_lastYear - m_firstYear + 1;
    int    yearIdx = 0;
    bool   useCatch = (m_abundanceDriver == "Fishery Catch");

    // Initialize Abundance matrix
    for (int age = 0; age < Abundance.size2(); ++age) {
        initialAbundance = m_initialAbundance(0,age);
        Abundance(0,age) = initialAbundance;
    }

    // Get first year's spawning biomass
    SpawningBiomassValue = getSpawningBiomass(yearIdx,Abundance);
    //SpawningBiomass.push_back(SpawningBiomassValue);

    for (int Year = 0; Year < NumYears-1; ++Year) {

        // First get abundance for first age group
        RecruitmentValue = getRecruitment(Year+1,SpawningBiomassValue);
        Abundance(Year+1,0) = RecruitmentValue;
        Recruitment.push_back(RecruitmentValue);
        SpawningBiomass.push_back(SpawningBiomassValue);

        // Get Abundance for all but last age group
        for (int Age=0; Age<m_maxAge-m_minAge-1; ++Age) {
            if (useCatch) {
                eFactorAge = std::exp(-m_naturalMortality(Year,Age)/2.0);
                Abundance(Year+1,Age+1) = (Abundance(Year,Age)*eFactorAge - m_fisheryCatch(Year,Age)) * eFactorAge;
            } else {
                eFactorAge = std::exp((-m_naturalMortality(Year,Age)-m_fishingMortality(Year,Age))/2.0);
                Abundance(Year+1,Age+1) = (Abundance(Year,Age)*eFactorAge) * eFactorAge;
            }
        }

        // Get Abundance for last age group
        int lastAge   = m_maxAge-m_minAge;
        int penultAge = lastAge-1;
        if (useCatch) {
            eFactorPenultAge = std::exp(-m_naturalMortality(Year,penultAge)/2.0);
            eFactorLastAge   = std::exp(-m_naturalMortality(Year,lastAge)/2.0);
            Abundance(Year+1,lastAge) =
                    (Abundance(Year,penultAge)*eFactorPenultAge - m_fisheryCatch(Year,penultAge))*eFactorPenultAge +
                    (Abundance(Year,lastAge)  *eFactorLastAge   - m_fisheryCatch(Year,lastAge))  *eFactorLastAge;
        } else {
            eFactorPenultAge = std::exp((-m_naturalMortality(Year,penultAge)-m_fishingMortality(Year,penultAge))/2.0);
            eFactorLastAge   = std::exp((-m_naturalMortality(Year,lastAge)  -m_fishingMortality(Year,lastAge))/2.0);
            Abundance(Year+1,lastAge) =
                    (Abundance(Year,penultAge)*eFactorPenultAge)*eFactorPenultAge +
                    (Abundance(Year,lastAge)  *eFactorLastAge)  *eFactorLastAge;
        }
        SpawningBiomassValue = getSpawningBiomass(Year+1,Abundance);
    }

}
*/

// R(y) = alpha * B(sp,y-1) exp[-beta(B(sp,y-1)^gamma]e^(zeta(y)-(sigma(R)^2/2)
//double
//nmfAbundance::getRecruitment(const int &yearIdx,
//                             const double &SpawningBiomass)
//{
////    double sigma = m_sigma[yearIdx];
////    return m_alpha[m_species] *
////            SpawningBiomass *
////            std::exp(-m_beta[m_species]*std::pow(SpawningBiomass,m_gamma[m_species]));

//    //            * std::exp(m_zeta[yearIdx]-(sigma*sigma)/2.0);
//return 0;
//}

//double
//nmfAbundance::getSpawningBiomass(const int &yearIdx,
//                                 const boost::numeric::ublas::matrix<double> &Abundance)
//{
////    double sum = 0;
////    int Ts = 6; // T^s hard coded to 6 (from Butterworth paper)

////    for (int age = 0; age < m_maxAge-m_minAge+1; ++age) {
////        sum += m_maturity(yearIdx,age) *
////               m_weight(yearIdx,age)   *
////               Abundance(yearIdx,age)  *
////               std::exp((-m_naturalMortality(yearIdx,age)-m_fishingMortality(yearIdx,age)) * (Ts/12.0));

//        //             std::exp(-m_naturalMortality(yearIdx,age) * (Ts/12.0));

////    }
////    return sum * nmfConstantsMSCAA::Kg2Mt;
//    return 0;
//}
