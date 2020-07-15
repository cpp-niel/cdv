#pragma once

#include <cdv/core/vec2.hpp>

namespace cdv::elem::detail
{
    template <typename Surface>
    void draw_rectangle(Surface& surface, const pixel_pos min, const pixel_pos max)
    {
        surface.draw_path({min, {max.x, min.y}, max, {min.x, max.y}, min});
        surface.stroke();
    }

    template <typename Surface>
    void fill_rectangle(Surface& surface, const pixel_pos min, const pixel_pos max)
    {
        surface.draw_path({min, {max.x, min.y}, max, {min.x, max.y}, min});
        surface.fill();
    }
}