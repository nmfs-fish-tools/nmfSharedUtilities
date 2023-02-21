
/**
 @file nmfQtUtils.cpp
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Jul 26, 2017
*/

#include <ctime>
#include "nmfUtilsQt.h"
#include "nmfConstants.h"
#include "nmfLogger.h"

#include <QAction>
#include <QShortcut>
#include <QComboBox>
#include <stdio.h>

nmfToolTip::nmfToolTip()
{
    setWindowFlag(Qt::ToolTip);
    m_Label = new QLabel();
    m_Label->setStyleSheet(("QLabel { background-color : #ffffd0; color : black; }"));
    QHBoxLayout* layt = new QHBoxLayout();
    layt->addWidget(m_Label);
    layt->setContentsMargins(1,1,1,1);
    setLayout(layt);
//  hide();
}

void
nmfToolTip::setLabel(const QString& label)
{
    m_Label->setText(label);
}



nmfFileViewer::nmfFileViewer(QWidget* parent,
                            const char* fileName) : QDialog(parent)
{
    const int Min_Font_Size = 9;
    const int Max_Font_Size = 24;

    setWindowTitle("Opt Files");

    QHBoxLayout* hlayt = new QHBoxLayout();
    QVBoxLayout* vlayt = new QVBoxLayout();
    m_TextEdit         = new QTextEdit();

    QFont font = m_TextEdit->font();
    m_TextEdit->setFontFamily("Courier New");
    m_TextEdit->setFont(font);
    QPushButton* okPB  = new QPushButton("Ok");
    QLabel* fontLBL    = new QLabel("Font Size:");
    QComboBox* fontCMB = new QComboBox();
    for (int i=Min_Font_Size; i<Max_Font_Size; ++i) {
        fontCMB->insertItem(i-Min_Font_Size,QString::number(i));
    }

    hlayt->addWidget(fontLBL);
    hlayt->addWidget(fontCMB);
    hlayt->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hlayt->addWidget(okPB);
    hlayt->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));
    hlayt->addSpacerItem(new QSpacerItem(100,1,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vlayt->addWidget(m_TextEdit);
    vlayt->addLayout(hlayt);
    setLayout(vlayt);

    connect(okPB,    SIGNAL(clicked(bool)),
            this,    SLOT(close()));
    connect(fontCMB, SIGNAL(activated(QString)),
            this,    SLOT(callback_FontSizePB(QString)));

    // Open file chosen, read it and display contents in dialog
    QFile file(fileName);
    m_TextEdit->setReadOnly(true);
    if (file.open(QIODevice::ReadOnly)) {
        m_TextEdit->setText(file.readAll());
        file.close();
    }
}

void
nmfFileViewer::callback_FontSizePB(QString fontSize)
{
    QTextCursor textCursor = m_TextEdit->textCursor();
    m_TextEdit->selectAll();
    m_TextEdit->setFontPointSize(fontSize.toInt());
    m_TextEdit->setTextCursor(textCursor);
}



/*
nmfTableView::nmfTableView(QWidget *parent)
    : QTableView(parent)
{
    QShortcut *copyShortcut      = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_C),           this);
    QShortcut *pasteShortcut     = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_V),           this);
    QShortcut *allPasteShortcut  = new QShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_V), this);
    QShortcut *clearShortcut     = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_X),           this);
    QShortcut *allClearShortcut  = new QShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_X), this);
    QShortcut *selectShortcut    = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_A),           this);
    QShortcut *deselectShortcut  = new QShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_A), this);

    // RSK - these may have something to do with fixing the shortcut problem with the menu shortcuts...tbd
    allPasteShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    allPasteShortcut->setContext(Qt::WidgetShortcut);


    QObject::connect(copyShortcut,      &QShortcut::activated,
                     this,              &nmfTableView::copy);
    QObject::connect(pasteShortcut,     &QShortcut::activated,
                     this,              &nmfTableView::paste);
    QObject::connect(allPasteShortcut,  &QShortcut::activated,
                     this,              &nmfTableView::pasteAll);
    QObject::connect(clearShortcut,     &QShortcut::activated,
                     this,              &nmfTableView::clear);
    QObject::connect(allClearShortcut,  &QShortcut::activated,
                     this,              &nmfTableView::clearAll);
    QObject::connect(selectShortcut,    &QShortcut::activated,
                     this,              &nmfTableView::selectAllItems);
    QObject::connect(deselectShortcut,  &QShortcut::activated,
                     this,              &nmfTableView::deselectAllItems);
}

void
nmfTableView::copy()
{
qDebug() << "copy";
    nmfUtilsQt::copy(qApp,this);
}

void
nmfTableView::paste()
{
    nmfUtilsQt::paste(qApp,this);
}

void
nmfTableView::pasteAll()
{
    nmfUtilsQt::pasteAll(qApp,this);
}


void
nmfTableView::clearAll()
{
    nmfUtilsQt::clearAll(this);
}

void
nmfTableView::clear()
{
    nmfUtilsQt::clear(qApp,this);
}

void
nmfTableView::selectAllItems()
{
    nmfUtilsQt::selectAll(this);
}

void
nmfTableView::deselectAllItems()
{
    nmfUtilsQt::deselectAll(this);
}


void
nmfTableView::keyPressEvent(QEvent *e)
{
  qDebug() << "key press event";
}


nmfTableView2::nmfTableView2(QWidget *parent)
  : QTableView(parent)
{
    QShortcut *copyShortcut      = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_C),           this);
    QShortcut *pasteShortcut     = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_V),           this);
    QShortcut *allPasteShortcut  = new QShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_V), this);
    QShortcut *clearShortcut     = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_X),           this);
    QShortcut *allClearShortcut  = new QShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_X), this);
    QShortcut *selectShortcut    = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_A),           this);
    QShortcut *deselectShortcut  = new QShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_A), this);

    allPasteShortcut->setContext(Qt::WidgetWithChildrenShortcut);


    QObject::connect(copyShortcut,      &QShortcut::activated,
                     this,              &nmfTableView2::copy);
    QObject::connect(pasteShortcut,     &QShortcut::activated,
                     this,              &nmfTableView2::paste);
    QObject::connect(allPasteShortcut,  &QShortcut::activated,
                     this,              &nmfTableView2::pasteAll);
    QObject::connect(clearShortcut,     &QShortcut::activated,
                     this,              &nmfTableView2::clear);
    QObject::connect(allClearShortcut,  &QShortcut::activated,
                     this,              &nmfTableView2::clearAll);
    QObject::connect(selectShortcut,    &QShortcut::activated,
                     this,              &nmfTableView2::selectAllItems);
    QObject::connect(deselectShortcut,  &QShortcut::activated,
                     this,              &nmfTableView2::deselectAllItems);
}

void
nmfTableView2::copy()
{
qDebug() << "copy";
    nmfUtilsQt::copy(qApp,this);
}

void
nmfTableView2::paste()
{
    nmfUtilsQt::paste(qApp,this);
}

void
nmfTableView2::pasteAll()
{
    nmfUtilsQt::pasteAll(qApp,this);
}

void
nmfTableView2::clearAll()
{
    nmfUtilsQt::clearAll(this);
}

void
nmfTableView2::clear()
{
    nmfUtilsQt::clear(qApp,this);
}

void
nmfTableView2::selectAllItems()
{
    nmfUtilsQt::selectAll(this);
}

void
nmfTableView2::deselectAllItems()
{
    nmfUtilsQt::deselectAll(this);
}
*/

namespace nmfUtilsQt {

bool fileExists(QString filenameWithPath)
{
    QFileInfo fileCheck(filenameWithPath);
    return (fileCheck.exists() && fileCheck.isFile());
}

void sendToOutputWindow(
        QTextEdit *te,
        bool clear)
{
    if (clear) {
        te->clear();
    }
}

void sendToOutputWindow(
        QTextEdit*  te,
        std::string label,
        std::string content)
{
    te->setTextColor(Qt::black);
    te->setFontFamily("Monospace");

    te->setFontWeight(QFont::Bold);
    te->append(label.c_str());

    te->setFontWeight(QFont::Normal);
    //te->moveCursor(QTextCursor::End);
    te->insertPlainText(content.c_str());
}

void sendToOutputWindow(
        QTextEdit*               te,
        std::string              title,
        std::vector<std::string> rowTitles,
        std::vector<std::string> colTitles,
        boost::numeric::ublas::matrix<double> &outMatrix,
        int                      numDigits,
        int                      numDecimals)
{
    char buf[1000];
    size_t numRows = outMatrix.size1();
    size_t numCols = outMatrix.size2();
    if (colTitles.size() <= numCols)
        return;
    std::string rowLabel = "";
    std::string content = title + "\n";
    te->setTextColor(Qt::black);
    te->setFontFamily("Monospace");
    te->setFontWeight(QFont::Bold);
    te->append(content.c_str());
    content = "";

    std::string format      = "%"+std::to_string(numDigits)+"."+std::to_string(numDecimals)+"f";
    std::string formatStr   = "%"+std::to_string(numDigits)+"s";
    std::string noFormatStr = "%s";
    std::string useFormat;

    // Load column labels into content
    for (unsigned int col=0; col<=numCols; ++col) {
        useFormat = (col == 0) ? noFormatStr : formatStr;
        sprintf(buf,useFormat.c_str(),colTitles[col].c_str());
        std::string valStr(buf);
        content += valStr + " ";
    }
    content += "\n";

    // Load row labels and data into content
    for (unsigned int row=0; row<numRows; ++row) {
        rowLabel = rowTitles[row];
        rowLabel.append("  ");
        content += rowLabel;
        for (unsigned int col=0; col<numCols; ++col) {
            sprintf(buf,format.c_str(),outMatrix(row,col));
            std::string valStr(buf);
            content += valStr + " ";
        }
        content += "\n";
    }
    te->setTextColor(Qt::black);
    te->setFontFamily("Monospace");
    te->setFontWeight(QFont::Normal);
    te->append(content.c_str());
    te->verticalScrollBar()->setValue(0);

} // end sendToOutputTextEdit

int getTabIndex(QTabWidget* tabWidget, QString tabName)
{
    for (int index=0; index<tabWidget->count(); ++index) {
        if (tabName == tabWidget->tabText(index)) {
            return index;
        }
    }
    return -1;
}

QString getCurrentTabName(QTabWidget* tabWidget)
{
    return tabWidget->tabText(tabWidget->currentIndex());
}

void equalizeQStringLengths(QString &s1, QString &s2)
{
    int diff = (s1.size()-s2.size());
    if (diff == 0)
        return;
    if (diff > 0) {
        for (int i=0;i<diff;++i)
            s2 = " " + s2;
    } else {
        for (int i=0;i<-diff;++i)
            s1 = " " + s1;
    }
} // end equalizeQStringLengths

bool allCellsArePopulated(QTabWidget *tabW,
                          QTableView *tv,
                          bool showError)
{
    QModelIndex index;
    QString value;
    QString msg;

    // Check if table as any empty cells
    int numRows = tv->model()->rowCount();
    int numCols = tv->model()->columnCount();
    for (int i=0; i<numRows; ++i) {
        for (int j=0; j<numCols; ++j) {
            index = tv->model()->index(i,j);
            value = index.data().toString();
            if (value.isEmpty()) { // Check if cell is blank
                if (showError) {
                    QMessageBox::question(tabW, QT_TR_NOOP("Missing Data"),
                                          QT_TR_NOOP("\nPlease complete missing table fields.\n"),
                                          QMessageBox::Ok);
                }
                return false;
            }
        } // end for j
    } // end for i

    return true;
}


bool allCellsArePopulated(QTabWidget *tabWidget,
                          QTableWidget *tableWidget,
                          bool showError,
                          bool skipFirstRow)
{
    int numRows  = tableWidget->rowCount();
    int numCols  = tableWidget->columnCount();
    int firstRow = (skipFirstRow) ? 1 : 0;
    QString value;
    QString msg;

    for (int i=firstRow; i<numRows; ++i) {
        for (int j=0; j<numCols; ++j) {
            value = tableWidget->item(i,j)->text();
            if (value.isEmpty()) { // Check if cell is blank
                if (showError) {
                    QMessageBox::question(tabWidget, QT_TR_NOOP("Missing Data"),
                                          QT_TR_NOOP("\nPlease complete missing table fields.\n"),
                                          QMessageBox::Ok);
                }
                return false;
            }
//            else if (value.contains(',')) {
//                if (showError) {
//                    msg = "Found an invalid numeric value of: " + value;
//                    msg += ". No commas or special characters allowed.";
//                    QMessageBox::warning(tabWidget, QT_TR_NOOP("Warning"),
//                                         QT_TR_NOOP("\n"+msg+"\n"),
//                                         QMessageBox::Ok);
//                }
//                return false;
//            }
        } // end for j
    } // end for i

    return true;
}


bool areAllCellsHoldingUniqueData(QTableView* tv1,
                                  QTableView* tv1Min,
                                  QTableView* tv1Max,
                                  QTableView* tv2,
                                  QTableView* tv2Min,
                                  QTableView* tv2Max,
                                  QString& badCell)
{
    badCell.clear();

    // First assure that no table is null
    if ((tv1    == nullptr) || (tv2    == nullptr) ||
        (tv1Min == nullptr) || (tv2Min == nullptr) ||
        (tv1Max == nullptr) || (tv2Max == nullptr)) {
        return true;
    }

    // Next assure that the tables are the same size (checking just tv1 and tv2 should be sufficient)
    QStandardItemModel* smodel1    = qobject_cast<QStandardItemModel*>(tv1->model());
    QStandardItemModel* smodel1Min = qobject_cast<QStandardItemModel*>(tv1Min->model());
    QStandardItemModel* smodel1Max = qobject_cast<QStandardItemModel*>(tv1Max->model());
    QStandardItemModel* smodel2    = qobject_cast<QStandardItemModel*>(tv2->model());
    QStandardItemModel* smodel2Min = qobject_cast<QStandardItemModel*>(tv2Min->model());
    QStandardItemModel* smodel2Max = qobject_cast<QStandardItemModel*>(tv2Max->model());
    if ((smodel1->rowCount()    != smodel2->rowCount()) ||
        (smodel1->columnCount() != smodel2->columnCount())) {
        return true;
    }

    // Finally check for values in same cells in each table
    bool cell1IsFixed,cell2IsFixed;
    int numRows = smodel1->rowCount();
    int numCols = smodel1->columnCount();

    QModelIndex index1,index1Min,index1Max;
    QModelIndex index2,index2Min,index2Max;
    QString value1,value1Min,value1Max;
    QString value2,value2Min,value2Max;
    for (int row=0; row<numRows; ++row) {
        for (int col=0; col<numCols; ++col) {
            index1    = smodel1->index(row,col);
            index1Min = smodel1Min->index(row,col);
            index1Max = smodel1Max->index(row,col);
            index2    = smodel2->index(row,col);
            index2Min = smodel2Min->index(row,col);
            index2Max = smodel2Max->index(row,col);
            value1    = index1.data().toString();
            value1Min = index1Min.data().toString();
            value1Max = index1Max.data().toString();
            value2    = index2.data().toString();
            value2Min = index2Min.data().toString();
            value2Max = index2Max.data().toString();
            cell1IsFixed = (value1 == value1Min && value1 == value1Max);
            cell2IsFixed = (value2 == value2Min && value2 == value2Max);
            if (! cell1IsFixed || ! cell2IsFixed) {
                badCell = QString("(")+QString::number(row)+","+QString::number(col)+")";
                return false;
            }
        }
    }

    return true;
}

bool areAllCellsNonBlank(QTableView* tv)
{
    QModelIndex index;
    QStandardItemModel* smodel = qobject_cast<QStandardItemModel*>(tv->model());
    QString value;

    for (int j=0; j<smodel->columnCount(); ++j) { // Species
        for (int i=0; i<smodel->rowCount(); ++i) { // Time
            index = smodel->index(i,j);
            value = index.data().toString().trimmed();
            if (value.isEmpty()) {
                return false;
            }
        }
    }
    return true;
}

bool areAllMaxCellsGreaterThanMinCells(
        QStandardItemModel* smodelMin,
        QStandardItemModel* smodelMax,
        QString& badCell)
{
    int numRows = smodelMin->rowCount();
    int numCols = smodelMin->columnCount();
    QModelIndex indexMin;
    QModelIndex indexMax;
    double valueMin;
    double valueMax;

    badCell.clear();

    for (int i=0; i<numRows; ++i) {
        for (int j=0; j<numCols; ++ j) {
            indexMin = smodelMin->index(i,j);
            indexMax = smodelMax->index(i,j);
            valueMin = indexMin.data().toString().remove(",").toDouble();
            valueMax = indexMax.data().toString().remove(",").toDouble();
            if (valueMin > valueMax) {
                badCell = QString("(")+QString::number(i)+","+QString::number(j)+")";
                return false;
            }
        }
    }

    return true;
}

// 3. Check initial cells are >= min cells and <= max cells
bool areAllInitialCellsWithinRange(
        QStandardItemModel* smodel,
        QStandardItemModel* smodelMin,
        QStandardItemModel* smodelMax,
        QString& badCell)
{
    int numRows = smodel->rowCount();
    int numCols = smodel->columnCount();
    QModelIndex index,indexMin,indexMax;
    double value,valueMin,valueMax;

    badCell.clear();

    for (int i=0; i<numRows; ++i) {
        for (int j=0; j<numCols; ++ j) {
            index    = smodel->index(i,j);
            indexMin = smodelMin->index(i,j);
            indexMax = smodelMax->index(i,j);
            value    = index.data().toString().remove(",").toDouble();
            valueMin = indexMin.data().toString().remove(",").toDouble();
            valueMax = indexMax.data().toString().remove(",").toDouble();
            if ((value < valueMin) || (value > valueMax)) {
                badCell = QString("(")+QString::number(i)+","+QString::number(j)+")";
                return false;
            }
        }
    }

    return true;
}

void buildColumnMap(
        nmfLogger* logger,
        QStandardItemModel* smodel,
        std::map<QString,int>& columnMap)
{
    int columnNumber = 0;
    int numCols = smodel->columnCount();

    while (columnNumber < numCols) {
        columnMap[smodel->horizontalHeaderItem(columnNumber)->text()] = columnNumber;
        ++columnNumber;
    }
}

void buildColumnMap(
        nmfLogger* logger,
        QTableWidget* tableW,
        std::map<QString,int>& columnMap)
{
    int columnNumber = 0;
    int numCols = tableW->columnCount();

    while (columnNumber < numCols) {
        columnMap[tableW->horizontalHeaderItem(columnNumber)->text()] = columnNumber;
        ++columnNumber;
    }
}

/*
 * This doesn't actually rename a file.  It copies the first
 * to the second.  I did this as the std:rename function didn't
 * work on Windows...and a system mv command caused a shell
 * to pop up briefly over the UI.
 */
int rename(QString fileIn, QString fileOut)
{
    QFile fin(fileIn);
    QFile fout(fileOut);
    QString line;

    // Open the files here....
    if (! fin.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "nmfQtUtils::rename: Error Opening Input File", fin.errorString());
        return -1;
    }
    if (! fout.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, "nmfQtUtils::rename: Error Opening Output File", fout.errorString());
        return -1;
    }
    QTextStream inStream(&fin);
    QTextStream outStream(&fout);

    while (! inStream.atEnd()) {
        line = inStream.readLine();
        outStream << line + "\n";
    }
    fin.close();
    fout.close();

    // Delete the tmp file
    int retv = std::remove(fileIn.toLatin1());
    if (retv != 0) {
        QMessageBox::information(0, "nmfQtUtils::rename: Error removing file: ", fileIn);
        //std::cout << "Error: Problem removing file: " << fileIn.toStdString() << std::endl;
    }

    return 1;
} // end rename


