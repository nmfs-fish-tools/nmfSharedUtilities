

//============================================================================
// Name        : nmfMortalityModel.cpp
// Author      : Ron Klasky
// Version     :
// Copyright   : NOAA - National Marine Fisheries Service
// Description :
//============================================================================

#include "nmfMortalityModel.h"

#define NMF_CONSTANTS
#include "nmfConstants.h"

#include <iostream>


nmfMortalityModel::nmfMortalityModel() //QObject *parent)
    //:QAbstractTableModel(parent)
{
    //std::cout << "nmfMortalityModel constructor" << std::endl;
}



int
nmfMortalityModel::count()
{
return model_data.count();
}


bool
nmfMortalityModel::removeRows(int row, int nrows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row+nrows-1);

    for (int theRow = 0; theRow < nrows; ++theRow) {
        model_data.removeAt(row);
    }

    endRemoveRows();
    return true;
}


QVariant
nmfMortalityModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant {};

    const nmfMortality & mortality = model_data[index.row()];

    switch (index.column()) {
        case 0:  return mortality.age();
        case 1:  return mortality.m1();
        case 2:  return mortality.m2();
        default: return QVariant {};
    };

}

QVariant
nmfMortalityModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return "Age";
        case 1:
            return "M1";
        case 2:
            return "M2";
        default:
            return QVariant { };
        }
    }
}

Qt::ItemFlags
nmfMortalityModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

}

void
nmfMortalityModel::append(const nmfMortality & mortality)
{
   beginInsertRows(QModelIndex{}, model_data.count(), model_data.count());
   model_data.append(mortality);
   endInsertRows();
}


bool nmfMortalityModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        int row = index.row();
        int col = index.column();

        int     currAge = model_data[row].age();
        QString currM1  = model_data[row].m1();
        QString currM2  = model_data[row].m2();

        // Assure the user enters in appropriate values for M1 and M2.
        float validatedValue = value.toFloat();
        if (validatedValue > nmfConstants::MortalityMax)
            validatedValue = nmfConstants::MortalityMax;
        else if (validatedValue < nmfConstants::MortalityMin)
            validatedValue = nmfConstants::MortalityMin;

        // Don't allow user to change Age
        if (col == 1) {        // change M1
            model_data[row] = nmfMortality { currAge, QString::number(validatedValue), currM2 };
        } else if (col == 2) { // change M2
            model_data[row] = nmfMortality { currAge, currM1, QString::number(validatedValue) };
        }

        emit dataChanged(index,index);

        return true;
    }

    return false;
}
