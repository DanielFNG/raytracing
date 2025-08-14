#include "Sphere.h"
#include "Camera.h"
#include "Vec3.h"
#include "Scene.h"

int main()
{
    constexpr Vec3 camera_origin{0,0,0};
    constexpr CameraConfig camera_config {};
    Camera camera{camera_config, camera_origin};
    //Material reflective{.reflective = true, .albedo = Vec3{0.8, 0.8, 0.8}};
    //Material nonreflective{.reflective = false, .albedo = Vec3{0.8, 0.6, 0.2}};
    Material lambertian1{.reflective = false, .albedo = Vec3{0.8, 0.8, 0.0}, .refractive_index = 0.0};
    //Material lambertian2{.reflective = false, .albedo = Vec3{0.1, 0.2, 0.5}};
    Material metal1{.reflective = true, .fuzz = 0.3, .albedo = Vec3{0.8, 0.8, 0.8}, .refractive_index = 0.0};
    Material metal3{.reflective = true, .fuzz = 0.0, .albedo = Vec3{0.1, 0.2, 0.5}, .refractive_index = 0.0};
    Material glass{.reflective = false, .fuzz = 0.0, .albedo = Vec3{1.0, 1.0, 1.0}, .refractive_index = 1.5};

    Scene world{1.0};
    world.add(Sphere{Vec3{0,-100.5,-1}, 100.0, lambertian1});
    world.add(Sphere{Vec3{0.0,0.0,-1.2}, 0.5, metal3});
    world.add(Sphere{Vec3{-1.0,0.0,-1.0}, 0.5, metal1});
    world.add(Sphere{Vec3{1.0,0.0,-1.0}, 0.5, glass});
    /*
    world.add(Sphere{Vec3{0,0,-1}, 0.5, reflective});
    world.add(Sphere{Vec3{-4,1,-3}, 0.3, nonreflective});
    world.add(Sphere{Vec3{4.0,0.5,-7}, 2.0, reflective});
    world.add(Sphere{Vec3{-30,20,-50}, 30, reflective});
    world.add(Sphere{Vec3{2.0,0,-1}, 0.5, nonreflective});
    world.add(Sphere{Vec3{0, -100.5, -1}, 100, nonreflective});
    */

    camera.render(world);
}