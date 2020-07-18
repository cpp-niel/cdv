#pragma once

#include <cdv/stdx/concepts.hpp>

#include <fmt/format.h>

#include <range/v3/algorithm/transform.hpp>
#include <range/v3/back.hpp>
#include <range/v3/front.hpp>

#include <array>
#include <charconv>
#include <iosfwd>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace cdv
{
    namespace detail
    {
        template <stdx::floating_point T>
        constexpr bool is_in_range(const T x)
        {
            constexpr auto min = T(0);
            constexpr auto max = T(1);
            return (x >= min) && (x <= max);
        }

        template <stdx::floating_point T>
        constexpr bool is_in_range(const T r, const T g, const T b, const T a)
        {
            return is_in_range(r) and is_in_range(g) and is_in_range(b) and is_in_range(a);
        }

        template <stdx::floating_point T>
        constexpr std::uint32_t to_uint32(const T r, const T g, const T b, const T a)
        {
            if (!is_in_range(r, g, b, a))
                throw std::domain_error("Floating point rgba values must be inside the [0, 1] range.");  // TODO clamp?

            constexpr auto lim = T(255.0f);
            return (std::uint32_t(r * lim) << 24u) | (std::uint32_t(g * lim) << 16u) | (std::uint32_t(b * lim) << 8u)
                   | std::uint32_t(a * lim);
        }

        constexpr std::uint32_t char_to_uint32(const char c)
        {
            switch (c)
            {
            case '0':
            case 'k':
                return 0x000000ff;
            case '1':
            case 'w':
                return 0xffffffff;
            case 'r':
                return 0xff0000ff;
            case 'g':
                return 0x00ff00ff;
            case 'b':
                return 0x0000ffff;
            case 'y':
                return 0xffff00ff;
            case 'm':
                return 0xff00ffff;
            case 'c':
                return 0x00ffffff;
            }

            throw std::invalid_argument(fmt::format("'{}' is not a valid single character color string", c));
        }

        std::uint32_t hex_string_to_uint32(std::string_view s);
        std::uint32_t to_uint32(std::string_view s);
        std::optional<std::uint32_t> css4_string_to_uint32(std::string_view s);
        std::optional<std::uint32_t> tableau_string_to_uint32(std::string_view s);
    }

    class rgba_color
    {
    public:
        constexpr rgba_color() = default;
        constexpr explicit rgba_color(const std::uint32_t value) : value_(value) {}
        constexpr rgba_color(const double r, const double g, const double b) : value_(detail::to_uint32(r, g, b, 1.0))
        {
        }
        constexpr rgba_color(const double r, const double g, const double b, const double a)
            : value_(detail::to_uint32(r, g, b, a))
        {
        }
        explicit rgba_color(std::string_view hex_string) : value_(detail::to_uint32(hex_string)) {}

        [[nodiscard]] constexpr rgba_color with_alpha(double a) const
        {
            if (!detail::is_in_range(a))
                throw std::domain_error("Floating point rgba values must be inside the [0, 1] range.");  // TODO clamp?

            return rgba_color((value_ & 0xffffff00) | std::uint32_t(a * 255.0));
        }

        [[nodiscard]] constexpr std::uint32_t as_uint32() const noexcept { return value_; }

        [[nodiscard]] constexpr std::array<double, 4> as_doubles() const
        {
            const auto rbyte = (value_ & 0xff000000u) >> 24u;
            const auto gbyte = (value_ & 0x00ff0000u) >> 16u;
            const auto bbyte = (value_ & 0x0000ff00u) >> 8u;

            return {rbyte / 255.0, gbyte / 255.0, bbyte / 255.0, alpha()};
        }

        [[nodiscard]] constexpr double alpha() const
        {
            return (value_ & 0x000000ffu) / 255.0;
        }

        friend bool operator==(const rgba_color c0, const rgba_color c1) { return c0.value_ == c1.value_; }
        friend bool operator!=(const rgba_color c0, const rgba_color c1) { return c0.value_ != c1.value_; }
        friend std::ostream& operator<<(std::ostream& os, const rgba_color c);

    private:
        std::uint32_t value_ = 0x000000ff;
    };

    namespace css4
    {
#define CDV_DEFINE_COLOR(name, value) constexpr auto name = rgba_color(value);
#include <cdv/core/color/css4.hpp>
#undef CDV_DEFINE_COLOR
    }

    namespace tab
    {
#define CDV_DEFINE_COLOR(name, value) constexpr auto name = rgba_color(value);
#include <cdv/core/color/tableau.hpp>
#undef CDV_DEFINE_COLOR
    }

    constexpr auto cdv_blue = rgba_color(0x1389daff);
}
