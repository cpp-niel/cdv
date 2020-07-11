#pragma once

#include <cdv/core/rgba_color.hpp>
#include <cdv/elem/line_properties.hpp>

#include <range/v3/view/repeat.hpp>
#include <range/v3/view/zip.hpp>

namespace cdv::elem
{
    struct fill_properties
    {
        rgba_color color = tab::blue;
        line_properties outline = {.width = points{0}};
    };
}