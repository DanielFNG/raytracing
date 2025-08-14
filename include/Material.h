#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"

struct Material {
    bool reflective{};
    double fuzz{};
    Vec3 albedo{};
    double refractive_index{0};
};

#endif //MATERIAL_H
