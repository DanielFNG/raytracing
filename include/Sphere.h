
#ifndef SPHERE_H
#define SPHERE_H

#include "HittableEntity.h"
#include "Vec3.h"
#include "Material.h"

class Sphere : public HittableEntity
{
public:
    Sphere(const Vec3& origin, const double radius, const Material& material)
        : HittableEntity(material), origin{origin}, radius{radius}
    {}

    Sphere(const Sphere& sphere)
        : HittableEntity(sphere.getMaterial()), origin{sphere.origin}, radius{sphere.radius}
    {}

    Hit getRayHit(const Ray& ray, const Interval& interval) const override;

private:
    Vec3 origin{};
    double radius{};
};

#endif //SPHERE_H
