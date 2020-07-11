#pragma once

#include "cdv/core/vec2.hpp"

#include <range/v3/view/transform.hpp>

namespace cdv::elem::detail
{
    template <typename Surface, size_t NumPoints>
    auto polygonal_marker(Surface& surface, const vec2<double> (&polygon)[NumPoints])
    {
        return [&, polygon](const pixel_pos pos, const pixels size) {
            const auto xform = [&](const auto& p) { return pos + scale(pixel_pos(size, size), p); };
            surface.draw_path(polygon | ranges::views::transform(xform));
            surface.fill();
        };
    }

    template <typename Surface>
    auto triangle_down_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{-0.5, 0.5}, {0.0, -0.5}, {0.5, 0.5}});
    }

    template <typename Surface>
    auto triangle_up_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.5}, {-0.5, -0.5}, {0.5, -0.5}});
    }

    template <typename Surface>
    auto triangle_left_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{-0.5, 0.0}, {0.5, -0.5}, {0.5, 0.5}});
    }

    template <typename Surface>
    auto triangle_right_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{-0.5, 0.5}, {-0.5, -0.5}, {0.5, 0.0}});
    }

    template <typename Surface>
    auto octagon_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{-0.5, 0.25},
                                           {-0.5, -0.25},
                                           {-0.25, -0.5},
                                           {0.25, -0.5},
                                           {0.5, -0.25},
                                           {0.5, 0.25},
                                           {0.25, 0.5},
                                           {-0.25, 0.5}});
    }

    template <typename Surface>
    auto square_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{-0.5, 0.5}, {-0.5, -0.5}, {0.5, -0.5}, {0.5, 0.5}});
    }

    template <typename Surface>
    auto pentagon_marker(Surface& surface)
    {
        return polygonal_marker(surface,
                                {{0.0, 0.5}, {-0.475, 0.155}, {-0.295, -0.405}, {0.295, -0.405}, {0.475, 0.155}});
    }

    template <typename Surface>
    auto plus_filled_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{-0.5, 0.166667},
                                           {-0.5, -0.166667},
                                           {-0.166667, -0.166667},
                                           {-0.166667, -0.5},
                                           {0.166667, -0.5},
                                           {0.166667, -0.166667},
                                           {0.5, -0.166667},
                                           {0.5, 0.166667},
                                           {0.166667, 0.166667},
                                           {0.166667, 0.5},
                                           {-0.166667, 0.5},
                                           {-0.166667, 0.166667}});
    }

    template <typename Surface>
    auto star_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.5},
                                           {-0.07375, 0.10125},
                                           {-0.475, 0.155},
                                           {-0.11875, -0.03875},
                                           {-0.295, -0.405},
                                           {0.0, -0.125},
                                           {0.295, -0.405},
                                           {0.11875, -0.03875},
                                           {0.475, 0.155},
                                           {0.07375, 0.10125}});
    }

    template <typename Surface>
    auto hexagon1_marker(Surface& surface)
    {
        return polygonal_marker(
            surface, {{0.0, 0.5}, {-0.433, 0.25}, {-0.433, -0.25}, {0.0, -0.5}, {0.433, -0.25}, {0.433, 0.25}});
    }

    template <typename Surface>
    auto hexagon2_marker(Surface& surface)
    {
        return polygonal_marker(
            surface, {{-0.25, 0.433}, {-0.5, 0.0}, {-0.25, -0.433}, {0.25, -0.433}, {0.5, 0.0}, {0.25, 0.433}});
    }

    template <typename Surface>
    auto x_filled_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.2},
                                           {-0.3, 0.5},
                                           {-0.5, 0.3},
                                           {-0.2, 0.0},
                                           {-0.5, -0.3},
                                           {-0.3, -0.5},
                                           {0.0, -0.2},
                                           {0.3, -0.5},
                                           {0.5, -0.3},
                                           {0.2, 0.0},
                                           {0.5, 0.3},
                                           {0.3, 0.5}});
    }

    template <typename Surface>
    auto diamond_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.5}, {-0.5, 0.0}, {0.0, -0.5}, {0.5, 0.0}});
    }

    template <typename Surface>
    auto thin_diamond_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.5}, {-0.3, 0.0}, {0.0, -0.5}, {0.3, 0.0}});
    }

    template <typename Surface>
    auto caret_left_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.0}, {0.5, -0.5}, {0.5, 0.5}});
    }

    template <typename Surface>
    auto caret_right_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.0}, {-0.5, 0.5}, {-0.5, -0.5}});
    }

    template <typename Surface>
    auto caret_up_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.0}, {-0.5, -0.5}, {0.5, -0.5}});
    }

    template <typename Surface>
    auto caret_down_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.0}, {0.5, 0.5}, {-0.5, 0.5}});
    }

    template <typename Surface>
    auto caret_left_base_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.5}, {-0.5, 0.0}, {0.0, -0.5}});
    }

    template <typename Surface>
    auto caret_right_base_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.5}, {0.0, -0.5}, {0.5, 0.0}});
    }

    template <typename Surface>
    auto caret_up_base_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{0.0, 0.5}, {-0.5, 0.0}, {0.5, 0.0}});
    }

    template <typename Surface>
    auto caret_down_base_marker(Surface& surface)
    {
        return polygonal_marker(surface, {{-0.5, 0.0}, {0.0, -0.5}, {0.5, 0.0}});
    }
}
