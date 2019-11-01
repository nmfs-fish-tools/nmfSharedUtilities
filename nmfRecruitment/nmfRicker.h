
#pragma once

#include <iostream>
#include <math.h>
#include <map>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

class nmfRicker {

private:
    std::string                           m_species;
    int                                   m_firstYear;
    int                                   m_lastYear;
    int                                   m_minAge;
    int                                   m_maxAge;
    std::map<std::string,double>          m_alpha;
    std::map<std::string,double>          m_beta;
    std::map<std::string,double>          m_gamma;
    std::vector<double>                   m_zeta;
    std::vector<double>                   m_sigma;
    boost::numeric::ublas::matrix<double> m_initialAbundance;
    std::string                           m_abundanceDriver;
    boost::numeric::ublas::matrix<double> m_weight;
    boost::numeric::ublas::matrix<double> m_maturity;
    boost::numeric::ublas::matrix<double> m_naturalMortality;
    boost::numeric::ublas::matrix<double> m_fishingMortality;
    boost::numeric::ublas::matrix<double> m_fisheryCatch;
    boost::numeric::ublas::matrix<double> m_abundance;
    std::vector<double> m_prevAbundance;

public:
    nmfRicker(
            const std::string                           &species,
            const int                                   &minAge,
            const int                                   &maxAge,
            const int                                   &firstYear,
            const int                                   &lastYear,
            const std::map<std::string,double>          &alpha,
            const std::map<std::string,double>          &beta,
            const std::map<std::string,double>          &gamma,
            const std::vector<double>                   &zeta,
            const std::vector<double>                   &sigma,
            const boost::numeric::ublas::matrix<double> &initialAbundance,
            const std::string                           &abundanceDriver,
            const boost::numeric::ublas::matrix<double> &weight,
            const boost::numeric::ublas::matrix<double> &maturity,
            const boost::numeric::ublas::matrix<double> &naturalMortality,
            const boost::numeric::ublas::matrix<double> &fishingMortality,
            const boost::numeric::ublas::matrix<double> &fisheryCatch);
   ~nmfRicker() {}

    void getAbundance(
            boost::numeric::ublas::matrix<double> &Abundance,
            std::vector<double> &Recruitment,
            std::vector<double> &SpawningBiomass);
    double getRecruitment(
            const int &year,
            const double &SpawningBiomass);
    double getSpawningBiomass(
            const int &year,
            const boost::numeric::ublas::matrix<double> &Abundance);

};

