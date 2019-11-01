

#include "nmfRicker.h"
#include "nmfConstantsMSCAA.h"

nmfRicker::nmfRicker(
        const std::string                  &species,
        const int                          &minAge,
        const int                          &maxAge,
        const int                          &firstYear,
        const int                          &lastYear,
        const std::map<std::string,double> &alpha,
        const std::map<std::string,double> &beta,
        const std::map<std::string,double> &gamma,
        const std::vector<double>          &zeta,
        const std::vector<double>          &sigma,
        const boost::numeric::ublas::matrix<double> &initialAbundance,
        const std::string                           &abundanceDriver,
        const boost::numeric::ublas::matrix<double> &weight,
        const boost::numeric::ublas::matrix<double> &maturity,
        const boost::numeric::ublas::matrix<double> &naturalMortality,
        const boost::numeric::ublas::matrix<double> &fishingMortality,
        const boost::numeric::ublas::matrix<double> &fisheryCatch)
{
    m_species          = species;
    m_minAge           = minAge;
    m_maxAge           = maxAge;
    m_firstYear        = firstYear;
    m_lastYear         = lastYear;
    m_alpha            = alpha;
    m_beta             = beta;
    m_gamma            = gamma;
    m_zeta             = zeta;
    m_sigma            = sigma;
    m_initialAbundance = initialAbundance;
    m_abundanceDriver  = abundanceDriver;
    m_weight           = weight;
    m_maturity         = maturity;
    m_naturalMortality = naturalMortality;
    m_fishingMortality = fishingMortality;
    m_fisheryCatch     = fisheryCatch;
    m_prevAbundance.clear();
}


void
nmfRicker::getAbundance(boost::numeric::ublas::matrix<double> &Abundance,
                        std::vector<double> &Recruitment,
                        std::vector<double> &SpawningBiomass)
{
    double RecruitmentValue = 0;
    double SpawningBiomassValue = 0;
    double eFactorAge       = 0;
    double eFactorPenultAge = 0;
    double eFactorLastAge   = 0;
    double initialAbundance;
    int    NumYears = m_lastYear - m_firstYear + 1;
    int    yearIdx = 0;
    bool   useCatch = (m_abundanceDriver == "Fishery Catch");

    // Initialize Abundance matrix
    for (int age = 0; age < Abundance.size2(); ++age) {
        initialAbundance = m_initialAbundance(0,age);
        Abundance(0,age) = initialAbundance;
    }

    // Get first year's spawning biomass
    SpawningBiomassValue = getSpawningBiomass(yearIdx,Abundance);
    //SpawningBiomass.push_back(SpawningBiomassValue);

    for (int Year = 0; Year < NumYears-1; ++Year) {

        // First get abundance for first age group
        RecruitmentValue = getRecruitment(Year+1,SpawningBiomassValue);
        Abundance(Year+1,0) = RecruitmentValue;
        Recruitment.push_back(RecruitmentValue);
        SpawningBiomass.push_back(SpawningBiomassValue);

        // Get Abundance for all but first and last age group
        for (int Age=0; Age<m_maxAge-m_minAge-1; ++Age) {
            if (useCatch) {
                eFactorAge = std::exp(-m_naturalMortality(Year,Age)/2.0);
                Abundance(Year+1,Age+1) = (Abundance(Year,Age)*eFactorAge - m_fisheryCatch(Year,Age)) * eFactorAge;
            } else {
                eFactorAge = std::exp((-m_naturalMortality(Year,Age)-m_fishingMortality(Year,Age))/2.0);
                Abundance(Year+1,Age+1) = (Abundance(Year,Age)*eFactorAge) * eFactorAge;
            }
        }

        // Get Abundance for last age group
        int lastAge   = m_maxAge-m_minAge;
        int penultAge = lastAge-1;
        if (useCatch) {
            eFactorPenultAge = std::exp(-m_naturalMortality(Year,penultAge)/2.0);
            eFactorLastAge   = std::exp(-m_naturalMortality(Year,lastAge)/2.0);
            Abundance(Year+1,lastAge) =
                    (Abundance(Year,penultAge)*eFactorPenultAge - m_fisheryCatch(Year,penultAge))*eFactorPenultAge +
                    (Abundance(Year,lastAge)  *eFactorLastAge   - m_fisheryCatch(Year,lastAge))  *eFactorLastAge;
        } else {
            eFactorPenultAge = std::exp((-m_naturalMortality(Year,penultAge)-m_fishingMortality(Year,penultAge))/2.0);
            eFactorLastAge   = std::exp((-m_naturalMortality(Year,lastAge)  -m_fishingMortality(Year,lastAge))/2.0);
            Abundance(Year+1,lastAge) =
                    (Abundance(Year,penultAge)*eFactorPenultAge)*eFactorPenultAge +
                    (Abundance(Year,lastAge)  *eFactorLastAge)  *eFactorLastAge;
        }
        SpawningBiomassValue = getSpawningBiomass(Year+1,Abundance);
//      SpawningBiomass.push_back(SpawningBiomassValue);
    }

}


// R(y) = alpha * B(sp,y-1) exp[-beta(B(sp,y-1)^gamma]e^(zeta(y)-(sigma(R)^2/2)
double
nmfRicker::getRecruitment(const int &yearIdx,
                          const double &SpawningBiomass)
{
//    double sigma = m_sigma[yearIdx];

    return m_alpha[m_species] *
            SpawningBiomass *
            std::exp(-m_beta[m_species]*std::pow(SpawningBiomass,m_gamma[m_species]));
//            * std::exp(m_zeta[yearIdx]-(sigma*sigma)/2.0);

}

double
nmfRicker::getSpawningBiomass(const int &yearIdx,
                              const boost::numeric::ublas::matrix<double> &Abundance)
{
    double sum = 0;
    int Ts = 6; // T^s hard coded to 6 (from Butterworth paper)

    for (int age = 0; age < m_maxAge-m_minAge+1; ++age) {
        sum += m_maturity(yearIdx,age) *
               m_weight(yearIdx,age)   *
               Abundance(yearIdx,age)  *
               std::exp((-m_naturalMortality(yearIdx,age)-m_fishingMortality(yearIdx,age)) * (Ts/12.0));
//             std::exp(-m_naturalMortality(yearIdx,age) * (Ts/12.0));
    }
    return sum * nmfConstantsMSCAA::Kg2Mt;
}
