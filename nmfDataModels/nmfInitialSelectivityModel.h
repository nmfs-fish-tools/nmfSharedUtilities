
/**
 @file nmfMortalityModel.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Dec 9, 2016
*/

#ifndef NMFINITIALSELECTIVITY_H
#define NMFINITIALSELECTIVITY_H

//#include "nmfinitialselectivitymodel_global.h"
#include <QAbstractTableModel>


class /*NMFINITIALSELECTIVITYSHARED_EXPORT*/ nmfSelectivity {

    int   m_age;
    float m_prf;

public:

    nmfSelectivity(const int & age, const float & prf) :
            m_age { age }, m_prf { prf } {
    }

    int age() const {
        return m_age;
    }
    float prf() const {
        return m_prf;
    }

};

class nmfInitialSelectivityModel : public QAbstractTableModel
{
    Q_OBJECT

    QList<nmfSelectivity> model_data;

public:
    nmfInitialSelectivityModel();
    ~nmfInitialSelectivityModel() {};
    int count();
    int rowCount(const QModelIndex &) const override { return model_data.count(); }
    int columnCount(const QModelIndex &) const override { return 2; }
    bool removeRows(int row, int nrows, const QModelIndex &parent);
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void append(const nmfSelectivity & selectivity);
    Qt::ItemFlags flags (const QModelIndex &index) const;
};




#endif // NMFINITIALSELECTIVITY_H
