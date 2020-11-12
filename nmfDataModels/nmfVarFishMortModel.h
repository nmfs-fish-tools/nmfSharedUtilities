
/**
 @file nmfVarFishMorttModel.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Jun 6, 2017
*/

#ifndef NMFVARFISHMORTMODEL_H
#define NMFVARFISHMORTMODEL_H

#include "nmfvarfishmortmodel_global.h"

#include <QAbstractTableModel>

class NMFVARFISHMORTMODELSHARED_EXPORT nmfVarFishMort {

    QString m_PredName;
    float   m_Linf;
    float   m_GrowthK;
    float   m_TZero;
    float   m_LWAlpha;
    float   m_LWBeta;

public:

    nmfVarFishMort(const QString &PredName,
               const float   &Linf,
               const float   &GrowthK,
               const float   &TZero,
               const float   &LWAlpha,
               const float   &LWBeta) :
            m_PredName { PredName },
            m_Linf     { Linf     },
            m_GrowthK  { GrowthK  },
            m_TZero    { TZero    },
            m_LWAlpha  { LWAlpha  },
            m_LWBeta   { LWBeta   } {
    }

    QString PredName() const {
        return m_PredName;
    }

    float Linf() const {
        return m_Linf;
    }
    float GrowthK() const {
        return m_GrowthK;
    }
    float TZero() const {
        return m_TZero;
    }
    float LWAlpha() const {
        return m_LWAlpha;
    }
    float LWBeta() const {
        return m_LWBeta;
    }
};

class nmfVarFishMortModel : public QAbstractTableModel
{
    Q_OBJECT

    QList<nmfVarFishMort> model_data;

public:
    nmfVarFishMortModel();
    ~nmfVarFishMortModel() {};
    int count();
    int rowCount(const QModelIndex &) const override { return model_data.count(); }
    int columnCount(const QModelIndex &) const override { return 6; }
    bool removeRows(int row, int nrows, const QModelIndex &parent);
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void append(const nmfVarFishMort& vonBert);
    Qt::ItemFlags flags (const QModelIndex &index) const;
};


#endif // NMFVARFISHMORTMODEL_H
