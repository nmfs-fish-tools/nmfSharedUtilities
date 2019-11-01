

//============================================================================
// Name        : nmfVonBertModel.cpp
// Author      : Ron Klasky
// Version     :
// Copyright   : NOAA - National Marine Fisheries Service
// Description :
//============================================================================

#include "nmfVonBertModel.h"

//#define NMF_CONSTANTS
//#include "nmfConstants.h"

#include <iostream>


nmfVonBertModel::nmfVonBertModel()
{
    //std::cout << "nmfVonBertModel constructor" << std::endl;
}



int
nmfVonBertModel::count()
{
    return model_data.count();
}


bool
nmfVonBertModel::removeRows(int row, int nrows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row+nrows-1);

    for (int theRow = 0; theRow < nrows; ++theRow) {
        model_data.removeAt(row);
    }

    endRemoveRows();
    return true;
}


QVariant
nmfVonBertModel::data(const QModelIndex &index, int role) const
{
    int flagVCenterLeft  = Qt::AlignVCenter |  Qt::AlignLeft;
    int flagVCenterRight = Qt::AlignVCenter |  Qt::AlignRight;

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == 0)
            return (flagVCenterLeft);
        else
            return (flagVCenterRight);
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant {};

    const nmfVonBert &vonBert = model_data[index.row()];

    switch (index.column()) {
        case 0:
            return vonBert.PredName();
        case 1:
            return vonBert.PredIndex();
        case 2:
            return vonBert.PredType();
        case 3:
            return vonBert.Linf();
        case 4:
            return vonBert.GrowthK();
        case 5:
            return vonBert.TZero();
        case 6:
            return vonBert.LWAlpha();
        case 7:
            return vonBert.LWBeta();
        default:
            return QVariant {};
    };

}

QVariant
nmfVonBertModel::headerData(int section, Qt::Orientation orientation, int role) const
{
     // Leave off row numbers for now. They don't really add anything.
     if (0) // (role == Qt::DisplayRole && orientation == Qt::Vertical)
        return section + 1;
     else {
        if (orientation != Qt::Horizontal)
            return QVariant { };
        if (role != Qt::DisplayRole)
            return QVariant { };

        switch (section) {
            case 0:
                return "PredName";
            case 1:
                return "PredIndex";
            case 2:
                return "PredType";
            case 3:
                return "Linf";
            case 4:
                return "GrowthK";
            case 5:
                return "TZero";
            case 6:
                return "LWAlpha";
            case 7:
                return "LWBeta";
            default:
                return QVariant { };
        }
    }
}

Qt::ItemFlags
nmfVonBertModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

}

void
nmfVonBertModel::append(const nmfVonBert & vonBert)
{
   beginInsertRows(QModelIndex{}, model_data.count(), model_data.count());
   model_data.append(vonBert);
   endInsertRows();
}


bool nmfVonBertModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        int row = index.row();
        int col = index.column();

        QString currPredName = model_data[row].PredName();
        int   currPredIndex  = model_data[row].PredIndex();
        int   currPredType   = model_data[row].PredType();
        float currLinf       = model_data[row].Linf();
        float currGrowthK    = model_data[row].GrowthK();
        float currTZero      = model_data[row].TZero();
        float currLWAlpha    = model_data[row].LWAlpha();
        float currLWBeta     = model_data[row].LWBeta();

        float validatedValue    = value.toFloat();
        //float validatedValueInt = value.toInt();
        if (col == 1)         // change PredIndex ...read only
            ; // model_data[row] = nmfVonBert {currPredName, validatedValueInt, currPredType, currLinf, currGrowthK, currTZero, currLWAlpha, currLWBeta };
        else if (col == 2)    // change PredType  ... read only
            ; // model_data[row] = nmfVonBert {currPredName, currPredIndex, validatedValueInt, currLinf, currGrowthK, currTZero, currLWAlpha, currLWBeta };
        else if (col == 3)    // change Linf
            model_data[row] = nmfVonBert {currPredName, currPredIndex, currPredType, validatedValue, currGrowthK, currTZero, currLWAlpha, currLWBeta };
        else if (col == 4)    // change GrowthK
            model_data[row] = nmfVonBert {currPredName, currPredIndex, currPredType, currLinf, validatedValue, currTZero, currLWAlpha, currLWBeta };
        else if (col == 5)    // change TZero
            model_data[row] = nmfVonBert {currPredName, currPredIndex, currPredType, currLinf, currGrowthK, validatedValue, currLWAlpha, currLWBeta };
        else if (col == 6)    // change LWAlpha
            model_data[row] = nmfVonBert {currPredName, currPredIndex, currPredType, currLinf, currGrowthK, currTZero, validatedValue, currLWBeta };
        else if (col == 7)    // change LWBeta
            model_data[row] = nmfVonBert {currPredName, currPredIndex, currPredType, currLinf, currGrowthK, currTZero, currLWAlpha, validatedValue };

        emit dataChanged(index,index);

        return true;
    }

    return false;
}
