#ifndef HITTABLEENTITY_H
#define HITTABLEENTITY_H

#include "Ray.h"
#include "Hit.h"
#include "Interval.h"
#include "Material.h"

bool inRange(double x, double a, double b);

class HittableEntity {
public:
    explicit HittableEntity(const Material& material)
        : material{material}
    {
    }

    virtual ~HittableEntity() = default;
    virtual Hit getRayHit(const Ray& ray, const Interval& interval) const = 0;

    Ray processHit(const Ray& ray, HittableEntity& entity);

    const Material& getMaterial() const {return material;}

private:
    Material material;
};

#endif //HITTABLEENTITY_H
