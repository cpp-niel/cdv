#pragma once

#include <cdv/elem/detail/draw_line_markers.hpp>
#include <cdv/elem/detail/draw_polygonal_markers.hpp>
#include <cdv/elem/detail/draw_round_markers.hpp>

#include <range/v3/view/repeat.hpp>

namespace cdv::elem
{
    constexpr char no_marker = -1;
}

namespace cdv::elem::detail
{
    template <typename Path, typename Sizes, typename DrawMarkerFunction>
    void draw_all_markers_impl(const Path& positions, const Sizes& sizes, const DrawMarkerFunction& draw_marker)
    {
        for (const auto& [pos, sz] : ranges::views::zip(positions, sizes))
        {
            draw_marker(pos, sz);
        }
    }

    template <typename Surface, typename Path, typename Sizes, typename DrawMarkerFunction>
    void draw_all_markers(Surface& surface, const Path& positions, const Sizes& sizes,
                          const DrawMarkerFunction& draw_marker)
    {
        namespace rv = ::ranges::views;
        if (ranges::distance(sizes) < 2)
        {
            const auto px_sizes =
                rv::repeat(ranges::empty(sizes) ? surface.to_pixels(points{6.0}) : surface.to_pixels(*ranges::begin(sizes)));
            draw_all_markers_impl(positions, px_sizes, draw_marker);
        }
        else
        {
            const auto px_sizes = sizes | rv::transform([&](const points p) { return surface.to_pixels(p); });
            draw_all_markers_impl(positions, px_sizes, draw_marker);
        }
    }

    template <typename Surface, typename Path, typename Sizes>
    void draw_markers(Surface& surface, const Path& positions, const Sizes& sizes, const char style)
    {
        if (style == no_marker) return;

        const auto draw_as = [&](const auto& draw_individual_marker) {
            draw_all_markers(surface, positions, sizes, draw_individual_marker);
        };

        if (style == '.')
            draw_as(point_marker(surface));
        else if (style == ',')
            draw_as(pixel_marker(surface));
        else if (style == 'o')
            draw_as(circle_marker(surface));
        else if (style == 'x')
            draw_as(x_marker(surface));
        else if (style == '+')
            draw_as(plus_marker(surface));
        else if (style == '1')
            draw_as(tri_down_marker(surface));
        else if (style == '2')
            draw_as(tri_up_marker(surface));
        else if (style == '3')
            draw_as(tri_left_marker(surface));
        else if (style == '4')
            draw_as(tri_right_marker(surface));
        else if (style == 'v')
            draw_as(triangle_down_marker(surface));
        else if (style == '^')
            draw_as(triangle_up_marker(surface));
        else if (style == '<')
            draw_as(triangle_left_marker(surface));
        else if (style == '>')
            draw_as(triangle_right_marker(surface));
        else if (style == '8')
            draw_as(octagon_marker(surface));
        else if (style == 's')
            draw_as(square_marker(surface));
        else if (style == 'p')
            draw_as(pentagon_marker(surface));
        else if (style == 'P')
            draw_as(plus_filled_marker(surface));
        else if (style == '*')
            draw_as(star_marker(surface));
        else if (style == 'h')
            draw_as(hexagon1_marker(surface));
        else if (style == 'H')
            draw_as(hexagon2_marker(surface));
        else if (style == 'X')
            draw_as(x_filled_marker(surface));
        else if (style == 'D')
            draw_as(diamond_marker(surface));
        else if (style == 'd')
            draw_as(thin_diamond_marker(surface));
        else if (style == '|')
            draw_as(vline_marker(surface));
        else if (style == '_')
            draw_as(hline_marker(surface));
        else if (style == 0)
            draw_as(tick_left_marker(surface));
        else if (style == 1)
            draw_as(tick_right_marker(surface));
        else if (style == 2)
            draw_as(tick_up_marker(surface));
        else if (style == 3)
            draw_as(tick_down_marker(surface));
        else if (style == 4)
            draw_as(caret_left_marker(surface));
        else if (style == 5)
            draw_as(caret_right_marker(surface));
        else if (style == 6)
            draw_as(caret_up_marker(surface));
        else if (style == 7)
            draw_as(caret_down_marker(surface));
        else if (style == 8)
            draw_as(caret_left_base_marker(surface));
        else if (style == 9)
            draw_as(caret_right_base_marker(surface));
        else if (style == 10)
            draw_as(caret_up_base_marker(surface));
        else if (style == 11)
            draw_as(caret_down_base_marker(surface));
    }
}
