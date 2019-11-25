#include "nmfDatabaseConnectDialog.h"
#include <QStandardItemModel>
#include <QTableWidget>


nmfDatabaseConnectDialog::nmfDatabaseConnectDialog(QMainWindow*       parent,
                                                   const std::string& settingsDir,
                                                   nmfDatabase*       databasePtr)
    : QDialog(parent)
{
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle("Connect to Database");
    m_mainWin     = parent;
    m_databasePtr = databasePtr;
    m_settingsDir = settingsDir;
    m_openOKBool  = false;
    m_username.clear();
    m_password.clear();

    QVBoxLayout *vLayout          = new QVBoxLayout();
    QHBoxLayout *hLayout          = new QHBoxLayout();
    QHBoxLayout *passwordLayt     = new QHBoxLayout();
    QLabel      *hostNameLBL      = new QLabel("Host name:");
    QLabel      *userNameLBL      = new QLabel("Database user name:");
    QLabel      *passwordLBL      = new QLabel("Database password:");
    QLabel      *sessionLBL       = new QLabel("Session comment for log file:");
    QLineEdit   *hostNameLE       = new QLineEdit();
    QLineEdit   *userNameLE       = new QLineEdit();
    QLineEdit   *passwordLE       = new QLineEdit();
    QLineEdit   *sessionLE        = new QLineEdit();
    //m_passwordCB                  = new QCheckBox();
    QPushButton *okDatabasePB     = new QPushButton("OK");
    QPushButton *cancelDatabasePB = new QPushButton("Cancel");
    QPushButton *resetGUIPB       = new QPushButton("Reset GUI");
    QPushButton *resetSettingsPB  = new QPushButton("Reset Settings");
    hostNameLE->setObjectName("hostNameLE");
    userNameLE->setObjectName("userNameLE");
    passwordLE->setObjectName("passwordLE");
    sessionLE->setObjectName("sessionLE");
    okDatabasePB->setObjectName("okDatabasePB");
    resetGUIPB->setObjectName("resetGUIPB");
    resetSettingsPB->setObjectName("resetSettingsPB");
    okDatabasePB->setDefault(true);
    resetGUIPB->setToolTip("Move main GUI back to (0,0). (Useful if GUI gets lost.)");
    resetGUIPB->setStatusTip("Move main GUI back to (0,0). (Useful if GUI gets lost.)");
    resetSettingsPB->setToolTip("Remove Settings file. (Useful if App won't start.)");
    resetSettingsPB->setStatusTip("Remove Settings file. (Useful if App won't start.)");
    sessionLE->setToolTip("This (optional) comment appears at the beginning of this session's log file.");
    sessionLE->setStatusTip("This (optional) comment appears at the beginning of this session's log file.");
    //m_passwordCB->setToolTip("Remember password");
    //m_passwordCB->setStatusTip("Remember password");
    hostNameLE->setClearButtonEnabled(true);
    userNameLE->setClearButtonEnabled(true);
    passwordLE->setClearButtonEnabled(true);
    sessionLE->setClearButtonEnabled(true);

    hLayout->addWidget(cancelDatabasePB);
    hLayout->addWidget(resetGUIPB);
    hLayout->addWidget(resetSettingsPB);
    hLayout->addWidget(okDatabasePB);
    vLayout->addWidget(hostNameLBL);
    vLayout->addWidget(hostNameLE);
    vLayout->addWidget(userNameLBL);
    vLayout->addWidget(userNameLE );
    vLayout->addWidget(passwordLBL);
    //passwordLayt->addWidget(passwordLE);
    //passwordLayt->addWidget(m_passwordCB);
    //vLayout->addLayout(passwordLayt);
    vLayout->addWidget(passwordLE);
    vLayout->addWidget(sessionLBL); // the comment is available after the first log message has already printed
    vLayout->addWidget(sessionLE);
    vLayout->addItem(new QSpacerItem(0,10,QSizePolicy::Expanding,QSizePolicy::Expanding));
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
    setMinimumWidth(250);

    // Load default values
    passwordLE->setEchoMode(QLineEdit::Password);
    hostNameLE->setText("localhost");
    userNameLE ->setText("root");

    connect(okDatabasePB,     SIGNAL(clicked()),
            this,             SLOT(callback_connectToDatabase()));
    connect(cancelDatabasePB, SIGNAL(clicked(bool)),
            this,             SLOT(close()));
    connect(resetGUIPB,       SIGNAL(clicked()),
            this,             SLOT(callback_resetGUI()));
    connect(resetSettingsPB,  SIGNAL(clicked()),
            this,             SLOT(callback_resetSettings()));

    passwordLE->setFocus();
}

