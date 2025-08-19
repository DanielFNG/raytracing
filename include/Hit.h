#ifndef HIT_H
#define HIT_H

#include "Vec3.h"
#include "Material.h"

struct Hit
{
    double t{};
    Vec3 point{};
    Vec3 normal{};
    const Material* material{nullptr};

    Hit() = default;

    Hit(const double t, const Vec3& point, const Vec3& normal, const Material& material)
        : t{t}, point{point}, normal{normal}, material{&material}
    {}

    explicit operator bool() const {return material != nullptr;}
};

#endif //HIT_H
