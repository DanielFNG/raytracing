#include "Ray.h"
#include <cassert>
#include "Utilities.h"
#include <cmath>

Vec3 Ray::at(const double t) const
{
    return getOrigin() + direction * t;
}

Vec3 Ray::attenuate(const Hit& hit)
{
    const Material* material{hit.getMaterial()};
    if (material->reflective)
    {
        reflect(hit.getPoint(), hit.getNormal(), material->fuzz);
    }
    else if (material->refractive_index > 0)
    {
        refract(hit.getPoint(), hit.getNormal(), material->refractive_index);
    }
    else
    {
        scatter(hit.getPoint(), hit.getNormal());
    }
    return material->refractive_index > 0 ? Vec3{1.0, 1.0, 1.0} : material->albedo;
}

void Ray::reflect(const Vec3& at_point, const Vec3& at_normal, const double fuzz)
{
    Vec3 in_direction{reflectVec3(direction, at_normal)};
    if (fuzz == 0)
    {
        update(at_point, in_direction);
        return;
    }
    while (true)
    {
        in_direction = normalised(in_direction) + fuzz * getRandomUnitVec3();
        if (dot(in_direction, at_normal) > 0)
        {
            update(at_point, in_direction);
            return;
        }
    }
}

void Ray::scatter(const Vec3& at_point, const Vec3& at_normal)
{
    Vec3 in_direction{at_normal + getRandomUnitVec3()};
    if (nearZero(direction))
    {
        assert("Tried to scatter but direction was near zero");
    }
    update(at_point, in_direction);
}

void Ray::refract(const Vec3& at_point, const Vec3& at_normal, const double refractive_index)
{
    const double cosine_term {std::min(dot(normalised(direction), at_normal), 1.0)};
    if (refracts(cosine_term, refractive_index))
    {
        const Vec3 perpendicular {refractive_index/refraction_log.back() * (normalised(direction) + cosine_term * at_normal)};
        const Vec3 parallel {-std::sqrt(std::abs(1.0 - perpendicular.length_squared())) * at_normal};
        const Vec3 refracted_direction{perpendicular + parallel};
        if (dot(direction, at_normal) < 0)
        {
            enter(at_point, refracted_direction, refractive_index);
        }
        else
        {
            leave(at_point, refracted_direction);
        }
    }
    else
    {
        scatter(at_point, at_normal);
    }

}

void Ray::update(const Vec3& at_position, const Vec3& in_direction)
{
    direction = in_direction;
    points.push_back(at_position);
}

void Ray::enter(const Vec3& at_position, const Vec3& in_direction, const double refractive_index)
{
    update(at_position, in_direction);
    refraction_log.push_back(refractive_index);
}

void Ray::leave(const Vec3& at_position, const Vec3& in_direction)
{
    update(at_position, in_direction);
    refraction_log.pop_back();
}

bool Ray::refracts(const double cosine_term, const double refractive_index)
{
    double r0 {(1 - refractive_index)/(1 + refractive_index)};
    r0 = r0*r0;
    return (r0 + (1 - r0)*std::pow((1 - cosine_term), 5) > Random::getRandom());
}