//bool
//nmfDatabaseConnectDialog::rememberPassword()
//{
//    return m_passwordCB->isChecked();
//}

void
nmfDatabaseConnectDialog::callback_connectToDatabase()
{
  std::cout << "callback_connectToDatabase" << std::endl;
  std::string errorMsg;

  std::string msg;
  QPushButton *ok_btn    = qobject_cast<QPushButton *>(QObject::sender());
  QDialog     *dlg       = qobject_cast<QDialog *>(ok_btn->parent());
  QLineEdit *hostNameLE  = dlg->findChild<QLineEdit *>("hostNameLE");
  QLineEdit *userNameLE  = dlg->findChild<QLineEdit *>("userNameLE");
  QLineEdit *passwordLE  = dlg->findChild<QLineEdit *>("passwordLE");
  QLineEdit *sessionLE   = dlg->findChild<QLineEdit *>("sessionLE");

  std::string Hostname = hostNameLE->text().toStdString();
  std::string Username = userNameLE->text().toStdString();
  std::string Password = passwordLE->text().toStdString();
  std::string Session  = sessionLE->text().toStdString();
  m_username = Username;
  m_password = Password;

  m_openOKBool = m_databasePtr->nmfOpenDatabase(Hostname,Username,Password,errorMsg);
  if (! m_openOKBool) {
      QMessageBox msgBox;
      msgBox.setWindowTitle("Error");
      msg  = "\nCouldn't open database.\n";
      msg += "\nError: "+errorMsg+"\n";
      msg += "\nFor Windows: Check that libmysql.dll is present in the directory\n";
      msg += "where the application executable is installed.\n";
      msgBox.setText(QString::fromStdString(msg));
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
  } else {
      close();
  }

}

std::string
nmfDatabaseConnectDialog::getUsername()
{
    return m_username;
}

std::string
nmfDatabaseConnectDialog::getPassword()
{
    return m_password;
}

bool
nmfDatabaseConnectDialog::openOK()
{
    return m_openOKBool;
}

void
nmfDatabaseConnectDialog::callback_resetGUI()
{
    QMessageBox::StandardButton reply =
            QMessageBox::question(this,
                                  tr("Reset GUI"),
                                  tr("\nOK to reset GUI position?\n"),
                                  QMessageBox::No|QMessageBox::Yes,
                                  QMessageBox::Yes);
    if (reply == QMessageBox::Yes) {
        m_mainWin->move(QPoint(0,0));
        SaveSettings();
    }
}

void
nmfDatabaseConnectDialog::callback_resetSettings()
{
    QMessageBox::StandardButton reply =
            QMessageBox::question(this,
                                  tr("Reset Settings"),
                                  tr("\nOK to reset application settings?\n"),
                                  QMessageBox::No|QMessageBox::Yes,
                                  QMessageBox::Yes);
    if (reply == QMessageBox::Yes) {
        nmfUtilsQt::removeSettingsFile();
        QMessageBox::information(this,
                                 tr("Reset Settings"),
                                 tr("\nSettings reset.\n\nUser interface settings will be set again upon application exit.\n"),
                                 QMessageBox::Ok);
    }
}


void
nmfDatabaseConnectDialog::SaveSettings()
{
    QSettings* settings = nmfUtilsQt::createSettings(m_settingsDir,QApplication::applicationName());

    settings->beginGroup("MainWindow");
    settings->setValue("pos", m_mainWin->pos());
    settings->endGroup();

    delete settings;
}

namespace nmfDatabaseUtils {


bool menu_connectToDatabase(QMainWindow* mainWin,
                            std::string settingsDir,
                            nmfDatabase* databasePtr,
                            std::string& username,
                            std::string& password)
{
    nmfDatabaseConnectDialog* dlg = new nmfDatabaseConnectDialog(
                mainWin,settingsDir,databasePtr);
    dlg->exec();
    username = dlg->getUsername();
    password = dlg->getPassword();

    return dlg->openOK();
}


}
