#pragma once

#include <cdv/core/rgba_color.hpp>
#include <cdv/core/units.hpp>

#include <string_view>
#include <vector>

namespace cdv::elem
{
    enum class cap_style
    {
        butt,
        square,
        round
    };

    enum class join_style
    {
        miter,
        round,
        bevel
    };

    struct line_type
    {
        line_type() = default;
        line_type(std::string_view s)
        {
            if ((s == "-") || (s == "solid")) {}
            else if ((s == "--") || (s == "dashed"))
            {
                dash_sequence = {3.7, 1.6};
            }
            else if ((s == "-.") || (s == "dashdot"))
            {
                dash_sequence = {6.4, 1.6, 1.0, 1.6};
            }
            else if ((s == ":") || (s == "dotted"))
            {
                dash_sequence = {1, 1.65};
            }
        }

        line_type(const char* s) : line_type(std::string_view(s)) {}

        std::vector<double> dash_sequence;
    };

    struct line_properties
    {
        rgba_color color = css4::black;
        points width{1.0};
        cap_style cap = cap_style::butt;
        join_style join = join_style::miter;
        line_type style{"-"};
    };
}