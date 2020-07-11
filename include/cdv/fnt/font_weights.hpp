#pragma once

#include <string>

namespace cdv::fnt
{
    enum class font_weights
    {
        ultralight = 40,
        light = 50,
        normal = 80,
        medium = 100,
        semibold = 180,
        bold = 200,
        heavy = 210,
        black = 215
    };

    std::string to_string(const font_weights weight);

    enum class font_slants
    {
        normal = 0,
        italic = 100,
        oblique = 110
    };

    std::string to_string(const font_slants slant);

    enum class font_stretches
    {
        condensed = 75,
        semicondensed = 87,
        normal = 100,
        semiexpanded = 113,
        expanded = 125
    };

    std::string to_string(const font_stretches stretch);
}
