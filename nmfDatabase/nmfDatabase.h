
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


#ifndef NMFDATABASE_H_
#define NMFDATABASE_H_

#include "nmfLogger.h"
#include "nmfUtilsQt.h"
#include "nmfConstantsMSSPM.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

/*
#include "fileIO.h"
#include "mySqlIO.h"
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
*/
#include <boost/numeric/ublas/matrix.hpp>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>

/**
 * @brief Allows user to interact with MySQL database
 *
 * This class allows the user to interact with the MySQL database stored locally
 * on the user's machine. The user can create or delete databases or tables and
 * read from or write to specific database tables. Before using this class the
 * user must assure that MySQL is installed and properly working on the host machine.
 *
 */
class nmfDatabase {

private:
    QString m_dbName;
    std::map<QString, void(*)(
            QString &table,
            QString &cmd
            )> m_FunctionMap;

public:

    /**
     * @brief Class constructor
     */
    nmfDatabase();
   ~nmfDatabase() {}

    /**
     * @brief authenticateDatabase - checks to see if database to be loaded is compatible with the application
     * @param databaseName - database name to be loaded
     * @return true or false signifying if database to be loaded is compatible with the application
     */
    bool authenticateDatabase(const std::string& databaseName);
    /**
     * @brief Checks for table and if not found, creates it (MSVPA)
     * @param tableName : table name to check/create
     */
    void checkForTableAndCreate(QString tableName);
    /**
     * @brief Checks that the vector has an indexth element and returns it
     * @param index : index of element to return
     * @param numValues : number of values to check that the vector has
     * @param vec : vector of values
     * @return Returns the indexth element of the vector
     */
    double checkForValues(
            const int& index,
            const int& numValues,
            const std::vector<double>& vec);
    /**
     * @brief Clears the corresponding table
     * @param Logger : logger pointer to log any errors
     * @param TableName : name of table to clear
     * @return True if cleared ok, false otherwise
     */
    bool clearTable(nmfLogger* Logger,
                    const std::string& TableName);
    /**
     * @brief Clears the corresponding table
     * @param Logger : logger pointer to log any errors
     * @param TableName : name of table to clear
     * @param ProjectName : name of project to clear inside table
     * @return
     */
    bool clearTable(nmfLogger* Logger,
                    const std::string& TableName,
                    const std::string& ProjectName);
    /**
     * @brief Removes all data associated with the passed project and model names from all of the relevant tables
     * @param logger : application logger
     * @param projectName : name of current project
     * @param modelName : name of model just deleted
     * @return true for successful delete and false otherwise
     */
    bool cleanTables(
            nmfLogger* logger,
            const std::string& projectName,
            const std::string& modelName);
    /**
     * @brief Creates a map of Scenario names and Forecast Labels that belong to each Scenario.
     * @param ProjectName : name of project
     * @param ModelName : name of model
     * @param ScenarioForecastMap : map to be returned
     */
    void createScenarioMap(
            const std::string& ProjectName,
            const std::string& ModelName,
            std::map<QString,QStringList>& ScenarioForecastMap);
    /**
     * @brief Determines if passed database exists
     * @param dbName : name of database
     * @return True if database exists, false otherwise
     */
    bool databaseExists(std::string dbName);
    /**
     * @brief Exports the passed database to a .sql output file
     * @param widget : widget over which to show any popups
     * @param ProjectDir : project directory in which to write the output database .sql file
     * @param Username : current user name
     * @param Password : current password
     * @param ProjectDatabase : name of the database to export
     */
    void exportDatabase(QWidget*     widget,
                        std::string& ProjectDir,
                        std::string& Username,
                        std::string& Password,
                        std::string& ProjectDatabase);
    /**
     * @brief Gets the identifying information for the current estimation algorithm selected
     * @param widget : widget over which to show any popups
     * @param logger : logger pointer used to log any errors
     * @param ProjectName : name of project
     * @param ModelName : name of model
     * @param Algorithm : name of estimation algorithm family
     * @param Minimizer : name of Minimizer algorithm
     * @param ObjectiveCriterion : name of objective criterion desired
     * @param Scaling : name of scaling algorithm desired
     * @param CompetitionForm : type of competition (for future use with AggProd functionality)
     * @param showPopupError : flag to show/hide any popup error
     * @return True if all identifying information was found from the database, false otherwise
     */
    bool getAlgorithmIdentifiers(
            QWidget*     widget,
            nmfLogger*   logger,
            const std::string& ProjectName,
            const std::string& ModelName,
            std::string& Algorithm,
            std::string& Minimizer,
            std::string& ObjectiveCriterion,
            std::string& Scaling,
            std::string& CompetitionForm,
            const bool&  showPopupError);
    /**
     * @brief Gets all the guilds in the current project
     * @param logger : logger pointer used to log any error messages
     * @param guilds : vector of guilds in the current project
     * @return True if some guilds were found, false otherwise
     */
    bool getGuilds(nmfLogger* logger,
                      std::vector<std::string>& guilds);
    /**
     * @brief Gets all the guilds in the current project
     * @param logger : logger pointer used to log any error messages
     * @param NumGuilds : number of guilds found
     * @param GuildList : list of guild names found
     * @return True if some guilds were found, false otherwise
     */
    bool getGuilds(
            nmfLogger* logger,
            int &NumGuilds,
            QStringList &GuildList);
    /**
     * @brief Gets all the species in the current project
     * @param logger : logger pointer used to log any error messages
     * @param species : vector of species in the current project
     * @return True if some species were found, false otherwise
     */
    bool getSpecies(nmfLogger* logger,
                       std::vector<std::string>& species);
    /**
     * @brief Gets all the species in the current project
     * @param logger : logger pointer used to log any error messages
     * @param NumSpecies : number of species found
     * @param SpeciesList : list of species names found
     * @return True if some species were found, false otherwise
     */
    bool getSpecies(
            nmfLogger* logger,
            int &NumSpecies,
            QStringList &SpeciesList);
    /**
     * @brief getAllTables - finds all of the table names in the current database
     * @param databaseTables - the list of table names in current database
     * @return true if at least one table was found, false otherwise
     */
    bool getAllTables(std::vector<std::string>& databaseTables);

