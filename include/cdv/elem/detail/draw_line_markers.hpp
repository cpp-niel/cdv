#pragma once

#include <cdv/core/vec2.hpp>

#include <range/v3/view/transform.hpp>

namespace cdv::elem::detail
{
    template <typename Surface, size_t NumPoints>
    void draw_polyline(Surface& surface, const vec2<double> (&line)[NumPoints], const pixels size, const pixel_pos pos)
    {
        const auto size_pos = pixel_pos(size, size);
        const auto xform = [&](const auto p) { return pos + scale(size_pos, p); };
        surface.draw_path(line | ranges::views::transform(xform));
    }

    template <typename Surface, size_t NumPointsLine0, size_t NumPointsLine1 = 1>
    auto line_marker(Surface& surface, const vec2<double> (&line0)[NumPointsLine0],
                     const vec2<double> (&line1)[NumPointsLine1] = {{0.0, 0.0}})
    {
        surface.set_line_properties_no_color({});
        return [&, line0, line1](const pixel_pos pos, const pixels size) {
            using namespace units_literals;
            draw_polyline(surface, line0, size, pos);
            (void)line1;
            if constexpr (NumPointsLine1 > 1) { draw_polyline(surface, line1, size, pos); }

            surface.stroke();
        };
    }

    template <typename Surface>
    auto x_marker(Surface& surface)
    {
        return line_marker(surface, {{-0.5, -0.5}, {0.5, 0.5}}, {{0.5, -0.5}, {-0.5, 0.5}});
    }

    template <typename Surface>
    auto plus_marker(Surface& surface)
    {
        return line_marker(surface, {{-0.5, 0.0}, {0.5, 0.0}}, {{0.0, -0.5}, {0.0, 0.5}});
    }

    template <typename Surface>
    auto tri_down_marker(Surface& surface)
    {
        return line_marker(surface, {{-0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}}, {{0.0, 0.0}, {0.0, -0.5}});
    }

    template <typename Surface>
    auto tri_up_marker(Surface& surface)
    {
        return line_marker(surface, {{-0.5, -0.5}, {0.0, 0.0}, {0.5, -0.5}}, {{0.0, 0.0}, {0.0, 0.5}});
    }

    template <typename Surface>
    auto tri_left_marker(Surface& surface)
    {
        return line_marker(surface, {{0.5, 0.5}, {0.0, 0.0}, {0.5, -0.5}}, {{0.0, 0.0}, {-0.5, 0.0}});
    }

    template <typename Surface>
    auto tri_right_marker(Surface& surface)
    {
        return line_marker(surface, {{-0.5, 0.5}, {0.0, 0.0}, {-0.5, -0.5}}, {{0.0, 0.0}, {0.5, 0.0}});
    }

    template <typename Surface>
    auto vline_marker(Surface& surface)
    {
        return line_marker(surface, {{0.0, -0.5}, {0.0, 0.5}});
    }

    template <typename Surface>
    auto hline_marker(Surface& surface)
    {
        return line_marker(surface, {{-0.5, 0.0}, {0.5, 0.0}});
    }

    template <typename Surface>
    auto tick_down_marker(Surface& surface)
    {
        return line_marker(surface, {{0.0, 0.0}, {0.0, -0.5}});
    }

    template <typename Surface>
    auto tick_up_marker(Surface& surface)
    {
        return line_marker(surface, {{0.0, 0.0}, {0.0, 0.5}});
    }

    template <typename Surface>
    auto tick_left_marker(Surface& surface)
    {
        return line_marker(surface, {{0.0, 0.0}, {-0.5, 0.0}});
    }

    template <typename Surface>
    auto tick_right_marker(Surface& surface)
    {
        return line_marker(surface, {{0.0, 0.0}, {0.5, 0.0}});
    }
}
