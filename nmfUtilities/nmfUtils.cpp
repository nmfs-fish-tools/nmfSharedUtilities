
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
        if (covariateAlgorithmType == nmfConstantsMSSPM::Linear) {
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

std::string
convertValues1DToOutputStr(const std::string& label,
                           const std::vector<double> &Values,
                           const bool& includeTotal)
{
    double val;
    double totalVal = 0;
    std::string bestFitnessStr = "";

    bestFitnessStr += "<br>"+label;
    bestFitnessStr += "<table>";
    bestFitnessStr += "<tr>";
    for (unsigned i=0; i<Values.size(); ++i) {
        val = Values[i];
        bestFitnessStr += "<td> "+convertToScientificNotation(val) + "</td>";
        totalVal += val;
    }
    bestFitnessStr += "</tr>";
    bestFitnessStr += "</table>";
    if (includeTotal) {
        bestFitnessStr += "<br>Total " + label + "<br>" +
                convertToScientificNotation(totalVal) + "<br>";
    }

    return bestFitnessStr;
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
        if (runBox.parameter == ParameterName) {
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
    for (unsigned i = 0; i < mat.size1(); ++i) {
        std::cout << "  ";
        for (unsigned j = 0; j < mat.size2(); ++j) {
            std::cout << std::setw(width)
                      << std::setprecision(precision)
                      << std::fixed << mat(i, j) << "  ";
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

/*
 * Rescales matrix Xij by: log base 10 (X)
 */
void
rescaleMatrixLog(boost::numeric::ublas::matrix<double> &matrix)
{
    int NumRows = matrix.size1();
    int NumCols = matrix.size2();

    // Rescale the matrix using the min and max values found for each species
    for (auto species=0; species<NumCols; ++species) {
        for (auto time=0; time<NumRows; ++time) {
            if (matrix(time,species) != nmfConstantsMSSPM::NoData) {
                matrix(time,species) = std::log10(matrix(time,species));
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
