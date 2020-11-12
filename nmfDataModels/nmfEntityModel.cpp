

//============================================================================
// Name        : nmfEntityModel.cpp
// Author      : Ron Klasky
// Version     :
// Copyright   : NOAA - National Marine Fisheries Service
// Description :
//============================================================================

#include "nmfEntityModel.h"
#include "nmfConstants.h"

#include <iostream>


nmfEntityModel::nmfEntityModel()
{
    //std::cout << "nmfEntityModel constructor" << std::endl;
}


int
nmfEntityModel::count()
{
    return model_data.count();
}


bool
nmfEntityModel::removeRows(int row, int nrows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row+nrows-1);

    for (int theRow = 0; theRow < nrows; ++theRow) {
        model_data.removeAt(row);
    }

    endRemoveRows();
    return true;
}


QVariant
nmfEntityModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant {};

    const nmfEntity& entity = model_data[index.row()];

    switch (index.column()) {
        case 0:  return entity.name();
        default: return QVariant {};
    };

}

QVariant
nmfEntityModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return "Name";
        default:
            return QVariant { };
        }
    }
}

Qt::ItemFlags
nmfEntityModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index); // | Qt::ItemIsEditable; // Put back in if want to make editable.

}

void
nmfEntityModel::append(const nmfEntity& entity)
{
   beginInsertRows(QModelIndex{}, model_data.count(), model_data.count());
   model_data.append(entity);
   endInsertRows();
}


bool
nmfEntityModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}

