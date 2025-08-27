#if defined(USE_OPENMP)
#include "omp.h"
#endif

#include "Camera.h"
#include "PGM.h"
#include "Utilities.h"
#include "Interval.h"
#include <execution>
#include <fstream>
#include <filesystem>
#include <iostream>

Camera::Camera(const CameraConfig& input_config, const Vec3& origin, const Vec3& direction, const double twist)
    : config{input_config}, origin{origin}, direction{direction}, twist{twist}
{
    deriveCameraParameters();
    deriveGeometricParameters();
}

void Camera::lookAt(const Vec3& position)
{
    direction = position - origin;
    deriveGeometricParameters();
}

void Camera::rotate(const double angle)
{
    twist = angle;
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
    viewport_width = 2*std::tan(config.field_of_view*M_PI/180.0/2)*config.focus_distance;
    viewport_height = viewport_width * static_cast<double>(image_height)/config.image_width;
}

void Camera::deriveGeometricParameters()
{
    const double twist_rad{twist*M_PI/180.0};
    const Vec3 twist_vector{-std::sin(twist_rad),std::cos(twist_rad),0};
    viewport_dz = (-direction).getNormalised();
    viewport_dx = twist_vector.cross(viewport_dz).getNormalised();
    viewport_dy = viewport_dz.cross(viewport_dx);
    const Vec3 scaled_viewport_dx{viewport_dx*viewport_width};
    const Vec3 scaled_viewport_dy{-viewport_dy*viewport_height};
    pixel_dx = scaled_viewport_dx/config.image_width;
    pixel_dy = scaled_viewport_dy/image_height;
    viewport_origin = Vec3{origin - config.focus_distance*viewport_dz - scaled_viewport_dx/2 - scaled_viewport_dy/2};
    pixel_origin = Vec3{viewport_origin + 0.5 * (pixel_dx + pixel_dy)};
    const double defocus_radius{config.focus_distance*std::tan(config.defocus_angle/2*M_PI/180.0)};
    defocus_region_dx = viewport_dx*defocus_radius;
    defocus_region_dy = viewport_dy*defocus_radius;
}

void Camera::render(Scene& scene, const std::string& filepath, const Interval& interval, const bool parallel) const
{
    // Allow user-requested sequential rendering. Otherwise, use parallel if possible.
    scene.setTimeInterval(interval);
    std::ofstream out{filepath};
    if (!parallel)
    {
        renderSequential(scene, out);
    }
    #if defined(USE_OPENMP)
        renderParallel(scene, out);
    #else
        renderSequential(scene);
    #endif
    out.close();
}

void Camera::render(Scene& scene, const std::string& filepath, const double time, const bool parallel) const
{
    render(scene, filepath, Interval{time, time}, parallel);
}

void Camera::renderAnimation(Scene& scene, const std::string& directory, const Interval& interval, const bool motion_blur, const bool parallel, const std::string& filename) const
{
    const double frametime{1/config.framerate};
    const std::filesystem::path folder{directory};
    std::filesystem::create_directory(folder);
    const int n_frames{static_cast<int>(std::ceil((interval.max - interval.min)*config.framerate))};
    double time{interval.min};
    for (int i{0}; i < n_frames; ++i)
    {
        const std::filesystem::path path{folder/(filename + std::to_string(i) + ".ppm")};
        if (motion_blur)
        {
            render(scene, path, Interval{time, std::min(interval.max, time + frametime)});
        }
        else
        {
            render(scene, path, time, parallel);
        }
        time += frametime;
    }
}

void Camera::renderParallel(Scene& scene, std::ofstream& file) const
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
    PGM::writeHeader(file, config.image_width, image_height);
    for (const Vec3& pixel_colour : pixel_colours)
    {
        PGM::writeRGBTriple(file, pixel_colour);
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

void Camera::renderSequential(Scene& scene, std::ofstream& file) const
{
    PGM::writeHeader(file, config.image_width, image_height);
    for (int j{0}; j < image_height; ++j)
    {
        std::clog << "Rendering. Rows remaining: " << image_height - j << " " << std::endl;
        for (int i{0}; i < config.image_width; ++i)
        {
            const Vec3 pixel_location {pixel_origin + (i * pixel_dx) + (j * pixel_dy)};
            const Vec3 pixel_colour {colourPixel(pixel_location, scene)};
            PGM::writeRGBTriple(file, pixel_colour);
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

Vec3 Camera::sampleOrigin() const
{
    while (true)
    {
        const Vec3 point_in_unit_disc
        {
            Random::getRandom(DefinedIntervals::unit),
            Random::getRandom(DefinedIntervals::unit),
            0.0
        };
        if (point_in_unit_disc.lengthSquared() < 1)
        {
            return origin + point_in_unit_disc[0] * defocus_region_dx + point_in_unit_disc[1] * defocus_region_dy;
        }
    }
}

Vec3 Camera::colourSubpixel(const Vec3& subpixel_location, Scene& scene) const
{
    const Vec3 ray_origin{sampleOrigin()};
    scene.sample();
    Ray ray_to_pixel{ray_origin, subpixel_location - ray_origin, scene.getRefractiveIndex()};
    return ray_colour(ray_to_pixel, scene);
}

Vec3 Camera::colourPixel(const Vec3& pixel_location, Scene& scene) const
{
    Vec3 colour{};
    const std::vector<Vec3> subpixel_locations {samplePixel(pixel_location)};
    for (const Vec3& subpixel_location : subpixel_locations)
    {
        colour += colourSubpixel(subpixel_location, scene);
    }
    return colour / static_cast<double>(subpixel_locations.size());
}

Vec3 Camera::getRandomSubpixel(const Vec3& pixel_location) const
{
    Vec3 random_multiplier{Random::getRandom(DefinedIntervals::random_pixel), Random::getRandom(DefinedIntervals::random_pixel), 0};
    return pixel_location + random_multiplier*(pixel_dx + pixel_dy);
}

Vec3 Camera::ray_colour(Ray& ray, const Scene& scene) const
{
    Vec3 attenuation{1,1,1};
    for (int i{0}; i < config.max_depth; ++i)
    {
        const Hit hit{scene.getClosestHit(ray, Interval(0.001, Constants::infinity))};
        if (!hit)
        {
            return attenuation*background_colour(ray);
        }
        attenuation = attenuation * hit.material->attenuate(ray, hit.point, hit.normal);
    }
    return Vec3{0,0,0};
}

Vec3 Camera::background_colour(const Ray& ray) const
{
    Vec3 normalised_direction{ray.getDirection().getNormalised()};
    const double a {0.5*(normalised_direction[1] + 1.0)};
    return Vec3{(1 - a)*Vec3{1,1,1} + a*Vec3{0.5,0.7,1.0} };
}