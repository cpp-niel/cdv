#pragma once

#include <cdv/core/vec2.hpp>
#include <cdv/stdx/numbers.hpp>

namespace cdv::elem::detail
{
    template <typename Surface>
    void draw_rounded_rectangle(Surface& surface, const pixel_pos min, const pixel_pos max, const pixels radius)
    {
        const auto quarter_circle = stdx::numbers::pi * 0.5;
        surface.draw_arc ({max.x - radius, min.y + radius}, radius, -quarter_circle, 0.0);
        surface.draw_arc (max - pixel_pos(radius, radius), radius, 0.0, quarter_circle);
        surface.draw_arc ({min.x + radius, max.y - radius}, radius, quarter_circle, 2.0 * quarter_circle);
        surface.draw_arc (min + pixel_pos(radius, radius), radius, 2.0 * quarter_circle, 3.0 * quarter_circle);
        surface.fill();
    }

    template <typename Surface>
    void draw_rectangle(Surface& surface, const pixel_pos min, const pixel_pos max, const pixels corner_radius)
    {
        if (corner_radius == pixels{})
            surface.draw_path({min, {max.x, min.y}, max, {min.x, max.y}, min});
        else
            draw_rounded_rectangle(surface, min, max, corner_radius);

        surface.stroke();
    }

    template <typename Surface>
    void fill_rectangle(Surface& surface, const pixel_pos min, const pixel_pos max, const pixels corner_radius)
    {
        if (corner_radius == pixels{})
            surface.draw_path({min, {max.x, min.y}, max, {min.x, max.y}, min});
        else
            draw_rounded_rectangle(surface, min, max, corner_radius);

        surface.fill();
    }
}