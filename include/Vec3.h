#ifndef VEC3_H
#define VEC3_H

#include <iostream>
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

    double length_squared() const;

    Vec3& normalise();
};

std::ostream& operator<<(std::ostream& out, const Vec3& vec3);

Vec3 operator+(const Vec3& v1, const Vec3& v2);

Vec3 operator-(const Vec3& v1, const Vec3& v2);

Vec3 operator*(const Vec3& v1, const Vec3& v2);

Vec3 operator*(double t, const Vec3& vec3);

Vec3 operator*(const Vec3& vec3, double t);

Vec3 operator/(const Vec3& vec3, double t);

double dot(const Vec3& v1, const Vec3& v2);

Vec3 cross(const Vec3& v1, const Vec3& v2);

Vec3 normalised(const Vec3& vec3);

Vec3 getRandomVec3(const Interval& interval = DefinedIntervals::canonical);

Vec3 getRandomUnitVec3();

Vec3 onHemisphere(const Vec3& unit_vector, const Vec3& normal);

Vec3 reflectVec3(const Vec3& vec, const Vec3& normal);

bool nearZero(const Vec3& vec);

#endif //VEC3_H
