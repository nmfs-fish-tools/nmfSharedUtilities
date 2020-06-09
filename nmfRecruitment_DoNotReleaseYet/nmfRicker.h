/**
 * @file nmfRicker.h
 * @brief This class represents the Ricker recruitment model
 * @date Dec 8, 2016
 *
 * This class defines the Ricker recruitment model which can be used to predict the abundance
 * of fish in a fishery.  It gives the expected number individuals in a generation as a
 * function of the number of individuals in the previous generation.  The exponential function
 * is as follows: N(t+1) = N(t)exp(r(1-N(t)/k)) where r = growth rate, k = carrying capacity,
 * and t is time in years.
 *
 */
#pragma once

#include <iostream>
#include <math.h>
#include <map>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

/**
 * @brief Class that contains the data and methods necessary to calculate Species abundance using the Ricker model
 */
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
    std::vector<double>                   m_prevAbundance;

    double m_getRecruitment(
            const int &year,
            const double &SpawningBiomass);
    double m_getSpawningBiomass(
            const int &year,
            const boost::numeric::ublas::matrix<double> &Abundance);

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

    /**
     * @brief getAbundance : returns Abundance information from the Ricker model
     * @param Abundance : returned Abundance information
     * @param Recruitment : returned Recruitment information
     * @param SpawningBiomass : returned Spawning Biomass information
     */
    void getAbundance(
            boost::numeric::ublas::matrix<double> &Abundance,
            std::vector<double> &Recruitment,
            std::vector<double> &SpawningBiomass);

};

