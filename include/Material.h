#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"

struct Material {
    bool reflective{};
    double fuzz{};
    Vec3 albedo{};
};

#endif //MATERIAL_H
