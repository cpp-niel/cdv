#pragma once

#include "cdv/fnt/font_weights.hpp"

#include <fmt/format.h>

#include <string>
#include <string_view>

namespace cdv::fnt
{
    struct font_properties
    {
        std::string family = "sans-serif";
        font_slants slant = font_slants::normal;
        font_weights weight = font_weights::normal;
        font_stretches stretch = font_stretches::normal;
    };
}

template <>
struct fmt::formatter<cdv::fnt::font_properties> : formatter<string_view>
{
    template <typename FormatContext>
    auto format(cdv::fnt::font_properties c, FormatContext& ctx)
    {
        return formatter<string_view>::format(fmt::format("{}[slant={}, weight={}, stretch={}]", c.family,
                                                          to_string(c.slant), to_string(c.weight), to_string(c.stretch)),
                                                          ctx);
    }
};