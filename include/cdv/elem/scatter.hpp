#pragma once

#include <cdv/core/units.hpp>
#include <cdv/elem/detail/draw_markers.hpp>
#include <cdv/elem/symbol_properties.hpp>

namespace cdv::elem
{
    template <ranges::range XRange, ranges::range YRange, stdx::range_of<points> SizeRange = std::vector<points>>
    struct scatter
    {
        XRange xs;
        YRange ys;
        SizeRange sizes;
        symbol_properties properties;
    };

    template <ranges::range XRange, ranges::range YRange, typename Surface, stdx::range_of<points> SizeRange>
    void draw(const scatter<XRange, YRange, SizeRange>& s, Surface& surface, const pixel_pos&)
    {
        surface.set_color(s.properties.color);
        const auto positions = ranges::views::zip_with(make_pos, s.xs, s.ys);
        detail::draw_markers(surface, positions, s.sizes, s.properties.style);
    }
}