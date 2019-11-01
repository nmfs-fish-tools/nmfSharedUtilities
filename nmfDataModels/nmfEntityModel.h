
/**
 @file nmfEntityModel.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Jan 12, 2017
*/

#ifndef NMFENTITYMODEL_H
#define NMFENTITYMODEL_H

//#include "nmfentitymodel_global.h"
#include <QAbstractTableModel>


class /*NMFENTITYMODELSHARED_EXPORT*/ nmfEntity {

    QString   m_name;

public:

    nmfEntity(const QString & name) :
            m_name { name } {
    }

    QString name() const {
        return m_name;
    }

};

class nmfEntityModel : public QAbstractTableModel
{
    Q_OBJECT

    QList<nmfEntity> model_data;

public:
    nmfEntityModel();
    ~nmfEntityModel() {};
    int count();
    int rowCount(const QModelIndex &) const override { return model_data.count(); }
    int columnCount(const QModelIndex &) const override { return 2; }
    bool removeRows(int row, int nrows, const QModelIndex &parent);
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void append(const nmfEntity &entity);
    Qt::ItemFlags flags (const QModelIndex &index) const;
};



#endif // NMFENTITYMODEL_H
