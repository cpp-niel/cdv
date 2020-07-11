#include "cdv/scl/ordinal_scale.hpp"

#include <cdv/core/rgba_color.hpp>
#include <cdv/core/units.hpp>
#include <doctest/doctest.h>
#include <range/v3/algorithm/equal.hpp>

namespace cdv::scl
{
    TEST_SUITE("ordinal scale")
    {
        TEST_CASE("construction throws if the codomain is empty")
        {
            CHECK_THROWS((make_ordinal_scale<int>(std::vector<int>())));
        }

        TEST_CASE("application to known domain entries results in valid codomain entries")
        {
            auto s = ordinal_scale(std::vector{1, 2, 3}, std::vector{"A", "B"});
            CHECK_EQ(s(1), "A");
            CHECK_EQ(s(2), "B");
            CHECK_EQ(s(3), "A");
        }

        TEST_CASE("application to unknown domain entries results in valid codomain entries")
        {
            auto s = ordinal_scale(std::vector{1, 2, 3}, std::vector{"A", "B"});
            CHECK_EQ(s(4), "B");
            CHECK_EQ(s(6), "A");
            CHECK_EQ(s(5), "B");
        }

        TEST_CASE("domain value retains its mapping as the domain is extended")
        {
            auto s = ordinal_scale(std::vector{1, 2, 3}, std::vector{"A", "B"});
            CHECK_EQ(s(4), "B");
            CHECK_EQ(s(4), "B");
            CHECK_EQ(s(5), "A");
            CHECK_EQ(s(6), "B");
            CHECK_EQ(s(4), "B");
            CHECK_EQ(s(5), "A");
        }

        TEST_CASE("domain retains insertion order")
        {
            auto s = ordinal_scale(std::vector{1, 2, 3}, std::vector{"A", "B"});
            CHECK_EQ(s(6), "B");
            CHECK_EQ(s(5), "A");
            CHECK_EQ(s(4), "B");
            CHECK(ranges::equal(s.domain(), std::vector{1, 2, 3, 6, 5, 4}));
        }

        TEST_CASE("default result is used for unknown domain values when set")
        {
            auto s = ordinal_scale(std::vector{1, 2, 3}, std::vector{"A", "B"}, {.default_result = "X"});
            CHECK_EQ(s(1), "A");
            CHECK_EQ(s(2), "B");
            CHECK_EQ(s(3), "A");
            CHECK_EQ(s(4), "X");
            CHECK_EQ(s(5), "X");
            CHECK_EQ(s(6), "X");
        }

        TEST_CASE("support for arbitrary types")
        {
            using namespace units_literals;
            auto s = ordinal_scale(std::vector{css4::red, css4::blue, css4::green}, std::vector{30_px, 20_px, 10_px});
            CHECK_EQ(s(css4::blue), 20_px);
            CHECK_EQ(s(css4::yellow), 30_px);
            CHECK_EQ(s(css4::red), 30_px);
            CHECK_EQ(s(css4::cyan), 20_px);
        }
    }
}
