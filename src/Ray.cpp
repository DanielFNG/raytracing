#include "Ray.h"

Vec3 Ray::at(const double t) const
{
    return origin + direction * t;
}