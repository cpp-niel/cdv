#pragma once

#include "cdv/core/rgba_color.hpp"
#include "cdv/stdx/concepts.hpp"

#include <range/v3/back.hpp>
#include <range/v3/front.hpp>
#include <range/v3/iterator/operations.hpp>

#include <algorithm>
#include <array>
#include <cmath>

namespace cdv
{
    namespace detail
    {
        constexpr double splinterp(const double x0, const double x1, const double x2, const double x3, const double t)
        {
            const double t2 = t * t;
            const double t3 = t2 * t;
            const double u = 1.0 - t;

            const double a = u * u * u;
            const double b = 3.0 * t3 - 6.0 * t2 + 4.0;
            const double c = -3.0 * t3 + 3.0 * t2 + 3.0 * t + 1.0;
            const double d = t3;

            return (x0 * a + x1 * b + x2 * c + x3 * d) / 6.0;
        }

        constexpr auto splinterp(const stdx::random_access_range_of<double> auto& control_points, const double t)
        {
            const auto t0 = std::clamp(t, 0.0, 1.0);
            const auto n = ranges::distance(control_points);
            const auto imax = static_cast<double>(n) - 1.0;
            const auto i = std::min(imax - 1.0, std::floor(t0 * imax));
            const auto idx = static_cast<unsigned long>(i);
            const auto x1 = control_points[idx];
            const auto x2 = control_points[idx + 1];
            const auto x3 = (i < (imax - 1)) ? control_points[idx + 2] : (2.0 * x2) - x1;
            const auto x0 = (i > 0) ? control_points[idx - 1] : (2.0 * x1) - x2;
            const auto piece_t = imax * (t0 - (i / imax));
            return splinterp(x0, x1, x2, x3, piece_t);
        }

        template <size_t N>
        constexpr auto color_interpolator(const std::array<std::uint32_t, N> colors)
        {
            return [=](const double t) {
                std::array<double, N> reds;
                std::array<double, N> greens;
                std::array<double, N> blues;
                for (auto i = 0u; i < N; ++i)
                {
                    const auto [r, g, b, a] = rgba_color(colors[i]).as_doubles();
                    reds[i] = r;
                    greens[i] = g;
                    blues[i] = b;
                }

                const auto red = detail::splinterp(reds, t);
                const auto green = detail::splinterp(greens, t);
                const auto blue = detail::splinterp(blues, t);
                const auto alpha =
                    std::lerp(rgba_color(ranges::front(colors)).alpha(), rgba_color(ranges::back(colors)).alpha(), t);
                return rgba_color(red, green, blue, alpha);
            };
        }
    }
}