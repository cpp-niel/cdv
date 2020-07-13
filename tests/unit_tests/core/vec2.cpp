#include <cdv/core/vec2.hpp>

#include <doctest/doctest.h>

namespace cdv::core
{
    TEST_SUITE("vec2")
    {
        TEST_CASE("default construction")
        {
            const auto v = vec2<int>();
            CHECK_EQ(v.x, 0);
            CHECK_EQ(v.y, 0);
        }
    }
}