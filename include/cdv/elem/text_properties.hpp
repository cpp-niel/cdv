#pragma once

#include "cdv/core/rgba_color.hpp"
#include "cdv/core/units.hpp"
#include "cdv/fnt/font_properties.hpp"

namespace cdv::elem
{
    struct text_properties
    {
        rgba_color color = css4::black;
        fnt::font_properties font;
        points font_size{8.0};
    };
}