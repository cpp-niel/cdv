#include <cdv/fnt/font_properties.hpp>

#include <doctest/doctest.h>
#include <fmt/format.h>

namespace cdv::fnt
{
    TEST_SUITE("font propoerties")
    {
        TEST_CASE("string formatting")
        {
            CHECK_EQ(fmt::format("{}", font_properties()), "sans-serif[slant=normal, weight=normal, stretch=normal]");
        }
    }
}
