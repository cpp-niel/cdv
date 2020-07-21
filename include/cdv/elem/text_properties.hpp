#pragma once

#include <cdv/core/rgba_color.hpp>
#include <cdv/core/units.hpp>
#include <cdv/fnt/font_properties.hpp>

namespace cdv::elem
{
    enum class horizontal_anchor
    {
        left = 0,
        center = 5,
        right = 10
    };

    enum class vertical_anchor
    {
        bottom = 0,
        middle = 5,
        top = 10
    };

    struct text_properties
    {
        rgba_color color = css4::black;
        fnt::font_properties font;
        points font_size{8.0};
    };
}