bool clearCSVFile(std::string  tableToClear,
                  std::string  projectDir,
                  std::string& errorMsg)
{
    int retv;
    QString line;
    QString csvFile;
    QString filePath;
    QString fileNameWithPath;
    QString tmpFileNameWithPath;
    errorMsg.clear();

    csvFile = QString::fromStdString(tableToClear) + ".csv";
    filePath = QDir(QString::fromStdString(projectDir)).filePath(QString::fromStdString(nmfConstantsMSVPA::InputDataDir));
    fileNameWithPath    = QDir(filePath).filePath(csvFile);
    tmpFileNameWithPath = QDir(filePath).filePath("."+csvFile);
    QFileInfo fi(fileNameWithPath);
    if (! fi.exists()) {
        std::cout << "Note: " + csvFile.toStdString() << " does not exist." << std::endl;
        return false;
    }

    QFile fin(fileNameWithPath);
    QFile fout(tmpFileNameWithPath);
    if (! fin.open(QIODevice::ReadOnly)) {
       errorMsg = "Error nmfQtUtils::clearCSVFile: Opening Input File\n" +
                   fin.errorString().toStdString() + ": " +
                   fileNameWithPath.toStdString();
       return false;
    }
    if (! fout.open(QIODevice::WriteOnly)) {
        errorMsg = "Error nmfQtUtils::clearCSVFile: Opening Output File\n" +
                    fout.errorString().toStdString() + ": " +
                    tmpFileNameWithPath.toStdString();
        return false;
    }
    QTextStream inStream(&fin);
    QTextStream outStream(&fout);

    // Just copy the first line and omit the rest.
    while (! inStream.atEnd()) {
        line = inStream.readLine();
        outStream << line << "\n";
        break;
    } // end while
    fin.close();
    fout.close();

    // Now, mv temp file to actual file.
    retv = rename(tmpFileNameWithPath,fileNameWithPath);
    if (retv < 0) {
        errorMsg = "Error nmfQtUtils::clearCSVFile: Couldn't rename " +
                     fileNameWithPath.toStdString() + " to " +
                     tmpFileNameWithPath.toStdString() +
                     ". Save aborted.\n";
        return false;
    }

    return true;

} // end clearCSVFile


QString
copy(QApplication* qtApp, QTableView* tableView)
{
    if (! tableView) {
        return "\nCopy not available.";
    }

    QString text;
    QString copiedText;
    QModelIndex current;
    QModelIndex previous;
    QItemSelectionModel *selection = tableView->selectionModel();
    if (! selection->hasSelection()) {
        return "\nNo valid selection found. Cells must have data prior to making a selection.";
    }
    QModelIndexList indexes = selection->selectedIndexes();

    // Sort the indexes in case the user selects from bottom right to top left.
//  qSort(indexes);
    std::sort(indexes.begin(),indexes.end());

    // Copy a single cell
    if (indexes.size() == 1) {

        copiedText = tableView->model()->data(indexes[0]).toString().remove(",");
        copiedText.append(QLatin1Char('\n'));

    } else if (indexes.size() > 1) { // Copy multiple cells

        // You need a pair of indexes to find the row changes
        previous = indexes.first();
        indexes.removeFirst();
        Q_FOREACH (current,indexes)
        {
            text = tableView->model()->data(previous).toString().remove(",");
            copiedText.append(text);
            text.replace("\n","<br>");

            // Add either a \n or \t if you're at the row end or just between fields.
            if (current.row() != previous.row()) {
                copiedText.append(QLatin1Char('\n'));
            } else {
                copiedText.append(QLatin1Char('\t'));
            }
            previous = current;
        }

        // Don't forget the last item
        copiedText.append(tableView->model()->data(current).toString().remove(","));

    } else {
        return "\nPlease select one or more cells to copy.";
    }

    // Need to add an extra newline since when you copy from an external
    // spreadsheet app, an extra newline is added.
    copiedText.append(QLatin1Char('\n'));

    QMimeData *md = new QMimeData();
    md->setText(copiedText);
    qtApp->clipboard()->setMimeData(md);

    return "";
}

