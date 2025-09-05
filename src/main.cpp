#include "Sphere.h"
#include "Cuboid.h"
#include "Camera.h"
#include "Vec3.h"
#include "Scene.h"
#include "Utilities.h"

int main()
{
    constexpr Vec3 camera_origin{13,2,3};
    constexpr CameraConfig camera_config
    {
        .aspect_ratio = 16.0/9.0,
        .image_width = 1920,
        .focus_distance = 10.0,
        .field_of_view = 34,
        .defocus_angle = 0.0,
        .framerate = 2,
        .anti_aliasing_samples = 50,
        .max_depth = 10,
    };
    Camera camera{camera_config, camera_origin};
    camera.lookAt(Vec3{0,0,0});

    const Lambertian ground{Vec3{0.5, 0.5, 0.5}};

    Scene scene{1.0};
    scene.add(Sphere(Vec3{0,-1000,0}, 1000, ground));

    /*
    for (int a{-11}; a < 11; ++a)
    {
        for (int b{-11}; b < 11; b++)
        {
            const double choose_mat{Random::getRandom()};
            const Vec3 centre{a + 0.9*Random::getRandom(), 0.2, b + 0.9*Random::getRandom()};
            const Newtonian dynamics{centre, {0.0, Random::getRandom(Interval{0.0, 0.5}), 0.0}, {0.0,0.0,0.0}};

            if ((centre - Vec3{4,0.2,0}).length() > 0.9)
            {
                if (choose_mat < 0.5)
                {
                    const Lambertian material{Vec3::getRandom()*Vec3::getRandom()};
                    //scene.add(Cuboid{centre, {0.4,0.4,0.4}, material, dynamics});
                }
                else if (choose_mat < 0.85)
                {
                    const Reflector material
                    {
                        Vec3::getRandom(Interval(0.5, 1)),
                        Random::getRandom(Interval(0, 0.4))
                    };
                    //scene.add(Cuboid{centre, {0.4,0.4,0.4}, material, dynamics});
                }
                else
                {
                    const Refractor material{Vec3::getRandom(), 1.5};
                    scene.add(Cuboid{centre, {0.4,0.4,0.4}, material, dynamics});
                }
            }
        }
    }
    */

    const Lambertian lambertian{Vec3{0.4, 0.2, 0.1}};
    const Reflector metal{Vec3{0.7, 0.6, 0.5}, 0.0};
    const Refractor glass{Vec3{1.0, 1.0, 1.0}, 1.5};

    scene.add(Sphere{Vec3{6.0,2.0,3.0}, 2.0, metal});
    scene.add(Sphere{Vec3{0.0,3.0,-5}, 3.0, metal});
    scene.add(Cuboid{Vec3{5,0.2,-1}, {0.4,0.4,0.4}, glass});
    scene.add(Cuboid{Vec3{5,0.2,-1}, {0.2,0.2,0.2}, lambertian});

    //camera.render(scene, "/Users/daniel/Documents/GitHub/raytracing/test1.ppm", 0.5);  // Render without motion blur
    camera.render(scene, "/Users/daniel/Documents/GitHub/raytracing/rt2.ppm", 0.0, true);  // Render with motion blur
    //camera.renderAnimation(scene, "/Users/daniel/Documents/GitHub/raytracing/no-motion-blur-2", Interval{0.0, 5.0}, false); // Render a series of frames without motion blur
    //camera.renderAnimation(scene, "/Users/daniel/Documents/GitHub/raytracing/motion-blur-2", Interval{0.0, 5.0}, true); // Render a series of frames with motion blur
}