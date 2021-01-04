
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
            } else if (value.contains(',')) {
                if (showError) {
                    msg = "Found an invalid numeric value of: " + value;
                    msg += ". No commas or special characters allowed.";
                    QMessageBox::warning(tabW, QT_TR_NOOP("Warning"),
                                         QT_TR_NOOP("\n"+msg+"\n"),
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
            } else if (value.contains(',')) {
                if (showError) {
                    msg = "Found an invalid numeric value of: " + value;
                    msg += ". No commas or special characters allowed.";
                    QMessageBox::warning(tabWidget, QT_TR_NOOP("Warning"),
                                         QT_TR_NOOP("\n"+msg+"\n"),
                                         QMessageBox::Ok);
                }
                return false;
            }
        } // end for j
    } // end for i

    return true;
}

bool allMaxCellsGreaterThanMinCells(
        QStandardItemModel* smodelMin,
        QStandardItemModel* smodelMax)
{
    int numRows = smodelMin->rowCount();
    int numCols = smodelMin->columnCount();
    QModelIndex indexMin;
    QModelIndex indexMax;
    double valueMin;
    double valueMax;

    for (int i=0; i<numRows; ++i) {
        for (int j=0; j<numCols; ++ j) {
            indexMin = smodelMin->index(i,j);
            indexMax = smodelMax->index(i,j);
            valueMin = indexMin.data().toDouble();
            valueMax = indexMax.data().toDouble();
            if (valueMin > valueMax) {
                return false;
            }
        }
    }

    return true;
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

        copiedText = tableView->model()->data(indexes[0]).toString();
        copiedText.append(QLatin1Char('\n'));

    } else if (indexes.size() > 1) { // Copy multiple cells

        // You need a pair of indexes to find the row changes
        previous = indexes.first();
        indexes.removeFirst();
        Q_FOREACH (current,indexes)
        {
            text = tableView->model()->data(previous).toString();
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
        copiedText.append(tableView->model()->data(current).toString());

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
    if (nrows == 0) // Check in case user double clicks in a cell and copies/paste like that (instead of clicking once on a cell)
        nrows = 1;
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


void
showAboutWidget(
        QWidget* parent,
        const QString& name,
        const QString& operatingSystem,
        const QString& version,
        const QString& specialAcknowledgement,
        const QString& appMsg)
{
    QString msg = "<strong><br>"+version+" (" + operatingSystem + ")</strong>";
    msg += "<strong><br>"+name+"</strong>";
    QString date = QDate::currentDate().toString();

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

void
checkForAndDeleteLogFiles(QString name,
                          std::string logDirName,
                          std::string logFilter)
{
    QString title = name + " Log Files";
    QMessageBox::StandardButton reply;
    size_t NumLogFiles = 0;
    boost::filesystem::path logDir(logDirName);
    boost::filesystem::recursive_directory_iterator endIter;
    std::vector<std::string> filenames;

    for (boost::filesystem::recursive_directory_iterator it(logDir); it != endIter; ++it) {
        ++NumLogFiles;
        filenames.push_back(it->path().string());
    }

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
            for (std::string filename : filenames) {
                delOK = (std::remove(filename.c_str()) == 0);
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
        fileToRemove = QDir("~/.config/NOAA").filePath(appName+".conf");
std::cout << "Removing: " << fileToRemove.toStdString() << std::endl;
        std::remove(fileToRemove.toLatin1());
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


void saveModelToCSVFile(std::string projectDir,
                        std::string tabName,
                        QTableView* table)
{
    bool ok;
    QString val;
    QString msg;
    std::string vHeaderTitle;
    QMessageBox::StandardButton reply;
    QString dataPath = QDir(QString::fromStdString(projectDir)).filePath("outputData");
    QString filename = QInputDialog::getText(table, QObject::tr("CSV Filename"),
                                         QObject::tr("Enter desired CSV filename:"), QLineEdit::Normal,
                                         QObject::tr(""), &ok);
    QStandardItemModel* smodel = qobject_cast<QStandardItemModel*>(table->model());
    int numColumns = smodel->columnCount();
    int numRows    = smodel->rowCount();

    if (ok && !filename.isEmpty())
    {
        QString fullPath = QDir(dataPath).filePath(filename);

        // if file exists, query user if they wan't to overwrite
        if (QFileInfo(fullPath).exists()) {
            msg = "\nFile exists: " + fullPath + "\n\nOK to overwrite?\n";
            reply = QMessageBox::warning(table, QObject::tr("File Exists"),
                 msg, QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::No) {
                return;
            }
        }

        // open output file
        std::ofstream outputFile;
        outputFile.open(fullPath.toLatin1());

        // get data from model and write to output file in csv fashion
        if (smodel->verticalHeaderItem(0) == nullptr) {
            vHeaderTitle.clear();
        } else {
            if (tabName == "Data") {
                vHeaderTitle = "Year";
            } else if (tabName == "Model Fit Summary") {
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
            outputFile << smodel->horizontalHeaderItem(j)->text().toStdString();
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
                val = smodel->item(i,j)->text();
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

        msg = "\nFile saved successfully:\n\n" + fullPath + "\n";
        QMessageBox::information(table, QObject::tr("Project"),
                                 msg,
                                 QMessageBox::Ok);
    }
}



void checkForAndReplaceInvalidCharacters(QString &stringValue)
{
    stringValue.remove(QRegExp("[^a-zA-Z_\\d\\s]"));
    stringValue.replace(" ","_");
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

} // end namespace







