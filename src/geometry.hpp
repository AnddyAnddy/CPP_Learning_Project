#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>

// struct Point2D
//{
//     float values[2] {};
//
//     Point2D() {}
//     Point2D(float x, float y) : values { x, y } {}
//
//     float& x() { return values[0]; }
//     float x() const { return values[0]; }
//
//     float& y() { return values[1]; }
//     float y() const { return values[1]; }
//
//     Point2D& operator+=(const Point2D& other)
//     {
//         x() += other.x();
//         y() += other.y();
//         return *this;
//     }
//
//     Point2D& operator*=(const Point2D& other)
//     {
//         x() *= other.x();
//         y() *= other.y();
//         return *this;
//     }
//
//     Point2D& operator*=(const float scalar)
//     {
//         x() *= scalar;
//         y() *= scalar;
//         return *this;
//     }
//
//     Point2D operator+(const Point2D& other) const
//     {
//         Point2D result = *this;
//         result += other;
//         return result;
//     }
//
//     Point2D operator*(const Point2D& other) const
//     {
//         Point2D result = *this;
//         result *= other;
//         return result;
//     }
//
//     Point2D operator*(const float scalar) const
//     {
//         Point2D result = *this;
//         result *= scalar;
//         return result;
//     }
// };
//
// struct Point3D
//{
//     std::array<float, 3> values;
//
//     Point3D() {}
//     Point3D(float x, float y, float z) : values { x, y, z } {}
//
//     float& x() { return values[0]; }
//     float x() const { return values[0]; }
//
//     float& y() { return values[1]; }
//     float y() const { return values[1]; }
//
//     float& z() { return values[2]; }
//     float z() const { return values[2]; }
//
//     Point3D& operator+=(const Point3D& other)
//     {
//         int i = 0;
//         std::transform(values.begin(), values.end(), values.begin(),
//                        [&other, &i](auto v) { return v + other.values[i++]; });
//         return *this;
//     }
//
//     Point3D& operator-=(const Point3D& other)
//     {
//         int i = 0;
//         std::transform(values.begin(), values.end(), values.begin(),
//                        [&other, &i](auto v) { return v - other.values[i++]; });
//         return *this;
//     }
//
//     Point3D& operator*=(const float scalar)
//     {
//         std::transform(values.begin(), values.end(), values.begin(), [scalar](auto v) { return v * scalar;
//         }); return *this;
//     }
//
//     Point3D operator+(const Point3D& other) const
//     {
//         Point3D result = *this;
//         result += other;
//         return result;
//     }
//
//     Point3D operator-(const Point3D& other) const
//     {
//         Point3D result = *this;
//         result -= other;
//         return result;
//     }
//
//     Point3D operator*(const float scalar) const
//     {
//         Point3D result = *this;
//         result *= scalar;
//         return result;
//     }
//
//     Point3D operator-() const { return Point3D { -x(), -y(), -z() }; }
//
//     float length() const
//     {
//         return std::sqrt(std::accumulate(values.begin(), values.end(), 0.f,
//                                          [](const auto v1, const auto v2) { return v1 + v2 * v2; }));
//     }
//
//     float distance_to(const Point3D& other) const { return (*this - other).length(); }
//
//     Point3D& normalize(const float target_len = 1.0f)
//     {
//         const float current_len = length();
//         if (current_len == 0)
//         {
//             throw std::logic_error("cannot normalize vector of length 0");
//         }
//
//         *this *= (target_len / current_len);
//         return *this;
//     }
//
//     Point3D& cap_length(const float max_len)
//     {
//         assert(max_len > 0);
//
//         const float current_len = length();
//         if (current_len > max_len)
//         {
//             *this *= (max_len / current_len);
//         }
//
//         return *this;
//     }
// };

template <typename T, int Dimension> struct Point
{
    std::array<T, Dimension> values;
    template <typename... Qs>
    Point(const T& first, Qs... others) : values { first, static_cast<T>(others)... }
    {
        static_assert(sizeof...(Qs) == Dimension - 1);
    };
    T& x() { return values[0]; }
    T x() const { return values[0]; }

    T& y()
    {
        static_assert(Dimension >= 2);
        return values[1];
    }
    T y() const
    {
        static_assert(Dimension >= 2);
        return values[1];
    }

    T& z()
    {
        static_assert(Dimension >= 3);
        return values[2];
    }
    T z() const
    {
        static_assert(Dimension >= 3);
        return values[2];
    }

    Point& operator+=(const Point& other)
    {
        int i = 0;
        std::transform(values.begin(), values.end(), values.begin(),
                       [&other, &i](auto v) { return v + other.values[i++]; });
        return *this;
    }

    Point& operator-=(const Point& other)
    {
        int i = 0;
        std::transform(values.begin(), values.end(), values.begin(),
                       [&other, &i](auto v) { return v - other.values[i++]; });
        return *this;
    }

    Point& operator*=(const T scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(), [scalar](auto v) { return v * scalar; });
        return *this;
    }

    Point& operator*=(const Point& other)
    {
        x() *= other.x();
        y() *= other.y();
        return *this;
    }

    Point operator+(const Point& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator-(const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result;
    }

    Point operator*(const T scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }

    Point operator*(const Point& other) const
    {
        Point result = *this;
        result *= other;
        return result;
    }

    T length() const
    {
        return std::sqrt(std::accumulate(values.begin(), values.end(), 0.f,
                                         [](const auto v1, const auto v2) { return v1 + v2 * v2; }));
    }

    Point operator-() const
    {
        Point result = *this;
        std::transform(values.begin(), values.end(), result.values.begin(), [](T elem) { return -elem; });
        return *this;
    }

    T distance_to(const Point& other) const { return (*this - other).length(); }

    Point& normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point& cap_length(const float max_len)
    {
        assert(max_len > 0);

        const T current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }
};

using Point2D = Point<float, 2>;
using Point3D = Point<float, 3>;
// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}
