
/**
 @file nmfVonBertModel.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Jun 6, 2017
*/

#ifndef NMFVONBERTMODEL_H
#define NMFVONBERTMODEL_H

//#include "nmfvonbertmodel_global.h"

#include <QAbstractTableModel>

class /*NMFVONBERTMODELSHARED_EXPORT*/ nmfVonBert {

    QString m_PredName;
    int     m_PredIndex;
    int     m_PredType;
    float   m_Linf;
    float   m_GrowthK;
    float   m_TZero;
    float   m_LWAlpha;
    float   m_LWBeta;

public:

    nmfVonBert(const QString &PredName,
               const int     &PredIndex,
               const int     &PredType,
               const float   &Linf,
               const float   &GrowthK,
               const float   &TZero,
               const float   &LWAlpha,
               const float   &LWBeta) :
            m_PredName  { PredName  },
            m_PredIndex { PredIndex },
            m_PredType  { PredType  },
            m_Linf      { Linf      },
            m_GrowthK   { GrowthK   },
            m_TZero     { TZero     },
            m_LWAlpha   { LWAlpha   },
            m_LWBeta    { LWBeta    } {
    }

    QString PredName() const {
        return m_PredName;
    }
    int PredIndex() const {
        return m_PredIndex;
    }
    int PredType() const {
        return m_PredType;
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

class nmfVonBertModel : public QAbstractTableModel
{
    Q_OBJECT

    QList<nmfVonBert> model_data;

public:
    nmfVonBertModel();
    ~nmfVonBertModel() {};
    int count();
    int rowCount(const QModelIndex &) const override { return model_data.count(); }
    int columnCount(const QModelIndex &) const override { return 8; }
    bool removeRows(int row, int nrows, const QModelIndex &parent);
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void append(const nmfVonBert& vonBert);
    Qt::ItemFlags flags (const QModelIndex &index) const;
};


#endif // NMFVONBERTMODEL_H