QString
paste(QApplication* qtApp, QTableView* tableView)
{
    if (! tableView) {
        return "\nPaste not available.";
    }

    int m = 0;
    int nrows;
    int ncols;
    int startRow;
    int startCol;
    QModelIndex startIndex;
    QModelIndex pasteIndex;
    QModelIndex currentIndex;
    QStringList copiedCells;
    QString     copiedText;
    QItemSelectionModel *selection    = tableView->selectionModel();
    QModelIndexList      pasteIndexes = selection->selectedIndexes();

    if (pasteIndexes.size() == 0) {
        return "\nPlease select one or more cells to paste into.";
    }

    // Get the copied data and strip off any white spaces from either end.
    copiedText  = qtApp->clipboard()->text();
    copiedCells = copiedText.split(QRegExp(QLatin1String("\\n|\\t")));

    while (! copiedCells.empty() && copiedCells.back().size() == 0) {
        // Remove last character
        copiedCells.pop_back();
    }

    // Paste the copied data into the paste selection.
    nrows = copiedText.count(QLatin1Char('\n'));
    nrows = (nrows == 0) ? 1 : nrows; // Check in case user double clicks in a cell and copies/paste like that (instead of clicking once on a cell)
    ncols = copiedCells.size() / nrows;

    // If copy selection is same size as paste selection
    if ((pasteIndexes.size() == 1) && (copiedCells.size() == 1)) {
        tableView->model()->setData(pasteIndexes[m],QVariant(copiedCells[m].trimmed()));
    } else if (pasteIndexes.size() == copiedCells.size()) {
        for (int i=0; i<nrows; ++i) {
            for (int j=0; j<ncols; ++j) {
                tableView->model()->setData(pasteIndexes[m],QVariant(copiedCells[m].trimmed()));
                ++m;
            }
        }

    } else if ((copiedCells.size() == 1) && (pasteIndexes.size() > 1)) { // fill paste selection with copied value

        Q_FOREACH (pasteIndex,pasteIndexes) {
            currentIndex = tableView->model()->index(pasteIndex.row(),pasteIndex.column());
            tableView->model()->setData(currentIndex,QVariant(copiedCells[0].trimmed()));
        }

    // If copy selection is different size then just use the first cell of the paste selection
    // to anchor the paste.
    } else {
        int numCopiedCells   = copiedCells.size();
        int numSelectedCells = pasteIndexes.size();
        QMessageBox::StandardButton reply = QMessageBox::Yes;
        if (numSelectedCells != 1) {
            QString msg = "\nThe number of copied cells (" + QString::number(numCopiedCells) +
                    ") is different from number of selected cells (" + QString::number(numSelectedCells) +
                    ") in which to paste.\n\nOK to continue?";
            reply = QMessageBox::warning(tableView,
                                         "Paste Warning",
                                         msg.toLatin1(),
                                         QMessageBox::No|QMessageBox::Yes,
                                         QMessageBox::Yes);
        }
        if (reply == QMessageBox::Yes) {
            startIndex = pasteIndexes[0];
            startRow = startIndex.row();
            startCol = startIndex.column();
            for (int i=0; i<nrows; ++i) {
                for (int j=0; j<ncols; ++j) {
                    currentIndex = tableView->model()->index(startRow+i,startCol+j);
                    tableView->model()->setData(currentIndex,QVariant(copiedCells[m].trimmed()));
                    ++m;
                }
            }
        }

    }

    // Ensure tableview is updated after the paste.
    tableView->viewport()->update();

    return "";
}

QString
pasteAll(QApplication* qtApp, QTableView* tableView)
{
    QString retv;

    retv = copy(qtApp,tableView);
    if (! retv.isEmpty())
        return retv;

    retv = selectAll(tableView);
    if (! retv.isEmpty())
        return retv;

    retv = paste(qtApp,tableView);
    if (! retv.isEmpty())
        return retv;

    retv = deselectAll(tableView);
    if (! retv.isEmpty())
        return retv;

    return "";
}

QString
clearAll(QTableView* tableView)
{
    if (! tableView) {
        return "\nClear All not available.";
    }

    QModelIndex pasteIndex;
    int nrows = tableView->model()->rowCount();
    int ncols = tableView->model()->columnCount();

    for (int i=0; i<nrows; ++i) {
        for (int j=0; j<ncols; ++j) {
            pasteIndex = tableView->model()->index(i,j);
            tableView->model()->setData(pasteIndex,QVariant(""));
        }
    }

    return "";
}

QString
clearSelection(QTableView* tableView, QModelIndexList indexes)
{
    for (QModelIndex index : indexes) {
        tableView->model()->setData(index,QVariant(""));
    }
    return "";
}

QString
clear(QApplication* qtApp,
      QTableView*   tableView)
{
    if (! tableView) {
        return "\nClear not available.";
    }

    QString retv;
    QItemSelectionModel *selection = tableView->selectionModel();
    QModelIndexList      indexes   = selection->selectedIndexes();
    int selectedCells              = indexes.size();
    int totalCells                 = tableView->model()->rowCount()*tableView->model()->columnCount();

    // Nothing was selected or everything was selected. Clear entire table.
    if ((selectedCells == 0) || (selectedCells == totalCells)) {
        retv = selectAll(tableView);
        if (! retv.isEmpty())
            return retv;

        retv = copy(qtApp,tableView);
        if (! retv.isEmpty())
            return retv;

        retv = deselectAll(tableView);
        if (! retv.isEmpty())
            return retv;

        retv = clearAll(tableView);
        if (! retv.isEmpty())
            return retv;
    } else {
        retv = copy(qtApp,tableView);
        if (! retv.isEmpty())
            return retv;
        retv = clearSelection(tableView,indexes);
        if (! retv.isEmpty())
            return retv;
    }

    return "";
}


QString
selectAll(QTableView* tableView)
{
    if (! tableView) {
        return "\nSelect All not available.";
    }

    tableView->selectAll();

    return "";
}


QString
deselectAll(QTableView* tableView)
{
    if (! tableView) {
        return "\nDeselect All not available.";
    }

    tableView->selectionModel()->clear();

    return "";
}

QTreeWidgetItem*
addTreeRoot(QTreeWidget* parent,
            QString label)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);

    item->setText(0, label);

    return item;
}

void
addTreeItem(QTreeWidgetItem *parent,
            QString label)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    item->setText(0, label);

    parent->addChild(item);
}

QString
formatTimeSeconds(const int& timeSeconds)
{
    QString formattedTime = "n/a";

    if (timeSeconds < 60) {
        formattedTime = QString::number(timeSeconds)+" sec(s)";
    } else if (timeSeconds < 3600) {
        formattedTime = QString::number((double)timeSeconds/   60.0,'f',1) + " min(s)";
    } else if (timeSeconds < 86400) {
        formattedTime = QString::number((double)timeSeconds/ 3600.0,'f',1) + " hr(s)";
    } else {
        formattedTime = QString::number((double)timeSeconds/86400.0,'f',1) + " day(s)";
    }

    return formattedTime;
}

