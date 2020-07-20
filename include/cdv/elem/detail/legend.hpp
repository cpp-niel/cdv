#pragma once

#include <cdv/core/units.hpp>

namespace cdv::elem::detail
{
    template <typename Legend, typename Surface>
    void draw_legend_title(const Legend& l, Surface& surface, const pixel_pos& pos)
    {
        if (!l.title.empty())
        {
            surface.set_text_properties(l.title_properties);
            surface.draw_text(l.title, pos + l.title_offset, {0.0, 0.0}, {});
        }
    }
}