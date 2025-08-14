#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"
#include "Ray.h"
#include "Scene.h"


struct CameraConfig
{
    double aspect_ratio{16.0/9.0};
    int image_width{800};
    double viewport_height{2.0};
    double focal_length{1.0};
    int anti_aliasing_samples{50};
    int max_depth{10};
};

class Camera {
public:
    Camera(const CameraConfig& input_config, const Vec3& origin);

    void updateConfig(const CameraConfig& new_config);

    void render(const Scene& scene, bool parallel = true) const;

private:
    void updateParameters();
    void deriveCameraParameters();
    void deriveGeometricParameters();

    void renderSequential(const Scene& scene) const;
    void renderParallel(const Scene& scene) const;

    std::vector<Vec3> samplePixel(const Vec3& pixel_location) const;
    Vec3 colourPixel(const Vec3& pixel_location, const Scene& scene) const;
    Vec3 colourSubpixel(const Vec3& subpixel_location, const Scene& scene) const;
    Vec3 getRandomSubpixel(const Vec3& pixel_location) const;

    Vec3 ray_colour(Ray& ray, const Scene& scene) const;
    Vec3 background_colour(const Ray& ray) const;

    CameraConfig config{};
    Vec3 origin{};

    int image_height{};
    double viewport_width{};
    Vec3 pixel_dx{};
    Vec3 pixel_dy{};

    Vec3 viewport_origin{};
    Vec3 pixel_origin{};
};



#endif //CAMERA_H