void
showAboutWidget(
        QWidget* parent,
        const QString& name,
        const QString& operatingSystem,
        const int& upTimeSeconds,
        const QString& version,
        const QString& specialAcknowledgement,
        const QString& appMsg)
{
    QString msg = "<strong><br>"+version+" (" + operatingSystem + ")</strong>";
    msg += "<strong><br>"+name+"</strong>";
    QString date = QDate::currentDate().toString();

    msg += QString("<br><br>Application runtime: ") + formatTimeSeconds(upTimeSeconds);
    msg += QString("<br><br>Built on: ") + date; //QDate::fromString(date,"dddYYYYMMdd").toString();

    msg += QString("<br><br>Produced by the NOAA National Marine Fisheries Service.");
    msg += specialAcknowledgement;
    msg += QString("<br><br>The author gratefully acknowledges the use of the following packages:");
    msg += QString("<ul>");

    // Show the message
    QPixmap pixmap(":/icons/NOAA.png");
    QPixmap scaled = pixmap.scaled(110,110,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QMessageBox msgBox(parent);
    QSpacerItem* horSP = new QSpacerItem(550, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    msgBox.setText(msg+appMsg);
    msgBox.setIconPixmap(scaled);
    QGridLayout* gridLT = qobject_cast<QGridLayout *>(msgBox.layout());
    gridLT->addItem(horSP, gridLT->rowCount(), 0, 1, gridLT->columnCount());
    msgBox.exec();
}

void sort(QComboBox* cmbox)
{
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel(cmbox);
    proxy->setSourceModel(cmbox->model());
    // Reparent the combobox's parent so QComboBox's setModel() doesn't delete it
    cmbox->model()->setParent(proxy);
    cmbox->setModel(proxy);
    // sort the model which will sort the items in the combobox
    cmbox->model()->sort(0);
}

double calculateUnitsScaleFactor(
        const QString& previousUnits,
        const QString& currentUnits)
{
    double scaleFactor = 1.0;

    // Find the appropriate scale factor
    if (previousUnits == "lbs") {
        if (currentUnits == "kg") {
            scaleFactor = nmfConstants::LBS_to_KG;
        } else if (currentUnits == "mt") {
            scaleFactor = nmfConstants::LBS_to_MT;
        }
    } else if (previousUnits == "kg") {
        if (currentUnits == "lbs") {
            scaleFactor = nmfConstants::KG_to_LBS;
        } else if (currentUnits == "mt") {
            scaleFactor = nmfConstants::KG_to_MT;
        }
    } else if (previousUnits == "mt") {
        if (currentUnits == "lbs") {
            scaleFactor = nmfConstants::MT_to_LBS;
        } else if (currentUnits == "kg") {
            scaleFactor = nmfConstants::MT_to_KG;
        }
    }

    return scaleFactor;
}

void
checkForAndDeleteLogFiles(QString name,
                          std::string logDirName,
                          std::string logFilter)
{
    QString title = name + " Log Files";
    QMessageBox::StandardButton reply;
    size_t NumLogFiles = 0;
    QString fileToRemoveWithPath;
    QDir directory(QString::fromStdString(logDirName));
    QStringList filenames = directory.entryList(QStringList() << "*.log", QDir::Files);
    NumLogFiles = filenames.size();

    // Ask if user wants to remove log files if there are a multiple of
    // nmfConstants::MaxNumberLogFiles of them.
    if ((NumLogFiles % nmfConstants::MaxNumberLogFiles == 0) && (NumLogFiles != 0)) {
        QString msg = "\nYou have ";
        msg += QString::number(NumLogFiles);
        msg += " log files.\n\n";
        msg += "Would you like to delete them?\n";
        reply = QMessageBox::question(0,
                             title,
                             msg.toLatin1(),
                             QMessageBox::No|QMessageBox::Yes,
                             QMessageBox::Yes);
        if (reply == QMessageBox::Yes) {
            bool delOK = true;
            bool allDeleted = true;
            for (QString filename : filenames) {
                fileToRemoveWithPath = QDir(QString::fromStdString(logDirName)).filePath(filename);
                delOK = (std::remove(fileToRemoveWithPath.toLatin1()) == 0);
                if (!delOK) {
                    allDeleted = false;
                }
            }
            if (allDeleted) {
                QMessageBox::information(0,
                             title,
                             "\nLog files deleted.\n",
                             QMessageBox::Ok);
            }
        }
    }
}


void checkForAndCreateDirectories(std::string dir,
                                  std::string dataDir,
                                  std::string logDir)
{
    QStringList dirs = {QString::fromStdString(dir),
                        QString::fromStdString(dataDir),
                        QString::fromStdString(logDir)};

    for (QString dir : dirs) {
        if (! QDir(dir).exists()) {
            QDir().mkdir(dir);
        }
    }
}

void convertTableWidget(
        QTableWidget* tableWidget,
        const int& col,
        const int& numSigDig,
        const QString& previousUnits,
        const QString& currentUnits)
{
    double scaleFactor = calculateUnitsScaleFactor(previousUnits,currentUnits);
    double valueWithoutComma;
    QString valueWithComma;
    QTableWidgetItem* item;

    // Apply the scale factor
    for (int row=0; row<tableWidget->rowCount(); ++row) {
        // Get the cell data
        valueWithoutComma = tableWidget->item(row,col)->text().remove(",").toDouble();
        // Convert the cell data and update the model
        valueWithComma = checkAndCalculateWithSignificantDigits(
                            scaleFactor*valueWithoutComma,numSigDig,6);
        item = new QTableWidgetItem(valueWithComma);
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row,col,item);
    }
    tableWidget->resizeColumnsToContents();
}

void convertTableView(
        QTableView* tableView,
        const int& numSigDig,
        const QString& previousUnits,
        const QString& currentUnits)
{
    double scaleFactor = calculateUnitsScaleFactor(previousUnits,currentUnits);
    double valueWithoutComma;
    QModelIndex index;
    QString valueWithComma;
    QStandardItem* item;

    // Apply the scale factor
    QStandardItemModel* smodel = qobject_cast<QStandardItemModel*>(tableView->model());
    for (int row=0; row<smodel->rowCount(); ++row) {
        for (int col=0; col<smodel->columnCount(); ++col) {
            // Get the cell data
            index = smodel->index(row,col);
            valueWithoutComma = index.data().toString().remove(",").toDouble();
            // Convert the cell data and update the model
            valueWithComma = checkAndCalculateWithSignificantDigits(
                                scaleFactor*valueWithoutComma,numSigDig,6);
            item = new QStandardItem(valueWithComma);
            item->setTextAlignment(Qt::AlignCenter);
            smodel->setItem(row,col,item);
        }
    }
    tableView->resizeColumnsToContents();
}

void convertVector(
        boost::numeric::ublas::vector<double>& vecData,
        const QString& previousUnits,
        const QString& currentUnits)
{
    double scaleFactor = calculateUnitsScaleFactor(previousUnits,currentUnits);

    for (int i=0; i<(int)vecData.size(); ++i) {
        vecData[i] *= scaleFactor;
    }
}

void convertMatrix(
        boost::numeric::ublas::matrix<double>& matData,
        const QString& previousUnits,
        const QString& currentUnits,
        const bool& convertBlanks)
{
    double scaleFactor = calculateUnitsScaleFactor(previousUnits,currentUnits);

    for (int row=0; row<(int)matData.size1(); ++row) {
        for (int col=0; col<(int)matData.size2(); ++col) {
            if (convertBlanks == nmfConstantsMSSPM::ConvertAll) {
                matData(row,col) *= scaleFactor;
            }
        }
    }
}

void convertVectorToStrList(const std::vector<std::string>& labels,
                            QStringList& slist)
{
    for (unsigned i=0; i<labels.size(); ++i) {
        slist << QString::fromStdString(labels[i]);
    }
}

QSettings*
createSettings(const std::string& winDir,
               const QString& name)
{
    QString arg1 = "NOAA";
    QString arg2 = name;
    QString windowsDir = QString::fromStdString(winDir);

    if (nmfUtils::isOSWindows()) {
        // Make .QtSettings dir first if not there
        QDir().mkdir(windowsDir);
        arg1 = QDir(windowsDir).filePath(name+"_Settings.ini");
        QSettings* settings = new QSettings(arg1,QSettings::IniFormat);
        return settings;
    } else {
        QSettings* settings = new QSettings(arg1,arg2);
        return settings;
    }
    return nullptr;
}

void
removeCommas(QTableView* tableView)
{
    double valueWithoutComma;
    QModelIndex index;
    QStandardItem* item;
    QString valueStr;

    // Apply the scale factor
    QStandardItemModel* smodel = qobject_cast<QStandardItemModel*>(tableView->model());
    for (int row=0; row<smodel->rowCount(); ++row) {
        for (int col=0; col<smodel->columnCount(); ++col) {
            index = smodel->index(row,col);
            valueStr = index.data().toString();
            if (! valueStr.isEmpty()) {
                valueWithoutComma = valueStr.remove(",").toDouble();
                item = new QStandardItem(QString::number(valueWithoutComma,'g',10)); // RSK replace this with precision value in preferences. Different precision for different data?
                item->setTextAlignment(Qt::AlignCenter);
                smodel->setItem(row,col,item);
            }
        }
    }
    tableView->resizeColumnsToContents();
}

bool
isEmpty(QTableView* tableview)
{
   return ((tableview == nullptr) ||
           (tableview->model() == nullptr) ||
           (tableview->model()->rowCount() == 0));
}

bool
removeSettingsFile()
{
    QString fileToRemove;
    QString appName = QApplication::applicationName();
    QDir dir;
    if (nmfUtils::isOSWindows()) {
        if (appName == "MSCAA") {
            fileToRemove = QDir(QString::fromStdString(nmfConstantsMSCAA::SettingsDirWindows)).filePath(appName+"_Settings.ini");
std::cout << "Removing: " << fileToRemove.toStdString() << std::endl;
            if (! dir.remove(fileToRemove)) {
                std::cout << "Error: failed to remove settings file" << std::endl;
            }
        } else if (appName == "MSSPM") {
            fileToRemove = QDir(QString::fromStdString(nmfConstantsMSSPM::SettingsDirWindows)).filePath(appName+"_Settings.ini");
std::cout << "Removing: " << fileToRemove.toStdString() << std::endl;
            if (! dir.remove(fileToRemove)) {
                std::cout << "Error: failed to remove settings file" << std::endl;
            }
        } else if (appName == "MSVPA_X2") {
            fileToRemove = QDir(QString::fromStdString(nmfConstantsMSVPA::SettingsDirWindows)).filePath(appName+"_Settings.ini");
std::cout << "Removing: " << fileToRemove.toStdString() << std::endl;
            if (! dir.remove(fileToRemove)) {
                std::cout << "Error: failed to remove settings file" << std::endl;
            }
        } else if (appName == "Donut") {
            fileToRemove = QDir(QString::fromStdString(nmfConstantsDonut::SettingsDirWindows)).filePath(appName+"_Settings.ini");
std::cout << "Removing: " << fileToRemove.toStdString() << std::endl;
            if (! dir.remove(fileToRemove)) {
                std::cout << "Error: failed to remove settings file" << std::endl;
            }
        } else {
          return false;
        }

    } else {
        fileToRemove = QDir(".config/NOAA").filePath(appName+".conf");
        fileToRemove = QDir(QDir::homePath()).filePath(fileToRemove);
std::cout << "Removing: " << fileToRemove.toStdString() << std::endl;
        if (! dir.remove(fileToRemove)) {
            std::cout << "Error: failed to remove settings file" << std::endl;
        }
    }
    return true;
}


int calculateMultiColumnWidth(
        QTableView *tv,
        const int& firstCol,
        const int& lastCol)
{
    int newWidth = 0;
    for (int col=firstCol; col<=lastCol; ++col) {
        newWidth += tv->columnWidth(col);
    }
    return newWidth;
}

bool loadModelFromCSVFile(nmfLogger* logger,
                          std::string projectDir,
                          std::string fileType,
                          QTableView* table,
                          QString fileName,
                          int& numRows,
                          int& numSignificantDigits)
{
    bool ok;
    int numCols=0;
    double value;
    std::string errorMsg;
    QStandardItemModel* smodel = qobject_cast<QStandardItemModel*>(table->model());
    QString dataPath = QDir(QString::fromStdString(projectDir)).filePath("outputData");
    QString line;
    QString valueWithComma;
    QString valueStr;
    QStringList parts;
    QStringList horizHeader;
    QList<QStringList> allLines;
    QStandardItem* item;
    QFile file(fileName);

    numRows = 0;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        while (stream.readLineInto(&line)) {
            parts = line.trimmed().split(",");
            if (numRows == 0) {
                if (line.trimmed() != QString::fromStdString(fileType).trimmed()) {
                    errorMsg = "nmfUtilsQt::loadModelFromCSVFile: ";
                    errorMsg += line.trimmed().toStdString() + " != " + fileType;
                    logger->logMsg(nmfConstants::Error,errorMsg);
                    return false;
                }
            }
            else if (numRows == 1) {
                horizHeader = parts;
                numCols = parts.size();
            } else {
                allLines << parts;
            }
            ++numRows;
        }
    }
    if (numRows < 2) {
        errorMsg = "nmfUtilsQt::loadModelFromCSVFile: num rows < 2";
        logger->logMsg(nmfConstants::Error,errorMsg);
        return true;
    }

    smodel = new QStandardItemModel( allLines.size(), numCols );
    int row = 0;
    int col = 0;
    for (QStringList line : allLines) {
        col = 0;
        for (QString part : line) {
            part.replace("||","\n");
            valueStr = part.trimmed();
            value    = valueStr.toDouble(&ok);

            if (ok && (numSignificantDigits > 0)) {
//                if (fileType == "Summary Diagnostic") {
//                    valueWithComma = checkAndCalculateWithSignificantDigits(
//                                value,numSignificantDigits,-4);
//                } else if ((fileType == "Summary Model Fit") ||
//                    (col == nmfConstantsMSSPM::Model_Review_Column_rSquared) ||
//                    (col == nmfConstantsMSSPM::Model_Review_Column_AIC)) {
//                    valueWithComma = checkAndCalculateWithSignificantDigits(
//                                value,numSignificantDigits,-4);
//                } else {
                    valueWithComma = checkAndCalculateWithSignificantDigits(
                                value,numSignificantDigits,4);
//                }
                valueStr = valueWithComma;
            }
            item = new QStandardItem(valueStr);
            item->setTextAlignment(Qt::AlignCenter);
            smodel->setItem(row, col, item);
            ++col;
        }
        ++row;
    }

    smodel->setHorizontalHeaderLabels(horizHeader);
    table->setModel(smodel);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    return true;
}

void saveModelToCSVFile(std::string projectDir,
                        std::string fileType,
                        std::string tabName,
                        QTableView* table,
                        bool queryFilename,
                        bool removeCommas,
                        QString theFilename,
                        bool verboseOn)
{
    bool ok = true;
    QString val;
    QString msg;
    std::string vHeaderTitle;
    QMessageBox::StandardButton reply;
    QStandardItemModel* smodel = qobject_cast<QStandardItemModel*>(table->model());
    int numColumns = smodel->columnCount();
    int numRows    = smodel->rowCount();
    QString dataPath = QDir(QString::fromStdString(projectDir)).filePath("outputData");
    QString filename;
    QString fullPath = "";
    QString value;

    if (queryFilename) {
        filename = QInputDialog::getText(table, QObject::tr("CSV Filename"),
                                         QObject::tr("Enter desired CSV filename:"),
                                         QLineEdit::Normal,
                                         QObject::tr(""), &ok);
        fullPath = QDir(dataPath).filePath(filename);

        // if file exists, query user if they wan't to overwrite
        if (QFileInfo(fullPath).exists()) {
            msg = "\nFile exists: " + fullPath + "\n\nOK to overwrite?\n";
            reply = QMessageBox::warning(table, QObject::tr("File Exists"),
                 msg, QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::No) {
                return;
            }
        }
    } else {
        fullPath = theFilename;
    }

    if (ok && !fullPath.isEmpty())
    {
        // open output file
        std::ofstream outputFile;
        outputFile.open(fullPath.toLatin1());
        outputFile << fileType << "\n";

        // get data from model and write to output file in csv fashion
        if (smodel->verticalHeaderItem(0) == nullptr) {
            vHeaderTitle.clear();
        } else {
            if (tabName == "Data") {
                vHeaderTitle = "Year";
            } else if (tabName == "Summary Model Fit") {
                vHeaderTitle = "Num";
            } else if (tabName == "Output Biomass:") {
                vHeaderTitle = "Year";
            } else {
                vHeaderTitle = "Species";
            }
        }

        if (! vHeaderTitle.empty()) {
            outputFile << vHeaderTitle << ", ";
        }

        for (int j=0; j<numColumns; ++j) {
            value = smodel->horizontalHeaderItem(j)->text().trimmed();
            if (removeCommas) {
                value.replace(",","");
            } else {
                value.replace(",",";");
            }
            outputFile << value.toStdString();
            if (j < numColumns-1) {
                outputFile << ", ";
            } else {
                outputFile << "\n";
            }
        }

        for (int i=0; i<numRows; ++i) {
            for (int j=0; j<numColumns; ++j) {
                if (j == 0) {
                    if (smodel->verticalHeaderItem(i) != nullptr) {
                        outputFile << smodel->verticalHeaderItem(i)->text().toStdString() << ", ";
                    }
                }
                val = smodel->item(i,j)->text().trimmed();
                if (removeCommas) {
                    val.replace(",","");
                } else {
                    val.replace(",",";");
                }
                val.replace("\n","||");
                outputFile << val.toStdString();
                if (j < numColumns-1) {
                    outputFile << ", ";
                } else {
                    outputFile << "\n";
                }
            }
        }

        // close output file
        outputFile.close();

        if (verboseOn) {
            msg = "\nFile saved successfully:\n\n" + fullPath + "\n";
            QMessageBox::information(table, QObject::tr("Project"),
                                     msg,
                                     QMessageBox::Ok);
        }

    }
}

