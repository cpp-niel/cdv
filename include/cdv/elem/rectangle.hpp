#pragma once

#include <cdv/elem/fill_properties.hpp>
#include <cdv/core/vec2.hpp>

namespace cdv::elem
{
    struct rectangle
    {
        pixel_pos min;
        pixel_pos max;
        fill_properties fill;
    };

    template <typename Surface>
    void draw(const rectangle& r, Surface& surface, pixel_pos)
    {
        using namespace units_literals;
        surface.set_color(r.fill.color);
        surface.fill_rectangle(r.min, r.max);

        if (r.fill.outline.width > 0_pt)
        {
            surface.set_line_properties(r.fill.outline);
            surface.draw_rectangle(r.min, r.max);
        }
    }
}