    /**
     * @brief Returns the competition data associated with the passed competition type
     * @param CompetitionType : type of competition
     * @param Logger : logger pointer used to log error messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param NumSpecies : number of species
     * @param NumGuilds : number of guilds
     * @param CompetitionAlpha : matrix holding any competition alpha data to be returned
     * @param CompetitionBetaSpecies : matrix holding any competition beta species data to be returned
     * @param CompetitionBetaGuild : matrix holding any competition beta guild data to be returned
     * @param CompetitionBetaGuildGuild : matrix holding any competition beta guild guild data to be returned
     * @return True if competition data were successfully read from the database
     */
    bool getCompetitionData(const std::string& CompetitionType,
                            nmfLogger* Logger,
                            const std::string& ProjectName,
                            const std::string& ModelName,
                            const int& NumSpecies,
                            const int& NumGuilds,
                            boost::numeric::ublas::matrix<double>& CompetitionAlpha,
                            boost::numeric::ublas::matrix<double>& CompetitionBetaSpecies,
                            boost::numeric::ublas::matrix<double>& CompetitionBetaGuild,
                            boost::numeric::ublas::matrix<double>& CompetitionBetaGuildGuild);
    /**
     * @brief Sends back the estimated biomass from the passed table and algorithm identifiers
     * @param NumSpecies : number of species
     * @param RunLength : run length in years
     * @param TableName : name of estimated biomass table
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param Algorithm : name of current algorithm family
     * @param Minimizer : name of current minimizer algorithm
     * @param ObjectiveCriterion : name of current objective criterion
     * @param Scaling : name of current scaling algorithm
     * @param isAggProd : True if is an AggProd model
     * @param EstBiomass : matrix of estimated (i.e., calculated) biomass to pass back
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getEstimatedBiomass(
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
            boost::numeric::ublas::matrix<double>& EstBiomass);
    /**
     * @brief Sends back the estimated parameter values from the passed table and algorithm identifiers
     * @param TableName : name of estimated parameter table
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param Algorithm : name of current algorithm family
     * @param Minimizer : name of current minimizer algorithm
     * @param ObjectiveCriterion : name of current objective criterion
     * @param Scaling : name of current scaling algorithm
     * @param isAggProd : True if is an AggProd model
     * @param EstParameter : vector of the estimated parameter to pass back
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getEstimatedParameter(
            const std::string& TableName,
            const std::string& ProjectName,
            const std::string& ModelName,
            const std::string& Algorithm,
            const std::string& Minimizer,
            const std::string& ObjectiveCriterion,
            const std::string& Scaling,
            const std::string& isAggProd,
            std::vector<double>& EstParameter);
    /**
     * @brief Sends back the forecast biomass for the passed in forecast name
     * @param Widget : widget over which to display any popups
     * @param Logger : logger pointer to log any error messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param ForecastName : name of current forecast
     * @param NumSpecies : number of species
     * @param RunLength : run length in years
     * @param Algorithm : name of current algorithm family
     * @param Minimizer : name of current minimizer algorithm
     * @param ObjectiveCriterion : name of current objective criterion
     * @param Scaling : name of current scaling algorithm
     * @param ForecastBiomass : matrix of the forecast biomass to pass back
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getForecastBiomass(
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
            std::vector<boost::numeric::ublas::matrix<double> >& ForecastBiomass);
    /**
     * @brief Sends back the Monte Carlo forecast biomass for the passed in forecast name
     * @param Widget : widget over which to display any popups
     * @param Logger : logger pointer to log any error messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param ForecastName : name of current forecast
     * @param NumSpecies : number of species
     * @param RunLength : run length in years
     * @param NumRuns : number of Monte Carlo runs
     * @param Algorithm : name of current algorithm family
     * @param Minimizer : name of current minimizer algorithm
     * @param ObjectiveCriterion : name of current objective criterion
     * @param Scaling : name of current scaling algorithm
     * @param ForecastBiomassMonteCarlo
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getForecastBiomassMonteCarlo(
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
            std::vector<boost::numeric::ublas::matrix<double> >& ForecastBiomassMonteCarlo);
    /**
     * @brief Sends back the Harvest data to be used for the forecast
     * @param Widget : widget over which to display any popups
     * @param Logger : logger pointer to log any error messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param ForecastName : name of current forecast
     * @param NumSpecies : number of species
     * @param RunLength : run length in years
     * @param Algorithm : name of current algorithm family
     * @param Minimizer : name of current minimizer algorithm
     * @param ObjectiveCriterion : name of current objective criterion
     * @param Scaling : name of current scaling algorithm
     * @param HarvestForm : name of the type of harvest
     * @param HarvestData : matrix of forecast harvest data
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getForecastHarvest(
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
            std::vector<boost::numeric::ublas::matrix<double> >& HarvestData);
    /**
     * @brief Sends back information about the passed in forecast
     * @param TableName : name of table containing the forecast information
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param ForecastName : name of current forecast
     * @param RunLength : forecast run length in years (passed back)
     * @param StartForecastYear : initial year of the forecast (passed back)
     * @param Algorithm : name of forecast algorithm family (passed back)
     * @param Minimizer : name of forecast minimizer algorithm (passed back)
     * @param ObjectiveCriterion : name of forecast objective criterion (passed back)
     * @param Scaling : name of forecast scaling algorithm (passed back)
     * @param NumRuns : number of runs in forecast (passed back)
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getForecastInfo(
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
            int&               NumRuns);
    /**
     * @brief Sends back the hover text data, for each monte carlo run, that describes the parameters used for the run
     * @param widget : widget over which to display any popup messages
     * @param logger : logger pointer to log any error messages
     * @param Species : current species name
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param ForecastName : name of current forecast
     * @param Algorithm : name of forecast algorithm family
     * @param Minimizer : name of forecast minimizer algorithm
     * @param ObjectiveCriterion : name of forecast objective criterion
     * @param Scaling : name of forecast scaling algorithm
     * @param HoverData : list of hover data strings for each monte carlo run
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getForecastMonteCarloHoverData(
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
            QStringList&         HoverData);
    /**
     * @brief Sends back data associated with each guild
     * @param Logger : logger pointer used to log any error messages
     * @param NumGuilds : number of guilds
     * @param RunLength : length of run in years
     * @param GuildList : list of guild names
     * @param GuildSpecies : map of species per guild number
     * @param GuildNum : vector of guild number per species (i.e. shows which guild contains which species)
     * @param ObservedBiomassByGuilds : matrix containing observed biomass per guild
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getGuildData(nmfLogger* Logger,
                      const int& NumGuilds,
                      const int& RunLength,
                      const QStringList& GuildList,
                      std::map<int,std::vector<int> >& GuildSpecies,
                      std::vector<int>& GuildNum,
                      boost::numeric::ublas::matrix<double>& ObservedBiomassByGuilds);
    /**
     * @brief Sends back harvest data associated with the passed in harvest type
     * @param HarvestType : type of harvest data (used to derive harvest table name)
     * @param Logger : logger pointer used to log any error messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param NumSpecies : number of species
     * @param RunLength : length of run in years
     * @param Catch : matrix of any Catch data associated with this harvest type
     * @param Effort : matrix of any Effort data associated with this harvest type
     * @param Exploitation : matrix of any Exploitation data associated with this harvest type
     * @param Catchability : vector of any Catchability data associated with this harvest type
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getHarvestData(const std::string& HarvestType,
                        nmfLogger* Logger,
                        const std::string& ProjectName,
                        const std::string& ModelName,
                        const int& NumSpecies,
                        const int& RunLength,
                        boost::numeric::ublas::matrix<double>& Catch,
                        boost::numeric::ublas::matrix<double>& Effort,
                        boost::numeric::ublas::matrix<double>& Exploitation,
                        std::vector<double>& Catchability);
    /**
     * @brief Sends back harvest data associated with the passed in harvest type; harvest data may be a combination of other terms (i.e., catchability, effort)
     * @param parent : widget over which to display any popup messages
     * @param logger : logger pointer used to log any error messages
     * @param projectName : name of current project
     * @param modelName : name of current model
     * @param numSpeciesOrGuilds : number of species or guilds
     * @param divideByOutputBiomass : boolean describing whether to divide harvest data by output biomass
     * @param multiplyByOutputBiomass : boolean describing whether to multiply harvest data by output biomass
     * @param multiplyByCatchability : boolean describing whether to multiply harvest data by catchability
     * @param EstCatchability : estimated catchability values to use for calculations
     * @param OutputBiomass : output biomass to use for calculations
     * @param harvestData : resultant harvest data to pass back to calling function
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getHarvestData(QWidget* parent,
                        nmfLogger* logger,
                        const std::string& projectName,
                        const std::string& modelName,
                        const int& numSpeciesOrGuilds,
                        const bool& divideByOutputBiomass,
                        const bool& multiplyByOutputBiomass,
                        const bool& multiplyByCatchability,
                        const std::vector<double>& EstCatchability,
                        const std::vector<boost::numeric::ublas::matrix<double> >& OutputBiomass,
                        boost::numeric::ublas::matrix<double>& harvestData);
    /**
     * @brief Sends back harvest data by guild
     * @param logger : logger pointer used to log any error messages
     * @param projectName : name of current project
     * @param modelName : name of current model
     * @param outputChartType : type of harvest output chart desired
     * @param numSpeciesOrGuilds : number of species or guilds displayed
     * @param chartLabel : chart label to display
     * @param outputBiomass : the output biomass
     * @param harvestData : the harvest data to pass back
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getHarvestDataByGuild(
            nmfLogger* logger,
            const std::string& projectName,
            const std::string& modelName,
            const QString& outputChartType,
            const int& numSpeciesOrGuilds,
            std::string& chartLabel,
            boost::numeric::ublas::matrix<double>& outputBiomass,
            boost::numeric::ublas::matrix<double>& harvestData);
    /**
     * @brief Sends back the harvest form data associated with the passed in parameters
     * @param logger : logger pointer used to log any error messages
     * @param projectName : name of current project
     * @param modelName : name of current model
     * @param runLength : run length in years
     * @param harvestForm : harvest form associated with preceding parameters
     * @return True if database query and fetch were successful, false otherwise
     */
    bool getHarvestFormData(
            nmfLogger* logger,
            const std::string& projectName,
            const std::string& modelName,
            int& runLength,
            std::string& harvestForm);
    /**
     * @brief getListOfAuthenticatedDatabaseNames - returns list of databases that are appropriate for this application
     * @param authenticatedDatabases - the list of authenticated databases
     * @return True if found at least one database, false otherwise
     */
    bool getListOfAuthenticatedDatabaseNames(
            QList<QString>& authenticatedDatabases);
    /**
     * @brief Sends back the current model form data for the passed in project and model name
     * @param Logger : logger pointer used to log any error messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param GrowthForm : growth form associated with the above project and model name
     * @param HarvestForm : harvest form associated with the above project and model name
     * @param CompetitionForm : competition form associated with the above project and model name
     * @param PredationForm : predation form associated with the above project and model name
     * @param RunLength : number of years in run associated with the above project and model name
     * @param InitialYear : first year in run associated with the above project and model name
     * @return true if no errors reading table data, false otherwise
     */
    bool getModelFormData(nmfLogger*   Logger,
                          const std::string& ProjectName,
                          const std::string& ModelName,
                          std::string& GrowthForm,
                          std::string& HarvestForm,
                          std::string& CompetitionForm,
                          std::string& PredationForm,
                          int&         RunLength,
                          int&         InitialYear,
                          bool&        isBiomassAbsolute);
    /**
     * @brief Sends back mortality data (for MSCAA)
     * @param logger : logger pointer used to log any error messages
     * @param ModelName : name of current model
     * @param Species : name of selected species
     * @param NumYears : number of years in data (x-dimension)
     * @param NumAges : number of age groups in data (y-dimension)
     * @param TableName : name of database table
     * @param TableData : matrix sent back with the mortality data
     * @return true if no errors reading table data, false otherwise
     */
    bool getMortalityData(
            nmfLogger* logger,
            const std::string& ModelName,
            const std::string &Species,
            const int &NumYears,
            const int &NumAges,
            const std::string TableName,
            boost::numeric::ublas::matrix<double> &TableData);
    /**
     * @brief Sends back predation data depending upon the predation type passed in
     * @param PredationType : type of predation
     * @param Logger : logger pointer used to log any errors
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param NumSpecies : number of species
     * @param Rho : matrix containing any predation rho values
     * @param Handling : matrix containing any predation handling values
     * @param Exponent : vector containing any predation exponent values
     * @return true if no errors reading table data, false otherwise
     */
    bool getPredationData(const std::string& PredationType,
                          nmfLogger* Logger,
                          const std::string& ProjectName,
                          const std::string& ModelName,
                          const int& NumSpecies,
                          boost::numeric::ublas::matrix<double>& Rho,
                          boost::numeric::ublas::matrix<double>& Handling,
                          std::vector<double>& Exponent);
    /**
     * @brief Sends back the run length and start year
     * @param logger : logger pointer used to log any errors
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param RunLength : run length from database
     * @param StartYear : start year from database
     * @return true if no errors reading table data, false otherwise
     */
    bool getRunLengthAndStartYear(
            nmfLogger* logger,
            const std::string& ProjectName,
            const std::string& ModelName,
            int &RunLength,
            int &StartYear);
    /**
     * @brief Sends back data about the passed in species (MSCAA)
     * @param logger : logger pointer used to log any errors
     * @param species : name of species whose information is desired
     * @param MinAge : minimum age group
     * @param MaxAge : maximum age group
     * @param FirstYear : first year of data
     * @param LastYear : last year of data
     * @param MinLength : minimum length
     * @param MaxLength : maximum length
     * @param NumLengthBins : number of length groups
     * @return true if no errors reading table data, false otherwise
     */
    bool getSpeciesData(nmfLogger*   logger,
                        const std::string& species,
                        int&         MinAge,
                        int&         MaxAge,
                        int&         FirstYear,
                        int&         LastYear,
                        float&       MinLength,
                        float&       MaxLength,
                        int&         NumLengthBins);
    /**
     * @brief Sends back initial data for all species
     * @param Logger : logger pointer to log any error messages
     * @param NumSpecies : number of species found
     * @param SpeciesList : list of species names found
     * @param InitBiomass : initial biomass for all species
     * @param GrowthRate : growth rate for all species
     * @param SpeciesK : carrying capacity for all species
     * @return true if no errors reading table data, false otherwise
     */
    bool getSpeciesInitialData(nmfLogger* Logger,
                               int& NumSpecies,
                               QStringList& SpeciesList,
                               std::vector<double>& InitBiomass,
                               std::vector<double>& GrowthRate,
                               std::vector<double>& SpeciesK);
    /**
     * @brief Creates and returns a species guild map, that is, a map of what species are in what guild
     * @param SpeciesGuildMap : the species guild string map
     */
    void getSpeciesGuildMap(std::map<std::string,std::string>& SpeciesGuildMap);
    /**
     * @brief Returns the index number associated with the passed species name (MSVPA)
     * @param SpeciesName : name of species
     * @return Species index corresponding to passed in species name
     */
    int getSpeciesIndex(std::string SpeciesName);
    /**
     * @brief getSurveyQData : Gets the Survey Q data from the models table
     * @param Logger : logger pointer to log any error messages
     * @param SurveyQ : vector of Survey Q data
     * @return true if no errors, false otherwise
     */
    bool getSurveyQData(
            nmfLogger* Logger,
            std::vector<double>& SurveyQ);
    /**
     * @brief Creates and returns a species name to index map (MSVPA)
     * @param SpeciesToIndexMap : the species to index map
     */
    void getSpeciesToIndexMap(std::map<std::string,int>& SpeciesToIndexMap);
    /**
     * @brief Sends back the time series (by species) associated with the passed table information
     * @param Widget : widget over which to display any popups
     * @param Logger : logger pointer to log any error messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param ForecastName : name of forecast (if it's forecast data)
     * @param TableName : name of table
     * @param NumSpecies : number of species
     * @param RunLength : length of run in years
     * @param TableData : matrix(time,species) containing the time series data
     * @return true if no errors reading table data, false otherwise
     */
    bool getTimeSeriesData(
            QWidget*           Widget,
            nmfLogger*         Logger,
            const std::string& ProjectName,
            const std::string& ModelName,
            const std::string  ForecastName,
            const std::string& TableName,
            const int&         NumSpecies,
            const int&         RunLength,
            boost::numeric::ublas::matrix<double>& TableData);
    /**
     * @brief Sends back the time series (by guild) associated with the passed table information
     * @param logger : logger pointer to log any error messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param ForecastName : name of forecast (if it's forecast data)
     * @param TableName : name of table
     * @param NumGuilds : number of guilds
     * @param RunLength : length of run in years
     * @param OutputChartType : type of output chart
     * @param Biomass : output biomass (needed for exploitation calculations)
     * @param TableData : matrix(time,guild) containing the time series data
     * @return True if no errors reading data, false otherwise
     */
    bool getTimeSeriesDataByGuild(
            nmfLogger* logger,
            const std::string& ProjectName,
            const std::string& ModelName,
            const std::string& ForecastName,
            const std::string& TableName,
            const int &NumGuilds,
            const int &RunLength,
            const QString& OutputChartType,
            boost::numeric::ublas::matrix<double> &Biomass,
            boost::numeric::ublas::matrix<double> &TableData);
    /**
     * @brief Returns list of current estimated parameter names
     * @param logger : logger pointer to log any error messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @return List of parameter names
     */
    QStringList getVectorParameterNames(
            nmfLogger*         logger,
            const std::string& ProjectName,
            const std::string& ModelName);
    /**
     * @brief Queries user and imports a .sql database from disk
     * @param widget : widget over which to display any popup widgets
     * @param logger ; logger pointer used to log any error messages
     * @param ProjectDir : project directory in which to look for the databases subdirectory from which to do the import
     * @param Username : username (needed to run the import command)
     * @param Password : password (needed to run the import command)
     * @return Complete filename (including path) of the .sql database file loaded
     */
    QString importDatabase(QWidget*     widget,
                           nmfLogger*   logger,
                           std::string& ProjectDir,
                           std::string& Username,
                           std::string& Password);
    /**
     * @brief Determines if the currently loaded model is set to relative biomass
     * @param projectName : name of project
     * @param modelName : name of model
     * @return true if data refers to relative biomass, false otherwise
     */
    bool isARelativeBiomassModel(const std::string& projectName,
                                 const std::string& modelName);
    /**
     * @brief Loads the estimated parameter names into the passed combo box widget
     * @param logger ; logger pointer used to log any error messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param cmbox : combo box in which to load the estimated parameter names
     */
    void loadEstimatedVectorParameters(
            nmfLogger*         logger,
            const std::string& ProjectName,
            const std::string& ModelName,
            QComboBox*         cmbox);
    /**
     * @brief Convenience routine to load a matrix from a passed in vector of table data maps
     * @param logger ; logger pointer used to log any error messages
     * @param dataMaps : table data maps to extract data from
     * @param numRows : number of rows in table
     * @param numCols : number of columns in table
     * @param matrix : matrix containing the table data
     * @return true if correct number of table records were found, false otherwise
     */
    bool loadMatrix(
            nmfLogger* logger,
            const std::vector<std::map<std::string, std::vector<std::string> > >& dataMaps,
            const int& numRows,
            const int& numCols,
            boost::numeric::ublas::matrix<double>& matrix);
    /**
     * @brief Convenience routine to load a vector from a passed in vector of table data maps
     * @param logger ; logger pointer used to log any error messages
     * @param dataMaps : table data maps to extract data from
     * @param numRows : number of elements in table
     * @param vector : vector containing the table data
     * @return true if correct number of table records were found, false otherwise
     */
    bool loadVector(
            nmfLogger* logger,
            const std::vector<std::map<std::string, std::vector<std::string> > >& dataMaps,
            const int& numRows,
            std::vector<double>& vector);
    /**
     * @brief nmfCloseDatabase
     */
    void nmfCloseDatabase();
    /**
     * @brief Deletes all records from a table (MSVPA)
     * @param table - the database table to clear
     * @param MSVPAName - the name of the MSVPA file
     * @return true if the records were deleted successfully, false otherwise
     */
    bool nmfDeleteRecordsFromTable(
            std::string table,
            std::string MSVPAName);
    /**
     * @brief Deletes all records from a table for given forecast and scenario (MSVPA)
     * @param table : the database table to clear
     * @param MSVPAName : the name of the MSVPA file
     * @param ForecastName : the name of the forecast
     * @param Scenario : the name of the scenario
     * @return true if the records were deleted successfully, false otherwise
     */
    bool nmfDeleteRecordsFromTable(std::string table,
           std::string MSVPAName,
           std::string ForecastName,
           std::string Scenario);
    /**
     * @brief Gets the name of the currently loaded database
     * @return Returns the name of the loaded database
     */
    std::string nmfGetCurrentDatabase();
    /**
     * @brief Returns vector of all of the database names
     * @return Vector containing all the database names
     */
    std::vector<std::string> nmfGetDatabaseNames();
    /**
     * @brief Sends back initial data (MSVPA)
     * @param MSVPAName : MSVPA name
     * @param NPreds : number of predators
     * @param NPrey : number of prey species
     * @param NOtherFood : number of other food species
     * @param NOtherPreds : number of other predators
     * @param FirstYear : first year of data
     * @param LastYear : last year of data
     * @param NSeas : number of seasons
     * @param NMSVPASpe : number of MSVPA species
     * @param NYears : number of years of data
     * @param FirstYearStr : first year as a string
     * @param LastYearStr : last year as a string
     * @return true
     */
    bool nmfGetInitialData(
            const std::string& MSVPAName,
            int& NPreds,
            int& NPrey,
            int& NOtherFood,
            int& NOtherPreds,
            int& FirstYear,
            int& LastYear,
            int& NSeas,
            int& NMSVPASpe,
            int& NYears,
            std::string& FirstYearStr,
            std::string& LastYearStr);
    /**
     * @brief Sends back initial data (MSVPA)
     * @param MSVPAName : MSVPA name
     * @param FirstYear : first year of data
     * @param LastYear : last year of data
     * @param NYears : number of years of data
     * @param NSeasons : number of seasons
     */
    void nmfGetMSVPAInitData(
            const std::string &MSVPAName,
            int &FirstYear,
            int &LastYear,
            int &NYears,
            int &NSeasons);
    /**
     * @brief Returns a vector of all of the tables in the currently loaded database
     * @return Vector of table names
     */
    std::vector<std::string> nmfGetTableNames();
    /**
     * @brief Opens/prepares a database for I/O.
     * @param hostName : The name of the host computer
     * @param userName : The user name
     * @param password : The MySQL password for the current user
     * @param errorMsg : If an error was encountered it'll be placed here, otherwise it'll be empty
     * @return true if database was opened successfully, otherwise false
     */
    bool nmfOpenDatabase(
            const std::string &hostName,
            const std::string &userName,
            const std::string &password,
            std::string&       errorMsg);
    /**
     * @brief Sends back the Catch data for the passed in parameters
     * @param xIndex : x index for the Catch matrix
     * @param Year : year of Catch data to extract from database table
     * @param Nage : number of age groups in Catch data (the y range of the Catch matrix)
     * @param species : species name
     * @param Catch : matrix(year,age) containing the requested Catch data
     */
    void nmfQueryAgeCatchData(
            const int &xIndex,
            const int &Year,
            const int &Nage,
            const std::string &species,
            boost::numeric::ublas::matrix<double> &Catch);
    /**
     * @brief Executes a database query of 4 commonly requested age-related fields (MSVPA)
     * @param table : name of database table
     * @param speciesIndex : species index value
     * @return std::tuple of integers containing the age fields: MaxAge, MinCatAge, MaxCatAge, PlusClass
     */
    std::tuple<int, int, int, int> nmfQueryAgeFields(
            const std::string &table, const int &speciesIndex);
    /**
     * @brief Sends back the Alpha and Beta vector parameters from the passed in table
     * @param table : table containing the data
     * @param speciesIndex : index of species
     * @param SSVPAName : name of the SSVPA
     * @param NIndex : number of parameters to pass back
     * @param Alpha : vector containing the Alpha data
     * @param Beta : vector containing the Beta data
     */
    void nmfQueryAlphaBetaFields(
            const std::string& table,
            const int& speciesIndex,
            const std::string& SSVPAName,
            const int& NIndex,
            boost::numeric::ublas::vector<double>& Alpha,
            boost::numeric::ublas::vector<double>& Beta);
    /**
     * @brief Sends back Catch data from within the passed age groups (MSVPA)
     * @param table : table containing catch data
     * @param speciesIndex : species index
     * @param FirstCatchAgeStr : smallest age group for catch data
     * @param LastCatchAgeStr : largest age group for catch data
     * @param OrderByStr : the "ORDER By" database table commande
     * @param Catch : matrix containing the relevant Catch data
     */
    void nmfQueryCatchFields(
            const std::string& table,
            const int& speciesIndex,
            const std::string& FirstCatchAgeStr,
            const std::string& LastCatchAgeStr,
            const std::string& OrderByStr,
            boost::numeric::ublas::matrix<double> &Catch);
    /**
     * @brief Executes a database query
     * @param query : the database query string
     * @param fields : the database fields to be queried
     * @param setNullToBlank : uses empty string if table field is NULL, else uses 0
     * @return the data map containing the table data
     */
    std::map< std::string, std::vector< std::string> > nmfQueryDatabase(
            const std::string &query,
            const std::vector<std::string> &fields,
            const bool setNullToBlank=false);
    /**
     * @brief Sends back forecast Catch data for the passed in arguments (MSVPA)
     * @param MSVPAName : name of MSVPA
     * @param ForecastName : name of forecast
     * @param ScenarioName : name of scenario
     * @param Species : name of species
     * @param Year : year of data
     * @param Nage : age group of data
     * @param index : x index for catch data matrix
     * @param Catch : matrix with Catch data
     */
    void nmfQueryForecastAgeCatchData(
            const std::string& MSVPAName,
            const std::string& ForecastName,
            const std::string& ScenarioName,
            const std::string& Species,
            const int& Year,
            const int& Nage,
            const int& index,
            boost::numeric::ublas::matrix<double>& Catch);
    /**
     * @brief Sends back forecast mortality data for the passed in arguments (MSVPA)
     * @param MSVPAName : name of MSVPA
     * @param ForecastName : name of forecast
     * @param ScenarioName : name of scenario
     * @param Species : name of species
     * @param Year : year of data
     * @param Nage : age group of data
     * @param index : x index for mortality matrix
     * @param FatAge : matrix with Fishing mortality at age data
     * @param M1atAge : matrix with M1 mortality at age data
     * @param M2atAge : matrix with M2 mortality at age data
     */
    void nmfQueryForecastMortalityData(
            const std::string& MSVPAName,
            const std::string& ForecastName,
            const std::string& ScenarioName,
            const std::string& Species,
            const int& Year,
            const int& Nage,
            const int& index,
            boost::numeric::ublas::matrix<double>& FatAge,
            boost::numeric::ublas::matrix<double>& M1atAge,
            boost::numeric::ublas::matrix<double>& M2atAge);
    /**
     * @brief Sends back forecast prey list for the passed in predator species  (MSVPA)
     * @param MSVPAName : name of MSVPA
     * @param ForecastName : name of forecast
     * @param ScenarioName : name of scenario
     * @param PredatorSpecies : name of predatorspecies
     * @param includePredAge : flag to include predator age clause
     * @param PredAge : predator age clause
     * @param seasonStr : string with current season
     * @param PreyList : matrix containing list of prey names
     */
    void nmfQueryForecastPreyList(
            const std::string& MSVPAName,
            const std::string& ForecastName,
            const std::string& ScenarioName,
            const std::string& PredatorSpecies,
            const bool& includePredAge,
            const std::string& PredAge,
            const std::string& seasonStr,
            QStringList& PreyList);
    /**
     * @brief Sends back forecast weight at age data  (MSVPA)
     * @param MSVPAName : name of MSVPA
     * @param ForecastName : name of forecast
     * @param ScenarioName : name of scenario
     * @param Species : name of species
     * @param Year : year of data
     * @param Nage : age group of data
     * @param index : x index for weight at age matrix
     * @param WtAtAge : matrix containing weight at age data
     */
    void nmfQueryForecastWeightAtAgeData(
            const std::string& MSVPAName,
            const std::string& ForecastName,
            const std::string& ScenarioName,
            const std::string& Species,
            const int& Year,
            const int& Nage,
            const int& index,
            boost::numeric::ublas::matrix<double>& WtAtAge);
    /**
     * @brief Returns a map of initial field values (MSVPA)
     * @param table : database table to query
     * @param MSVPAName : name of MSVPA
     * @return map of field name and value
     */
    std::map<std::string, int> nmfQueryInitFields(
            const std::string& table,
            const std::string& MSVPAName);
    /**
     * @brief Sends back the maturity data using the passed arguments (MSVPA)
     * @param index : x index for the maturity matrix
     * @param Year : year to use for database query
     * @param Nage : age group to use for database query
     * @param Species : species to user for database query
     * @param Pmature : matrix containing the maturity probability data
     */
    void nmfQueryMaturityData(
            const int& index,
            const int& Year,
            const int& Nage,
            const std::string& Species,
            boost::numeric::ublas::matrix<double>& Pmature);
    /**
     * @brief Sends back mortality data (MSVPA)
     * @param MSVPAName : name of MSVPA
     * @param index : x index for mortality matrices
     * @param Year : year to use for database query
     * @param Nage : age group to use for database query
     * @param species : species to user for database query
     * @param FatAge : matrix with Fishing mortality at age data
     * @param M1atAge : matrix with M1 mortality at age data
     * @param M2atAge : matrix with M2 mortality at age data
     */
    void nmfQueryMortalityData(
            const std::string& MSVPAName,
            const int& index,
            const int& Year,
            const int& Nage,
            const std::string& species,
            boost::numeric::ublas::matrix<double>& FatAge,
            boost::numeric::ublas::matrix<double>& M1atAge,
            boost::numeric::ublas::matrix<double>& M2atAge);
    /**
     * @brief Sends back the Prey list for the passed in predator species (MSVPA)
     * @param selectedPredator : selected predator species
     * @param MSVPAName : name of MSVPA
     * @param includedPredAge : boolean flag to include predator age
     * @param selectedSpeciesAgeSizeClass : age size class for the selected predator species
     * @param PredAge : age group of predator
     * @param NPrey : number of prey
     * @param PreyList : list of prey names
     */
    void nmfQueryMsvpaPreyList(
            const std::string& selectedPredator,
            const std::string& MSVPAName,
            const bool& includedPredAge,
            std::string& selectedSpeciesAgeSizeClass,
            std::string& PredAge,
            int& NPrey,
            QStringList& PreyList);
    /**
     * @brief Sends back the natural mortality data based upon the passed in arguments (MSVPA)
     * @param SpeciesIndex : Species index
     * @param SSVPAIndex : SSVPA index
     * @param MaxCatchAge : maximum catch age group
     * @param NCatchYears : number of catch years
     * @param NaturalMortality : matrix of natural mortality data
     */
    void nmfQueryNaturalMortalityData(
            const int &SpeciesIndex,
            const int &SSVPAIndex,
            const int &MaxCatchAge,
            const int &NCatchYears,
            boost::numeric::ublas::matrix<double> &NaturalMortality);
    /**
     * @brief Creates and returns map of predator, prey, and otherPredator groups (MSVPA)
     * @param table : database table containing the relevant information
     * @param MSVPAName : name of MSVPA group
     * @return Map of species in predator, prey, otherPredator groups
     */
    std::map<std::string, std::vector<std::string> > nmfQueryPredatorPreyFields(
            const std::string &table,
            const std::string &MSVPAName);
    /**
     * @brief Sends back a Weight at Age matrix for the passed in values
     * @param index : x index for the Weight at Age matrix
     * @param Year : the year of interest for the Weight at Age matrix
     * @param Nage : the age group for the Weight at Age matrix
     * @param species : the species in question
     * @param WtAtAge : the matrix containing the Weight at Age matrix
     * @return true if table data were found, false otherwise
     */
    bool nmfQueryWeightAtAgeData(
            const int &index,
            const int &Year,
            const int &Nage,
            const std::string &species,
            boost::numeric::ublas::matrix<double> &WtAtAge);
    /**
     * @brief Sets the database connection by using the passed in connection name
     * @param name - database connection name (i.e., something like: qt_sql_default_connection)
     */
    void nmfSetConnectionByName(QString name);
    /**
     * @brief Sets the current database to the passed in database name
     * @param newDatabaseName : name of database to set as the current database
     */
    void nmfSetDatabase(std::string newDatabaseName);
    /**
     * @brief Executes a non-query database command (i.e. delete).
     * @param query : the database query string
     * @return std::string error msg (if any)
     */
    std::string nmfUpdateDatabase(std::string qry);
    /**
     * @brief Restores the appropriate CSV from from the given table
     * @param TableName : the MySQL table whose CSV file is to be restored
     * @param ProjectDir : the project directory for the current session
     * @param fields : vector of table field names
     */
    void RestoreCSVFile(QString &TableName,
                        std::string &ProjectDir,
                        std::vector<std::string> &fields);
    /**
     * @brief saveApplicationTable : Saves a table that identifies the type of database (i.e., MSVPA, MSSPM, MSCAA)
     * @param widget : the calling widget, needed to know where to show popup error dialogs
     * @param logger : logger pointer for logging errors and messages
     * @param tableName : name of application table
     */
    void saveApplicationTable(
            QWidget*       widget,
            nmfLogger*     logger,
            std::string&   tableName);
    /**
     * @brief updateForecastMonteCarloParameters
     * @param widget : the parent widget over which to display any popup widgets
     * @param logger : logger pointer for logging errors and messages
     * @param ProjectName : name of current project
     * @param ModelName : name of current model
     * @param ForecastName : name of forecast (if it's forecast data)
     * @param Algorithm : name of forecast algorithm family
     * @param Minimizer : name of forecast minimizer algorithm
     * @param ObjectiveCriterion : name of forecast objective criterion
     * @param Scaling : name of forecast scaling algorithm
     * @param Species : current species
     * @param RunNumber : the current monte carlo run number
     * @param GrowthRandomValues : random values from user inputted uncertainty error
     * @param CarryingCapacityRandomValues : random values from user inputted uncertainty error
     * @param CatchabilityRandomValues : random values from user inputted uncertainty error
     * @param ExponentRandomValues : random values from user inputted uncertainty error
     * @param CompetitionAlphaRandomValues : random values from user inputted uncertainty error
     * @param CompetitionBetaSpeciesRandomValues : random values from user inputted uncertainty error
     * @param CompetitionBetaGuildsRandomValues : random values from user inputted uncertainty error
     * @param CompetitionBetaGuildsGuildsRandomValues : random values from user inputted uncertainty error
     * @param PredationRandomValues : random values from user inputted uncertainty error
     * @param HandlingRandomValues : random values from user inputted uncertainty error
     * @param HarvestRandomValues : random values from user inputted uncertainty error
     * @return true if table was read correctly, otherwise false
     */
    bool updateForecastMonteCarloParameters(
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
            const std::vector<double>& HarvestRandomValues);
    /**
     * @brief Updates all of the models in the current project (necessary when observed biomass changes for one model)
     * @param parent : parent widget over which to display any popups
     * @param type : type of observed biomass
     * @param projectName : name of project
     * @param currentModel name of current model
     * @param modelsInProject : vector of all models in current project
     * @return true if all tables were read correctly
     */
    bool updateAllModelsInProject(
            QWidget* parent,
            const std::string& type,
            const std::string& projectName,
            const std::string& currentModel,
            std::vector<std::string>& modelsInProject);