void
getNumSpeciesFromImportFile(
        const QString& filename,
        int& numRows)
{
    QString allLines;
    QStringList lineList;
    QStringList dataParts;
    QStringList ColumnHeader  = {};

    numRows = 0;
    if (! filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            allLines = file.readAll().trimmed();
            lineList = allLines.split('\n');
            numRows = lineList.count()-1; // -1 for the header
            //numCols = lineList[0].split(',').count();
        }
        file.close();
    }
}

bool
loadTableWidgetData(QTabWidget* parentTabWidget,
                    QTableWidget* tableWidget,
                    const QString& filename,
                    const int& numRows,
                    const int& numCols,
                    QStringList& guildValues,
                    const std::vector<int>& ColumnsToLoad,
                    QString& errorMsg)
{
    int colNum = 0;
    QString allLines;
    QStringList lineList;
    QStringList dataParts;
    QStringList ColumnHeader  = {};
    bool isSpeciesTable = tableWidget->objectName().contains("Species");

    errorMsg.clear();

    if (tableWidget == nullptr) {
        errorMsg = "Error: No model found in table. Please save initial table data.";
        return false;
    }

    if (! filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            allLines = file.readAll().trimmed();
            lineList = allLines.split('\n');
            if (tableWidget->rowCount() == numRows) {
                // Create the header
                QStringList speciesParts = lineList[0].split(',');
                for (int col=0; col<(int)ColumnsToLoad.size(); ++col) {
                    ColumnHeader << speciesParts[ColumnsToLoad[col]];
                }
                // Create and load the table widgets
                for (int row=1;row<=numRows; ++row) {
                    dataParts = lineList[row].split(',');
                    for (int col=0; col<(int)ColumnsToLoad.size(); ++col) {
                        colNum = ColumnsToLoad[col];
                        if (isSpeciesTable && (colNum == nmfConstantsMSSPM::Column_Species_Guild)) {
                            QComboBox* cbox = new QComboBox();
                            cbox->addItems(guildValues);
                            cbox->setCurrentText(dataParts[colNum]);
                            tableWidget->setCellWidget(row-1,col,cbox);
                        } else if (! isSpeciesTable) { // it's the guild table
                            QTableWidgetItem* item = new QTableWidgetItem();
                            item->setTextAlignment(Qt::AlignCenter);
                            item->setText(dataParts[colNum]);
                            tableWidget->setItem(row-1,col,item);
                            if (colNum == 0) { // nmfConstantsMSSPM::Column_Species_Guild) {
                                guildValues << dataParts[0];
                            }
                        } else {
                            QTableWidgetItem* item = new QTableWidgetItem();
                            item->setTextAlignment(Qt::AlignCenter);
                            item->setText(dataParts[colNum]);
                            tableWidget->setItem(row-1,col,item);
                        }
                    }
                }
                tableWidget->setHorizontalHeaderLabels(ColumnHeader);
                tableWidget->resizeColumnsToContents();
            } else {
                errorMsg = "Error: table size from .csv file (" +
                        QString::number(numRows) + "x" + QString::number(numCols) +
                        ") does not equal current size of table (" +
                        QString::number(tableWidget->rowCount()) + "x" +
                        QString::number(tableWidget->columnCount()) + ")";
            }
            file.close();
        }
    }
    return true;
}

bool
loadGuildsSpeciesTableview(QTabWidget* parentTabWidget,
                           QStandardItemModel* smodel,
                           const QString& type,
                           const QString& inputDataPath,
                           const QString& inputFilename,
                           QList<QString>& SpeciesGuilds,
                           const bool& queryForFilename,
                           QString& filename,
                           const int& nameColumn,
                           QString& errorMsg)
{    bool retv = false;
     bool isSpecies = (type == "Species");
     int offset = (isSpecies) ? 1 : 0;
     QString allLines;
     QStringList lineList;
     QStringList dataParts;
     QStringList ColumnLabelList  = {};
     QStandardItem* item;
     errorMsg.clear();
     SpeciesGuilds.clear();
     QLocale locale(QLocale::English);
     QString valueWithComma;

     if (smodel == nullptr) {
         errorMsg = "Error: No model found in table. Please save initial table data.";
         return false;
     }

     if (queryForFilename) {
         filename = (inputFilename.isEmpty()) ?
                     QFileDialog::getOpenFileName(parentTabWidget,
                                                  QObject::tr("Select CSV file"), inputDataPath,
                                                  QObject::tr("Data Files (species*.csv guilds*.csv)")) :
                     inputFilename;
     }

     if (! filename.isEmpty()) {
         QFile file(filename);
         if (file.open(QIODevice::ReadOnly)) {
             allLines = file.readAll().trimmed();
             lineList = allLines.split('\n');
             int numLines   = lineList.count()-1; // -1 for the header
             int numColumns = lineList[0].split(',').count()-offset; // offset needed because we don't want the Guild field from the Species .csv file in the model
             if ((smodel->rowCount()    == numLines) &&
                 (smodel->columnCount() == numColumns))
             {
                 QStringList columnLabelParts = lineList[0].split(',');
                 if (isSpecies) {
                     columnLabelParts.removeAt(nmfConstantsMSSPM::Column_Species_Guild);
                 }
                 for (int j=0;j<columnLabelParts.count();++j) {
                     ColumnLabelList << columnLabelParts[j];
                 }
                 for (int i=1; i<lineList.count(); ++i) {
                     dataParts = lineList[i].split(',');
                     if (isSpecies) {
                         SpeciesGuilds.push_back(dataParts[nmfConstantsMSSPM::Column_Species_Guild]);
                         dataParts.removeAt(nmfConstantsMSSPM::Column_Species_Guild);
                     }
                     for (int j=0; j<dataParts.count(); ++j) {
                         if (j != nameColumn) {
                             valueWithComma = locale.toString(dataParts[j].toDouble());
                             item = new QStandardItem(valueWithComma);
                         } else {
                             item = new QStandardItem(dataParts[j]);
                         }
                         item->setTextAlignment(Qt::AlignCenter);
                         smodel->setItem(i-1, j, item);
                     }
                 }
                 smodel->setHorizontalHeaderLabels(ColumnLabelList);
             } else {
                 errorMsg = "Error: table size from .csv file (" +
                         QString::number(numLines) + "x" + QString::number(numColumns) +
                         ") does not equal current size of table (" +
                         QString::number(smodel->rowCount()) + "x" +
                         QString::number(smodel->columnCount()) + ")";
             }
             file.close();
         }
         retv = true;
     } else {
         retv = false;
     }
     return retv;
}

bool
loadCSVFileComboBoxes(QTabWidget* parentTabWidget,
                      QTableView* tableView,
                      const QString& inputDataPath,
                      const QString& inputFilename,
                      QString& errorMsg)
{
    QString allLines;
    QString filename;
    QStringList lineList;
    QStringList dataParts;
    QStringList SpeciesList = {};
    QStringList ParameterList  = {};
    QStandardItemModel* smodel = qobject_cast<QStandardItemModel*>(tableView->model());
    errorMsg.clear();
    QString value;
    QLocale locale(QLocale::English);
    QString valueWithComma;
    QComboBox* cbox;
    QModelIndex index;

    if (smodel == nullptr) {
        errorMsg = "Error: No model found in table. Please save initial table data.";
        return false;
    }

    filename = (inputFilename.isEmpty()) ?
                QFileDialog::getOpenFileName(parentTabWidget,
                   QObject::tr("Select CSV file"), inputDataPath,
                   QObject::tr("Data Files (*.csv)")) :
                inputFilename;

    if (! filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            allLines = file.readAll().trimmed();
            lineList = allLines.split('\n');
            int numSpecies    = lineList.count()-1;               // -1 for the top-left
            int numParameters = lineList[1].split(',').count()-1; // -1 to remove the top-left

            if ((smodel->rowCount()    == numSpecies) &&
                (smodel->columnCount() == numParameters))
            {
                QStringList parameters = lineList[0].split(',');
                for (int j=1;j<parameters.count();++j) {
                    ParameterList << parameters[j].replace(",",";");
                }
                for (int i=1; i<lineList.count(); ++i) {
                    dataParts = lineList[i].split(',');
                    SpeciesList << " " + dataParts[0] + " ";
                    for (int j=1; j<dataParts.count(); ++j) {
                        index = smodel->index(i-1,j-1);
                        cbox  = qobject_cast<QComboBox*>(tableView->indexWidget(index));
                        cbox->setCurrentText(dataParts[j]);
                    }
                }
                smodel->setVerticalHeaderLabels(SpeciesList);
                smodel->setHorizontalHeaderLabels(ParameterList);
                tableView->setModel(smodel);
                tableView->resizeColumnsToContents();
            } else {
                errorMsg = "Error: table size from .csv file (" +
                        QString::number(numSpecies) + "x" + QString::number(numParameters) +
                        ") does not equal current size of table (" +
                        QString::number(smodel->rowCount()) + "x" +
                        QString::number(smodel->columnCount()) + ")";
            }
            file.close();
        }
    }
    return true;
}

bool
loadCVSFileCovariate(QTabWidget* parentTabWidget,
                     QTableView* tableView,
                     const QString& inputDataPath,
                     const QString& inputFilename,
                     QString& errorMsg)
{
    QString allLines;
    QString filename;
    QStringList lineList;
    QStringList dataParts;
    QStringList VerticalList = {};
    QStringList HorizontalList  = {};
    QStandardItem* item;
    QStandardItemModel* smodel = qobject_cast<QStandardItemModel*>(tableView->model());
    errorMsg.clear();
    QString value;
    QLocale locale(QLocale::English);
    QString valueWithComma;

    if (smodel == nullptr) {
        errorMsg = "Error: No model found in table. Please save initial table data.";
        return false;
    }

    filename = (inputFilename.isEmpty()) ?
                QFileDialog::getOpenFileName(parentTabWidget,
                   QObject::tr("Select CSV file"), inputDataPath,
                   QObject::tr("Data Files (*.csv)")) :
                inputFilename;

    if (! filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            allLines = file.readAll().trimmed();
            lineList = allLines.split('\n');
            int numRows   = lineList.count()-1; // -1 for the header
            int numCols = lineList[1].split(',').count()-1; // -1 to remove the year

            QStringList parts = lineList[0].split(',');
            for (int j=1;j<parts.count();++j) {
                HorizontalList << parts[j];
            }
            for (int i=1; i<=numRows; ++i) {
                dataParts = lineList[i].split(',');
                VerticalList << " " + dataParts[0] + " ";
                for (int j=1; j<numCols; ++j) {
                    value = dataParts[j];
                    item  = new QStandardItem();
                    item->setTextAlignment(Qt::AlignCenter);
                    if (! value.isEmpty()) {
                        item->setText(value);
                    } else {
                        item->setText("");
                        setItemEditable(nmfConstantsMSSPM::NotEditable,
                                        nmfConstantsMSSPM::GrayedIfNotEditable,
                                        item);
                    }
                    smodel->setItem(i-1, j-1,item);
                }
            }
            smodel->setVerticalHeaderLabels(VerticalList);
            smodel->setHorizontalHeaderLabels(HorizontalList);
            tableView->setModel(smodel);
            tableView->resizeColumnsToContents();

            file.close();
        }
    }
    return true;
}

