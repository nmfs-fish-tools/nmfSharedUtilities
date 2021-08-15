#include "nmfMessageWithDontShowDialog.h"
#include <iostream>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QTimer>
#include <QApplication>

nmfMessageWithDontShowDialog::nmfMessageWithDontShowDialog(QWidget *parent, QString msg)
    : QDialog()
{
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle("Warning");

    m_yes = false;

    QVBoxLayout* mainLT  = new QVBoxLayout();
    QHBoxLayout* btnLT   = new QHBoxLayout();
    QHBoxLayout* introLT = new QHBoxLayout();
    QVBoxLayout* vertLT  = new QVBoxLayout();
    QIcon*  icon         = new QIcon();
    QLabel* msgLBL       = new QLabel("\nPlease Note:\n");
    msgLBL->setText(msgLBL->text()+msg);

    *icon = this->style()->standardIcon(QStyle::SP_MessageBoxInformation);
    QPushButton* iconBtn = new QPushButton();
    iconBtn->setFlat(true);
    iconBtn->setFixedSize(60,60);
    iconBtn->setIconSize(QSize(60,60));
    iconBtn->setIcon(*icon);
    m_yesBtn = new QPushButton("Yes");
    m_noBtn  = new QPushButton("No");
    m_cbox   = new QCheckBox("Don't show this again.");

    vertLT->addWidget(iconBtn);
    vertLT->addSpacerItem(new QSpacerItem(1,2,QSizePolicy::Fixed,QSizePolicy::Expanding));

    introLT->addLayout(vertLT);
    introLT->addWidget(msgLBL);
    introLT->addSpacerItem(new QSpacerItem(2,1,QSizePolicy::Expanding,QSizePolicy::Fixed));
    btnLT->addSpacerItem(new QSpacerItem(2,1,QSizePolicy::Expanding,QSizePolicy::Fixed));
    btnLT->addWidget(m_noBtn);
    btnLT->addWidget(m_yesBtn);
    btnLT->addSpacerItem(new QSpacerItem(2,1,QSizePolicy::Expanding,QSizePolicy::Fixed));
    mainLT->addLayout(introLT);
    mainLT->addWidget(m_cbox);
    mainLT->addLayout(btnLT);

    setLayout(mainLT);

    connect(m_yesBtn, SIGNAL(clicked()),
            this,     SLOT(callback_yesClicked()));
    connect(m_noBtn, SIGNAL(clicked()),
            this,     SLOT(callback_noClicked()));

    // Center dialog over parent widget
    move(parent->x()+parent->width()/2-(width()/4),
         parent->y()+parent->height()/2-(height()/4));
}

bool
nmfMessageWithDontShowDialog::showFutureDialogs()
{
    return (! m_cbox->isChecked());
}

bool
nmfMessageWithDontShowDialog::wasYesClicked()
{
    return m_yes;
}

void
nmfMessageWithDontShowDialog::callback_yesClicked()
{
    m_yes = true;
    close();
}

void
nmfMessageWithDontShowDialog::callback_noClicked()
{
    m_yes = false;
    close();
}
