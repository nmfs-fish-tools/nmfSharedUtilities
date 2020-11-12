#include "nmfTestDialog.h"
#include <iostream>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QTimer>
#include <QApplication>

nmfTestDialog::nmfTestDialog()
    : QDialog()
{
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle("Test Dialog");

    QVBoxLayout* layt = new QVBoxLayout();
    m_label = new QLabel("0");
    layt->addWidget(m_label);
    setLayout(layt);

   exec();

}




namespace nmfTestUtils {

QDialog* ShowDialog(std::string filename)
{
std::cout << "ShowDialog:  filename: " << filename << std::endl;
    nmfTestDialog* dlg = new nmfTestDialog();

    return dlg;
}


}
