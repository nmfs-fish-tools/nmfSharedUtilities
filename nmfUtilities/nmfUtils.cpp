
/** @file nmfUtils.h
 * @brief Definition for common typedefs and structs (i.e., non-graphical)
 * @date Dec 8, 2016
 *
 * This file contains typedefs, structs, and functions common to the MultiSpecies
 * applications. The functions that print various structures are located in this
 * file as well.
 */

#include <ctime>
#include <fstream>
#include "nmfUtils.h"
#include "nmfConstants.h"
#include "nmfLogger.h"

namespace nmfUtils {

void
append(std::vector<double>& newVec,
       std::vector<double>& currentVec)
{
    currentVec.insert(currentVec.end(), newVec.begin(), newVec.end());
}

double
applyCovariate(nmfLogger *logger,
               const std::string& covariateAlgorithmType,
               const double& parameter,
               const double& covariateCoeff,
               const double& covariateValue)
{
    double retv = 0;
    std::string msg;

    if (covariateValue == nmfConstantsMSSPM::NoData) {
        retv = parameter;
    } else {
        if (covariateCoeff == 0) {
            retv = parameter;
        } else if (covariateAlgorithmType == nmfConstantsMSSPM::Linear) {
            retv = parameter*(1.0+covariateCoeff*covariateValue);
        } else if (covariateAlgorithmType == nmfConstantsMSSPM::Exponential) {
            retv = std::pow(parameter,(covariateCoeff*covariateValue));
        } else {
            msg = "nmfUtils::applyCovariate: Found invalid covariate algorithm type of: " +
                    covariateAlgorithmType;
            if (logger != nullptr) {
                logger->logMsg(nmfConstants::Error,msg);
            } else {
                std::cout << "Error: " << msg << std::endl;
            }
        }
    }
//std::cout << "***** covVal: " << covariateValue << ", retv: " << retv << std::endl;
    return retv;
}

bool checkForError(nmfLogger *logger,
                   std::string table,
                   std::string field,
                   std::map<std::string, std::vector<std::string> > &dataMap,
                   std::string queryStr)
{
    if (dataMap[field].size() == 0) {
        logger->logMsg(nmfConstants::Error,"Error - Check " + table + " table. ");
        logger->logMsg(nmfConstants::Error,"Error - No data found for cmd: "+queryStr);
        return true; // there was an error
    }

    return false; // there was no error

} // end errorCheck

double convertCatchUnitsToValue(std::string catchUnits)
{
    double CatchUnitsVal = 0.0;

    if (catchUnits == "Hundreds of Fish")
        CatchUnitsVal =    0.1;
    else if (catchUnits == "Thousands of Fish")
        CatchUnitsVal =    1.0;
    else if (catchUnits == "Millions of Fish")
        CatchUnitsVal = 1000.0;

    return CatchUnitsVal;
}

double convertSizeUnitsToValue(std::string sizeUnits)
{
    double SizeUnitsVal = 0.0;

    if (sizeUnits == "Millimeters")
        SizeUnitsVal = 0.1;
    else if (sizeUnits == "Centimeters")
        SizeUnitsVal = 1.0;
    else if (sizeUnits == "Inches")
        SizeUnitsVal = 2.54;

    return SizeUnitsVal;
}

std::string
convertToScientificNotation(double val)
{
    std::ostringstream streamDouble; // Needed to stuff a double into a string with scientific notation

    streamDouble.str("");
    streamDouble << val;

    return streamDouble.str();
}


std::string convertValues2DToOutputStr(const std::string& label,
                                       const boost::numeric::ublas::matrix<double> &matrix)
{
    std::string bestFitnessStr = "";

    bestFitnessStr += "<br>"+label;
    bestFitnessStr += "<table>";

    for (unsigned i=0; i<matrix.size1(); ++i) {
        bestFitnessStr += "<tr>";
        for (unsigned j=0; j<matrix.size2(); ++j) {
            bestFitnessStr += "<td> " + convertToScientificNotation(matrix(i,j)) + "</td>";
        }
        bestFitnessStr += "</tr>";
    }

    bestFitnessStr += "</table>";

    return bestFitnessStr;
}

boost::numeric::ublas::matrix<double>
convertVectorToMatrix(std::vector<double>& vec)
{
    int i = 0;
    boost::numeric::ublas::matrix<double> matrix;

    initialize(matrix,vec.size(),1);
    for (double val : vec) {
        matrix(i++,0) = val;
    }

    return matrix;
}

double convertWeightUnitsToValue(std::string weightUnits)
{
    double WeightUnitsVal = 0.0;

    if (weightUnits == "Gram")
        WeightUnitsVal = 0.001;
    else if (weightUnits == "Kilograms")
        WeightUnitsVal = 1.0;
    else if (weightUnits == "Pounds")
        WeightUnitsVal = (1.0/2.2);

    return WeightUnitsVal;
}

void copyFile(std::string fileA,
         std::string fileB)
{
    std::ifstream fileIn( fileA, std::ios::in);
    std::ofstream fileOut(fileB, std::ios::out);

    fileOut << fileIn.rdbuf();

    fileIn.close();
    fileOut.close();
}

void getFilesWithExt(std::string path,
                std::string ext,
                QStringList& filesToCopy)
{
    QString fullExt = "*" + QString::fromStdString(ext);
    filesToCopy.clear();

    QDir directory(QString::fromStdString(path));
    filesToCopy = directory.entryList(QStringList() << fullExt << fullExt,QDir::Files);
}

double getMatrixMax(boost::numeric::ublas::matrix<double> &mat,
                         bool roundOff=false)
{
    // find largest value in mat
    double maxVal = 0.0;
    double val = 0.0;

    for (boost::numeric::ublas::matrix<double>::iterator1 it1 =
            mat.begin1(); it1 != mat.end1(); ++it1) {
        for (boost::numeric::ublas::matrix<double>::iterator2 it2 = it1.begin();
                it2 != it1.end(); ++it2) {
            val = *it2;
            if (val > maxVal) {
                maxVal = val;
            }
        }
    }
    if (maxVal > 1.0) {
        if (roundOff) {
            int numDigits = int(log10(maxVal) + 1);
            int firstDigit = int((maxVal / pow(10.0, numDigits - 1)) + 0.5);
            maxVal = firstDigit * pow(10.0, numDigits - 1);
        }
    } else {
        maxVal = 1.0;
    }

    return maxVal;
}

double getMatrixMean(const boost::numeric::ublas::matrix<double>& mat)
{
    int numItems = 0; // Not equal to nrows*ncols if there are blanks in observed biomass
    int nrows = mat.size1();
    int ncols = mat.size2();
    double mean = 0;

    for (int time=0; time<nrows; ++time) {
        for (int species=0; species<ncols; ++species) {
            if (mat(time,species) != nmfConstantsMSSPM::NoData) {
                mean += mat(time,species);
                ++numItems;
            }
        }
    }

    if (numItems > 0) {
        mean /= numItems;
    } else {
        std::cout << "Error: Trying to find the mean of an empty matrix" << std::endl;
    }

    return mean;
}

double getMatrixSum(const boost::numeric::ublas::matrix<double> &mat) {
    return sum(prod(boost::numeric::ublas::scalar_vector<double>(mat.size1()), mat));
}

std::string getOS()
{
    // The _WIN64 check should be first, since _WIN32 is 1 for both 32-bit and 64-bit
    #ifdef _WIN64
        return "Windows 64-bit";
    #elif _WIN32
        return "Windows 32-bit";
    #elif __APPLE__ || __MACH__
        return "Mac OSX";
    #elif __linux__
        return "Linux";
    #elif __FreeBSD__
        return "FreeBSD";
    #elif __unix || __unix__
        return "Unix";
    #else
        return "Other";
    #endif
}

/*
 * Returns a random number between the passed limits: [lowerLimit,upperLimit)
 * If seed < 0, no seed is used and the algorithm is stochastic.
 * If seed >= 0, then that seed is used and the algorithm is deterministic.
 */
double getRandomNumber(int seedValue, double lowerLimit, double upperLimit)
{
//    unsigned RandomSeed = nmfUtilsQt::getCurrentTime().currentMSecsSinceEpoch();
//    unsigned Seed = (seedValue < 0) ? RandomSeed : seedValue;
//    std::uniform_real_distribution<double> dist(lowerLimit,upperLimit);
//    std::mt19937_64 rng(Seed);
//    return dist(rng);

    double retv = 0;
    double span = upperLimit - lowerLimit;

    if (seedValue < 0) { // stochastic
        retv = span*QRandomGenerator::global()->generateDouble() + lowerLimit; // - 0.5*span;
    } else {             // deterministic
        QRandomGenerator randomGenerator(seedValue);
        retv = span*randomGenerator.generateDouble() + lowerLimit; // - 0.5*span;
    }

    return retv;
}

double getVectorSum(const boost::numeric::ublas::vector<double> &vec) {
    return sum(vec);
}

bool invertMatrix(
        boost::numeric::ublas::matrix<double>& matrix,
        boost::numeric::ublas::matrix<double>& inverseMatrix)
{
    bool retv = true;

    initialize(inverseMatrix,int(matrix.size1()),int(matrix.size2()));

    try {
        inverseMatrix = boost::numeric::ublas::identity_matrix<float>(matrix.size1());
        boost::numeric::ublas::permutation_matrix<std::size_t> pm(matrix.size1());
        boost::numeric::ublas::lu_factorize(matrix,pm);
        boost::numeric::ublas::lu_substitute(matrix, pm, inverseMatrix);
    }  catch (...) {
        retv = false;
    }

    return retv;
}

bool isEstimateParameterChecked(
        const nmfStructsQt::ModelDataStruct& dataStruct,
        const std::string& ParameterName)
{
    std::vector<nmfStructsQt::EstimateRunBox> EstimateRunBoxes = dataStruct.EstimateRunBoxes;
    for (nmfStructsQt::EstimateRunBox runBox : EstimateRunBoxes) {
        if ((runBox.parameter == ParameterName) &&
            (runBox.state.first == true) &&
            (runBox.state.second == true)) {
            return true;
        }
    }

    return false;
}

bool isNearlyZero(const double& value)
{
    return (std::abs(value) < nmfConstants::NearlyZero);
}
bool isOSWindows()
{
    std::string osName = getOS();

    return ((osName == "Windows 32-bit") ||
            (osName == "Windows 64-bit"));
}

bool isStopped(std::string& runName,
          std::string& msg1,
          std::string& msg2,
          std::string& stopRunFile,
          std::string& state)
{
    std::string cmd;
    std::ifstream inputFile(stopRunFile);
    if (inputFile) {
        std::getline(inputFile,cmd);
        std::getline(inputFile,runName);
        std::getline(inputFile,msg1);
        std::getline(inputFile,msg2);
    }
    inputFile.close();

    state = cmd;

    return ((cmd == "Stop")           ||
            (cmd == "StopAllOk")      ||
            (cmd == "StopIncomplete") ||
            (cmd == "StoppedByUser"));

}

bool isWholeNumber(double value)
{
    return (floor(value) == ceil(value));
}

void printDataStruct(
        const std::string& msg,
        const nmfStructsQt::ModelDataStruct& dataStruct)
{
    std::cout << "\n" << msg << std::endl;
    std::cout << "isMohnsRho: " << dataStruct.isMohnsRho << std::endl;
    std::cout << "isRelativeBiomass: " << dataStruct.isRelativeBiomass << std::endl;
    std::cout << "showDiagnosticChart: " << dataStruct.showDiagnosticChart << std::endl;
    std::cout << "useApplicationFixedSeedNLopt: " << dataStruct.useApplicationFixedSeedNLopt << std::endl;
    std::cout << "useApplicationFixedSeedBees: " << dataStruct.useApplicationFixedSeedBees << std::endl;
    std::cout << "useUserFixedSeedNLopt: " << dataStruct.useUserFixedSeedNLopt << std::endl;
    std::cout << "useUserFixedSeedBees: " << dataStruct.useUserFixedSeedBees << std::endl;
    std::cout << "userFixedSeedVal: " << dataStruct.userFixedSeedVal << std::endl;
    std::cout << "incrementFixedSeed: " << dataStruct.incrementFixedSeed << std::endl;
    std::cout << "NLoptUseStopVal: " << dataStruct.NLoptUseStopVal << std::endl;
    std::cout << "NLoptUseStopAfterTime: " << dataStruct.NLoptUseStopAfterTime << std::endl;
    std::cout << "NLoptUseStopAfterIter: " << dataStruct.NLoptUseStopAfterIter << std::endl;
    std::cout << "NLoptStopVal: " << dataStruct.NLoptStopVal << std::endl;
    std::cout << "NLoptStopAfterTime: " << dataStruct.NLoptStopAfterTime << std::endl;
    std::cout << "NLoptStopAfterIter: " << dataStruct.NLoptStopAfterIter << std::endl;
    std::cout << "NLoptNumberOfRuns: " << dataStruct.NLoptNumberOfRuns << std::endl;
    std::cout << "NLoptUseInitialPopulationSize: " << dataStruct.NLoptUseInitialPopulationSize << std::endl;
    std::cout << "NLoptInitialPopulationSize: " << dataStruct.NLoptInitialPopulationSize << std::endl;
    std::cout << "NumMohnsRhoMultiRuns: " << dataStruct.NumMohnsRhoMultiRuns << std::endl;

    std::cout << "MultiRunSpeciesFilename: " << dataStruct.MultiRunSpeciesFilename << std::endl;
    std::cout << "MultiRunModelFilename: " << dataStruct.MultiRunModelFilename << std::endl;
    std::cout << "MultiRunSetupFilename: " << dataStruct.MultiRunSetupFilename << std::endl;
    std::cout << "RunLength: " << dataStruct.RunLength << std::endl;
    std::cout << "NumSpecies: " << dataStruct.NumSpecies << std::endl;
    std::cout << "NumGuilds: " << dataStruct.NumGuilds << std::endl;
    std::cout << "BeesMaxGenerations: " << dataStruct.BeesMaxGenerations << std::endl;
    std::cout << "BeesNumTotal: " << dataStruct.BeesNumTotal << std::endl;
    std::cout << "BeesNumBestSites: " << dataStruct.BeesNumBestSites << std::endl;
    std::cout << "BeesNumEliteSites: " << dataStruct.BeesNumEliteSites << std::endl;
    std::cout << "BeesNumElite: " << dataStruct.BeesNumElite << std::endl;
    std::cout << "BeesNumOther: " << dataStruct.BeesNumOther << std::endl;
    std::cout << "BeesNeighborhoodSize: " << dataStruct.BeesNeighborhoodSize << std::endl;
    std::cout << "BeesNumRepetitions: " << dataStruct.BeesNumRepetitions << std::endl;
    std::cout << "GAGenerations: " << dataStruct.GAGenerations << std::endl;
    std::cout << "GAConvergence: " << dataStruct.GAConvergence << std::endl;

    std::cout << "TotalNumberParameters: " << dataStruct.TotalNumberParameters << std::endl;
    std::cout << "Benchmark: " << dataStruct.Benchmark << std::endl;
    std::cout << "GrowthForm: " << dataStruct.GrowthForm << std::endl;
    std::cout << "HarvestForm: " << dataStruct.HarvestForm << std::endl;
    std::cout << "CompetitionForm: " << dataStruct.CompetitionForm << std::endl;
    std::cout << "PredationForm: " << dataStruct.PredationForm << std::endl;
    std::cout << "EstimationAlgorithm: " << dataStruct.EstimationAlgorithm << std::endl;
    std::cout << "MinimizerAlgorithm: " << dataStruct.MinimizerAlgorithm << std::endl;
    std::cout << "ObjectiveCriterion: " << dataStruct.ObjectiveCriterion << std::endl;
    std::cout << "ScalingAlgorithm: " << dataStruct.ScalingAlgorithm << std::endl;
    std::cout << "LogScale: " << dataStruct.LogScale << std::endl;
    std::cout << "CovariateAlgorithmType: " << dataStruct.CovariateAlgorithmType << std::endl;
    std::cout << "ForecastHarvestType: " << dataStruct.ForecastHarvestType << std::endl;

    printVector("SpeciesWeights",10,dataStruct.SpeciesWeights);

    printMatrix("ObservedBiomassBySpecies",dataStruct.ObservedBiomassBySpecies,10,4);
    printMatrix("ObservedBiomassByGuilds",dataStruct.ObservedBiomassByGuilds,10,4);
    printMatrix("Catch",dataStruct.Catch,10,4);
    printMatrix("Effort",dataStruct.Effort,10,4);
    printMatrix("FitWeights",dataStruct.FitWeights,10,4);

    printVector("InitBiomass",10,dataStruct.InitBiomass);
    printVector("InitBiomassMin",10,dataStruct.InitBiomassMin);
    printVector("InitBiomassMax",10,dataStruct.InitBiomassMax);
    printVector("GrowthRate",10,dataStruct.GrowthRate);
    printVector("GrowthRateMin",10,dataStruct.GrowthRateMin);
    printVector("GrowthRateMax",10,dataStruct.GrowthRateMax);
    printVector("GrowthRateShape",10,dataStruct.GrowthRateShape);
    printVector("GrowthRateShapeMin",10,dataStruct.GrowthRateShapeMin);
    printVector("GrowthRateShapeMax",10,dataStruct.GrowthRateShapeMax);
    printVector("CarryingCapacity",10,dataStruct.CarryingCapacity);
    printVector("CarryingCapacityMin",10,dataStruct.CarryingCapacityMin);
    printVector("CarryingCapacityMax",10,dataStruct.CarryingCapacityMax);
    printVector("Catchability",10,dataStruct.Catchability);
    printVector("CatchabilityMin",10,dataStruct.CatchabilityMin);
    printVector("CatchabilityMax",10,dataStruct.CatchabilityMax);
    printVector("SurveyQ",10,dataStruct.SurveyQ);
    printVector("SurveyQMin",10,dataStruct.SurveyQMin);
    printVector("SurveyQMax",10,dataStruct.SurveyQMax);

    printMatrix("Competition",dataStruct.Competition,10,4);
    printMatrix("CompetitionMin",dataStruct.CompetitionMin,10,4);
    printMatrix("CompetitionMax",dataStruct.CompetitionMax,10,4);
    printMatrix("CompetitionBetaSpecies",dataStruct.CompetitionBetaSpecies,10,4);
    printMatrix("CompetitionBetaSpeciesMin",dataStruct.CompetitionBetaSpeciesMin,10,4);
    printMatrix("CompetitionBetaSpeciesMax",dataStruct.CompetitionBetaSpeciesMax,10,4);
    printMatrix("CompetitionBetaGuilds",dataStruct.CompetitionBetaGuilds,10,4);
    printMatrix("CompetitionBetaGuildsMin",dataStruct.CompetitionBetaGuildsMin,10,4);
    printMatrix("CompetitionBetaGuildsMax",dataStruct.CompetitionBetaGuildsMax,10,4);
    printMatrix("CompetitionBetaGuildsGuilds",dataStruct.CompetitionBetaGuildsGuilds,10,4);
    printMatrix("CompetitionBetaGuildsGuildsMin",dataStruct.CompetitionBetaGuildsGuildsMin,10,4);
    printMatrix("CompetitionBetaGuildsGuildsMax",dataStruct.CompetitionBetaGuildsGuildsMax,10,4);
    printMatrix("PredationRho",dataStruct.PredationRho,10,12);
    printMatrix("PredationRhoMin",dataStruct.PredationRhoMin,10,12);
    printMatrix("PredationRhoMax",dataStruct.PredationRhoMax,10,12);
    printMatrix("PredationHandling",dataStruct.PredationHandling,10,4);
    printMatrix("PredationHandlingMin",dataStruct.PredationHandlingMin,10,4);
    printMatrix("PredationHandlingMax",dataStruct.PredationHandlingMax,10,4);

    printVector("PredationExponent",10,dataStruct.PredationExponent);
    printVector("PredationExponentMin",10,dataStruct.PredationExponentMin);
    printVector("PredationExponentMax",10,dataStruct.PredationExponentMax);
    printVector("Parameters",0,dataStruct.Parameters);

    for (unsigned i=0; i<dataStruct.EstimateRunBoxes.size(); ++i) {
        std::cout << "EstimateRunBox(" << i << "): "
                  << dataStruct.EstimateRunBoxes[i].parameter    << ", "
                  << dataStruct.EstimateRunBoxes[i].state.first  << ", "
                  << dataStruct.EstimateRunBoxes[i].state.second << std::endl;
    }

//    std::map<std::string,std::vector<double> >          CovariateMap;
//    std::map<std::string,std::string>                   CovariateAssignment;
//    std::map<std::string,CovariateStruct>               GrowthRateCovariateCoeff;
//    std::map<std::string,CovariateStruct>               CarryingCapacityCovariateCoeff;
//    std::map<std::string,CovariateStruct>               CatchabilityCovariateCoeff;
//    std::map<std::string,CovariateStruct>               SurveyQCovariateCoeff;
//    std::map<QString,QString> PreviousUnits;
//    boost::numeric::ublas::matrix<double> Exploitation;
//    boost::numeric::ublas::vector<double> ExploitationRateMin; // RSK - change this to matrix?
//    boost::numeric::ublas::vector<double> ExploitationRateMax; // RSK - change this to matrix?
//    std::vector<std::string>              SpeciesNames;
//    std::map<int,std::vector<int> >       GuildSpecies; // List of species numbers that make up guild num
//    std::vector<int>                      GuildNum;     // Specifies which species are members of which guilds


}


void print3DArray(
        const std::string &name,
        const std::vector<int> &dimensions,
        const Boost3DArrayDouble &array,
        const int &width,
        const int &precision)
{
    int imax = dimensions[0];
    int jmax = dimensions[1];
    int kmax = dimensions[2];

    std::cout <<  name.c_str() << " (" << imax << "x" << jmax << "x" << kmax << "):\n" << std::endl;
    for (int i = 0; i < imax; ++i) {
        for (int j = 0; j < jmax; ++j) {
            for (int k = 0; k < kmax; ++k) {
                std::cout << std::fixed << std::setw(width) << std::setprecision(precision) << array[i][j][k] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void print4DArray(
        const std::string &name,
        const std::vector<int> &dimensions,
        const Boost4DArrayDouble &array)
{
    int imax = dimensions[0];
    int jmax = dimensions[1];
    int kmax = dimensions[2];
    int lmax = dimensions[3];

    std::cout << name.c_str() << " (" << imax << "x" << jmax << "x" << kmax << "x" << lmax
            << "):\n" << std::endl;
    for (int i = 0; i < imax; ++i) {
        for (int j = 0; j < jmax; ++j) {
            for (int k = 0; k < kmax; ++k) {
                for (int l = 0; l < lmax; ++l) {
                    std::cout << array[i][j][k][l] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void printError(const std::string msg, const std::string errorMsg)
{
    std::cout << "\nError: " << errorMsg << std::endl;
    std::cout << msg << std::endl;
}

void printMap(const std::string name, const std::map<std::string, std::vector<std::string> > &dataMap) {
    std::cout << "Map: " << name << std::endl;
    std::for_each(dataMap.begin(), dataMap.end(), printMapPair);
}

void printMapPair(const std::pair<std::string, std::vector<std::string> > &pair) {
    std::cout << "Key: " << pair.first << std::endl;
    copy(pair.second.begin(), pair.second.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
}

void printMatrix(const std::string &name,
        const boost::numeric::ublas::matrix<double> &mat,
        const int width,
        const int precision) {
    std::cout << "\n" << name << " (" << mat.size1() << "x" << mat.size2()
            << ")" << std::endl;
    for (unsigned row = 0; row < mat.size1(); ++row) {
        std::cout << "  ";
        for (unsigned col = 0; col < mat.size2(); ++col) {
            std::cout << std::setw(width)
                      << std::setprecision(precision)
                      << std::fixed << mat(row, col) << "  ";
        }
        std::cout << std::endl;
    }
}

void printMsg(std::string msg) {
    std::cout << msg << std::endl;
}

void readTableDescriptions(std::string TableName,
                          std::map<std::string,std::string> &TableDescription)
{
    std::string msg;
    std::string line;
    std::vector<std::string> parts;

    TableDescription.clear();

    std::ifstream fptr(TableName);
    if (fptr) {
        while (std::getline(fptr,line))
        {
            boost::algorithm::trim(line); // trims any spaces
            if (line[0] != '#') {
                boost::split(parts,line,boost::is_any_of("-"),boost::token_compress_on);
                if (parts.size() == 2) {
                    boost::algorithm::trim(parts[0]);
                    boost::algorithm::trim(parts[1]);
                    TableDescription[parts[0]] = parts[1];
                }
            }
        }
        fptr.close();
    } else {
        msg = "\nnmfUtils::readTableDescription Error: Couldn't open file: " + TableName;
        //QMessageBox::warning(0,tr("File Open Error"),tr(msg.c_str()),QMessageBox::Ok);
        std::cout << msg << std::endl;
    }
} // end readTableDescription

void readTableNames(std::map<std::string,std::vector<std::string> > *TableNames)
{
    int modelType;
    std::string file = nmfConstantsMSVPA::TableNamesFile;
    std::string line;
    std::string msg;
    std::vector<std::string> parts;
    std::ifstream fptr(file);
    std::vector<std::string> fields;

    for (auto i=0;i<nmfConstantsMSVPA::NumModelTypes; ++i) {
        TableNames[i].clear();
    }
    if (fptr) {
        while (std::getline(fptr,line))
        {
            boost::algorithm::trim(line); // trims any spaces
            if (line[0] != '#') {
                boost::split(parts,line,boost::is_any_of("\t "),boost::token_compress_on);
                if (parts.size() > 1) {
                    fields.clear();
                    for (int i=0; i<std::stoi(parts[2]); ++i) {
                        fields.push_back(parts[i+3]);
                    }
                    modelType = (parts[1] == "SSVPA") ? 0 : (parts[1] == "MSVPA") ? 1 : 2 ;
                    TableNames[modelType][parts[0]] = fields;
                } else {
                    std::cout << "nmfUtils::readTableNames Error in .MSVPA_X2/data/tableNames.dat." << std::endl;
                    break;
                }
            }
        }
        fptr.close();
    } else {
        msg = "\nnmfUtils::readTableNames Error: Couldn't open file: " + file;
        //QMessageBox::warning(0,tr("File Open Error"),tr(msg.c_str()),QMessageBox::Ok);
        std::cout << msg << std::endl;
    }
} // end readTableNames

void
removeFirstRow(boost::numeric::ublas::matrix<double> &matrixIn,
               boost::numeric::ublas::matrix<double> &matrixOut)
{
    int numRows = matrixIn.size1();
    int numCols = matrixIn.size2();

    boost::numeric::ublas::matrix_range<boost::numeric::ublas::matrix<double> > matrixTrimmed (
                matrixIn,
                boost::numeric::ublas::range(1,numRows),
                boost::numeric::ublas::range(0,numCols));

    matrixOut = matrixTrimmed;
}


/*
 * Rescales matrix Xij by: log base 10 (X)
 */
void
rescaleMatrixLog10(const boost::numeric::ublas::matrix<double> &unscaledMatrix,
                         boost::numeric::ublas::matrix<double> &rescaledMatrix)
{
    int NumRows = rescaledMatrix.size1();
    int NumCols = rescaledMatrix.size2();
    double unscaledValue;

    // Rescale the matrix using the min and max values found for each species
    for (auto species=0; species<NumCols; ++species) {
        for (auto time=0; time<NumRows; ++time) {
            unscaledValue = unscaledMatrix(time,species);
            if (unscaledValue <= 0) {
                unscaledValue = 0;
            }
            unscaledValue += 1.0;
            if (unscaledValue != nmfConstantsMSSPM::NoData) {
                rescaledMatrix(time,species) = std::log10(unscaledValue);
            }
        }
    }
}

void
rescaleMatrixMean(const boost::numeric::ublas::matrix<double> &unscaledatrix,
                        boost::numeric::ublas::matrix<double> &rescaledMatrix)
{
    // NumYears will change if running a Mohn's Rho. Use rescaleMatrix for the correct
    // number of years as size1(). Don't use unscaledatrix as an incoming NLoptDataStruct.Catch
    // will have the full number of years, but the rescaledMatrix will be the appropriate
    // size for a Mohn's Rho run.
    int numYears   = rescaledMatrix.size1();
    int numSpecies = rescaledMatrix.size2();
    int numYearsWithoutBlanks = 0; // Not equal to numYears if there are blanks in observed biomass
    double den;
    double minVal;
    double maxVal;
    double avgVal;
    double tmpVal;
    std::vector<double> minValues(numSpecies,0);
    std::vector<double> maxValues(numSpecies,0);
    std::vector<double> avgValues(numSpecies,0);
    std::vector<double> tmp;
    std::vector<double> numBlankYears;

    // Find min,max values for each column of matrix
    for (int species=0; species<numSpecies; ++species) {
        avgVal = 0;
        numBlankYears.push_back(0);
        tmp.clear();
        numYearsWithoutBlanks = 0;
        for (int time=0; time<numYears; ++time) {
            if (unscaledatrix(time,species) != nmfConstantsMSSPM::NoData) {
                tmpVal = unscaledatrix(time,species);
                tmp.push_back(tmpVal);

                avgVal += tmpVal;
                ++numYearsWithoutBlanks;
            } else {
                ++numBlankYears[species];
            }
        }
        avgVal /= (numYearsWithoutBlanks-numBlankYears[species]);
        std::sort(tmp.begin(),tmp.end());
        minValues[species] = tmp.front();
        maxValues[species] = tmp.back();
        avgValues[species] = avgVal;
    }

    // Rescale each column of the matrix with (x - ave)/(max-min) formula.
    for (int species=0; species<numSpecies; ++species) {
        minVal = minValues[species];
        maxVal = maxValues[species];
        den    = maxVal - minVal;
        avgVal = avgValues[species];
        for (int time=0; time<numYears; ++time) {
            if (unscaledatrix(time,species) != nmfConstantsMSSPM::NoData) {
                rescaledMatrix(time,species) = (den != 0) ? (unscaledatrix(time,species) - avgVal) / den : 0; // mean normalization
            } else {
                rescaledMatrix(time,species) = nmfConstantsMSSPM::NoData;
            }
        }
    }
}

void
rescaleMatrixMinMax(const boost::numeric::ublas::matrix<double> &unscaledMatrix,
                          boost::numeric::ublas::matrix<double> &rescaledMatrix)
{
    int numYears   = rescaledMatrix.size1();
    int numSpecies = rescaledMatrix.size2();
    double den;
    double minVal;
    double maxVal;
    std::vector<double> minValues(numSpecies,0);
    std::vector<double> maxValues(numSpecies,0);
    std::vector<double> tmp;

    // Find min,max values for each column of matrix
    for (int species=0; species<numSpecies; ++species) {
        tmp.clear();
        for (int time=0; time<numYears; ++time) {
            if (unscaledMatrix(time,species) != nmfConstantsMSSPM::NoData) {
                tmp.push_back(unscaledMatrix(time,species));
            }
        }
        std::sort(tmp.begin(),tmp.end());
        minValues[species] = tmp.front();
        maxValues[species] = tmp.back();
    }

    // Rescale each column of the matrix with (x - min)/(max-min) formula.
    for (int species=0; species<numSpecies; ++species) {
        minVal = minValues[species];
        maxVal = maxValues[species];
        den    = maxVal - minVal;
        for (int time=0; time<numYears; ++time) {
            if (unscaledMatrix(time,species) != nmfConstantsMSSPM::NoData) {
                rescaledMatrix(time,species) = (den != 0) ? (unscaledMatrix(time,species) - minVal) / den : 0;
            } else {
                rescaledMatrix(time,species) = nmfConstantsMSSPM::NoData;
            }
        }
    }
}

void
rescaleMatrixZScore(const boost::numeric::ublas::matrix<double> &unscaledMatrix,
                          boost::numeric::ublas::matrix<double> &rescaledMatrix)
{
    int numYears   = rescaledMatrix.size1();
    int numSpecies = rescaledMatrix.size2();
    int numYearsWithoutBlanks = 0; // Not equal to numYears since there may be blanks in observed biomass
    double avgVal;
    double val;
    double sigVal;
    double diff;
    std::vector<double> avgValues(numSpecies,0);
    std::vector<double> sigma(numSpecies,0);

    // Find min,max values for each column of matrix
    for (int species=0; species<numSpecies; ++species) {
        avgVal = 0;
        numYearsWithoutBlanks = 0;
        for (int time=0; time<numYears; ++time) {
            if (unscaledMatrix(time,species) != nmfConstantsMSSPM::NoData) {
                avgVal += unscaledMatrix(time,species);
                ++numYearsWithoutBlanks;
            }
        }
        avgVal /= numYearsWithoutBlanks;
        avgValues[species] = avgVal;
    }

    // Find standard deviation
    for (int species=0; species<numSpecies; ++species) {
        val = 0;
        numYearsWithoutBlanks = 0;
        for (int time=0; time<numYears; ++time) {
            if (unscaledMatrix(time,species) != nmfConstantsMSSPM::NoData) {
                diff = (unscaledMatrix(time,species) - avgVal);
                val += diff*diff;
                ++numYearsWithoutBlanks;
            }
        }
        val /= numYearsWithoutBlanks;
        sigma[species] = std::sqrt(val);
    }

    // Rescale each column of the matrix with (x - min)/(sigma) formula.
    for (int species=0; species<numSpecies; ++species) {
        avgVal = avgValues[species];
        sigVal = sigma[species];
        for (int time=0; time<numYears; ++time) {
            if (unscaledMatrix(time,species) != nmfConstantsMSSPM::NoData) {
                rescaledMatrix(time,species) = (unscaledMatrix(time,species) - avgVal) / sigVal; // standardization or z score normalization
            } else {
                rescaledMatrix(time,species) =  nmfConstantsMSSPM::NoData;
            }
        }
    }
}

void
reset(boost::numeric::ublas::matrix<double>& mat,
           const double& resetValue)
{
    for (int i=0; i<int(mat.size1()); ++i) {
        for (int j=0; j<int(mat.size2()); ++j) {
            mat(i,j) = resetValue;
        }
    }
}

double round(double number, int decimalPlaces)
{
    if (decimalPlaces < 0) {
        std::cout << "Error: decimalPlaces argument in nmfUtils::round() function must be >= 0." << std::endl;
        decimalPlaces = 0;
    }

    double factor = std::pow(10,decimalPlaces);
    return  std::floor(number*factor+0.5)/factor;
}

void split(std::string main, std::string delim, std::string &str1, std::string &str2)
{
    if (main.empty()) {
        return;
    }
    std::string main2 = main;
    size_t sep = main2.find(delim);
    if (sep == 0) {
        return;
    }

    str1 = main2.erase(sep,main2.size());
    main2 = main;
    str2 = main2.erase(0,sep+1);
}

}
