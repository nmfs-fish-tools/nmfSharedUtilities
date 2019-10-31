/*
#ifndef NMFUTILITIES_H
#define NMFUTILITIES_H

#include "nmfutilities_global.h"

class NMFUTILITIESSHARED_EXPORT nmfUtilities
{

public:
    nmfUtilities();
};

#endif // NMFUTILITIES_H
*/

/**
 @file nmfUtils.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This header file defines MSVPA_X2 Qt utility functions.
 @date Jul 26, 2017
*/

#pragma once


#include <iostream>
#include <string>
#include <vector>

#include <QString>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>



namespace nmfStructsQt {


// Create a dialog class that allows user to enter a filename
// and select either jpg or png for the file type.
class FileDialogWithType : public QDialog
{

public:

    QLineEdit *le;
    QComboBox *cmb;

    FileDialogWithType()
    {
        QVBoxLayout *vlayout       = new QVBoxLayout();
        QHBoxLayout *hlayout       = new QHBoxLayout();
        QHBoxLayout *hButtonLayout = new QHBoxLayout();
        QLabel *label              = new QLabel("Chart image file name:");
        QPushButton *cancelPB      = new QPushButton("Cancel");
        QPushButton *okPB          = new QPushButton("OK");
        le                         = new QLineEdit();
        cmb                        = new QComboBox();

        le->setToolTip(   "Enter filename for screen grab of chart.");
        le->setStatusTip( "Enter filename for screen grab of chart.");
        cmb->setToolTip(  "Select type of image file.");
        cmb->setStatusTip("Select type of image file.");
        cmb->addItems({"jpg","png"});
        okPB->setDefault(true);

        hlayout->addWidget(le);
        hlayout->addWidget(cmb);
        hButtonLayout->addWidget(cancelPB);
        hButtonLayout->addWidget(okPB);
        vlayout->addWidget(label);
        vlayout->addLayout(hlayout);
        vlayout->addLayout(hButtonLayout);
        this->setLayout(vlayout);

        connect(cancelPB, SIGNAL(pressed()), this, SLOT(close()));
        connect(okPB,     SIGNAL(pressed()), this, SLOT(accept()));
    };
    ~FileDialogWithType() {}

    //QString filename() {return le->text()+"."+cmb->currentText();};
    QString filename() {
        QFileInfo fileInfo(le->text());
        return fileInfo.completeBaseName()+"."+cmb->currentText();
    }

};



struct UpdateDataStruct {
    nmfDatabase* databasePtr;
    nmfLogger*   logger;
    std::string  MSVPAName;
    std::string  ForecastName;
    std::string  ScenarioName;
    std::string  ModelName;
    int  Theme;
    int  NumAgeSizeClasses;
    bool HorizontalGridLines;
    bool VerticalGridLines;
    std::string DataTypeLabel;
    std::string XLabel;
    std::string YLabel;
    std::string TitlePrefix;
    std::string TitleSuffix;
    QString SelectDataType;
    QString SelectPredator;
    QString SelectVariable;
    QString SelectByVariables;
    QString SelectSeason;
    QString SelectPredatorAgeSizeClass;
    QString SelectPreyName;
    QString SelectYPRAnalysisType;
    QString SelectFullyRecruitedAge;
    QString MaxScaleY;
    QString SelectMode;
    std::vector<std::string> SelectedYears;
};

}

