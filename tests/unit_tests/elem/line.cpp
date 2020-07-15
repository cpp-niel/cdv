#include <test/mock_surface.hpp>

#include <cdv/elem/line.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    using namespace units_literals;

    TEST_SUITE("line")
    {
        TEST_CASE("draw empty line does not throw")
        {
            const auto l = line(std::vector<pixels>{}, std::vector<pixels>{}, {});
            auto result = test::mock_surface();
            CHECK_NOTHROW(draw(l, result, {}));
        }

        TEST_CASE("draw with default properties")
        {
            const auto l = line(std::vector{1_px, 2_px, 3_px}, std::vector{4_px, 5_px, 6_px}, {});
            auto result = test::mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.draw_path_counter(), 1);
        }

        TEST_CASE("draw dashed")
        {
            const auto l = line(std::vector{1_px, 2_px, 3_px}, std::vector{4_px, 5_px, 6_px}, {.style = "--"});
            auto result = test::mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.draw_path_counter(), 1);
        }

        TEST_CASE("draw dash dotted")
        {
            const auto l = line(std::vector{1_px, 2_px, 3_px}, std::vector{4_px, 5_px, 6_px}, {.style = "-."});
            auto result = test::mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.draw_path_counter(), 1);
        }

        TEST_CASE("draw hline with default properties")
        {
            const auto l = hline(1_px, 2_px, 3_px);
            auto result = test::mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.draw_path_counter(), 1);
        }

        TEST_CASE("draw vline with default properties")
        {
            const auto l = vline(1_px, 2_px, 3_px);
            auto result = test::mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.draw_path_counter(), 1);
        }
    }
}