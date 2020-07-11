#pragma once

#include <cdv/core/units.hpp>
#include <cdv/elem/detail/draw_markers.hpp>
#include <cdv/elem/symbol_properties.hpp>

namespace cdv::elem
{
    struct symbol
    {
        pixel_pos position;
        points size{6.0};
        symbol_properties properties;
    };

    template <typename Surface>
    void draw(const symbol& s, Surface& surface, const pixel_pos)
    {
        surface.set_color(s.properties.color);
        detail::draw_markers(surface, std::array{s.position}, std::array{s.size}, s.properties.style);
    }
}