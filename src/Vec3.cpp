#include "Vec3.h"
#include "Utilities.h"

double& Vec3::operator[](const int i)
{
    return values[static_cast<size_t>(i)];
};

double Vec3::operator[](const int i) const
{
    return values[static_cast<size_t>(i)];
};

Vec3 Vec3::operator-() const
{
    return {-values[0], -values[1], -values[2]};
}

Vec3& Vec3::operator+=(const Vec3& vec3)
{
    values[0] += vec3.values[0];
    values[1] += vec3.values[1];
    values[2] += vec3.values[2];
    return *this;
}

Vec3& Vec3::operator*=(const double t)
{
    values[0] *= t;
    values[1] *= t;
    values[2] *= t;
    return *this;
}

Vec3& Vec3::operator/=(const double t)
{
    return *this *= (1/t);
}

double Vec3::length() const
{
    return std::sqrt(lengthSquared());
}

double Vec3::lengthSquared() const
{
    return values[0]*values[0] + values[1]*values[1] + values[2]*values[2];
}

Vec3& Vec3::normalise()
{
    return *this /= length();
}


std::ostream& operator<<(std::ostream& out, const Vec3& vec3)
{
    return out << vec3.values[0] << " " << vec3.values[1] << " " << vec3.values[2];
}

void operator<<(std::ofstream& outfile, const Vec3& vec3)
{
    outfile << vec3.values[0] << " " << vec3.values[1] << " " << vec3.values[2];
}

Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    return Vec3{v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]};
}

Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
    return Vec3{v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]};
}

Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
    return Vec3{v1[0]*v2[0], v1[1]*v2[1], v1[2]*v2[2]};
}

Vec3 operator*(const Vec3& vec3, const double t)
{
    return Vec3{t*vec3[0], t*vec3[1], t*vec3[2]};
}

Vec3 operator*(const double t, const Vec3& vec3)
{
    return vec3 * t;
}

Vec3 operator/(const Vec3& vec3, const double t)
{
    return vec3 * (1/t);
}

double Vec3::dot(const Vec3& vec) const
{
    return values[0]*vec[0] + values[1]*vec[1] + values[2]*vec[2];
}

Vec3 Vec3::cross(const Vec3& vec) const
{
    return Vec3
    {
        values[1] * vec[2] - values[2] * vec[1],
        values[2] * vec[0] - values[0] * vec[2],
        values[0] * vec[1] - values[1] * vec[0]
    };
}

Vec3 Vec3::getNormalised() const
{
    return *this/this->length();
}

Vec3 Vec3::getRandom(const Interval& interval)
{
    return Vec3{Random::getRandom(interval), Random::getRandom(interval), Random::getRandom(interval)};
}

Vec3 Vec3::getRandomUnit()
{
    while (true)
    {
        Vec3 sample{getRandom(DefinedIntervals::unit)};
        const double length_squared{sample.lengthSquared()};
        if (1e-160 < length_squared && length_squared <= 1)
        {
            return sample / std::sqrt(length_squared);
        }
    }
}

Vec3 Vec3::getOnHemisphere(const Vec3& unit_vector, const Vec3& normal)
{
    if (unit_vector.dot(normal) > 0.0)
    {
        return unit_vector;
    }
    return -unit_vector;
}

Vec3 Vec3::getReflected(const Vec3& vec, const Vec3& normal)
{
    return vec - 2*vec.dot(normal)*normal;
}

bool Vec3::isNearZero() const
{
    const double limit {1e-8};
    return std::fabs(values[0]) < limit && std::fabs(values[1]) < limit && std::fabs(values[2]) < limit;
}