    //  std::string nmfLastError();
    //  void nmfDeleteConnection();
    //  void nmfStartTransaction();
    //  void nmfSaveDatabase();



    // The following are all of the create-if-table-doesn't-exist functions.
    // Made the following static for use in a function map.
    // TBD Make these not static in next version of MSVPA.
    static void createApplication(QString &table, QString &qcmd);
    static void createForeEnergyDens(QString &table, QString &qcmd);
    static void createForeOutput(QString &table, QString &qcmd);
    static void createForePredGrowth(QString &table, QString &qcmd);
    static void createForePredVonB(QString &table, QString &qcmd);
    static void createForeSRQ(QString &table, QString &qcmd);
    static void createForeSRR(QString &table, QString &qcmd);
    static void createForeSuitPreyBiomass(QString &table, QString &qcmd);
    static void createForecasts(QString &table, QString &qcmd);
    static void createMSVPAEnergyDens(QString &table, QString &qcmd);
    static void createMSVPAOthPrey(QString &table, QString &qcmd);
    static void createMSVPAOthPreyAnn(QString &table, QString &qcmd);
    static void createMSVPASeasBiomass(QString &table, QString &qcmd);
    static void createMSVPASeasInfo(QString &table, QString &qcmd);
    static void createMSVPASizePref(QString &table, QString &qcmd);
    static void createMSVPASpaceO(QString &table, QString &qcmd);
    static void createMSVPAStomCont(QString &table, QString &qcmd);
    static void createMSVPASuitPreyBiomass(QString &table, QString &qcmd);
    static void createMSVPAlist(QString &table, QString &qcmd);
    static void createMSVPAprefs(QString &table, QString &qcmd);
    static void createMSVPAspecies(QString &table, QString &qcmd);
    static void createOthPredSizeData(QString &table, QString &cmd);
    static void createOtherPredBM(QString &table, QString &cmd);
    static void createOtherPredSpecies(QString &table, QString &qcmd);
    static void createSSVPAAgeM(QString &table, QString &qcmd);
    static void createScenarioF(QString &table, QString &qcmd);
    static void createScenarioOthPred(QString &table, QString &qcmd);
    static void createScenarioOthPrey(QString &table, QString &qcmd);
    static void createScenarioRec(QString &table, QString &qcmd);
    static void createScenarios(QString &table, QString &qcmd);
    static void createSpeCatch(QString &table, QString &qcmd);
    static void createSpeMaturity(QString &table, QString &qcmd);
    static void createSpeSSVPA(QString &table, QString &qcmd);
    static void createSpeSize(QString &table, QString &qcmd);
    static void createSpeTuneCatch(QString &table, QString &qcmd);
    static void createSpeTuneEffort(QString &table, QString &qcmd);
    static void createSpeWeight(QString &table, QString &qcmd);
    static void createSpeXSAData(QString &table, QString &qcmd);
    static void createSpeXSAIndices(QString &table, QString &qcmd);
    static void createSpecies(QString &table, QString &qcmd);

};

#endif /* NMFDATABASE_H_ */
