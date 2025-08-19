#ifndef HITTABLEENTITY_H
#define HITTABLEENTITY_H

#include "Ray.h"
#include "Hit.h"
#include "Interval.h"
#include "Material.h"
#include <memory>

bool inRange(double x, double a, double b);

class HittableEntity {
public:
    explicit HittableEntity(const Material& material)
        : material{material.make_unique()}
    {}

    virtual ~HittableEntity() = default;
    virtual Hit getRayHit(const Ray& ray, const Interval& interval) const = 0;

    Ray processHit(const Ray& ray, HittableEntity& entity);

    const Material& getMaterial() const {return material;}

private:
    const std::unique_ptr<Material> material{nullptr};
};

#endif //HITTABLEENTITY_H
