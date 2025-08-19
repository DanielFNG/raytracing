#ifndef RAY_H
#define RAY_H

#include "Vec3.h"
#include <vector>

class Ray {
public:

    Ray(const Vec3& origin, const Vec3& direction, const double initial_refractive_index)
        : points{origin}, direction{direction}, refraction_log{initial_refractive_index}
    {
        // Update this to be the space for max_bounce.
        // Also refractive_index vec can be less than the number of objects in the scene.
        // Or would that be too crazy on memory?
    }

    Vec3 at(double t) const;

    const Vec3& getDirection() const {return direction;}
    const Vec3& getOrigin() const {return points.back();}
    double getRefractiveIndex() const {return refraction_log.back();}

    void reflect(const Vec3& at_point, const Vec3& at_normal, double fuzz);
    void scatter(const Vec3& at_point, const Vec3& at_normal);
    void refract(const Vec3& at_point, const Vec3& at_normal, double refractive_index);

    void update(const Vec3& at_position, const Vec3& in_direction);
    void enter(const Vec3& at_position, const Vec3& in_direction, double refractive_index);
    void leave(const Vec3& at_position, const Vec3& in_direction);

    static bool refracts(double cosine_term, double refraction_index);

    std::vector<Vec3> points{};
    Vec3 direction{};
    std::vector<double> refraction_log{};
};


#endif //RAY_H
