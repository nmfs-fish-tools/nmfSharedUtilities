/**
 * @file nmfDatabaseConnectDialog.h
 * @brief Definition for the Connect to Database GUI
 * @date Feb 16, 2018
 *
 * This file contains the definition for the Connect to Database GUI that prompts the user
 * for database login information.
 *
 * @copyright
 * Public Domain Notice\n
 *
 * National Oceanic And Atmospheric Administration\n\n
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
 * for any particular purpose.\n\n
 *
 * Please cite the author(s) in any work or product based on this material.
 */

#pragma once

#include "nmfDatabase.h"
#include "nmfConstantsMSCAA.h"
#include "nmfUtilsQt.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

/**
 * @brief The nmfDatabaseConnectDialog class which prompts the user with a GUI
 * for database login information.
 */
class nmfDatabaseConnectDialog: public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief nmfDatabaseConnectDialog class constructor
     * @param mainWindow : the main GUI window over which to display this dialog
     * @param settingsDir : Qt settings directory
     * @param databasePtr : database library pointer
     */
    explicit nmfDatabaseConnectDialog(QMainWindow*       mainWindow  = 0,
                                      const std::string& settingsDir = "",
                                      nmfDatabase*       databasePtr = 0);
    virtual ~nmfDatabaseConnectDialog() {}

    bool         m_openOKBool;
    std::string  m_settingsDir;
    std::string  m_username;
    std::string  m_password;
    QMainWindow* m_mainWin;
    nmfDatabase* m_databasePtr;

    /**
     * @brief Gets the current user name
     * @return the user name entered by the user
     */
    std::string getUsername();
    /**
     * @brief Gets the current password
     * @return the password entered by the user
     */
    std::string getPassword();
    /**
     * @brief Boolean describing if the database has been opened successfully
     * @return true if database opened ok, false otherwise
     */
    bool openOK();
    /**
     * @brief Save current Qt settings
     */
    void saveSettings();

public slots:
    /**
     * @brief Callback invoked when the user clicks the OK button to connect to the database
     */
    void callback_connectToDatabase();
    /**
     * @brief Callback invoked when the user clicks the Reset GUI button. This button is
     * needed since it's possible that the GUI may be "stuck" offscreen if 2 monitors are
     * used and then only 1 monitor is used.
     */
    void callback_resetGUI();
    /**
     * @brief Callback invoked when the user clicks the Reset Settings button. This button is
     * useful if a bad UI setting has somehow been saved.
     */
    void callback_resetSettings();
};

/**
 * @brief This namespace contains the connect to database function.
 */
namespace nmfDatabaseUtils {

    /**
     * @brief raises the Connect to Database GUI
     * @param mainWin : main window over which to display Connect to Database GUI
     * @param settingsDir : Qt settings directory
     * @param databasePtr : database library pointer
     * @param username : the current user name
     * @param password : the current database password
     * @return boolean signifying if the database has been succesfully opened
     */
    bool menu_connectToDatabase(QMainWindow* mainWin,
                                std::string settingsDir,
                                nmfDatabase* databasePtr,
                                std::string& username,
                                std::string& password);

}

