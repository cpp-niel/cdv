#pragma once

#include <cdv/core/vec2.hpp>
#include <cdv/elem/text_properties.hpp>

#include <utility>
#include <range/v3/view/zip_with.hpp>

namespace cdv::elem
{
    struct text
    {
        std::string string;
        pixel_pos pos;
        text_properties properties;
        horizontal_anchor x_anchor = horizontal_anchor::center;
        vertical_anchor y_anchor = vertical_anchor::middle;
        radians rotation;
    };

    template <typename Surface>
    void draw(const text& t, Surface& surface, const pixel_pos&)
    {
        surface.set_text_properties(t.properties);
        surface.draw_text(t.string, t.pos, t.x_anchor, t.y_anchor, t.rotation);
    }
}