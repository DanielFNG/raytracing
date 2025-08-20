#include "Material.h"
#include "Utilities.h"
#include <cmath>

Vec3 Lambertian::attenuate(Ray& ray, const Vec3& point, const Vec3& normal) const
{
    ray.scatter(point, normal);
    return albedo;
}

std::unique_ptr<Material> Lambertian::make_unique() const
{
    return std::make_unique<Lambertian>(*this);
}

Vec3 Reflector::attenuate(Ray& ray, const Vec3& point, const Vec3& normal) const
{
    ray.reflect(point, normal, fuzz);
    return albedo;
}

std::unique_ptr<Material> Reflector::make_unique() const
{
    return std::make_unique<Reflector>(*this);
}

Vec3 Refractor::attenuate(Ray& ray, const Vec3& point, const Vec3& normal) const
{
    const bool entering{ray.getDirection().dot(normal) < 0};
    const Vec3 normal_against_ray{entering ? normal : -normal};
    const double cosine_term{computeCosineTerm(ray.getDirection().getNormalised(), normal_against_ray)};
    const double refractive_ratio{ray.getRefractiveRatio(entering, refractive_index)};
    if (canRefract(cosine_term, refractive_ratio) && doesRefract(cosine_term, refractive_ratio))
    {
        ray.refract(point, normal_against_ray, refractive_index, cosine_term, entering);
    }
    else
    {
        ray.reflect(point, normal_against_ray, 0.0);
    }
    return Vec3{1.0, 1.0, 1.0};
}

std::unique_ptr<Material> Refractor::make_unique() const
{
    return std::make_unique<Refractor>(*this);
}

double Refractor::computeCosineTerm(const Vec3& normalised_direction, const Vec3& normal)
{
    return std::min((-normalised_direction).dot(normal), 1.0);
}

bool Refractor::canRefract(double cosine_term, const double refractive_ratio)
{
    const double sin_term{std::sqrt(1.0 - cosine_term*cosine_term)};
    return refractive_ratio * sin_term <= 1.0;
}

bool Refractor::doesRefract(const double cosine_term, const double refractive_ratio)
{
    return computeSchickApproximation(cosine_term, refractive_ratio) < Random::getRandom();
}

double Refractor::computeSchickApproximation(const double cosine_term, const double refractive_ratio)
{
    double r0 {(1 - refractive_ratio)/(1 + refractive_ratio)};
    r0 = r0*r0;
    return r0 + (1 - r0)*std::pow((1 - cosine_term), 5);
}