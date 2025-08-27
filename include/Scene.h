#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "HittableEntity.h"
#include "Hit.h"
#include <memory>

class Scene {
public:
    explicit Scene(const double refractive_index)
        : refractive_index{refractive_index}
    {
    }

    template <typename T>
    void add(const T& hittable_entity)
    {
        static_assert(std::is_base_of_v<HittableEntity, T>, "Can only add subclasses of HittableEntity to scene");
        entities.push_back(std::make_unique<T>(hittable_entity));
    }

    template <typename T>
    void add(T&& hittable_entity)
    {

        static_assert(std::is_base_of_v<HittableEntity, T>, "Can only add subclasses of HittableEntity to scene");
        entities.push_back(std::make_unique<T>(std::forward<T>(hittable_entity)));
    }

    const std::vector<std::unique_ptr<HittableEntity>>& getHittableEntities() const {return entities;}
    double getRefractiveIndex() const {return refractive_index;}

    Hit getClosestHit(const Ray& ray, const Interval& interval) const;

    void setTimeInterval(const Interval& new_interval);
    void sample();


private:
    std::vector<std::unique_ptr<HittableEntity>> entities{};
    double refractive_index{};
    Interval interval{DefinedIntervals::zero};
};

#endif //SCENE_H
