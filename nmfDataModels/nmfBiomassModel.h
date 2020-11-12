
/**
 @file nmfMortalityModel.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief
 @date Dec 9, 2016
*/

#ifndef NMFBIOMASSMODEL_H
#define NMFBIOMASSMODEL_H

#include <iostream>
#include "nmfbiomassmodel_global.h"

#include <QAbstractTableModel>
#include <QStandardItemModel>


//class SModel : public QStandardItemModel
//{
//    Q_OBJECT

//public:
//    SModel(int NYears, int NSeasons)
//        : m_numRows(NYears),
//          m_numColumns(NSeasons)
//    {}

//    ~SModel() {
//std::cout << "Clearing data in SModel" << std::endl;
//        clear();
//    }

////    bool setData(const QModelIndex &index,
////                 const QVariant &value,
////                 int role = Qt::EditRole)
////    {
////        std::cout << "Setting data at: " << index.row() << ", " << index.column() << std::endl;
////    }

//private:
//    int m_numRows;
//    int m_numColumns;
//};






#endif // NMFBIOMASSMODEL_H