bool
loadTimeSeries(QTabWidget* parentTabWidget,
               QTableView* tableView,
               const QString& inputDataPath,
               const QString& inputFilename,
               const bool& firstLineReadOnly,
               const bool& scientificNotation,
               const bool& allowBlanks,
               std::pair<int,int>& nonZeroCell,
               QString& errorMsg)
{
    bool retv = true;
    QString allLines;
    QString filename;
    QStringList lineList;
    QStringList dataParts;
    QStringList VerticalList = {};
    QStringList SpeciesList  = {};
    QStandardItem* item;
    QStandardItemModel* smodel = qobject_cast<QStandardItemModel*>(tableView->model());
    errorMsg.clear();
    double value;
    QLocale locale(QLocale::English);
    QString valueWithComma;

    if (smodel == nullptr) {
        errorMsg = "Error: No model found in table. Please save initial table data.";
        return false;
    }

    filename = (inputFilename.isEmpty()) ?
                QFileDialog::getOpenFileName(parentTabWidget,
                   QObject::tr("Select CSV file"), inputDataPath,
                   QObject::tr("Data Files (*.csv)")) :
                inputFilename;

    if (! filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            allLines = file.readAll().trimmed();
            lineList = allLines.split('\n');
            int numYears   = lineList.count()-1; // -1 for the header
            int numSpecies = lineList[1].split(',').count()-1; // -1 to remove the year
            int numCols = smodel->columnCount();
            int numRows = smodel->rowCount();

            // If the table is empty, create the columns and rows required
            if (numCols == 0) {
                smodel->insertColumns(0,numSpecies);
                numCols = smodel->columnCount();
            }

            if ((numRows == numYears) &&
                (numCols == numSpecies))
            {
                QStringList speciesParts = lineList[0].split(',');
                for (int j=1;j<speciesParts.count();++j) {
                    SpeciesList << speciesParts[j];
                }
                for (int i=1; i<lineList.count(); ++i) {
                    dataParts = lineList[i].split(',');
                    VerticalList << " " + dataParts[0] + " ";
                    for (int j=1; j<dataParts.count(); ++j) {
                        if (allowBlanks &&
                                (dataParts[j].trimmed().isEmpty() ||
                                 dataParts[j].trimmed().toLower() == QString::fromStdString(nmfConstants::NoData))) {
                            item = new QStandardItem("");
                        } else {
                            value = dataParts[j].toDouble();
                            if (value != 0) {
                                nonZeroCell = std::make_pair(i-1,j-1);
                            }
                            if (scientificNotation) {
                                valueWithComma = locale.toString(value,'g');
                                item = new QStandardItem(valueWithComma);
                            } else {
                                valueWithComma = locale.toString(value,'f',8);
                                item = new QStandardItem(valueWithComma);
                            }
                            item->setTextAlignment(Qt::AlignCenter);
                            if (firstLineReadOnly && (i == 1)) {
                                setItemEditable(nmfConstantsMSSPM::NotEditable,
                                                nmfConstantsMSSPM::GrayedIfNotEditable,
                                                item);
                            }
                        }
                        smodel->setItem(i-1, j-1, item);
                    }
                }
                smodel->setVerticalHeaderLabels(VerticalList);
                smodel->setHorizontalHeaderLabels(SpeciesList);
                tableView->setModel(smodel);
                tableView->resizeColumnsToContents();
            } else {
                errorMsg = "Error: table size from .csv file (" +
                        QString::number(numYears) + "x" + QString::number(numSpecies) +
                        ") does not equal current size of table (" +
                        QString::number(smodel->rowCount()) + "x" +
                        QString::number(smodel->columnCount()) + ")";
                retv = false;
            }
            file.close();
        }
    } else {
        return false;
    }
    return retv;
}

void
saveTableWidgetData(QTabWidget* parentTabWidget,
                    QTableWidget* tableWidget,
                    QString& inputDataPath,
                    const QString& outputFilename)
{
    QString filename;
    QComboBox* cbox;
    bool isSpeciesTable = tableWidget->objectName().contains("Species");

    filename = (outputFilename.isEmpty()) ?
        QFileDialog::getSaveFileName(parentTabWidget,
            QObject::tr("Output CSV file"), inputDataPath,
            QObject::tr("Data Files (*.csv)")) :
        outputFilename;

    if (! filename.isEmpty()) {
        // Assure that file has a .csv extension
        QFileInfo fileInfo(filename);
        if (fileInfo.suffix().toLower() != "csv") {
            filename += ".csv";
        }
        QFile file(filename);
        QString value;
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            int numRows = tableWidget->rowCount();
            int numCols = tableWidget->columnCount();
            stream << tableWidget->horizontalHeaderItem(0)->text();
            for (int col=1; col<numCols; ++col) {
                stream << "," << tableWidget->horizontalHeaderItem(col)->text();
            }

            stream << "\n";
            for (int row=0; row<numRows; ++row) {
                value = tableWidget->item(row,0)->text();
                stream << value;
                for (int col=1; col<numCols; ++col) {
                    // Account for combobox in 2nd column of Species table
                    if (isSpeciesTable && (col == 1)) {
                        cbox  = qobject_cast<QComboBox *>(tableWidget->cellWidget(row,col));
                        value = cbox->currentText();
                    } else {
                        value = tableWidget->item(row,col)->text();
                    }
                    stream << "," << value;
                }
                stream << "\n";
            }
            file.close();
        }
    }
}


bool
saveSpeciesTableView(
        QTabWidget* parentTabWidget,
        QStandardItemModel* smodel,
        std::map<QString,int>& columnNumberMap,
        QString& inputDataPath,
        QString& outputFilename,
        QList<QString>& SpeciesName,
        QList<QString>& SpeciesGuild,
        QList<QString>& SpeciesInitialBiomass,
        QList<QString>& SpeciesGrowthRate,
        QList<QString>& SpeciesK)
{
    bool retv = true;
    bool fromTableWidget = (SpeciesName.size() != 0);
    QString filename;
    QString valueWithoutComma;

    filename = (outputFilename.isEmpty()) ?
        QFileDialog::getSaveFileName(parentTabWidget,
            QObject::tr("Output CSV file"), inputDataPath,
            QObject::tr("Data Files (*.csv)")) :
        outputFilename;

    if (! filename.isEmpty()) {
        // Assure that file has a .csv extension
        QFileInfo fileInfo(filename);
        if (fileInfo.suffix().toLower() != "csv") {
            filename += ".csv";
        }
        QFile file(filename);
        QString value;
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            int numRows = smodel->rowCount();
            int numCols = smodel->columnCount();
            for (int col=0; col<numCols; ++col) {
                if (col == columnNumberMap["SpeName"]) {
                    stream << smodel->headerData(columnNumberMap["SpeName"],Qt::Horizontal).toString() << ",Guild";
                } else {
                    stream << smodel->headerData(col,Qt::Horizontal).toString();
                }
                if (col < numCols-1) {
                    stream << ",";
                }
            }
            stream << "\n";
            for (int row=0; row<numRows; ++row) {
                for (int col=0; col<numCols; ++col) {
                    if (fromTableWidget) {
                        if (col == columnNumberMap["SpeName"]) {
                            value = SpeciesName[row];
                            stream << value << ",";
                            value = SpeciesGuild[row];
                        } else if (col == columnNumberMap["InitBiomass"]) {
                            value = SpeciesInitialBiomass[row];
                        } else if (col == columnNumberMap["GrowthRate"]) {
                            value = SpeciesGrowthRate[row];
                        } else if (col == columnNumberMap["SpeciesKMin"]) {
                            value = SpeciesK[row];
                        } else {
                            valueWithoutComma = smodel->index(row,col).data().toString().remove(",");
                            value = valueWithoutComma;
                        }
                    } else {
                        valueWithoutComma = smodel->index(row,col).data().toString().remove(",");
                        value = valueWithoutComma;
                        if (col == columnNumberMap["SpeName"]) {
                            stream << value << ",";
                            value = SpeciesGuild[row];
                        }
                    }
                    stream << value;
                    if (col < numCols-1) {
                        stream << ",";
                    }
                }
                stream << "\n";
            }
            file.close();
        }
    } else {
        retv = false;
    }

    outputFilename = filename;
    return retv;
}

bool
saveGuildsTableView(QTabWidget* parentTabWidget,
                    QStandardItemModel* smodel,
                    QString& inputDataPath,
                    QString& outputFilename,
                    QList<QString>& GuildName,
                    QList<QString>& GrowthRate,
                    QList<QString>& GuildK)
{
    bool retv = true;
    bool fromTableWidget = (GuildName.size() != 0);
    QString filename;

    filename = (outputFilename.isEmpty()) ?
        QFileDialog::getSaveFileName(parentTabWidget,
            QObject::tr("Output CSV file"), inputDataPath,
            QObject::tr("Data Files (*.csv)")) :
        outputFilename;

    if (! filename.isEmpty()) {
        // Assure that file has a .csv extension
        QFileInfo fileInfo(filename);
        if (fileInfo.suffix().toLower() != "csv") {
            filename += ".csv";
        }
        QFile file(filename);
        QString value;
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            int numRows = smodel->rowCount();
            int numCols = smodel->columnCount();
            stream << smodel->headerData(0,Qt::Horizontal).toString();
            for (int col=1; col<numCols; ++col) {
                stream << "," << smodel->headerData(col,Qt::Horizontal).toString();
            }
            stream << "\n";
            for (int row=0; row<numRows; ++row) {
                for (int col=0; col<numCols; ++col) {
                    if (fromTableWidget) {
                        if (col == nmfConstantsMSSPM::Column_Supp_Guild_Name) {
                            value = GuildName[row];
                        } else if (col == nmfConstantsMSSPM::Column_Supp_Guild_GrowthRate) {
                            value = GrowthRate[row];
                        } else if (col == nmfConstantsMSSPM::Column_Supp_Guild_CarryingCapacity) {
                            value = GuildK[row];
                        } else {
                            value = smodel->index(row,col).data().toString().remove(",");
                        }
                    } else {
                        value = smodel->index(row,col).data().toString().remove(",");
                    }
                    stream << value;
                    if (col < numCols-1) {
                        stream << ",";
                    }
                }
                stream << "\n";
            }
            file.close();
        }
    } else {
        retv = false;
    }

    outputFilename = filename;
    return retv;
}

bool
saveTimeSeries(QTabWidget* parentTabWidget,
               QStandardItemModel* smodel,
               QString& inputDataPath,
               QString& outputFilename)
{
    bool retv = true;
    QString filename;

    filename = (outputFilename.isEmpty()) ?
        QFileDialog::getSaveFileName(parentTabWidget,
            QObject::tr("Output CSV file"), inputDataPath,
            QObject::tr("Data Files (*.csv)")) :
        outputFilename;

    if (! filename.isEmpty()) {
        // Assure that file has a .csv extension
        QFileInfo fileInfo(filename);
        if (fileInfo.suffix().toLower() != "csv") {
            filename += ".csv";
        }
        QFile file(filename);
        QString value;
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            int numRows = smodel->rowCount();
            int numCols = smodel->columnCount();
            for (int col=0; col<numCols; ++col) {
                stream << "," << smodel->headerData(col,Qt::Horizontal).toString().remove(",");
            }
            stream << "\n";
            for (int row=0; row<numRows; ++row) {
                stream << smodel->headerData(row,Qt::Vertical).toString().trimmed().remove(",");
                for (int col=0; col<numCols; ++col) {
                    value = smodel->index(row,col).data().toString().remove(",");
                    stream << "," << value;
                }
                stream << "\n";
            }
            file.close();
        }

    } else {
        retv = false;
    }

    outputFilename = filename;
    return retv;
}

bool
saveCSVFileComboBoxes(QTabWidget* parentTabWidget,
                      QTableView* tableView,
                      QStandardItemModel* smodel,
                      QString& inputDataPath,
                      QString& outputFilename)
{
    bool retv = true;
    QString filename;
    QComboBox* cbox;
    QModelIndex index;

    filename = (outputFilename.isEmpty()) ?
        QFileDialog::getSaveFileName(parentTabWidget,
            QObject::tr("Output CSV file"), inputDataPath,
            QObject::tr("Data Files (*.csv)")) :
        outputFilename;

    if (! filename.isEmpty()) {
        // Assure that file has a .csv extension
        QFileInfo fileInfo(filename);
        if (fileInfo.suffix().toLower() != "csv") {
            filename += ".csv";
        }
        QFile file(filename);
        QString value;
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            int numRows = smodel->rowCount();
            int numCols = smodel->columnCount();
            for (int col=0; col<numCols; ++col) {
                stream << "," << smodel->headerData(col,Qt::Horizontal).toString().remove(",");
            }
            stream << "\n";
            for (int row=0; row<numRows; ++row) {
                stream << smodel->headerData(row,Qt::Vertical).toString().trimmed().remove(",");
                for (int col=0; col<numCols; ++col) {
                    index = smodel->index(row,col);
                    cbox  = qobject_cast<QComboBox*>(tableView->indexWidget(index));
                    value = cbox->currentText();
                    //          index = cbox->model()->index(0,0);
                    //          value = cbox->model()->data(index).toString();
                    stream << "," << value;
                }
                stream << "\n";
            }
            file.close();
        }

    } else {
        retv = false;
    }

    outputFilename = filename;
    return retv;
}

