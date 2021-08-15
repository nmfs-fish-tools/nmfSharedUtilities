#pragma once


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
#include <QIcon>

class nmfMessageWithDontShowDialog: public QDialog
{
    Q_OBJECT

    bool       m_yes;
    QCheckBox* m_cbox;
    QPushButton* m_yesBtn;
    QPushButton* m_noBtn;

public:
    explicit nmfMessageWithDontShowDialog(QWidget* parent,
                                          QString msg);
    virtual ~nmfMessageWithDontShowDialog() {}

    bool showFutureDialogs();
    bool wasYesClicked();

public slots:
    void callback_noClicked();
    void callback_yesClicked();

};


