#include "cdv/scl/sequential_scale.hpp"

#include <doctest/doctest.h>

namespace cdv::scl
{
    TEST_SUITE("sequential scale")
    {
        TEST_CASE("uses interpolator to generate codomain")
        {
            constexpr auto s = sequential_scale(0.0, 1.0, [](const auto x) { return static_cast<int>(x * 100); });
            CHECK_EQ(s(0.0), 0);
            CHECK_EQ(s(0.03), 3);
            CHECK_EQ(s(0.3), 30);
            CHECK_EQ(s(0.3333), 33);
            CHECK_EQ(s(0.99), 99);
            CHECK_EQ(s(1.0), 100);
            CHECK_EQ(s(1.01), 101);
            CHECK_EQ(s(-0.01), -1);
        }

        TEST_CASE("application is constexpr")
        {
            constexpr auto s = sequential_scale(0.0, 1.0, [](const auto x) { return static_cast<int>(x * 100); });
            CHECK_EQ(std::array<int, s(0.03)>().size(), 3);
        }
    }
}