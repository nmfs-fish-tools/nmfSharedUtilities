
#pragma once

#include <iostream>
#include <math.h>
#include <map>

#include <QSettings>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "nmfDatabase.h"
#include "nmfLogger.h"
//#include "nmfMSCAATableIO.h"
#include "nmfUtils.h"
#include "nmfUtilsQt.h"

struct AbundanceSystemStruct {
    double      TotalBiomass;
    int         Trophic;
    int         FH_FirstYear;
    int         FH_LastYear;
    int         NumSpInter;
    std::string SystemName;
    std::string AbundanceDriver;
};

struct NuOther {
    bool useUserNuOther;
    double nuOther;
};

class nmfAbundance {

private:
    nmfDatabase* m_databasePtr;
    nmfLogger*   m_logger;
    std::string  m_ProjectSettingsConfig;
    double       m_nuOther;
    boost::numeric::ublas::matrix<double> m_PreferredRatioEta;
    boost::numeric::ublas::matrix<double> m_PreferredGTRatio;
    boost::numeric::ublas::matrix<double> m_PreferredLTRatio;
    boost::numeric::ublas::matrix<double> m_VulnerabilityRho;
    std::map<std::string,boost::numeric::ublas::matrix<double> > m_WeightMap;
    std::map<std::string,boost::numeric::ublas::matrix<double> > m_InitialAbundance;
//    std::map<std::string,boost::numeric::ublas::matrix<double> > m_Abundance;
    std::map<std::string,boost::numeric::ublas::matrix<double> > m_ConsumptionBiomassRatio;
    std::map<std::string,boost::numeric::ublas::matrix<double> > m_Maturity;
    std::map<std::string,boost::numeric::ublas::matrix<double> > m_NaturalMortality;
    std::map<std::string,boost::numeric::ublas::matrix<double> > m_FishingMortality;
    std::map<std::string,boost::numeric::ublas::matrix<double> > m_PredationMortality;
    std::map<std::string,boost::numeric::ublas::matrix<double> > m_FisheryCatch;
    AbundanceSystemStruct m_SystemStruct;

      void ReadSettings();
      void getYearAgeData(const std::string& species,
                          const int& numYear,
                          const int& numAges,
                          const std::string tableName,
                          std::map<std::string,boost::numeric::ublas::matrix<double> >& tableMap);
      double getSpawningBiomass(std::string species,
                                int year, int numAges,
                                std::map<std::string,boost::numeric::ublas::matrix<double> >& Abundance);
      void getMortalityData(const std::string& species,
                            const int& numYears,
                            const int& numAges,
                            const std::string tableName,
                            std::map<std::string,boost::numeric::ublas::matrix<double> >& tableMap);
      bool getTheMortalityData(
              const std::string &Species,
              const int &NumYears,
              const int &NumAges,
              const std::string &TableName,
              boost::numeric::ublas::matrix<double> &TableData);

      double getRecruitment(const std::string species,
                            const QString RecruitmentType,
                            const int year,
                            std::map<std::string,double> alpha,
                            std::map<std::string,double> beta,
                            std::map<std::string,double> gamma,
                            std::map<std::string,std::vector<double> > sigma,
                            std::map<std::string,std::vector<double> > zeta,
                            const double SpawningBiomass);

      bool getSpeciesParameters(
              const std::string TableName,
              std::string RecruitmentType,
              std::map<std::string,double>& alpha,
              std::map<std::string,double>& beta,
              std::map<std::string,double>& gamma);

      bool getYearlyParameters(
              const std::string &Species,
              const int &NumYears,
              const std::string TableName,
              std::map<std::string,std::vector<double> > &SigmaMap,
              std::map<std::string,std::vector<double> > &ZetaMap);

      void getFisheryCatch(const std::string &Species,
                           const int &NumYears,
                           const int &NumAges,
                           const std::string TableName,
                           std::map<std::string,boost::numeric::ublas::matrix<double> >& m_FisheryCatch);

      bool getFleetCatchTotals(const std::string &Species,
                               const int &NumYears,
                               const int &NumAges,
                               const std::string &TableName,
                               std::map<std::string,boost::numeric::ublas::matrix<double> >& m_FisheryCatch);

