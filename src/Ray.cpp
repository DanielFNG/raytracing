#include "Ray.h"
#include <cassert>
#include "Utilities.h"
#include <cmath>

Vec3 Ray::at(const double t) const
{
    return getOrigin() + direction * t;
}

double Ray::at(const Vec3& point_on_ray) const
{
    return (point_on_ray[0] - getOrigin()[0])/direction[0];
}

void Ray::reflect(const Vec3& at_point, const Vec3& at_normal, const double fuzz)
{
    Vec3 in_direction{Vec3::getReflected(direction, at_normal)};
    if (fuzz == 0)
    {
        update(at_point, in_direction);
        return;
    }
    while (true)
    {
        in_direction = in_direction.getNormalised() + fuzz * Vec3::getRandomUnit();
        if (in_direction.dot(at_normal) > 0)
        {
            update(at_point, in_direction);
            return;
        }
    }
}

void Ray::scatter(const Vec3& at_point, const Vec3& at_normal)
{
    Vec3 in_direction{at_normal + Vec3::getRandomUnit()};
    if (direction.isNearZero())
    {
        assert("Tried to scatter but direction was near zero");
    }
    update(at_point, in_direction);
}

void Ray::refract(const Vec3& at_point, const Vec3& at_normal, const double refractive_index, const double cosine_term, const bool entering)
{
    double refractive_ratio{};
    if (entering)
    {
        refractive_ratio = getRefractiveRatio(true, refractive_index);
        refraction_log.push_back(refractive_index);
    }
    else
    {
        refractive_ratio = getRefractiveRatio(false);
        refraction_log.pop_back();
    }
    const Vec3 perpendicular {refractive_ratio * (direction.getNormalised() + cosine_term * at_normal)};
    const Vec3 parallel {-std::sqrt(std::abs(1.0 - perpendicular.lengthSquared())) * at_normal};
    const Vec3 refracted_direction{perpendicular + parallel};
    update(at_point, refracted_direction);
}

void Ray::update(const Vec3& at_position, const Vec3& in_direction)
{
    direction = in_direction;
    points.push_back(at_position);
}

double Ray::getRefractiveRatio(const bool entering, const double refractive_index)
{
    if (entering)
    {
        return refraction_log.back()/refractive_index;
    }
    const size_t size {refraction_log.size()};
    return refraction_log[size - 1]/refraction_log[size - 2];

}