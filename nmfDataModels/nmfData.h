
/**
 @file nmfData.h
 @author rklasky
 @copyright 2017 NOAA - National Marine Fisheries Service
 @brief This file contains the NMFS Data class definition.
 @date Nov 21, 2016
 */

#ifndef NMFDATA_H
#define NMFDATA_H

#include "nmfdata_global.h"

#include <vector>
#include <string>
#include <map>

#include "nmfModelDelegates.h"
#include "nmfDatabase.h"
#include "nmfMaturityModel.h"
#include "nmfMortalityModel.h"
#include "nmfInitialSelectivityModel.h"
#include "nmfEntityModel.h"
#include "nmfVonBertModel.h"


//class NMFDATASHARED_EXPORT nmfData {

class nmfData {

    std::map< std::string, std::vector< std::string> > ageMapVar;
    nmfTableItemDelegate* tvDelegateVar;
    nmfDatabase* databasePtrVar;
    nmfMortalityModel mortalityModelVar;

public:
    nmfData();
    ~nmfData() {};

    void setAgeMap(std::map< std::string, std::vector< std::string> > ageMapArg);
    std::map< std::string, std::vector< std::string> > ageMap();

    void setTVDelegate(nmfTableItemDelegate* tvDelegateArg);
    nmfTableItemDelegate* tvDelegate();

    void setDatabasePtr(nmfDatabase *db_ptr);
    nmfDatabase *databasePtr();

};




#endif // NMFDATA_H
