#pragma once

#include "cdv/core/units.hpp"

namespace cdv::fig
{
    struct frame_dimensions
    {
        pixels width{640};
        pixels height{480};
        dots_per_inch dpi{100};
    };
}