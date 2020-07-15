#pragma once

#include <cdv/core/units.hpp>

#include <utility>

namespace cdv
{
    template<typename T>
    struct vec2
    {
        using value_type = T;

        constexpr vec2() = default;
        constexpr vec2(const value_type x, const value_type y) : x(x), y(y) {}
        explicit constexpr vec2(const std::pair<value_type, value_type> p) : x(p.first), y(p.second) {}

        value_type x = value_type(0);
        value_type y = value_type(0);

        vec2& operator+=(const vec2& v) { x += v.x; y += v.y; return *this; }
        vec2& operator-=(const vec2& v) { x -= v.x; y -= v.y; return *this; }
        vec2& operator*=(const double factor) { x *= factor; y *= factor; return *this; }
        vec2& operator/=(const double factor) { x /= factor; y /= factor; return *this; }
    };

    template<typename T>
    bool operator==(const vec2<T>& v0, const vec2<T>& v1)
    {
        return v0.x == v1.x && v0.y == v1.y;
    }

    template<typename T>
    bool operator!=(const vec2<T>& v0, const vec2<T>& v1)
    {
        return !(v0 == v1);
    }

    template<typename T>
    [[nodiscard]] constexpr vec2<T> scale(const vec2<T>& v, const vec2<double> factors)
    {
        return vec2<T>{v.x * factors.x, v.y * factors.y};
    }

    template<typename T>
    [[nodiscard]] constexpr vec2<T> operator+(const vec2<T>& v0, const vec2<T>& v1)
    {
        auto result = v0;
        result += v1;
        return result;
    }

    template<typename T>
    [[nodiscard]] constexpr vec2<T> operator-(const vec2<T>& v0, const vec2<T>& v1)
    {
        auto result = v0;
        result -= v1;
        return result;
    }

    template<typename T>
    [[nodiscard]] constexpr vec2<T> operator*(const vec2<T>& v, const double factor)
    {
        auto result = v;
        result *= factor;
        return result;
    }

    template<typename T>
    [[nodiscard]] constexpr vec2<T> operator*(const double factor, const vec2<T>& v)
    {
        return v * factor;
    }

    template<typename T>
    [[nodiscard]] constexpr vec2<T> operator/(const vec2<T>& v, const double factor)
    {
        auto result = v;
        result /= factor;
        return result;
    }

    template<typename T>
    [[nodiscard]] constexpr vec2<T> operator-(const vec2<T>& v)
    {
        return {-v.x, -v.y};
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const vec2<T>& v)
    {
        return os << "[" << v.x << ", " << v.y << "]";
    }

    using pixel_pos = vec2<pixels>;
    inline pixel_pos make_pos(const pixels x, const pixels y) { return pixel_pos{x, y}; }
}
