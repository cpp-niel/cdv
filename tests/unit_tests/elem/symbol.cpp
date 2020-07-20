#include <test/mock_surface.hpp>

#include <cdv/elem/symbol.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    using namespace units_literals;

    TEST_SUITE("symbol")
    {
        TEST_CASE("draw with default properties")
        {
            const auto s = symbol{.position = {1_px, 2_px}};
            auto result = test::mock_surface();
            draw(s, result, {});
            CHECK_EQ(result.draw_arc_counter(), 1);
            CHECK_EQ(result.draw_path_counter(), 0);
        }

        TEST_CASE("draw with line symbol")
        {
            const auto s = symbol{.position = {1_px, 2_px}, .properties = {.style = '+'}};
            auto result = test::mock_surface();
            draw(s, result, {});
            CHECK_EQ(result.draw_arc_counter(), 0);
            CHECK_EQ(result.draw_path_counter(), 2);  // Each of the two strokes of the + is a path
        }
    }
}