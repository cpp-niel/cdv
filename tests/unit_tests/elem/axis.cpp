#include <test/mock_surface.hpp>

#include <cdv/elem/axis.hpp>
#include <cdv/scl/linear_scale.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    using namespace units_literals;

    TEST_SUITE("axis")
    {
        TEST_CASE("right axis with default properties")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = right_axis(s, 10_px);
            auto result = test::mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 2);      // one for the spine, one for the ticks
            CHECK_EQ(result.draw_text_counter(), 11);  // labels for 0, 1, 2, ... , 10
        }

        TEST_CASE("left axis with default properties")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = left_axis(s, 10_px);
            auto result = test::mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 2);      // one for the spine, one for the ticks
            CHECK_EQ(result.draw_text_counter(), 11);  // labels for 0, 1, 2, ... , 10
        }

        TEST_CASE("top axis with default properties")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = top_axis(s, 10_px);
            auto result = test::mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 2);      // one for the spine, one for the ticks
            CHECK_EQ(result.draw_text_counter(), 11);  // labels for 0, 1, 2, ... , 10
        }

        TEST_CASE("bottom axis with default properties")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = bottom_axis(s, 10_px);
            auto result = test::mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 2);      // one for the spine, one for the ticks
            CHECK_EQ(result.draw_text_counter(), 11);  // labels for 0, 1, 2, ... , 10
        }

        TEST_CASE("without spine")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = bottom_axis(s, 10_px, {.spine = {.width = 0_pt}});
            auto result = test::mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 1);  // only one for the ticks
        }

        TEST_CASE("with grid")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = bottom_axis(s, 10_px, {.grid_length = 10_px});
            auto result = test::mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 3);  // spine, ticks and grid
        }

        TEST_CASE("without ticks")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = bottom_axis(s, 10_px, {.num_ticks_hint = 0});
            auto result = test::mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.draw_text_counter(), 0);
        }
    }
}