#pragma once



#include <QComboBox>
#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>

class nmfTestDialog: public QDialog
{
    Q_OBJECT

    QLabel* m_label;

public:
    explicit nmfTestDialog();
    virtual ~nmfTestDialog() {}
//    void updateValue();

//public slots:
//    void callback_timer();

};

namespace nmfTestUtils {

    QDialog* ShowDialog(std::string filename);

}

