
/**
 @file nmfMaturityModel.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Jan 12, 2017
*/

#ifndef NMFMATURITYMODEL_H
#define NMFMATURITYMODEL_H

//#include "nmfmaturitymodel_global.h"

#include <QAbstractTableModel>


class /*NMFMATURITYMODELSHARED_EXPORT*/ nmfMaturity {

    int   m_age;
    float m_pmat;

public:

    nmfMaturity(const int & age, const float & pmat) :
            m_age { age }, m_pmat { pmat } {
    }

    ~nmfMaturity() {};

    int age() const {
        return m_age;
    }
    float pmat() const {
        return m_pmat;
    }

};

class nmfMaturityModel : public QAbstractTableModel
{
    Q_OBJECT

    QList<nmfMaturity> model_data;

public:
    nmfMaturityModel();
    ~nmfMaturityModel();
    int count();
    int rowCount(const QModelIndex &) const override { return model_data.count(); }
    int columnCount(const QModelIndex &) const override { return 2; }
    bool removeRows(int row, int nrows, const QModelIndex &parent);
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void append(const nmfMaturity & maturity);
    Qt::ItemFlags flags (const QModelIndex &index) const;
};




#endif // NMFMATURITYMODEL_H
