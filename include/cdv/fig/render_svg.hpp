#pragma once

#include <cdv/back_end/cairo.hpp>
#include <cdv/fig/draw.hpp>
#include <cdv/fig/frame_dimensions.hpp>
#include <cdv/fig/render_surface.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

namespace cdv::fig
{
    template <typename... Elements>
    void render_to_svg_stream(std::ostream& os, const frame_dimensions& dimensions, const Elements&... elements)
    {
        const auto extents = pixel_pos{dimensions.width, dimensions.height};
        render_surface<back_end::cairo> surface(back_end::cairo(extents.x, extents.y, os), dimensions.dpi);
        (draw(elements, surface, extents), ...);
    }

    template <typename... Elements>
    std::string render_to_svg_string(const frame_dimensions& dimensions, const Elements&... elements)
    {
        std::ostringstream os;
        render_to_svg_stream(os, dimensions, elements...);
        return os.str();
    }

    template <typename... Elements>
    void render_svg(const std::string& file_name, const frame_dimensions& dimensions, const Elements&... elements)
    {
        std::ofstream os(file_name);
        render_to_svg_stream(os, dimensions, elements...);
    }
}