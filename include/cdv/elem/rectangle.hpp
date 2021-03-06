#pragma once

#include <cdv/elem/detail/draw_rectangle.hpp>
#include <cdv/elem/fill_properties.hpp>
#include <cdv/core/vec2.hpp>

namespace cdv::elem
{
    struct rectangle
    {
        pixel_pos min;
        pixel_pos max;
        pixels corner_radius;
        fill_properties fill;
    };

    template <typename Surface>
    void draw(const rectangle& r, Surface& surface, pixel_pos)
    {
        using namespace units_literals;
        surface.set_color(r.fill.color);
        detail::fill_rectangle(surface, r.min, r.max, r.corner_radius);

        if (r.fill.outline.width > 0_pt)
        {
            surface.set_line_properties(r.fill.outline);
            detail::draw_rectangle(surface, r.min, r.max, r.corner_radius);
        }
    }
}
