#include <cdv/fnt/font_weights.hpp>

#include <doctest/doctest.h>

namespace cdv::fnt
{
    TEST_SUITE("font weights")
    {
        TEST_CASE("weights")
        {
            CHECK_EQ(to_string(font_weights::normal), "normal");
            CHECK_EQ(to_string(font_weights::semibold), "semibold");
        }

        TEST_CASE("slants")
        {
            CHECK_EQ(to_string(font_slants::normal), "normal");
            CHECK_EQ(to_string(font_slants::italic), "italic");
        }

        TEST_CASE("stretches")
        {
            CHECK_EQ(to_string(font_stretches::normal), "normal");
            CHECK_EQ(to_string(font_stretches::expanded), "expanded");
        }
    }
}