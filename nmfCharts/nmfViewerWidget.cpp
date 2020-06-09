
//============================================================================
// Name        : nmfViewerWidget.cpp
// Author      : Ron Klasky
// Version     :
// Copyright   : NOAA - National Marine Fisheries Service
// Description : Widget containing logic for image file viewer widget
//============================================================================

#include "nmfViewerWidget.h"


nmfViewerWidget::nmfViewerWidget(QWidget* parent,
                                 QString imagePath,
                                 nmfLogger* logger)
{
    m_Parent    = parent;
    m_ImagePath = imagePath;
    m_Logger    = logger;

    setupOutputScreenShotViewerWidgets();
}

nmfViewerWidget::~nmfViewerWidget() {

}


/*
 * To use the class, just set mainLayt to be the layout of the
 * parent widget.  For example, if the parent widget is:
 *
 * ui->viewerTab just call:
 *
 * viewerWidget = nmfViewerWidget(...)
 * ui->viewerTab->setLayout(viewerWidget->mainLayt);
 *
 */
void
nmfViewerWidget::setupOutputScreenShotViewerWidgets()
{
    QHBoxLayout* filenameLayt = new QHBoxLayout();
    QLabel*      filenameLBL  = new QLabel("Output images:");
    mainLayt                  = new QVBoxLayout();
    ScreenShotViewerCMB       = new QComboBox();
    ScreenShotViewerPixmapLBL = new QLabel();
    ScreenShotViewerDelPB     = new QPushButton("del");
    ScreenShotViewerRenPB     = new QPushButton("ren");
    ScreenShotViewerRefPB     = new QPushButton("ref");
    ScreenShotViewerCMB->setFixedWidth(300);
    filenameLayt->addWidget(filenameLBL);
    filenameLayt->addWidget(ScreenShotViewerCMB);
    filenameLayt->addWidget(ScreenShotViewerDelPB);
    filenameLayt->addWidget(ScreenShotViewerRenPB);
    filenameLayt->addWidget(ScreenShotViewerRefPB);
    filenameLayt->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Fixed));
    mainLayt->addLayout(filenameLayt);
    mainLayt->addWidget(ScreenShotViewerPixmapLBL);
    ScreenShotViewerPixmapLBL->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ScreenShotViewerDelPB->setToolTip("Delete the current output image");
    ScreenShotViewerDelPB->setFixedWidth(40);
    ScreenShotViewerRefPB->setToolTip("Refreshes the output image list");
    ScreenShotViewerRefPB->setFixedWidth(40);
    ScreenShotViewerRenPB->setToolTip("Rename the current output image");
    ScreenShotViewerRenPB->setFixedWidth(40);

    connect(ScreenShotViewerCMB,   SIGNAL(activated(QString)),
            this,                  SLOT(callback_ScreenShotViewerCMB(QString)));
    connect(ScreenShotViewerDelPB, SIGNAL(clicked()),
            this,                  SLOT(callback_ScreenShotViewerDelPB()));
    connect(ScreenShotViewerRefPB, SIGNAL(clicked()),
            this,                  SLOT(callback_ScreenShotViewerRefPB()));
    connect(ScreenShotViewerRenPB, SIGNAL(clicked()),
            this,                  SLOT(callback_ScreenShotViewerRenPB()));

    updateScreenShotViewer("");
}

void
nmfViewerWidget::refreshList()
{
    QDir directory(m_ImagePath);
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.png", QDir::Files);
    ScreenShotViewerCMB->clear();
    ScreenShotViewerCMB->addItems(images);
}

void
nmfViewerWidget::updateScreenShotViewer(QString filename)
{
    // Load filenames into combo box
    refreshList();

    if (filename.isEmpty()) {
        if (ScreenShotViewerCMB->count() > 0) {
            QString firstImage = ScreenShotViewerCMB->itemText(0);
            callback_ScreenShotViewerCMB(firstImage);
        } else {
            ScreenShotViewerPixmapLBL->clear();
        }
    } else {
        ScreenShotViewerCMB->blockSignals(true);
        ScreenShotViewerCMB->setCurrentText(filename);
        ScreenShotViewerCMB->blockSignals(false);
        callback_ScreenShotViewerCMB(filename);
    }
}


void
nmfViewerWidget::callback_ScreenShotViewerRefPB()
{
    updateScreenShotViewer(ScreenShotViewerCMB->currentText());
}

