#include "HittableEntity.h"

bool inRange(const double x, const double a, const double b)
{
    return x >= a &&  x < b;
}

std::shared_ptr<Hit> getClosestHit(const Ray& ray, const std::vector<std::shared_ptr<HittableEntity>>& entities, const Interval& interval)
{
    double closest_so_far{interval.max};
    std::shared_ptr<Hit> closest_hit {nullptr};
    for (const std::shared_ptr<HittableEntity>& entity : entities)
    {
        std::shared_ptr hit_this_entity = entity->getRayHit(ray, Interval{interval.min, closest_so_far});
        if (hit_this_entity != nullptr)
        {
            closest_so_far = hit_this_entity->getT();
            closest_hit = hit_this_entity;
        }
    }
    return closest_hit;
}