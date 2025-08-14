#if defined(USE_OPENMP)
#include "omp.h"
#endif

#include "Camera.h"
#include "PGM.h"
#include "Utilities.h"
#include "Interval.h"
#include <execution>

Camera::Camera(const CameraConfig& input_config, const Vec3& origin)
    : config{input_config}, origin{origin}
{
    deriveCameraParameters();
    deriveGeometricParameters();
}

void Camera::updateConfig(const CameraConfig& new_config)
{
    config = new_config;
    updateParameters();
}

void Camera::updateParameters()
{
    deriveCameraParameters();
    deriveGeometricParameters();
}

void Camera::deriveCameraParameters()
{
    image_height = std::max(static_cast<int>(config.image_width/config.aspect_ratio), 1);
    viewport_width = config.viewport_height * static_cast<double>(config.image_width)/image_height;
    pixel_dx = Vec3{viewport_width/config.image_width, 0, 0};
    pixel_dy = Vec3{0, -config.viewport_height/image_height, 0};
}

void Camera::deriveGeometricParameters()
{
    viewport_origin = Vec3{origin + Vec3{-viewport_width/2, config.viewport_height/2, -config.focal_length}};
    pixel_origin = Vec3{viewport_origin + 0.5 * (pixel_dx + pixel_dy)};
}

void Camera::render(const Scene& scene, const bool parallel) const
{
    // Allow user-requested sequential rendering. Otherwise, use parallel if possible.
    if (!parallel)
    {
        renderSequential(scene);
    }
    #if defined(USE_OPENMP)
        renderParallel(scene);
    #else
        renderSequential(scene);
    #endif
}

void Camera::renderParallel(const Scene& scene) const
{
    int n_rows{image_height};
    std::clog << "Rendering rows. Remaining: " << n_rows << "\n";
    std::vector<Vec3> pixel_colours(static_cast<size_t>(image_height*config.image_width));
    int j{};
    #pragma omp parallel for
    for (j = 0; j < image_height; ++j)
    {
        for (int i{0}; i < config.image_width; ++i)
        {
            Vec3 pixel_location{pixel_origin + (i * pixel_dx) + (j * pixel_dy)};
            pixel_colours[static_cast<size_t>(i + j*config.image_width)] = {colourPixel(pixel_location, scene)};
        }
        std::clog << "Rendering rows. Remaining: " << n_rows << "\n";
        n_rows -= 1;
    }
    PGM::writeHeader(config.image_width, image_height);
    for (const Vec3& pixel_colour : pixel_colours)
    {
        PGM::writeRGBTriple(pixel_colour);
    }

    // I wanted to try a native C++ implementation but this wouldn't run in parallel on my Mac,
    // neither with g++ or AppleClang. Might revisit this later.
    /*
    std::vector<Vec3> pixel_colours(image_height*config.image_width);
    std::transform(
        std::execution::par_unseq,
        pixel_locations.begin(),
        pixel_locations.end(),
        pixel_colours.begin(),
            [this, scene](const Vec3& pixel_location) -> Vec3
            {
                std::clog << "Rendering pixel.\n";
                return this->colourPixel(pixel_location, scene);
            });
    */
}

void Camera::renderSequential(const Scene& scene) const
{
    PGM::writeHeader(config.image_width, image_height);
    for (int j{0}; j < image_height; ++j)
    {
        std::clog << "Rendering. Rows remaining: " << image_height - j << " " << std::endl;
        for (int i{0}; i < config.image_width; ++i)
        {
            const Vec3 pixel_location {pixel_origin + (i * pixel_dx) + (j * pixel_dy)};
            const Vec3 pixel_colour {colourPixel(pixel_location, scene)};
            PGM::writeRGBTriple(pixel_colour);
        }
    }
    std::clog << "Rendering complete.\n";
}

std::vector<Vec3> Camera::samplePixel(const Vec3& pixel_location) const
{
    std::vector<Vec3> subpixel_locations{};
    subpixel_locations.push_back(pixel_location);
    for (int i{0}; i < config.anti_aliasing_samples; ++i)
    {
        const Vec3 random_subpixel {getRandomSubpixel(pixel_location)};
        subpixel_locations.push_back(random_subpixel);
    }
    return subpixel_locations;
}

Vec3 Camera::colourSubpixel(const Vec3& subpixel_location, const Scene& scene) const
{
    const Ray ray_to_pixel{origin, subpixel_location - origin};
    return ray_colour(ray_to_pixel, scene, config.max_depth);
}

Vec3 Camera::colourPixel(const Vec3& pixel_location, const Scene& scene) const
{
    Vec3 colour{};
    const std::vector<Vec3> subpixel_locations {samplePixel(pixel_location)};
    for (const Vec3& subpixel_location : subpixel_locations)
    {
        colour += colourSubpixel(subpixel_location, scene);
    }
    return colour / static_cast<double>(subpixel_locations.size());
}

Vec3 Camera::samplePixelColour(const Vec3& pixel_location, const Scene& scene) const
{
    // Initialise the colour at the centre point of the pixel (1 sample)
    const Ray ray_to_pixel{origin, pixel_location - origin};
    Vec3 pixel_colour{ray_colour(ray_to_pixel, scene, config.max_depth)};

    // Take config.anti_aliasing_samples random samples (1 + config.anti_aliasing_samples samples)
    int i{};
    //#pragma omp parallel for  // This needs significant anti_aliasing_samples to be much faster than Release
    for (i = 1; i < config.anti_aliasing_samples; ++i)
    {
        //std::println(std::clog, "hello from thread {}, total threads {}", omp_get_thread_num(), omp_get_num_threads());
        const Vec3 subpixel_location{getRandomSubpixel(pixel_location)};
        const Ray ray_to_subpixel{origin, subpixel_location - origin};
        pixel_colour += ray_colour(ray_to_subpixel, scene, config.max_depth);
        // If we have no samples, this should never execute, right?
    }

    return pixel_colour/(1 + config.anti_aliasing_samples);
}

Vec3 Camera::getRandomSubpixel(const Vec3& pixel_location) const
{
    Vec3 random_multiplier{Random::getRandom(DefinedIntervals::random_pixel), Random::getRandom(DefinedIntervals::random_pixel), 0};
    return pixel_location + random_multiplier*(pixel_dx + pixel_dy);
}

Vec3 Camera::ray_colour(const Ray& ray, const Scene& scene, const int current_depth) const
{
    if (current_depth == 0)
    {
        return Vec3{0,0,0};
    }

    std::shared_ptr hit{getClosestHit(ray, scene.getHittableEntities(), Interval(0.001, Constants::infinity))};
    if (hit != nullptr)
    {
        Vec3 attenuation {hit->attenuate()};  // For now this has no absorbance
        Ray next_ray {hit->bounce(ray)};
        return attenuation * ray_colour(next_ray, scene, current_depth-1);
        //Shade by normal: return 0.5 * (hit->getNormal() + Vec3{1,1,1});  // Make it positive and squeeze it within 0-1
    }

    return background_colour(ray);
}

Vec3 Camera::background_colour(const Ray& ray) const
{
    Vec3 direction{normalised(ray.getDirection())};
    const double a {0.5*(direction[1] + 1.0)};
    return Vec3{(1 - a)*Vec3{1,1,1} + a*Vec3{0.5,0.7,1.0} };
}