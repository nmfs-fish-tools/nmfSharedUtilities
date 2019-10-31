#pragma once

#include "nmfDatabase.h"


#include <QComboBox>
#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>

class nmfDatabaseConnectDialog: public QDialog
{
    Q_OBJECT

public:
    bool openOKBool;

    explicit nmfDatabaseConnectDialog(QMainWindow *mainWindow = 0,
                                   nmfDatabase* databasePtr = 0);
    QMainWindow* mainWin;
    nmfDatabase* databasePtr;
    bool openOK();

    virtual ~nmfDatabaseConnectDialog() {}

public slots:
    void callback_connectToDatabase();
    void callback_resetGUI();

};

namespace nmfDatabaseUtils {

    bool menu_connectToDatabase(QMainWindow* mainWin,
                                nmfDatabase* databasePtr);

}

