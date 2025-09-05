#include "Cuboid.h"
#include "Vec3.h"
#include <algorithm>
#include <cmath>

Vec3 Cuboid::getNormalAtPoint(const Vec3& point) const
{
    const Vec3 offset{(point - position)/dimensions * 2};
    const Vec3 abs_offset{offset.getAbsolute()};
    const double maximum_abs_offset{*std::max_element(abs_offset.values.begin(), abs_offset.values.end())};
    const Vec3 sign{offset/abs_offset};
    const Vec3 normal{
        std::floor(abs_offset[0]/maximum_abs_offset) * sign[0],
        std::floor(abs_offset[1]/maximum_abs_offset) * sign[1],
        std::floor(abs_offset[2]/maximum_abs_offset) * sign[2]
    };
    return normal;
}

Interval Cuboid::getRayIntersection(const Ray& ray) const
{
    const Vec3 ltb = (lower_bounds - ray.getOrigin())/ray.getDirection();
    const Vec3 rtb = (upper_bounds - ray.getOrigin())/ray.getDirection();
    const Interval x_interval{ltb[0] < rtb[0] ? Interval{ltb[0], rtb[0]} : Interval{rtb[0], ltb[0]}};
    const Interval y_interval{ltb[1] < rtb[1] ? Interval{ltb[1], rtb[1]} : Interval{rtb[1], ltb[1]}};
    const Interval z_interval{ltb[2] < rtb[2] ? Interval{ltb[2], rtb[2]} : Interval{rtb[2], ltb[2]}};
    return x_interval.intersect(y_interval).intersect(z_interval);
}

Hit Cuboid::getRayHit(const Ray& ray, const Interval& interval) const
{
    const Interval ray_interval{getRayIntersection(ray)};
    const Interval valid_ray_interval{ray_interval.intersect(interval)};
    if (valid_ray_interval.max < valid_ray_interval.min)
    {
        return Hit();
    }
    const double closest_edge{interval.min == valid_ray_interval.min ? valid_ray_interval.max : valid_ray_interval.min};
    const Vec3 point_on_edge{ray.at(closest_edge)};
    const Vec3 normal{getNormalAtPoint(point_on_edge)};
    return Hit{closest_edge, point_on_edge, normal.getNormalised(), getMaterial()};
}