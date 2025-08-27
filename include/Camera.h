#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"
#include "Ray.h"
#include "Scene.h"
#include <fstream>


struct CameraConfig
{
    double aspect_ratio{16.0/9.0};
    int image_width{800};
    double focus_distance{1.0};
    double field_of_view{90};
    double defocus_angle{0};
    double framerate{10.0};
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

    void render(Scene& scene, const std::string& filepath, double time = 0.0, bool parallel = true) const;
    void render(Scene& scene, const std::string& filepath, const Interval& interval, bool parallel = true) const;
    void renderAnimation(Scene& scene, const std::string& directory, const Interval& interval, bool motion_blur = false, bool parallel = true, const std::string& filename = "frame") const;

private:
    void updateParameters();
    void deriveCameraParameters();
    void deriveGeometricParameters();

    void renderSequential(Scene& scene, std::ofstream& file) const;
    void renderParallel(Scene& scene, std::ofstream& file) const;

    std::vector<Vec3> samplePixel(const Vec3& pixel_location) const;
    Vec3 colourPixel(const Vec3& pixel_location, Scene& scene) const;
    Vec3 colourSubpixel(const Vec3& subpixel_location, Scene& scene) const;
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
