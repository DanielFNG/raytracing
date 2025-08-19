#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"
#include "Ray.h"
#include <memory>
#include <cmath>
#include "Utilities.h"

struct Material {
    Vec3 albedo{};

    explicit Material(const Vec3& albedo)
        : albedo{albedo}
    {}

    virtual Vec3 attenuate(Ray& ray, const Vec3& point, const Vec3& normal) const = 0;

    virtual std::unique_ptr<Material> make_unique() const = 0;

    virtual ~Material() = default;
};

struct Lambertian : Material
{
    explicit Lambertian(const Vec3& albedo)
        : Material(albedo)
    {}

    Vec3 attenuate(Ray& ray, const Vec3& point, const Vec3& normal) const override
    {
        ray.scatter(point, normal);
        return albedo;
    }

    std::unique_ptr<Material> make_unique() const override
    {
        return std::make_unique<Lambertian>(*this);
    }

};

struct Reflector : Material
{
    double fuzz{};

    Reflector(const Vec3& albedo, double fuzz)
        : Material(albedo), fuzz{fuzz}
    {}

    Vec3 attenuate(Ray& ray, const Vec3& point, const Vec3& normal) const override
    {
        ray.reflect(point, normal, fuzz);
        return albedo;
    }

    std::unique_ptr<Material> make_unique() const override
    {
        return std::make_unique<Reflector>(*this);
    }

};

struct Refractor : Material
{
    double refractive_index{0};

    Refractor(const Vec3& albedo, double refractive_index)
        : Material(albedo), refractive_index{refractive_index}
    {}

    Vec3 attenuate(Ray& ray, const Vec3& point, const Vec3& normal) const override
    {
        const bool entering{dot(ray.getDirection(), normal) < 0};
        const Vec3 normal_against_ray{entering ? normal : -normal};
        const double cosine_term{computeCosineTerm(normalised(ray.getDirection()), normal_against_ray)};
        if (refracts(cosine_term, ray.getRefractiveRatio(entering, refractive_index)))
        {
            ray.refract(point, normal_against_ray, refractive_index, cosine_term, entering);
        }
        else
        {
            ray.reflect(point, normal_against_ray, 0.0);
        }
        return Vec3{1.0, 1.0, 1.0};
    }

    double computeCosineTerm(const Vec3& normalised_direction, const Vec3& normal) const
    {
        return std::min(dot(-normalised_direction, normal), 1.0);
    }

    bool refracts(double cosine_term, const double refractive_ratio) const
    {
        return canRefract(cosine_term, refractive_ratio) && doesRefract(cosine_term, refractive_ratio);
    }

    bool canRefract(double cosine_term, const double refractive_ratio) const
    {
        const double sin_term{std::sqrt(1.0 - cosine_term*cosine_term)};
        return refractive_ratio * sin_term <= 1.0;
    }

    bool doesRefract(const double cosine_term, const double refractive_ratio) const
    {
        return computeSchickApproximation(cosine_term, refractive_ratio) < Random::getRandom();
    }

    double computeSchickApproximation(const double cosine_term, const double refractive_ratio) const
    {
        double r0 {(1 - refractive_ratio)/(1 + refractive_ratio)};
        r0 = r0*r0;
        return r0 + (1 - r0)*std::pow((1 - cosine_term), 5);
    }

    std::unique_ptr<Material> make_unique() const override
    {
        return std::make_unique<Refractor>(*this);
    }

};


#endif //MATERIAL_H
