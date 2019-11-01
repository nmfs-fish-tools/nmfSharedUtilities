

/**
 @file nmfDat.cpp
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This file contains a data class that holds data used by the application.
 @date Nov 21, 2016
 */

/**
 * @mainpage Overview
 *
 * @section intro Introduction
 *
 * This is the introduction.
 *
 * @section Background
 *
 * This is some background.
 *
 */

#include "nmfData.h"

nmfData::nmfData() {

}

void
nmfData::setAgeMap(std::map< std::string, std::vector< std::string> > ageMapArg)
{
    ageMapVar = ageMapArg;

}

std::map< std::string, std::vector< std::string> >
nmfData::ageMap()
{
    return ageMapVar;
}



void
nmfData::setTVDelegate(nmfTableItemDelegate* tvDelegateArg)
{
    tvDelegateVar = tvDelegateArg;

}

nmfTableItemDelegate*
nmfData::tvDelegate()
{
    return tvDelegateVar;
}

void
nmfData::setDatabasePtr(nmfDatabase * dbPtrArg)
{
    databasePtrVar = dbPtrArg;
}

nmfDatabase *
nmfData::databasePtr()
{
    return databasePtrVar;
}
