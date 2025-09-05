#ifndef RAYTRACER_CUBOID_H
#define RAYTRACER_CUBOID_H

#include "HittableEntity.h"

class Cuboid : public HittableEntity
{
public:
    Cuboid(const Vec3& origin, const Vec3& dimensions, const Material& material, const Dynamics& dynamics)
        :
        HittableEntity(origin, material, dynamics),
        dimensions{dimensions},
        lower_bounds{origin - 0.5*dimensions},
        upper_bounds{origin + 0.5*dimensions}
    {
    }

    Cuboid(const Vec3& origin, const Vec3& dimensions, const Material& material)
        :
        HittableEntity(origin, material),
        dimensions{dimensions},
        lower_bounds{origin - 0.5*dimensions},
        upper_bounds{origin + 0.5*dimensions}
    {
    }

    // Is this needed or equivalent to a default constructor?
    Cuboid(const Cuboid& cuboid)
        : HittableEntity(cuboid),
          dimensions{cuboid.dimensions},
          lower_bounds{cuboid.lower_bounds},
          upper_bounds(cuboid.upper_bounds)
    {
    }

    Hit getRayHit(const Ray& ray, const Interval& interval) const override;

private:
    Vec3 getNormalAtPoint(const Vec3& point) const;
    Interval getRayIntersection(const Ray& ray) const;

    Vec3 dimensions{};
    Vec3 lower_bounds{};
    Vec3 upper_bounds{};
};

#endif //RAYTRACER_CUBOID_H