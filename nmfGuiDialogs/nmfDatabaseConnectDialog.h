#pragma once

#include "nmfDatabase.h"

#include <QCheckBox>
#include <QComboBox>
#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>

#include "nmfUtilsQt.h"
#include "nmfConstantsMSCAA.h"

class nmfDatabaseConnectDialog: public QDialog
{
    Q_OBJECT

public:


    explicit nmfDatabaseConnectDialog(QMainWindow*       mainWindow  = 0,
                                      const std::string& settingsDir = "",
                                      nmfDatabase*       databasePtr = 0);
    bool         m_openOKBool;
    std::string  m_settingsDir;
    QMainWindow* m_mainWin;
    nmfDatabase* m_databasePtr;
    //QCheckBox*   m_passwordCB;

    bool openOK();
    void SaveSettings();
//    bool rememberPassword();

    virtual ~nmfDatabaseConnectDialog() {}

public slots:
    void callback_connectToDatabase();
    void callback_resetGUI();
    void callback_resetSettings();

};

namespace nmfDatabaseUtils {

    bool menu_connectToDatabase(QMainWindow* mainWin,
                                std::string settingsDir,
                                nmfDatabase* databasePtr);

}

