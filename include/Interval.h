#ifndef INTERVAL_H
#define INTERVAL_H

#include "Constants.h"

struct Interval
{
    double min{};
    double max{};

    bool contains(double x) const;
};

namespace DefinedIntervals
{
    static constexpr Interval empty{Constants::infinity, -Constants::infinity};
    static constexpr Interval universe{-Constants::infinity, Constants::infinity};
    static constexpr Interval visible_universe{0, Constants::infinity};
    static constexpr Interval random_pixel{-0.5, 0.5};
    static constexpr Interval canonical{0, 1};
    static constexpr Interval unit{-1, 1};
}

#endif //INTERVAL_H
