#include <cdv/fig/frame.hpp>

#include <doctest/doctest.h>

namespace cdv::fig
{
    using namespace ::cdv::units_literals;

    TEST_SUITE("frame")
    {
        TEST_CASE("default")
        {
            constexpr auto f = frame();
            CHECK_EQ(f.width, 640_px);
            CHECK_EQ(f.height, 480_px);
            CHECK_EQ(f.x0(), 40_px);
            CHECK_EQ(f.x1(), 620_px);
            CHECK_EQ(f.y0(), 30_px);
            CHECK_EQ(f.y1(), 450_px);
            CHECK_EQ(f.x_center(), 330_px);  // not 320 because left margin is bigger!
            CHECK_EQ(f.y_center(), 240_px);
        }

        TEST_CASE("with explicit margins")
        {
            constexpr auto f = frame{.left_margin = 60_px};
            CHECK_EQ(f.width, 640_px);
            CHECK_EQ(f.x0(), 60_px);
            CHECK_EQ(f.y1(), 450_px);
        }

        TEST_CASE("with fractional margins")
        {
            constexpr auto f = frame{.left_margin = 10_percent, .top_margin = 50_percent};
            CHECK_EQ(f.width, 640_px);
            CHECK_EQ(f.x0(), 64_px);
            CHECK_EQ(f.y1(), 240_px);
        }

        TEST_CASE("with fractional margin and non default width")
        {
            constexpr auto f = frame{.width = 1000_px, .left_margin = 10_percent};
            CHECK_EQ(f.width, 1000_px);
            CHECK_EQ(f.x0(), 100_px);
        }
    }
}
