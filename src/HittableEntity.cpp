#include "HittableEntity.h"

void HittableEntity::at(const double time)
{
    position = dynamics->at(time).position;
}

bool inRange(const double x, const double a, const double b)
{
    return x >= a &&  x < b;
}