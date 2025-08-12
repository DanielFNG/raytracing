#ifndef HIT_H
#define HIT_H

#include "Ray.h"
#include "Vec3.h"
#include "Material.h"

class Hit {
public:
    Hit() = default;
    Hit(const double t, const Vec3& point, const Vec3& normal, const Material& material)
        : t{t}, point{point}, normal{normal}, material{material}
    {}

    const Vec3& getNormal() const {return normal;}
    double getT() const {return t;}
    const Vec3& getPoint() const {return point;}

    Vec3 attenuate() const;
    Ray bounce(const Ray& ray) const;

private:
    Vec3 scatter() const;
    Vec3 reflect(const Ray& ray) const;

    double t{};
    Vec3 point{};
    Vec3 normal{};
    const Material& material{};

};


#endif //HIT_H