      void calculatePredation(
              int year,
              std::vector<std::string>& AllSpecies,
              Boost3DArrayDouble& Biomass,
              Boost5DArrayDouble& BiomassAvailablePreyPhi,
              Boost3DArrayDouble& BiomassTotalPreyPhi,
              std::map<std::string,boost::numeric::ublas::matrix<double> >& PredationMortality,
              std::map<std::string,boost::numeric::ublas::matrix<double> >& AbundanceTable);

      void calculateBiomassForCurrentYear(
              int year,
              std::vector<std::string>& AllSpecies,
              std::map<std::string,boost::numeric::ublas::matrix<double> >& WeightMap,
              std::map<std::string,boost::numeric::ublas::matrix<double> >& Abundance,
              Boost3DArrayDouble& Biomass,
              double& sumBiomass);

      void calculateBiomassAvailablePreyPhi(
              int year,
              std::vector<std::string>& AllSpecies,
              Boost5DArrayDouble& ScaledSuitabilityNuTilde,
              Boost3DArrayDouble& Biomass,
              Boost5DArrayDouble& BiomassAvailablePreyPhi);

      void calculateBiomassTotalPreyPhi(
              int year,
              double nuOther,
              std::vector<std::string>& AllSpecies,
              std::vector<double>& Bother,
              Boost5DArrayDouble& BiomassAvailablePreyPhi,
              Boost3DArrayDouble& BiomassTotalPreyPhi);

      void calculateSuitabilityNuOther(
              const std::vector<std::string>& AllSpecies,
              const Boost5DArrayDouble& VulnerabilityNu,
              double& nuOther);

public:
      nmfAbundance(
            nmfDatabase* databasePtr,
            nmfLogger*   logger,
            std::map<std::string,boost::numeric::ublas::matrix<double> >& Abundance,
            std::map<std::string,std::vector<double> >& Recruitment,
            std::map<std::string,std::vector<double> >& SpawningStockBiomass,
            std::map<std::string,boost::numeric::ublas::matrix<double> >& NaturalMortality,
            std::map<std::string,boost::numeric::ublas::matrix<double> >& FishingMortality,
            std::map<std::string,boost::numeric::ublas::matrix<double> >& PredationMortality);
   ~nmfAbundance() {}

    double getNuOther();
    void setNuOther(double nuOther);

    void getData(NuOther& nuOtherUser,
                 QString RecruitmentType,
                 std::map<std::string,boost::numeric::ublas::matrix<double> >& Abundance,
                 std::map<std::string,std::vector<double> >& Recruitment,
                 std::map<std::string,std::vector<double> >& SpawningStockBiomass,
                 std::map<std::string,boost::numeric::ublas::matrix<double> >& PredationMortality);

    bool getSystemData(
            const std::string&  TableName,
                  AbundanceSystemStruct& SystemData);

    bool getDatabaseData(
            const std::string &Species,
            const int &NumYears,
            const int &NumAges,
            const std::string &TableName,
            boost::numeric::ublas::matrix<double> &TableData);

    bool getPredatorPreyData(
            std::string TableName,
            boost::numeric::ublas::matrix<double>& TableData);

    void calculateSizePreference(
            const std::vector<std::string>& AllSpecies,
            const boost::numeric::ublas::matrix<double>& PreferredRatioEta,
            const boost::numeric::ublas::matrix<double>& PreferredLTRatio,
            const boost::numeric::ublas::matrix<double>& PreferredGTRatio,
            Boost5DArrayDouble& SizePreferenceG);
    void calculateSuitability(
            const std::vector<std::string>& AllSpecies,
            const boost::numeric::ublas::matrix<double>& VulnerabilityRho,
            Boost5DArrayDouble& SizePreferenceG,
            Boost5DArrayDouble& Suitability);
    void calculateScaledSuitability(
            const NuOther& nuOtherUser,
            const std::vector<std::string>& allSpecies,
            Boost5DArrayDouble& vulnerabilityNu,
            Boost5DArrayDouble& scaledSuitabilityNuTilde,
            double& nuOther);

    void getData(QString RecruitmentType);
//    void getAbundance(
//            boost::numeric::ublas::matrix<double> &Abundance,
//            std::vector<double> &Recruitment,
//            std::vector<double> &SpawningBiomass);
//    double getRecruitment(
//            const int &year,
//            const double &SpawningBiomass);
//    double getSpawningBiomass(
//            const int &year,
//            const boost::numeric::ublas::matrix<double> &Abundance);

};

