#include <test/mock_surface.hpp>

#include <cdv/elem/rectangle.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    using namespace units_literals;

    TEST_SUITE("rectangle")
    {
        TEST_CASE("draw with default properties")
        {
            const auto r = rectangle{.min = {1_px, 2_px}, .max = {4_px, 5_px}};
            auto result = test::mock_surface();
            draw(r, result, {});
            CHECK_EQ(result.draw_path_counter(), 1);
        }

        TEST_CASE("draw with outline")
        {
            const auto r = rectangle{.min = {1_px, 2_px}, .max = {4_px, 5_px}, .fill = {.outline = {.width = 2_pt}}};
            auto result = test::mock_surface();
            draw(r, result, {});
            CHECK_EQ(result.draw_path_counter(), 2);
        }
    }
}