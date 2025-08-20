#include "Sphere.h"
#include "Camera.h"
#include "Vec3.h"
#include "Scene.h"

int main()
{
    constexpr Vec3 camera_origin{13,2,3};
    constexpr CameraConfig camera_config
    {
        .aspect_ratio = 16.0/9.0,
        .image_width = 1920,
        .focus_distance = 10.0,
        .field_of_view = 35,
        .defocus_angle = 0.2,
        .anti_aliasing_samples = 100,
        .max_depth = 50,
    };
    Camera camera{camera_config, camera_origin};
    camera.lookAt(Vec3{0,0,0});

    const Lambertian ground{Vec3{0.5, 0.5, 0.5}};

    Scene world{1.0};
    world.add(Sphere(Vec3{0,-1000,0}, 1000, ground));

    for (int a{-11}; a < 11; ++a)
    {
        for (int b{-11}; b < 11; b++)
        {
            const double choose_mat{Random::getRandom()};
            const Vec3 centre{a + 0.9*Random::getRandom(), 0.2, b + 0.9*Random::getRandom()};

            if ((centre - Vec3{4,0.2,0}).length() > 0.9)
            {
                if (choose_mat < 0.5)
                {
                    const Lambertian material{getRandomVec3()*getRandomVec3()};
                    world.add(Sphere{centre, 0.2, material});
                }
                else if (choose_mat < 0.85)
                {
                    const Reflector material
                    {
                        getRandomVec3(Interval(0.5, 1)),
                        Random::getRandom(Interval(0, 0.4))
                    };
                    world.add(Sphere{centre, 0.2, material});
                }
                else
                {
                    const Refractor material{getRandomVec3(), 1.5};
                    world.add(Sphere{centre, 0.2, material});
                }
            }
        }
    }

    const Lambertian lambertian{Vec3{0.4, 0.2, 0.1}};
    const Reflector metal{Vec3{0.7, 0.6, 0.5}, 0.0};
    const Refractor glass{Vec3{1.0, 1.0, 1.0}, 1.5};

    world.add(Sphere{Vec3{0.0,1.0,0.0}, 1.0, glass});
    world.add(Sphere{Vec3{-4,1,0}, 1.0, lambertian});
    world.add(Sphere{Vec3{4.0,1.0,0.0}, 1.0, metal});

    camera.render(world);
}