#pragma once

#include <cdv/core/units.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/fill_properties.hpp>

namespace cdv::elem
{
    struct arc
    {
        pixel_pos center;
        pixels inner_radius;
        pixels outer_radius;
        radians start_angle;
        radians end_angle;
        radians pad_angle;
        fill_properties fill;
    };

    template <typename Surface>
    void draw(const arc& a, Surface& surface, const pixel_pos)
    {
        using namespace units_literals;

        const auto pad_arc_length = a.pad_angle.value() * a.outer_radius;
        const auto inner_pad_angle = (a.inner_radius > 0_px) ? radians(pad_arc_length / a.inner_radius) : a.pad_angle;

        const auto outer_a0 = a.start_angle + (a.pad_angle * 0.5);
        const auto outer_a1 = a.end_angle - (a.pad_angle * 0.5);
        const auto inner_a0 = a.start_angle + (inner_pad_angle * 0.5);
        const auto inner_a1 = a.end_angle - (inner_pad_angle * 0.5);
        surface.set_color(a.fill.color);
        surface.draw_arc(a.center, a.outer_radius, outer_a0, outer_a1);
        surface.draw_arc(a.center, a.inner_radius, inner_a1, inner_a0);
        surface.fill();

        if (a.fill.outline.width > 0_pt)
        {
            surface.set_line_properties(a.fill.outline);
            surface.draw_arc(a.center, a.outer_radius, outer_a0, outer_a1);
            surface.draw_arc(a.center, a.inner_radius, inner_a1, inner_a0);
            const auto p0 = pixel_pos{cos(outer_a0.value()) * a.outer_radius, sin(outer_a0.value()) * a.outer_radius};
            surface.line_to(a.center + p0);
            surface.stroke();
        }
    }
}