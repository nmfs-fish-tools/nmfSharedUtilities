
#include "Bee.h"

Bee::Bee(double fitness,
         std::vector<double> &theParameters)
{
    setFitness(fitness);
    setParameters(theParameters);
}

Bee::Bee()
{

}

void
Bee::setFitness(const double &theFitness)
{
    Fitness = theFitness;
}

double
Bee::getFitness()
{
    return Fitness;
}

void
Bee::setParameters(const std::vector<double> &theParameters)
{
    Parameters = theParameters;
}

std::vector<double>
Bee::getParameters()
{
    return Parameters;
}

