#include "Sphere.h"
#include <cmath>

Hit Sphere::getRayHit(const Ray& ray, const Interval& interval) const
{
    const Vec3 origin_to_origin {position - ray.getOrigin()};
    const double a{ray.getDirection().lengthSquared()};
    const double h{ray.getDirection().dot(origin_to_origin)};
    const double c{origin_to_origin.lengthSquared() - radius * radius};
    const double discriminant{h*h - a*c};
    const double first_part{h/a};
    const double second_part{std::sqrt(discriminant)/a};
    if (discriminant < 0)
    {
        return Hit();
    }
    double root {first_part - second_part};
    if (!interval.contains(root))
    {
        root = first_part + second_part;
        if (!interval.contains(root))
        {
            return Hit();
        }
    }
    Vec3 point{ray.at(root)};
    return Hit{root, point, (point - position)/radius, getMaterial()};
}