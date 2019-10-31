
#include "nmfConstants.h"
#include "nmfRandom.h"

nmfRandom::nmfRandom(int seedValue, double lowerLimit, double upperLimit)
{
    setLimits(lowerLimit, upperLimit);
    setSeed(seedValue);
}

nmfRandom::nmfRandom()
{
    setLimits(0.0, 1.0);
    setSeed(std::chrono::system_clock::now().time_since_epoch().count());
}

nmfRandom::~nmfRandom()
{
}

void
nmfRandom::setLimits(double lowerLimit, double upperLimit)
{
    dist = std::uniform_real_distribution<double>(
                lowerLimit,upperLimit);
}

void
nmfRandom::setSeed(int seedValue)
{
    unsigned RandomSeed = std::chrono::system_clock::now().time_since_epoch().count();
    unsigned Seed = (seedValue < 0) ? RandomSeed : seedValue;
    rng.seed(Seed);
}

double
nmfRandom::value()
{
    return dist(rng);
}
