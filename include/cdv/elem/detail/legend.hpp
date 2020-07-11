#pragma once

#include "cdv/core/units.hpp"

namespace cdv::elem::detail
{
    template <typename Legend, typename Surface>
    void draw_legend_title(const Legend& l, Surface& surface, const pixel_pos& pos)
    {
        if (!l.title.empty())
        {
            surface.set_font_size(l.title_properties.font_size);
            surface.set_color(l.title_properties.color);
            surface.set_font(l.title_properties.font);
            surface.draw_text(l.title, pos + l.title_offset, {0.0, 0.0}, 0.0);
        }
    }
}