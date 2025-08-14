#ifndef HIT_H
#define HIT_H

#include "Vec3.h"
#include "Material.h"

class Hit {
public:
    Hit() = default;

    Hit(const double t, const Vec3& point, const Vec3& normal, const Material& material)
        : t{t}, point{point}, normal{normal}, material{&material}
    {}

    const Vec3& getNormal() const {return normal;}
    double getT() const {return t;}
    const Vec3& getPoint() const {return point;}
    const Material* getMaterial() const {return material;}

    explicit operator bool() const {return material != nullptr;}

private:
    double t{};
    Vec3 point{};
    Vec3 normal{};
    const Material* material{nullptr};
    // Choosing const Material* over & so that we can have failed hits with nullptr materials
};


#endif //HIT_H
