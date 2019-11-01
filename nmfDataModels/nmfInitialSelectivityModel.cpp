

//============================================================================
// Name        : nmfInitialSelectivityModel.cpp
// Author      : Ron Klasky
// Version     :
// Copyright   : NOAA - National Marine Fisheries Service
// Description :
//============================================================================

#include "nmfInitialSelectivityModel.h"
#include "nmfConstants.h"

#include <iostream>


nmfInitialSelectivityModel::nmfInitialSelectivityModel()
{
    //std::cout << "nmfInitialSelectivityModel constructor" << std::endl;
}


int
nmfInitialSelectivityModel::count()
{
return model_data.count();
}


bool
nmfInitialSelectivityModel::removeRows(int row, int nrows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row+nrows-1);

    for (int theRow = 0; theRow < nrows; ++theRow) {
        model_data.removeAt(row);
    }

    endRemoveRows();
    return true;
}


QVariant
nmfInitialSelectivityModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant {};

    const nmfSelectivity & selectivity = model_data[index.row()];

    switch (index.column()) {
        case 0:  return selectivity.age();
        case 1:  return selectivity.prf();
        default: return QVariant {};
    };

}

QVariant
nmfInitialSelectivityModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return "PRF";
        default:
            return QVariant { };
        }
    }
}

Qt::ItemFlags
nmfInitialSelectivityModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

}

void
nmfInitialSelectivityModel::append(const nmfSelectivity & selectivity)
{
   beginInsertRows(QModelIndex{}, model_data.count(), model_data.count());
   model_data.append(selectivity);
   endInsertRows();
}


bool
nmfInitialSelectivityModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        int row = index.row();
        int col = index.column();

        int   currAge = model_data[row].age();
        //float currPrf = model_data[row].prf();

        // Assure the user enters in appropriate values for M1 and M2.
        float validatedValue = value.toFloat();
        if (validatedValue > nmfConstants::SelectivityMax)
            validatedValue = nmfConstants::SelectivityMax;
        else if (validatedValue < nmfConstants::SelectivityMin)
            validatedValue = nmfConstants::SelectivityMin;

        // Don't allow user to change Age
        if (col == 1) {        // change Prf
            model_data[row] = nmfSelectivity { currAge, validatedValue };
        }

        emit dataChanged(index,index);

        return true;
    }

    return false;


}
