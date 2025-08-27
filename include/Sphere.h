
#ifndef SPHERE_H
#define SPHERE_H

#include "HittableEntity.h"
#include "Vec3.h"
#include "Material.h"

class Sphere : public HittableEntity
{
public:
    Sphere(const Vec3& origin, const double radius, const Material& material, const Dynamics& dynamics)
        : HittableEntity(origin, material, dynamics), radius{radius}
    {}

    Sphere(const Vec3& origin, const double radius, const Material& material)
        : HittableEntity(origin, material), radius{radius}
    {}

    Sphere(const Sphere& sphere)
        : HittableEntity(sphere), radius{sphere.radius}
    {}

    Hit getRayHit(const Ray& ray, const Interval& interval) const override;

private:
    double radius{};
};

#endif //SPHERE_H
