
//============================================================================
// Name        : nmfMaturityModel.cpp
// Author      : Ron Klasky
// Version     :
// Copyright   : NOAA - National Marine Fisheries Service
// Description :
//============================================================================

#include "nmfMaturityModel.h"
#include "nmfConstants.h"

#include <iostream>


nmfMaturityModel::nmfMaturityModel()
{
    //std::cout << "nmfMaturityModel constructor" << std::endl;
}

nmfMaturityModel::~nmfMaturityModel()
{

}

int
nmfMaturityModel::count()
{
return model_data.count();
}


bool
nmfMaturityModel::removeRows(int row, int nrows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row+nrows-1);

    for (int theRow = 0; theRow < nrows; ++theRow) {
        model_data.removeAt(row);
    }

    endRemoveRows();
    return true;
}


QVariant
nmfMaturityModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant {};

    const nmfMaturity & maturity = model_data[index.row()];

    switch (index.column()) {
        case 0:  return maturity.age();
        case 1:  return maturity.pmat();
        default: return QVariant {};
    };

}

QVariant
nmfMaturityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
     // Leave off row numbers for now.  They don't really add anything.
     if (0) // (role == Qt::DisplayRole && orientation == Qt::Vertical)
         return section + 1;
     else {
        if (orientation != Qt::Horizontal)
             return QVariant{};
        if (role != Qt::DisplayRole)
            return QVariant { };

        switch (section) {
        case 0:
            return "Age";
        case 1:
            return "PMature";
        default:
            return QVariant { };
        }
    }
}

Qt::ItemFlags
nmfMaturityModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

}

void
nmfMaturityModel::append(const nmfMaturity & maturity)
{
   beginInsertRows(QModelIndex{}, model_data.count(), model_data.count());
   model_data.append(maturity);
   endInsertRows();
}


bool
nmfMaturityModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        int row = index.row();
        int col = index.column();

        int currAge = model_data[row].age();

        // Assure the user enters in appropriate values for PMature.
        float validatedValue = value.toFloat();
        if (validatedValue > nmfConstants::MaturityMax)
            validatedValue = nmfConstants::MaturityMax;
        else if (validatedValue < nmfConstants::MaturityMin)
            validatedValue = nmfConstants::MaturityMin;

        // Don't allow user to change Age
        if (col == 1) {        // change Prf
            model_data[row] = nmfMaturity { currAge, validatedValue };
        }

        emit dataChanged(index,index);

        return true;
    }

    return false;


}