void
nmfViewerWidget::callback_ScreenShotViewerRenPB()
{
    bool ok;
    bool okToWrite = false;
    QString msg;
    QString renamedFilenameWithPath;
    QString currentText = ScreenShotViewerCMB->currentText();;
    QFileInfo fileInfo(currentText);
    QString currentSuffix = fileInfo.completeSuffix();
    QString currentFilenameWithPath;

    msg  = "\nCurrent image: " + currentText;
    msg += "\n\nRename to:";
    QString renamedFilename = QInputDialog::getText(
                m_Parent, "Rename Image",
                msg, QLineEdit::Normal,
                currentText, &ok);
    renamedFilenameWithPath = QDir(m_ImagePath).filePath(renamedFilename);
    QFileInfo newfile(renamedFilenameWithPath);

    QString suffix = newfile.completeSuffix();
    if ((! suffix.endsWith("jpg")) && (! suffix.endsWith("png"))) {
        renamedFilename += ".jpg";
        renamedFilenameWithPath += ".jpg";
    }

    if (ok && !renamedFilenameWithPath.isEmpty()) {
        if (nmfUtilsQt::fileExists(renamedFilenameWithPath)) {
            okToWrite = okToOverwrite(renamedFilenameWithPath);
            if (okToWrite) {
                QFile::remove(renamedFilenameWithPath);
            }
        } else {
            okToWrite = true;
        }
        if (okToWrite) {
            currentFilenameWithPath = QDir(m_ImagePath).filePath(currentText);
            QFile::rename(currentFilenameWithPath,
                          renamedFilenameWithPath);
//            msg = "Couldn't rename " + currentFilenameWithPath + " to " +
//                    renamedFilenameWithPath;
//            m_Logger->logMsg(nmfConstants::Error,msg.toStdString());
            refreshList();
            ScreenShotViewerCMB->setCurrentText(renamedFilename);
        }
    }
}


bool
nmfViewerWidget::okToOverwrite(QString filenameWithPath)
{
    QString msg;

    QFileInfo filenameNoPath(filenameWithPath);
    msg = "\nOK to overwrite: " + filenameNoPath.fileName() + "?\n";
    QMessageBox::StandardButton reply = QMessageBox::question(
                m_Parent,"Overwrite",msg,
                QMessageBox::No|QMessageBox::Yes,
                QMessageBox::No);
    return (reply == QMessageBox::Yes);
}

void
nmfViewerWidget::callback_ScreenShotViewerDelPB()
{
    QString msg;
    QString nextFilename = "";
    QMessageBox::StandardButton reply;

    if (ScreenShotViewerCMB->count() == 0) {
        QMessageBox::information(m_Parent, "No Output Images",
                                 "\nNo output image files to delete.\n",
                                 QMessageBox::Ok);
        return;
    }

    msg  = "\nThis will permanently delete the image: " + ScreenShotViewerCMB->currentText();
    msg += "\n\nOK to delete? ";
    msg += "\n\nN.B. This cannot be undone.";
    reply = QMessageBox::warning(m_Parent, tr("Image Delete"), tr(msg.toLatin1()),
                                  QMessageBox::No|QMessageBox::Yes,
                                  QMessageBox::Yes);
    if (reply == QMessageBox::Yes) {
        int numImages = ScreenShotViewerCMB->count();
        int curImage  = ScreenShotViewerCMB->currentIndex();
        int nextImage = 0;
        QString filename = ScreenShotViewerCMB->itemText(curImage);
        if (curImage == numImages-1) {
            nextImage = numImages-2;
        } else {
            nextImage = (curImage+1 > numImages-1) ? curImage : curImage+1;
        }
        if (nextImage >= 0) {
            nextFilename = ScreenShotViewerCMB->itemText(nextImage);
        }

        QFile::remove(QDir(m_ImagePath).filePath(filename));

        updateScreenShotViewer(nextFilename);
    }
}

void
nmfViewerWidget::callback_ScreenShotViewerCMB(QString filename)
{
    QImage image;
    image.load(QDir(m_ImagePath).filePath(filename));

    ScreenShotViewerPixmapLBL->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ScreenShotViewerPixmapLBL->setScaledContents(true);
    ScreenShotViewerPixmapLBL->setPixmap(QPixmap::fromImage(image));
}


