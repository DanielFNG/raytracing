#include "Vec3.h"
#include "Utilities.h"

double& Vec3::operator[](const int i)
{
    return values[i];
};

double Vec3::operator[](const int i) const
{
    return values[i];
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
    return std::sqrt(length_squared());
}

double Vec3::length_squared() const
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

double dot(const Vec3& v1, const Vec3& v2)
{
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

Vec3 cross(const Vec3& v1, const Vec3& v2)
{
    return Vec3
    {
        v1[1] * v2[2] - v1[2] * v2[1],
        v1[2] * v2[0] - v1[0] * v2[2],
        v1[0] * v2[1] - v1[1] * v2[0]
    };
}

Vec3 normalised(const Vec3& vec3)
{
    return vec3 / vec3.length();
}

Vec3 getRandomVec3(const Interval& interval)
{
    return Vec3{Random::getRandom(interval), Random::getRandom(interval), Random::getRandom(interval)};
}

Vec3 getRandomUnitVec3()
{
    while (true)
    {
        Vec3 sample{getRandomVec3(DefinedIntervals::unit)};
        const double length_squared{sample.length_squared()};
        if (1e-160 < length_squared && length_squared <= 1)
        {
            return sample / std::sqrt(length_squared);
        }
    }
}

Vec3 onHemisphere(const Vec3& unit_vector, const Vec3& normal)
{
    if (dot(unit_vector, normal) > 0.0)
    {
        return unit_vector;
    }
    return -unit_vector;
}

Vec3 reflectVec3(const Vec3& vec, const Vec3& normal)
{
    return vec - 2*dot(vec, normal)*normal;
}

bool nearZero(const Vec3& vec)
{
    const double limit {1e-8};
    return std::fabs(vec[0]) < limit && std::fabs(vec[1]) < limit && std::fabs(vec[2]) < limit;
}