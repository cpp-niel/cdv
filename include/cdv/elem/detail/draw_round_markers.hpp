#pragma once

#include <cdv/core/vec2.hpp>

#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

#include <vector>

namespace cdv::elem::detail
{
    constexpr auto point_marker_size_factor = 0.3;

    template <typename Surface>
    auto point_marker(Surface& surface)
    {
        return [&](const pixel_pos pos, const pixels size) {
            surface.circle(pos, size * point_marker_size_factor);
        };
    }

    template <typename Surface>
    auto pixel_marker(Surface& surface)
    {
        return [&](const pixel_pos pos, const pixels) {
            using namespace units_literals;
            surface.circle(pos, 1_px);
        };
    }

    template <typename Surface>
    auto circle_marker(Surface& surface)
    {
        return [&](const pixel_pos pos, const pixels size) {
          surface.circle(pos, size * 0.5);
        };
    }
}
