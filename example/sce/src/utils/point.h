

#pragma once

#include <cmath>

template <typename T>
class Point {
    static_assert(
        std::is_integral_v<T> || std::is_floating_point_v<T>,
        "T must be an integral or floating-point type"
    );

public:
    T x, y, z;

    Point(T x = 0, T y = 0, T z = 0) : 
        x(x), y(y), z(z) {}

    double Distance(const Point& other) const {
        double dx = static_cast<double>(other.x - x);
        double dy = static_cast<double>(other.y - y);
        double dz = static_cast<double>(other.z - z);
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};

