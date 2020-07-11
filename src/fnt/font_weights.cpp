#include <cdv/fnt/font_properties.hpp>

namespace cdv::fnt
{
    std::string to_string(const font_weights weight)
    {
        if (weight == font_weights::ultralight) return "ultralight";
        if (weight == font_weights::light) return "light";
        if (weight == font_weights::medium) return "medium";
        if (weight == font_weights::semibold) return "semibold";
        if (weight == font_weights::bold) return "bold";
        if (weight == font_weights::heavy) return "heavy";
        if (weight == font_weights::black) return "black";

        return "normal";
    }

    std::string to_string(const font_slants slant)
    {
        if (slant == font_slants::italic) return "italic";
        if (slant == font_slants::oblique) return "oblique";

        return "normal";
    }

    std::string to_string(const font_stretches stretch)
    {
        if (stretch == font_stretches::condensed) return "condensed";
        if (stretch == font_stretches::semicondensed) return "semicondensed";
        if (stretch == font_stretches::expanded) return "expanded";
        if (stretch == font_stretches::semiexpanded) return "semiexpanded";

        return "normal";
    }
}