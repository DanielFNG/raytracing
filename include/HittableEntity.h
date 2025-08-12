#ifndef HITTABLEENTITY_H
#define HITTABLEENTITY_H

#include "Ray.h"
#include "Hit.h"
#include "Interval.h"
#include <vector>
#include <memory>
#include "Material.h"

bool inRange(double x, double a, double b);

class HittableEntity {
public:
    explicit HittableEntity(const Material& material)
        : material{material}
    {
    }

    virtual ~HittableEntity() = default;
    virtual std::shared_ptr<Hit> getRayHit(const Ray& ray, const Interval& interval) const = 0;

    Ray processHit(const Ray& ray, HittableEntity& entity);

    const Material& getMaterial() const {return material;}

private:
    Material material;
};

std::shared_ptr<Hit> getClosestHit(const Ray& ray, const std::vector<std::shared_ptr<HittableEntity>>& entities, const Interval& interval);

#endif //HITTABLEENTITY_H
