#include "cdv/scl/point_scale.hpp"

#include <cdv/core/rgba_color.hpp>
#include <cdv/core/units.hpp>

#include <doctest/doctest.h>

namespace cdv::scl
{
    TEST_SUITE("point scale")
    {
        TEST_CASE("step is total width when there are no points")
        {
            const auto s = point_scale(std::vector<int>{}, 100.0, 200.0);
            CHECK_EQ(s.step(), 100.0);
        }

        TEST_CASE("step is total width when there is one point")
        {
            const auto s = point_scale(std::vector({3}), 100.0, 200.0);
            CHECK_EQ(s.step(), 100.0);
            CHECK_EQ(s(3), 100.0);
        }

        TEST_CASE("step is total width when there are two points")
        {
            const auto s = point_scale(std::vector({3, 7}), 100.0, 200.0);
            CHECK_EQ(s.step(), 100.0);
            CHECK_EQ(s(3), 100.0);
            CHECK_EQ(s(7), 200.0);
        }

        TEST_CASE("step is total width minus padding when there are two points")
        {
            // When there are two points and the padding is a third of the step then we are
            // essentially splitting the total width into fifths. One fifth is the initial
            // padding, one fifth the final padding and the step in the middle is three times
            // the size of the padding - i.e. three fifths.
            const auto s = point_scale(std::vector({3, 7}), 100.0, 200.0, {.padding = 1.0 / 3.0});
            CHECK_EQ(s.step(), ::doctest::Approx(60.0));
            CHECK_EQ(s(3), ::doctest::Approx(120.0));
            CHECK_EQ(s(7), ::doctest::Approx(180.0));
        }

        TEST_CASE("alignment < 0.5 shifts the points left")
        {
            const auto s =
                point_scale(std::vector({3, 7}), 100.0, 200.0, {.padding = 1.0 / 3.0, .alignment_factor = 0.1});
            CHECK_EQ(s.step(), ::doctest::Approx(60.0));
            CHECK_EQ(s(3), ::doctest::Approx(104.0));
            CHECK_EQ(s(7), ::doctest::Approx(164.0));
        }

        TEST_CASE("alignment > 0.5 shifts the points right")
        {
            const auto s =
                point_scale(std::vector({3, 7}), 100.0, 200.0, {.padding = 1.0 / 3.0, .alignment_factor = 0.9});
            CHECK_EQ(s.step(), ::doctest::Approx(60.0));
            CHECK_EQ(s(3), ::doctest::Approx(136.0));
            CHECK_EQ(s(7), ::doctest::Approx(196.0));
        }

        TEST_CASE("multiple points are spaced out evenly")
        {
            const auto s = point_scale(std::vector({2, 3, 7, 11, 13, 17}), 100.0, 200.0);
            CHECK_EQ(s.step(), 20.0);
            CHECK_EQ(s(2), 100.0);
            CHECK_EQ(s(3), 120.0);
            CHECK_EQ(s(7), 140.0);
            CHECK_EQ(s(11), 160.0);
            CHECK_EQ(s(13), 180.0);
            CHECK_EQ(s(17), 200.0);
        }
    }
}
