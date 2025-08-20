#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"
#include "Ray.h"
#include <memory>

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

    Vec3 attenuate(Ray& ray, const Vec3& point, const Vec3& normal) const override;

    std::unique_ptr<Material> make_unique() const override;

};

struct Reflector : Material
{
    double fuzz{};

    Reflector(const Vec3& albedo, double fuzz)
        : Material(albedo), fuzz{fuzz}
    {}

    Vec3 attenuate(Ray& ray, const Vec3& point, const Vec3& normal) const override;

    std::unique_ptr<Material> make_unique() const override;

};

struct Refractor : Material
{
    double refractive_index{0};

    Refractor(const Vec3& albedo, double refractive_index)
        : Material(albedo), refractive_index{refractive_index}
    {}

    Vec3 attenuate(Ray& ray, const Vec3& point, const Vec3& normal) const override;

    std::unique_ptr<Material> make_unique() const override;

    static double computeCosineTerm(const Vec3& normalised_direction, const Vec3& normal);
    static bool canRefract(double cosine_term, double refractive_ratio);
    static bool doesRefract(double cosine_term, double refractive_ratio);
    static double computeSchickApproximation(double cosine_term, double refractive_ratio);
};


#endif //MATERIAL_H
