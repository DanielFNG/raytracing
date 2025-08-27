#include "Scene.h"
#include "Utilities.h"

Hit Scene::getClosestHit(const Ray& ray, const Interval& interval) const
{
    double closest_so_far{interval.max};
    Hit closest_hit {};
    for (const std::unique_ptr<HittableEntity>& entity : entities)
    {
        Hit hit{entity->getRayHit(ray, Interval{interval.min, closest_so_far})};
        if (hit)
        {
            closest_so_far = hit.t;
            closest_hit = hit;
        }
    }
    return closest_hit;
}

void Scene::setTimeInterval(const Interval& new_interval)
{
    interval = new_interval;
}

void Scene::sample()
{
    const double time{Random::getRandom(interval)};
    for (const std::unique_ptr<HittableEntity>& entity: entities)
    {
        entity->at(time);
    }
}