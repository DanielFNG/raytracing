#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "HittableEntity.h"

class Scene {
public:
    Scene() = default;

    template <typename T>
    void add(T hittable_entity)
    {
        static_assert(std::is_base_of_v<HittableEntity, T>, "Can only add subclasses of HittableEntity to scene");
        entities.push_back(std::make_shared<T>(hittable_entity));
    }

    const std::vector<std::shared_ptr<HittableEntity>>& getHittableEntities() const {return entities;}

private:
    std::vector<std::shared_ptr<HittableEntity>> entities{};
};



#endif //SCENE_H
