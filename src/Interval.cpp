#include "Interval.h"
#include <algorithm>

bool Interval::contains(const double x) const
{
    return min <= x && x <= max;
}

Interval Interval::intersect(const Interval& interval) const
{
    return Interval{std::max(min, interval.min), std::min(max, interval.max)};
}