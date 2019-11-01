
/**
 @file nmfMortalityModel.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Dec 9, 2016
*/

#ifndef NMFMORTALITYMODEL_H
#define NMFMORTALITYMODEL_H

//#include "nmfmortalitymodel_global.h"

#include <QAbstractTableModel>

class /*NMFMORTALITYMODELSHARED_EXPORT*/ nmfMortality {

    int m_age;
    QString m_m1;
    QString m_m2;
public:

    nmfMortality(const int & age, const QString & m1, const QString & m2) :
            m_age { age }, m_m1 { m1 }, m_m2 { m2 } {
    }

    virtual ~nmfMortality() {}

    int age() const {
        return m_age;
    }
    QString m1() const {
        return m_m1;
    }
    QString m2() const {
        return m_m2;
    }
};

class nmfMortalityModel : public QAbstractTableModel
{
    Q_OBJECT

    QList<nmfMortality> model_data;

public:
    nmfMortalityModel(); //QObject *parent);
    virtual ~nmfMortalityModel() {};
    int count();
    int rowCount(const QModelIndex &) const override { return model_data.count(); }
    int columnCount(const QModelIndex &) const override { return 3; }
    bool removeRows(int row, int nrows, const QModelIndex &parent);
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void append(const nmfMortality & mortality);
    Qt::ItemFlags flags (const QModelIndex &index) const;
};


#endif // NMFMORTALITYMODEL_H
