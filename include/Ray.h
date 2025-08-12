#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray {

public:
    Ray(const Vec3& origin, const Vec3& direction)
        : origin{origin}, direction{direction}
    {}

    Vec3 at(double t) const;

    const Vec3& getDirection() const {return direction;}
    const Vec3& getOrigin() const {return origin;}

private:
    Vec3 origin{};
    Vec3 direction{};

};



#endif //RAY_H
