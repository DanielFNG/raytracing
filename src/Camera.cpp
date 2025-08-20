#if defined(USE_OPENMP)
#include "omp.h"
#endif

#include "Camera.h"
#include "PGM.h"
#include "Utilities.h"
#include "Interval.h"
#include <execution>

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
    viewport_dz = normalised(-direction);
    viewport_dx = normalised(cross(twist_vector, viewport_dz));
    viewport_dy = cross(viewport_dz, viewport_dx);
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
        if (point_in_unit_disc.length_squared() < 1)
        {
            return origin + point_in_unit_disc[0] * defocus_region_dx + point_in_unit_disc[1] * defocus_region_dy;
        }
    }
}

Vec3 Camera::colourSubpixel(const Vec3& subpixel_location, const Scene& scene) const
{
    const Vec3 ray_origin{sampleOrigin()};
    Ray ray_to_pixel{ray_origin, subpixel_location - ray_origin, scene.getRefractiveIndex()};
    return ray_colour(ray_to_pixel, scene);
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
    Vec3 normalised_direction{normalised(ray.getDirection())};
    const double a {0.5*(normalised_direction[1] + 1.0)};
    return Vec3{(1 - a)*Vec3{1,1,1} + a*Vec3{0.5,0.7,1.0} };
}