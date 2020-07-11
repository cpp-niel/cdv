#pragma once

#include <cdv/core/rgba_color.hpp>

namespace cdv::elem
{
    struct symbol_properties
    {
        rgba_color color;
        char style = 'o';
    };
}