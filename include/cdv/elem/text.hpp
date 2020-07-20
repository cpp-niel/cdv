#pragma once

#include <cdv/core/vec2.hpp>
#include <cdv/elem/text_properties.hpp>

#include <utility>
#include <range/v3/view/zip_with.hpp>

namespace cdv::elem
{
    struct text
    {
        text(const std::string& string, const pixel_pos pos, const text_properties& properties = {})
            : string(string), pos(pos), properties(properties)
        {}

        std::string string;
        pixel_pos pos;
        text_properties properties;
    };

    template <typename Surface>
    void draw(const text& t, Surface& surface, const pixel_pos&)
    {
        surface.set_text_properties(t.properties);
        surface.draw_text(t.string, t.pos, {0.5, 0.5}, {});
    }
}