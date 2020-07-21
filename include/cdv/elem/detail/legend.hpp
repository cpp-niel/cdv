#pragma once

#include <cdv/core/units.hpp>
#include <cdv/elem/text_properties.hpp>

namespace cdv::elem::detail
{
    template <typename Legend, typename Surface>
    void draw_legend_title(const Legend& l, Surface& surface, const pixel_pos& pos)
    {
        if (!l.title.empty())
        {
            surface.set_text_properties(l.title_properties);
            surface.draw_text(l.title, pos + l.title_offset, horizontal_anchor::left, vertical_anchor::bottom, {});
        }
    }
}