void setItemEditable(const bool& editable,
                     const bool& grayedIfNotEditable,
                     QStandardItem* item)
{
    Qt::ItemFlags flags;

    item->setEditable(editable);
    if (!editable && grayedIfNotEditable) {
        item->setBackground(QBrush(QColor(240,240,240)));
    } else {
        item->setBackground(QBrush(Qt::white));
    }
    flags  = item->flags();
    flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEditable); // reset/clear the flag
    item->setFlags(flags);
}

void transposeModel(QTableView* tv)
{
    QStandardItemModel* smodel = qobject_cast<QStandardItemModel*>(tv->model());
    int numRows = smodel->rowCount();
    int numCols = smodel->columnCount();
    QString value;
    QStandardItem* item;
    QModelIndex index;
    std::vector< std::vector<QString> > tmpStringMatrix;
    std::vector<QString> tmpStringVector;

    // Can only take the transpose of a square matrix
    if (smodel->rowCount() != smodel->columnCount()) {
        return;
    }

    // Get model data
    for (int i=0; i<smodel->rowCount(); ++i) {
        tmpStringVector.clear();
        for (int j=0; j<smodel->columnCount(); ++j) {
            index = smodel->index(i,j);
            value = index.data().toString();
            tmpStringVector.push_back(value);
        }
        tmpStringMatrix.push_back(tmpStringVector);
    }

    // Put model data, in transpose order, back into model
    for (int i=0; i<numRows; ++i) {
        tmpStringVector = tmpStringMatrix[i];
        for (int j=0; j<numCols; ++j) {
            item  = new QStandardItem(tmpStringVector[j]);
            item->setTextAlignment(Qt::AlignCenter);
            smodel->setItem(j,i,item);
        }
    }
}

void checkForAndReplaceInvalidCharacters(QString &stringValue)
{
    stringValue.remove(QRegExp("[^a-zA-Z_\\d\\s]"));
    stringValue.replace(" ","_");
}

bool extractTag(const QString filename,
                QString& tag)
{
    QStringList parts = filename.split("_");
    int numParts = parts.size();
    if (numParts <= 1) {
        return false;
    }

    tag = parts[1];
    for (int i=2; i<numParts; ++i) {
        tag += "_"+parts[i];
    }
    return true;
}

void switchFileExtensions(QString& filename,
                          const QString& newExt,
                          const QStringList& oldExts)
{
    for (QString ext : oldExts) {
        filename.replace(ext,newExt);
    }
}

void clearTableView(const QList<QTableView*>& tables)
{
    QStandardItemModel *smodel;

    for (QTableView* table : tables) {
        smodel = qobject_cast<QStandardItemModel*>(table->model());
        if (smodel) {
            smodel->clear();
        }
    }
}

bool emptyField(QStringList fields)
{
    for (int i=0; i<fields.size(); ++i) {
        if (fields[i].isEmpty()) {
            return true;
        }
    }
    return false;
}

bool outOfRange(QStringList fields, QString& badParam)
{
    double value;
    for (int i=0; i<fields.size(); i+=4) {
        value = fields[i+1].toDouble();
        if ((value < fields[i+2].toDouble()) ||
            (value > fields[i+3].toDouble())) {
            badParam = fields[i];
            return true;
        }
    }
    return false;
}


bool isAnError(std::string errorMsg)
{
    return (! QString::fromStdString(errorMsg).trimmed().isEmpty() );
}

void setAxisX(QChart*      chart,
              QValueAxis*  axisX,
              QLineSeries* series)
{
    chart->removeAxis(chart->axes(Qt::Horizontal).back());
    chart->addAxis(axisX, Qt::AlignBottom);
    if (series != nullptr) {
        series->attachAxis(axisX);
    }
}

void setAxisY(QChart*      chart,
              QValueAxis*  axisY,
              QLineSeries* series)
{
    chart->removeAxis(chart->axes(Qt::Vertical).back());
    chart->addAxis(axisY, Qt::AlignLeft);
    if (series != nullptr) {
        series->attachAxis(axisY);
    }
}

void setAxisX(QChart*            chart,
              QBarCategoryAxis*  axisX,
              QStackedBarSeries* series)
{
    chart->removeAxis(chart->axes(Qt::Horizontal).back());
    chart->addAxis(axisX, Qt::AlignBottom);
    if (series != nullptr) {
        series->attachAxis(axisX);
    }
}

void setAxisX(QChart*            chart,
              QBarCategoryAxis*  axisX,
              QBarSeries*        series)
{
    chart->removeAxis(chart->axes(Qt::Horizontal).back());
    chart->addAxis(axisX, Qt::AlignBottom);
    if (series != nullptr) {
        series->attachAxis(axisX);
    }
}
void setAxisY(QChart*            chart,
              QValueAxis*        axisY,
              QStackedBarSeries* series)
{
    chart->removeAxis(chart->axes(Qt::Vertical).back());
    chart->addAxis(axisY, Qt::AlignLeft);
    if (series != nullptr) {
        series->attachAxis(axisY);
    }
}

void setAxisY(QChart*     chart,
              QValueAxis* axisY,
              QBarSeries* series)
{
    chart->removeAxis(chart->axes(Qt::Vertical).back());
    chart->addAxis(axisY, Qt::AlignLeft);
    if (series != nullptr) {
        series->attachAxis(axisY);
    }
}


bool thereAreSelections(QTableView* tableView1, QTableView* tableView2)
{
    QItemSelectionModel *selectionModel1;
    QItemSelectionModel *selectionModel2;
    QModelIndexList indexes1;
    QModelIndexList indexes2;

    if (tableView1 != nullptr) {
        selectionModel1 = tableView1->selectionModel();
        indexes1        = selectionModel1->selectedIndexes();
    }
    if (tableView2 != nullptr) {
        selectionModel2 = tableView2->selectionModel();
        indexes2        = selectionModel2->selectedIndexes();
    }

    return ((indexes1.size() > 0) || (indexes2.size() > 0));
}

void setMinMaxOnSelections(const double& pct,
               QTableView* tableView0,
               QTableView* tableView1,
               QTableView* tableView2)
{
    int row;
    int col;
    std::vector<int> maxOrMin = {-1,1};
    double value;
    double delta;
    QStandardItemModel* smodel = nullptr;
    QStandardItemModel* smodel0 = qobject_cast<QStandardItemModel*>(tableView0->model());
    QModelIndex index0;

    // Process the tables...RSK continue here....
    int i=0;
    for (QTableView* tableView : {tableView1,tableView2}) {
        if (tableView != nullptr) {
            QItemSelectionModel *selectionModel = tableView->selectionModel();
            QModelIndexList indexes = selectionModel->selectedIndexes();
            smodel0 = qobject_cast<QStandardItemModel*>(tableView0->model());
            smodel  = qobject_cast<QStandardItemModel*>(tableView->model());

            for (QModelIndex index : indexes) {
                row = index.row();
                col = index.column();
                index0 = smodel0->index(row,col);
                value = index0.data().toString().remove(",").toDouble();
                delta = std::fabs(value)*pct;
                smodel->setData(index,value+maxOrMin[i]*delta);
            }
            tableView->resizeColumnsToContents();
        }
        ++i;
    }

}

void setMinMax(const double& pct,
               QTableView* tableView,
               QTableView* tableView1,
               QTableView* tableView2)
{
    double value;
    double minValue;
    double maxValue;
    double delta;
    QModelIndex index;
    QModelIndex indexMin;
    QModelIndex indexMax;
    QStandardItemModel* smodel1 = nullptr;
    QStandardItemModel* smodel2 = nullptr;
    QStandardItemModel* smodel  = qobject_cast<QStandardItemModel*>(tableView->model());

    if (tableView1 != nullptr) {
        smodel1 = qobject_cast<QStandardItemModel*>(tableView1->model());
    }
    if (tableView2 != nullptr) {
        smodel2 = qobject_cast<QStandardItemModel*>(tableView2->model());
    }

    for (int row=0; row<smodel->rowCount(); ++row) {
        for (int col=0; col<smodel->columnCount(); ++col) {
            index = smodel->index(row,col);
            value = index.data().toString().remove(",").toDouble();
            delta = std::fabs(value)*pct;
            if (smodel1 != nullptr) {
                minValue = value - delta;
                indexMin = smodel1->index(row,col);
                smodel1->setData(indexMin,minValue);
            }
            if (smodel2 != nullptr) {
                maxValue = value + delta;
                indexMax = smodel2->index(row,col);
                smodel2->setData(indexMax,maxValue);
            }
        }
    }
    if (tableView1 != nullptr) {
        tableView1->resizeColumnsToContents();
    }
    if (tableView2 != nullptr) {
        tableView2->resizeColumnsToContents();
    }
}

void
modelCopy(QStandardItemModel* originalModel, QStandardItemModel* copyModel)
{
    QList<QStandardItem*> rowItems;

    copyModel->removeRows(0,copyModel->rowCount());
    for (int row=0; row<originalModel->rowCount(); row++) {
        rowItems.clear();
        for (int col=0; col<originalModel->columnCount(); col++) {
            rowItems.append(originalModel->item(row,col)->clone());
        }
        copyModel->appendRow(rowItems);
    }
}

bool
loadMultiRunData(const nmfStructsQt::ModelDataStruct& dataStruct,
                 std::vector<QString>& MultiRunLines,
                 int& TotalIndividualRuns)
{
    bool retv = false;

    TotalIndividualRuns = 0;

    std::string line;
    QString lineStr;
std::cout << "nmfUtilsQt::loadMultiRunData: Loading multi run file: " << dataStruct.MultiRunSetupFilename << std::endl;
    std::ifstream multiRunFile(dataStruct.MultiRunSetupFilename);
    if (multiRunFile.is_open()) {
        getline(multiRunFile,line); // First line is the header
        while (getline(multiRunFile,line)) {
            lineStr = QString::fromStdString(line);
            TotalIndividualRuns += lineStr.split(',')[0].toInt();
            MultiRunLines.push_back(lineStr);
        }
        multiRunFile.close();
        retv = true;
    }
    return retv;
}

void
reloadDataStruct(nmfStructsQt::ModelDataStruct& dataStruct,
                 const QString& MultiRunLine)
{
    QStringList parts = MultiRunLine.split(",");

    dataStruct.NLoptNumberOfRuns     = parts[0].toInt();

    dataStruct.ObjectiveCriterion    = parts[1].toStdString();
    dataStruct.EstimationAlgorithm   = parts[2].toStdString();
    dataStruct.MinimizerAlgorithm    = parts[3].toStdString();
    dataStruct.ScalingAlgorithm      = parts[4].toStdString();
    dataStruct.userFixedSeedVal      = parts[5].toInt();
    dataStruct.LogScale              = parts[6].toInt();

    dataStruct.BeesMaxGenerations    = parts[7].toInt();
    dataStruct.BeesNumTotal          = parts[8].toInt();
    dataStruct.BeesNumBestSites      = parts[9].toInt();
    dataStruct.BeesNumEliteSites     = parts[10].toInt();
    dataStruct.BeesNumElite          = parts[11].toInt();
    dataStruct.BeesNumOther          = parts[12].toInt();
    dataStruct.BeesNeighborhoodSize  = parts[13].toFloat();
    dataStruct.BeesNumRepetitions    = parts[14].toInt();

    dataStruct.NLoptUseStopVal       = parts[16].toInt();
    dataStruct.NLoptStopVal          = parts[17].toDouble();
    dataStruct.NLoptUseStopAfterTime = parts[19].toInt();
    dataStruct.NLoptStopAfterTime    = parts[20].toInt();
    dataStruct.NLoptUseStopAfterIter = parts[22].toInt();
    dataStruct.NLoptStopAfterIter    = parts[23].toInt();

    dataStruct.EstimateRunBoxes.clear();
    int startIndex = 24;
    for (int col=24; col<parts.size()-2; col+=3) { // -2 since still need to include the last parameter's name field
        bool enabled = (parts[col+1].toInt()==1);
        bool checked = (parts[col+2].toInt()==1);
        nmfStructsQt::EstimateRunBox runBox;
        runBox.parameter = nmfConstantsMSSPM::EstimateCheckboxNames[(col-startIndex)/3];
        runBox.state     = std::make_pair(enabled,checked);
        dataStruct.EstimateRunBoxes.push_back(runBox);
    }
}

