#ifndef UTILITIES_H
#define UTILITIES_H

#include <random>
#include "Interval.h"

namespace Random
{
    static std::uniform_real_distribution distribution(0.0, 1.0);
    inline std::random_device device{};
    static std::mt19937 generator{device()};

    inline double getRandom(const Interval& interval = DefinedIntervals::canonical)
    {
        return interval.min + (interval.max - interval.min) * distribution(generator);
    }
}

#endif //UTILITIES_H
