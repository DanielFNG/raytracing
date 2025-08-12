#include "Hit.h"
#include "Vec3.h"
#include <cassert>

Vec3 Hit::attenuate() const
{
    return material.albedo;
}

Ray Hit::bounce(const Ray& ray) const
{
    if (!material.reflective)
    {
        return Ray(point, scatter());
    }
    return Ray(point, reflect(ray));
}

Vec3 Hit::scatter() const
{
    Vec3 direction{normal + getRandomUnitVec3()};
    if (nearZero(direction))
    {
        assert("Tried to scatter but direction was near zero");
    }
    return direction;
}

Vec3 Hit::reflect(const Ray& ray) const
{
    const Vec3 reflected_direction {reflectVec3(ray.getDirection(), normal)};
    if (material.fuzz == 0)
    {
        return reflected_direction;
    }

    while (true)
    {
        const Vec3 fuzzed_reflection{
            normalised(reflected_direction) + (material.fuzz * getRandomUnitVec3())
        };
        if (dot(fuzzed_reflection, normal) > 0)
        {
            return fuzzed_reflection;
        }
    }

}