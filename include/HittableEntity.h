#ifndef HITTABLEENTITY_H
#define HITTABLEENTITY_H

#include "Ray.h"
#include "Hit.h"
#include "Dynamics.h"
#include "Interval.h"
#include "Material.h"
#include <memory>

bool inRange(double x, double a, double b);

class HittableEntity {
public:
    HittableEntity(const Vec3& origin, const Material& material, const Dynamics& dynamics)
        : material{material.make_unique()}, origin{origin}, dynamics{dynamics.make_unique()}
    {}

    HittableEntity(const Vec3& origin, const Material& material)
        : HittableEntity{origin, material, Static{origin}}
    {}

    explicit HittableEntity(const HittableEntity& entity)
        : HittableEntity(entity.origin, *entity.material, *entity.dynamics)
    {}

    virtual ~HittableEntity() = default;
    virtual Hit getRayHit(const Ray& ray, const Interval& interval) const = 0;
    virtual void at(double time);

    const Material& getMaterial() const {return *material;}

private:
    const std::unique_ptr<Material> material{nullptr};
    Vec3 origin{};
    const std::unique_ptr<Dynamics> dynamics{nullptr};

protected:
    Vec3 position{};
};

#endif //HITTABLEENTITY_H
