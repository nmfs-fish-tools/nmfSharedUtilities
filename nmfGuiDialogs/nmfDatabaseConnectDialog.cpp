#include "nmfDatabaseConnectDialog.h"
#include <QStandardItemModel>
#include <QTableWidget>


nmfDatabaseConnectDialog::nmfDatabaseConnectDialog(QMainWindow *parent,
                                             nmfDatabase* theDatabasePtr)
    : QDialog(parent)
{
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle("Connect to Database");
    mainWin = parent;
    databasePtr = theDatabasePtr;
    openOKBool = false;

    QVBoxLayout *vLayout          = new QVBoxLayout();
    QHBoxLayout *hLayout          = new QHBoxLayout();
    QLabel      *hostNameLBL      = new QLabel("Host name:");
    QLabel      *userNameLBL      = new QLabel("Database user name:");
    QLabel      *passwordLBL      = new QLabel("Database password:");
    QLabel      *sessionLBL       = new QLabel("Session comment for log file:");
    QLineEdit   *hostNameLE       = new QLineEdit();
    QLineEdit   *userNameLE       = new QLineEdit();
    QLineEdit   *passwordLE       = new QLineEdit();
    QLineEdit   *sessionLE        = new QLineEdit();
    QPushButton *okDatabasePB     = new QPushButton("OK");
    QPushButton *cancelDatabasePB = new QPushButton("Cancel");
    QPushButton *resetGUIPB       = new QPushButton("Reset GUI");
    hostNameLE->setObjectName("hostNameLE");
    userNameLE->setObjectName("userNameLE");
    passwordLE->setObjectName("passwordLE");
    sessionLE->setObjectName("sessionLE");
    okDatabasePB->setObjectName("okDatabasePB");
    resetGUIPB->setObjectName("resetGUIPB");
    okDatabasePB->setDefault(true);
    resetGUIPB->setToolTip("Move main GUI back to (0,0). (Useful if it gets lost.)");
    resetGUIPB->setStatusTip("Move main GUI back to (0,0). (Useful if it gets lost.)");
    sessionLE->setToolTip("This (optional) comment appears at the beginning of this session's log file.");
    sessionLE->setStatusTip("This (optional) comment appears at the beginning of this session's log file.");
    hostNameLE->setClearButtonEnabled(true);
    userNameLE->setClearButtonEnabled(true);
    passwordLE->setClearButtonEnabled(true);
    sessionLE->setClearButtonEnabled(true);

    hLayout->addWidget(cancelDatabasePB);
    hLayout->addWidget(resetGUIPB);
    hLayout->addWidget(okDatabasePB);
    vLayout->addWidget(hostNameLBL);
    vLayout->addWidget(hostNameLE);
    vLayout->addWidget(userNameLBL);
    vLayout->addWidget(userNameLE );
    vLayout->addWidget(passwordLBL);
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

    // RSK - remove this line eventually.....just for testing purposes to speed things along.....
std::cout << "\n*** *** Remove hardcoded password for debugging! *** *** \n" << std::endl;
    passwordLE->setText("rklasky$$");

    connect(okDatabasePB,     SIGNAL(clicked()),
                     this,             SLOT(callback_connectToDatabase()));
    connect(cancelDatabasePB, SIGNAL(clicked(bool)),
                     this,              SLOT(close()));
    connect(resetGUIPB,       SIGNAL(clicked()),
                     this,             SLOT(callback_resetGUI()));

    passwordLE->setFocus();
}

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

  openOKBool = databasePtr->nmfOpenDatabase(Hostname,Username,Password,errorMsg);
  if (! openOKBool) {
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

bool
nmfDatabaseConnectDialog::openOK()
{
    return openOKBool;
}

void
nmfDatabaseConnectDialog::callback_resetGUI()
{
    std::cout << "callback_resetGUI" << std::endl;
    mainWin->move(QPoint(0,0));

}


namespace nmfDatabaseUtils {


bool menu_connectToDatabase(QMainWindow* mainWin,
                            nmfDatabase* databasePtr)
{
    nmfDatabaseConnectDialog* dlg = new nmfDatabaseConnectDialog(
                mainWin,databasePtr);
    dlg->exec();

    return dlg->openOK();
}


}
