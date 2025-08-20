#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"
#include "Ray.h"
#include "Scene.h"


struct CameraConfig
{
    double aspect_ratio{16.0/9.0};
    int image_width{800};
    double focus_distance{1.0};
    double field_of_view{90};
    double defocus_angle{0};
    int anti_aliasing_samples{50};
    int max_depth{10};
};

class Camera {
public:
    Camera(const CameraConfig& input_config, const Vec3& origin, const Vec3& direction = {0.0, 0.0, -1.0}, double twist = 0.0);

    void lookAt(const Vec3& position);
    void rotate(double angle);
    void updateConfig(const CameraConfig& new_config);

    Vec3 sampleOrigin() const;

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
    Vec3 direction{};
    double twist{};

    int image_height{};
    double viewport_height{};
    double viewport_width{};

    Vec3 viewport_dx{};
    Vec3 viewport_dy{};
    Vec3 viewport_dz{};
    Vec3 pixel_dx{};
    Vec3 pixel_dy{};
    Vec3 defocus_region_dx{};
    Vec3 defocus_region_dy{};

    Vec3 viewport_origin{};
    Vec3 pixel_origin{};
};



#endif //CAMERA_H
