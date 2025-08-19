#include "Sphere.h"
#include "Camera.h"
#include "Vec3.h"
#include "Scene.h"

int main()
{
    constexpr Vec3 camera_origin{0,0,0};
    constexpr CameraConfig camera_config {};
    Camera camera{camera_config, camera_origin};

    const Lambertian ground{Vec3{0.8, 0.8, 0.0}};
    const Lambertian lambertian{Vec3{0.1, 0.2, 0.5}};
    const Reflector metal{Vec3{0.8, 0.6, 0.2}, 1.0};
    const Refractor glass{Vec3{1.0, 1.0, 1.0}, 1.5};
    const Refractor air{Vec3{1.0, 1.0, 1.0}, 1.0};

    Scene world{1.0};
    const Sphere sphere{Sphere{Vec3{0,-100.5,-1}, 100.0, ground}};
    world.add(sphere);
    world.add(Sphere{Vec3{0.0,0.0,-1.2}, 0.5, lambertian});
    world.add(Sphere{Vec3{-1.0,0.0,-1.0}, 0.5, glass});
    world.add(Sphere{Vec3{-1.0,0.0,-1.0}, 0.4, air});
    world.add(Sphere{Vec3{1.0,0.0,-1.0}, 0.5, metal});

    camera.render(world);
}