#pragma once

#include "cdv/core/rgba_color.hpp"

#include "cdv/stdx/numbers.hpp"

#include <cmath>

namespace cdv::interpolator
{
    constexpr auto sinebow = [](const double t) {
        const auto pi = stdx::numbers::pi;
        const auto theta = pi * (0.5 - t);
        const auto r = std::sin(theta);
        const auto g = std::sin(theta + (pi / 3.0));
        const auto b = std::sin(theta + ((pi / 3.0) * 2.0));
        return rgba_color(r * r, g * g, b * b);
    };
}