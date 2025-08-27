#ifndef VEC3_H
#define VEC3_H

#include <fstream>
#include <array>
#include "Interval.h"

struct Vec3
{
    std::array<double, 3> values{};

    double& operator[](int i);
    double operator[](int i) const;

    Vec3 operator-() const;
    Vec3& operator+=(const Vec3& vec3);
    Vec3& operator*=(double t);
    Vec3& operator/=(double t);

    double length() const;
    double lengthSquared() const;

    Vec3& normalise();
    Vec3 getNormalised() const;

    double dot(const Vec3& vec) const;
    Vec3 cross(const Vec3& vec) const;

    bool isNearZero() const;

    static Vec3 getRandom(const Interval& interval = DefinedIntervals::canonical);
    static Vec3 getRandomUnit();
    static Vec3 getOnHemisphere(const Vec3& unit_vector, const Vec3& normal);
    static Vec3 getReflected(const Vec3& vec, const Vec3& normal);

};

std::ostream& operator<<(std::ostream& out, const Vec3& vec3);
void operator<<(std::ofstream& outfile, const Vec3& vec3);
Vec3 operator+(const Vec3& v1, const Vec3& v2);
Vec3 operator-(const Vec3& v1, const Vec3& v2);
Vec3 operator*(const Vec3& v1, const Vec3& v2);
Vec3 operator*(double t, const Vec3& vec3);
Vec3 operator*(const Vec3& vec3, double t);
Vec3 operator/(const Vec3& vec3, double t);

#endif //VEC3_H
