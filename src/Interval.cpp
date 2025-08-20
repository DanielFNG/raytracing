#include "Interval.h"

bool Interval::contains(const double x) const
{
    return min <= x && x <= max;
}