QDateTime
getCurrentTime()
{
    return QDateTime::currentDateTime();
}

std::string
elapsedTime(QDateTime startTime)
{
    std::string elapsedTimeStr = "";
    char buffer[50];

    QDateTime endTime = QDateTime::currentDateTime();
    auto elapsedTimeMSec = startTime.msecsTo(endTime);

    if (elapsedTimeMSec >= 60000) {
        sprintf(buffer,"%6.1f",elapsedTimeMSec/60000.0);
        elapsedTimeStr = std::string(buffer) + " minute(s)";
    } else if (elapsedTimeMSec >= 1000) {
        sprintf(buffer,"%6.1f",elapsedTimeMSec/1000.0);
        elapsedTimeStr = std::string(buffer) + " second(s)";
    } else {
        elapsedTimeStr = std::to_string(elapsedTimeMSec) + " millisecond(s)";
    }
    return elapsedTimeStr;
}

std::string
elapsedTimeCondensed(QDateTime startTime)
{
    std::string elapsedTimeStr = "";
    auto endTime        = getCurrentTime();
    //auto elapsedTimeSec = startTime.secsTo(endTime); // this is about a second short
    auto elapsedTimeSec = endTime.toSecsSinceEpoch()-startTime.toSecsSinceEpoch();

    std::string day  = std::to_string(int(elapsedTimeSec) / (24*3600));
    std::string hour = std::to_string((int(elapsedTimeSec) % (24*3600)) / 3600);
    std::string min  = std::to_string((int(elapsedTimeSec) % 3600) / 60);
    std::string sec  = std::to_string(int(elapsedTimeSec) % 60);

    // add leading zero if needed
    std::string dd = std::string(2 - day.length(), '0') + day;
    std::string hh = std::string(2 - hour.length(),'0') + hour;
    std::string mm = std::string(2 - min.length(), '0') + min;
    std::string ss = std::string(2 - sec.length(), '0') + sec;

    if (day == "0") {
        return hh + ':' + mm + ":" + ss;
    } else {
        return dd + ":" + hh + ':' + mm + ":" + ss;
    }
}


void
updateProgressDlg(nmfLogger* logger,
                  QProgressDialog* dlg,
                  const std::string& msg,
                  int& pInc)
{
    logger->logMsg(nmfConstants::Normal,msg);
    dlg->setValue(++pInc);
    dlg->update();
    dlg->setLabelText(QString::fromStdString(msg));
    QCoreApplication::processEvents();
}

void
setBackgroundLineEdit(QLineEdit* lineEdit,
                      const QString& backgroundStyleSheet)
{
    lineEdit->setStyleSheet(backgroundStyleSheet);
}


QString
checkAndCalculateWithSignificantDigits(
        const double& val,
        const int& numSignificantDigits,
        const int& numDecimalPlaces)
{
    QString retv = "";
    QLocale locale(QLocale::English);
    bool useEE = (numDecimalPlaces < 0);

    int numDecimalPlacesAbs = std::abs(numDecimalPlaces);

    if (numSignificantDigits >= 0) {
        QString s1 = QString("%1").arg(val,0,'G',numSignificantDigits);
        double  v1 = s1.toDouble();
        int numDecimals = 0;
        int lengthNumber = 1 + (int)std::log10(std::fabs(v1));
        if (v1 == 0) {
            numDecimals = 1;
        } else {
            numDecimals = (numSignificantDigits > lengthNumber) ? numSignificantDigits-lengthNumber : 0;
        }
        if (useEE) {
            numDecimals = numSignificantDigits;
        }
        retv = (useEE) ? locale.toString(v1,'g',numDecimals) :
                         locale.toString(v1,'f',numDecimals);

    } else {
//        retv = locale.toString(val,'f',numDecimalPlacesAbs);
//std::cout << "==> val: " << val << ", numd: " << numDecimalPlacesAbs << std::endl;
        retv = (useEE) ? locale.toString(val,'g',numDecimalPlacesAbs) :
                         locale.toString(val,'f',numDecimalPlacesAbs);
//std::cout << "==> retv: " << retv.toStdString() << std::endl;
    }

    return retv;
}

void
getCovariates(
        nmfStructsQt::ModelDataStruct& dataStruct,
        const int& numYears,
        const std::string& parameterName,
        boost::numeric::ublas::matrix<double>& covariateMatrix)
{
    int numSpecies = dataStruct.SpeciesNames.size();
    std::string index;
    std::string covariateName;

    nmfUtils::initialize(covariateMatrix, numYears, numSpecies);

    for (int year=0; year<numYears; ++year) {
        for (int species=0; species<numSpecies; ++species) {
            index         = dataStruct.SpeciesNames[species]+","+parameterName;
            covariateName = dataStruct.CovariateAssignment[index];
            covariateName = QString::fromStdString(covariateName).trimmed().toStdString();
            if (covariateName.empty()) {
                covariateMatrix(year,species) = 0.0;
            } else {
                covariateMatrix(year,species) = dataStruct.CovariateMap[covariateName][year];
            }
        }
    }
}

bool getCSVFileName(QWidget* parent,
                    QString& tableName)
{
    bool retv = true;
    // Save time series data to a .csv file
    QString msg = "\nOK to use default file name for " + tableName + " .csv file and overwrite any previous file?";
    QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Default "+tableName.toLatin1()+" CSV File"),
                                                              QObject::tr(msg.toLatin1()),
                                                              QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,
                                                              QMessageBox::Yes);
    if (reply == QMessageBox::Cancel) {
        retv = false;
    } else if (reply == QMessageBox::No) {
        bool ok;
        QString tag = QInputDialog::getText(parent, QObject::tr(tableName.toLatin1()+" Files"),
                                            QObject::tr("Enter "+tableName.toLatin1()+" CSV filename version tag (omit any '_'): "),
                                            QLineEdit::Normal, "", &ok);
        if (ok && !tag.isEmpty()) {
            tableName += "_"+tag;
            retv = true;
        } else if (tag.isEmpty()) {
            QMessageBox::warning(parent, "Tag Error",
                                 "\nError: Please enter a valid (i.e., non-blank) tag.\n",
                                 QMessageBox::Ok);
            retv = false;
        }
    }

    return retv;
}


void
reselectTableViewCells(QTableView* tv,
                       QModelIndexList indexes)
{
    if (indexes.size() > 0) {
        QItemSelectionModel *selectionModel = tv->selectionModel();
        for (QModelIndex index : indexes) {
            selectionModel->select(index,QItemSelectionModel::Select);
        }
    }
}

bool
runAllTableChecks(nmfLogger*  logger,
                  QWidget*    parent,
                  QTableView* tableview,
                  QTableView* minTableview,
                  QTableView* maxTableview)
{
    QString badCell = "";

    // 1. Check alpha tables for blanks
    if (! areAllCellsNonBlank(tableview)    ||
        ! areAllCellsNonBlank(minTableview) ||
        ! areAllCellsNonBlank(maxTableview))
    {
        QString msg = "No blanks allowed in any of the tables.";
        logger->logMsg(nmfConstants::Error,msg.toStdString());
        QMessageBox::critical(parent, "Save Error", "\n"+msg, QMessageBox::Ok);
        parent->setCursor(Qt::ArrowCursor);
        return false;
    }

    // 2. Check max cells are greater than min cells
    if (! nmfUtilsQt::areAllMaxCellsGreaterThanMinCells(
                qobject_cast<QStandardItemModel*>(minTableview->model()),
                qobject_cast<QStandardItemModel*>(maxTableview->model()),
                badCell))
    {
        logger->logMsg(nmfConstants::Error,"[Table Save]: Cell " + badCell.toStdString() + " has a max cell less than a min cell.");
        QMessageBox::critical(parent, "Error",
                             "\nError: Cell " + badCell + " has a max cell less than a min cell. Please check tables.\n",
                             QMessageBox::Ok);
        parent->setCursor(Qt::ArrowCursor);
        return false;
    }

    // 3. Check initial cells are >= min cells and <= max cells
    if (! nmfUtilsQt::areAllInitialCellsWithinRange(
                qobject_cast<QStandardItemModel*>(tableview->model()),
                qobject_cast<QStandardItemModel*>(minTableview->model()),
                qobject_cast<QStandardItemModel*>(maxTableview->model()),
                badCell))
    {
        logger->logMsg(nmfConstants::Error,"[Table Save]: Cell " + badCell.toStdString() + " is outside of min max range.");
        QMessageBox::critical(parent, "Error",
                             "\nError: Cell " + badCell + " is outside of min max range. Please check tables.\n",
                             QMessageBox::Ok);
        parent->setCursor(Qt::ArrowCursor);
        return false;
    }

    return true;
}

bool
runAllTableChecks(nmfLogger*  logger,
                  QWidget*    parent,
                  QTableView* tableview,
                  QTableView* minTableview,
                  QTableView* maxTableview,
                  QTableView* tableviewCompare,
                  QTableView* minTableviewCompare,
                  QTableView* maxTableviewCompare)
{
    QString badCell = "";

    bool isCompareTable =
            ((tableviewCompare->model()->rowCount()    != 0) &&
             (minTableviewCompare->model()->rowCount() != 0) &&
             (maxTableviewCompare->model()->rowCount() != 0));

    // 4. Check if there are values in any of the same cells between the Competition (alpha) and Predation (rho) tables
    if (isCompareTable && ! areAllCellsHoldingUniqueData(tableview,
                                                         minTableview,
                                                         maxTableview,
                                                         tableviewCompare,
                                                         minTableviewCompare,
                                                         maxTableviewCompare,
                                                         badCell)) {
        std::string msgShort = "Warning: Found values in similar cells in the Competition (alpha) and Predation (rho) matrices.";
        std::string msgLong = msgShort;
        msgLong += "\n\nThere shouldn't be any similar cells (i.e., the same cell in both tables) that have the same ";
        msgLong += "initial values with a non-zero range.  If so, there may be ambiguous values estimated for alpha and rho. ";
        msgLong += "For example, if there are values in cell (1,1) in each table and estimation produces alpha(1,1) = 2 and ";
        msgLong += "rho(1,1) = 3, this would be equivalent to alpha(1,1) = 3 and rho(1,1) = 2. So, it would be unclear as ";
        msgLong += "to which is the correct answer.";
        msgLong += "\n\nFor a more detailed explanation, please see the User's Manual.";
        logger->logMsg(nmfConstants::Warning,msgShort);
        QMessageBox::warning(parent,"Warning",QString("\n")+QString::fromStdString(msgLong)+"\n",QMessageBox::Ok);
        parent->setCursor(Qt::ArrowCursor);
        return false;
    }

    // Run remaining table checks
    if (! runAllTableChecks(logger, parent, tableview, minTableview, maxTableview)) {
        return false;
    }
    if (! runAllTableChecks(logger, parent, tableviewCompare, minTableviewCompare, maxTableviewCompare)) {
        return false;
    }

    return true;
}


QModelIndexList
getSelectedTableViewCells(QTableView* tv)
{
    QItemSelectionModel *selectionModel = tv->selectionModel();
    return selectionModel->selectedIndexes();
}

//bool
//saveSigDigState(QAction* sigDigActn)
//{
//    // If user selected significant digits, temporarily turn them off so the highest
//    // precision of the values will be saved.  Then turn them on later.
//    bool isSigDigChecked = sigDigActn->isChecked();
//    sigDigActn->setChecked(false);
//    return isSigDigChecked;
//}

//void
//restoreSigDigState(QAction* sigDigActn,
//                   bool state)
//{
//    // If the significant digits were just turned off a few lines up, turn
//    // them back on.  This is so the highest precision of values will be saved which
//    // is necessary so as not to lose an numerical precision when toggling between
//    // significant digits on and off.
//    sigDigActn->setChecked(state);
//}


} // end namespace




