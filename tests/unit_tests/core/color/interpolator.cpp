#include <cdv/core/color/interpolator.hpp>

#include <doctest/doctest.h>

namespace cdv::detail
{
    TEST_SUITE("splinterp for single cubic spline piece")
    {
        TEST_CASE("all control points in a line")
        {
            CHECK_EQ(splinterp(1.0, 2.0, 3.0, 4.0, 0.0), 2.0);
            CHECK_EQ(splinterp(1.0, 2.0, 3.0, 4.0, 0.5), 2.5);
            CHECK_EQ(splinterp(1.0, 2.0, 3.0, 4.0, 1.0), 3.0);
            CHECK_EQ(splinterp(1.0, 2.0, 3.0, 4.0, 1.5), 3.5);
            CHECK_EQ(splinterp(1.0, 2.0, 3.0, 4.0, -0.5), 1.5);
        }

        TEST_CASE("natural cubic curve shape")
        {
            CHECK_EQ(splinterp(0.0, 2.0, 1.0, 4.0, 0.0), 1.5);
            CHECK_EQ(splinterp(0.0, 2.0, 1.0, 4.0, 0.5), ::doctest::Approx(1.52083333));
            CHECK_EQ(splinterp(0.0, 2.0, 1.0, 4.0, 1.0), ::doctest::Approx(1.6666667));
            CHECK_EQ(splinterp(0.0, 2.0, 1.0, 4.0, 1.5), 2.8125);
            CHECK_EQ(splinterp(0.0, 2.0, 1.0, 4.0, -0.5), ::doctest::Approx(0.7291666));
        }
    }

    TEST_SUITE("splinterp for piecewise cubic spline")
    {
        TEST_CASE("t is clamped at 0 and 1")
        {
            constexpr auto control_points = std::array{0.0, 20.0, 10.0, 40.0};
            CHECK_EQ(splinterp(control_points, 0.0), splinterp(control_points, -1.0));
            CHECK_EQ(splinterp(control_points, 1.0), splinterp(control_points, 2.0));
        }

        TEST_CASE("descending control points")
        {
            constexpr auto control_points = std::array{100.0, 20.0, 10.0};
            CHECK_EQ(splinterp(control_points, 0.0), 100.0);
            CHECK_EQ(splinterp(control_points, 0.25), ::doctest::Approx(61.4583333));
            CHECK_EQ(splinterp(control_points, 0.5), ::doctest::Approx(31.6666667));
            CHECK_EQ(splinterp(control_points, 0.75), ::doctest::Approx(16.4583333));
            CHECK_EQ(splinterp(control_points, 1.0), 10.0);
        }

        TEST_CASE("natural cubic curve shape")
        {
            constexpr auto control_points = std::array{0.0, 20.0, 10.0, 40.0};
            CHECK_EQ(splinterp(control_points, 0.0), 0.0);
            CHECK_EQ(splinterp(control_points, 0.25), ::doctest::Approx(12.890625));
            CHECK_EQ(splinterp(control_points, 0.5), ::doctest::Approx(15.20833333));
            CHECK_EQ(splinterp(control_points, 0.75), 20.3125);
            CHECK_EQ(splinterp(control_points, 1.0), 40.0);
        }
    }
}