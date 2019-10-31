#pragma once

#include "nmfUtils.h"

class Bee {

private:
    double Fitness;
    std::vector<double> Parameters;

public:
    Bee(double fitness, std::vector<double> &parameters);
    Bee();
   ~Bee(){}

    void setFitness(const double &fitness);
    double getFitness();
    void setParameters(const std::vector<double> &theParameters);
    std::vector<double> getParameters();

    // Define this operator for sorting purposes.
    bool operator< (const Bee &other) const {
        return (Fitness < other.Fitness);
    }

};

