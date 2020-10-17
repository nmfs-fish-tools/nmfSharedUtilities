/**
 * @file nmfStructsQt.h
 * @brief Definition for common qt gui widgets
 * @date Jan 14, 2020
 *
 * This file contains Qt widget definitions that are common across applications.
 *
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

class nmfDatabase;

/**
 * @brief These are common Qt structures and classes
 */
namespace nmfStructsQt {

/**
 * @brief This class displays a QDialog and allows the user to do a screen
 * grab of the displayed chart. The user may save the file to either a jpg
 * or png file format.
 */
class ChartSaveDlg : public QDialog
{
    QLineEdit *m_filenameLE;
    QComboBox *m_fileTypeCMB;

public:
    /**
     * @brief Class  constructor, builds the dialog allowing the user to save the current chart image
     */
    ChartSaveDlg(QWidget* parent) : QDialog(parent)
    {
        QVBoxLayout *vlayout       = new QVBoxLayout();
        QHBoxLayout *hlayout       = new QHBoxLayout();
        QHBoxLayout *hButtonLayout = new QHBoxLayout();
        QLabel *label              = new QLabel("Chart image file name:");
        QPushButton *cancelPB      = new QPushButton("Cancel");
        QPushButton *okPB          = new QPushButton("OK");
        m_filenameLE               = new QLineEdit();
        m_fileTypeCMB              = new QComboBox();

        m_filenameLE->setToolTip(   "Enter filename for screen grab of chart.");
        m_filenameLE->setStatusTip( "Enter filename for screen grab of chart.");
        m_fileTypeCMB->setToolTip(  "Select type of image file.");
        m_fileTypeCMB->setStatusTip("Select type of image file.");
        m_fileTypeCMB->addItems({"jpg","png"});
        okPB->setDefault(true);

        hlayout->addWidget(m_filenameLE);
        hlayout->addWidget(m_fileTypeCMB);
        hButtonLayout->addWidget(cancelPB);
        hButtonLayout->addWidget(okPB);
        vlayout->addWidget(label);
        vlayout->addLayout(hlayout);
        vlayout->addLayout(hButtonLayout);
        this->setLayout(vlayout);

        connect(cancelPB, SIGNAL(pressed()), this, SLOT(close()));
        connect(okPB,     SIGNAL(pressed()), this, SLOT(accept()));
    };
    ~ChartSaveDlg() {}

    /**
     * @brief Gets the filename for the chart image
     * @return The filename entered by the user
     */
    QString getFilename() {
        QString retv = "";
        QFileInfo fileInfo(m_filenameLE->text());
        QString basename = fileInfo.completeBaseName();
        if (basename.isEmpty()) {
            return retv;
        }

        QString suffix = fileInfo.completeSuffix();
        if ((suffix != "jpg") && (suffix != "png") && (!suffix.isEmpty())) {
            basename += "." + suffix;
        }
        return basename + "." + m_fileTypeCMB->currentText();
    }

};


/**
 * @brief The MSVPA data struct
 */
struct UpdateDataStruct {
    nmfDatabase* databasePtr;
    nmfLogger*   logger;
    std::string  MSVPAName;
    std::string  ForecastName;
    std::string  ScenarioName;
    std::string  ModelName;
    int          Theme;
    int          NumAgeSizeClasses;
    bool         HorizontalGridLines;
    bool         VerticalGridLines;
    std::string  DataTypeLabel;
    std::string  XLabel;
    std::string  YLabel;
    std::string  TitlePrefix;
    std::string  TitleSuffix;
    QString      SelectDataType;
    QString      SelectPredator;
    QString      SelectVariable;
    QString      SelectByVariables;
    QString      SelectSeason;
    QString      SelectPredatorAgeSizeClass;
    QString      SelectPreyName;
    QString      SelectYPRAnalysisType;
    QString      SelectFullyRecruitedAge;
    QString      MaxScaleY;
    QString      SelectMode;
    std::vector<std::string> SelectedYears;
};

}

