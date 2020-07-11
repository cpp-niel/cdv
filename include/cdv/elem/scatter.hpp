#pragma once

#include <cdv/core/units.hpp>
#include <cdv/elem/detail/draw_markers.hpp>
#include <cdv/elem/symbol_properties.hpp>

namespace cdv::elem
{
    template <ranges::range XRange, ranges::range YRange, stdx::range_of<points> SizeRange = std::vector<points>>
    struct scatter
    {
        scatter(const XRange& xs, const YRange& ys, const symbol_properties& properties = {})
            : xs(xs), ys(ys), properties(properties)
        {
        }

        scatter(const XRange& xs, const YRange& ys, const SizeRange& sizes, const symbol_properties& properties = {})
            : xs(xs), ys(ys), sizes(sizes), properties(properties)
        {
        }

        XRange xs;
        YRange ys;
        SizeRange sizes;
        symbol_properties properties;
    };

    template <ranges::range XRange, ranges::range YRange, typename Surface, stdx::range_of<points> SizeRange>
    void draw(const scatter<XRange, YRange, SizeRange>& s, Surface& surface, const pixel_pos&)
    {
        surface.set_color(s.properties.color);
        const auto positions = ranges::views::zip_with(
            [&](const auto x, const auto y) {
                return pixel_pos{x, y};
            },
            s.xs, s.ys);
        detail::draw_markers(surface, positions, s.sizes, s.properties.style);
    }
}