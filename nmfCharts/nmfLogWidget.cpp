
#include "nmfLogWidget.h"
#include "nmfConstants.h"

#include <QtCharts/QChartView>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

nmfLogWidget::nmfLogWidget(nmfLogger *theLogger,
                           std::string theLogDir)
{
    m_logger = theLogger;
    m_logDir = theLogDir;

    // Create layouts and widgets
    filenameLBL = new QLabel();
    vMainLayt   = new QVBoxLayout();
    hMainLayt   = new QHBoxLayout();
    vGroupLayt  = new QVBoxLayout();
    logTE       = new QTextEdit();
    controlsGB  = new QGroupBox();
    browsePB    = new QPushButton("Browse...");
    refreshPB   = new QPushButton("Refresh");

    browsePB->setToolTip("Find and load a log file.");
    browsePB->setStatusTip("Find and load a log file.");
    refreshPB->setToolTip("Load most recent log file.");
    refreshPB->setStatusTip("Load most recent log file.");

    // Add widgets to layouts
    vMainLayt->addWidget(filenameLBL);
    vMainLayt->addLayout(hMainLayt);
    hMainLayt->addWidget(logTE);
    hMainLayt->addWidget(controlsGB);
    vGroupLayt->addWidget(browsePB);
    vGroupLayt->addSpacerItem(new QSpacerItem(1,2,QSizePolicy::Minimum,QSizePolicy::Expanding));
    vGroupLayt->addWidget(refreshPB);

    // Set connections
    connect(refreshPB, SIGNAL(clicked()),
            this,      SLOT(callback_refreshPB()));
    connect(browsePB,  SIGNAL(clicked()),
            this,      SLOT(callback_browsePB()));

    // Set defaults
    controlsGB->setLayout(vGroupLayt);
    controlsGB->setTitle("Controls");

    // Show empty text edit
    logTE->clear();
}

nmfLogWidget::~nmfLogWidget() {

}


void
nmfLogWidget::loadLogFile(QString filename)
{
    QString line;
    QStringList parts;
    QString color;

    logTE->clear();

    QFile fin(filename);
    if (! fin.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,
                                 "Error Opening Log File:",
                                 "\n"+fin.errorString()+" "+filename);
        return;
    }
    QTextStream inStream(&fin);
    QString text;
    while (! inStream.atEnd()) {
        line = inStream.readLine();
        color = line.section(" ",0,0);
        line  = line.section(" ",1);
        switch (color.toInt()) {
            case nmfConstants::Normal:
                line = "<font color=\"black\">"+line+"</font>";
                break;
            case nmfConstants::Bold:
                line = "<strong><font color=\"black\">"+line+"</font></strong>";
                break;
            case nmfConstants::Warning:
                line = "<font color=\"red\">"+line+"</font>";
                break;
            case nmfConstants::Error:
                line = "<strong><font color=\"red\">"+line+"</font></strong>";
                break;
            case nmfConstants::Highlight:
                line = "<span style=\"background-color: #FFFF00\">"+line+"</span>";
                break;
            case nmfConstants::Section:
                line = "<strong><font color=\"blue\">"+line+"</font></strong>";
                break;
            default:
                break;
        }

        text += line;
        text += "<br>";
        //logTE->append(line);
    }
    logTE->setHtml(text);
    fin.close();

    // Scroll to bottom of text edit
    logTE->verticalScrollBar()->setValue(logTE->verticalScrollBar()->maximum());

} // end loadLogFile


void
nmfLogWidget::callback_browsePB()
{
    QString LogFile = QFileDialog::getOpenFileName(logTE,
        tr("Load Log File"),
        QString::fromStdString(m_logDir),
        tr("*.log"));
    if (LogFile.isEmpty())
        return;
    QFile file(LogFile);
    QFileInfo fileInfo(file.fileName());

    loadLogFile(LogFile);
    filenameLBL->setText("File Name:  " + fileInfo.fileName());

} // end callback_browsePB

void
nmfLogWidget::callback_refreshPB()
{
    QString filename = QString::fromStdString(m_logger->getLogFile());
    QFile file(filename);
    QFileInfo fileInfo(file.fileName());

    loadLogFile(filename);
    filenameLBL->setText("File Name:  " + fileInfo.fileName());

} // end callback_refreshPB
