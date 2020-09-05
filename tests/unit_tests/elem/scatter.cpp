#include <test/mock_surface.hpp>

#include <cdv/elem/scatter.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    using namespace units_literals;

    TEST_SUITE("scatter")
    {
        TEST_CASE("draw with default properties")
        {
            const auto s = scatter{.xs = std::array{1_px, 2_px, 3_px}, .ys = std::array{4_px, 5_px, 6_px}};
            auto result = test::mock_surface();
            draw(s, result, {});
            CHECK_EQ(result.draw_arc_counter(), 3);
            CHECK_EQ(result.draw_path_counter(), 0);
        }

        TEST_CASE("draw with line symbol")
        {
            const auto s = scatter{.xs = std::array{1_px, 2_px, 3_px},
                                   .ys = std::array{4_px, 5_px, 6_px},
                                   .properties = symbol_properties{.style = 'x'}};
            auto result = test::mock_surface();
            draw(s, result, {});
            CHECK_EQ(result.draw_arc_counter(), 0);
            CHECK_EQ(result.draw_path_counter(), 6);  // Each stroke of the x is a path => 3 xs means 6 paths
        }

        TEST_CASE("draw with varying sizes")
        {
            const auto s = scatter{.xs = std::array{1_px, 2_px, 3_px},
                                   .ys = std::array{4_px, 5_px, 6_px},
                                   .sizes = std::array{7_pt, 8_pt, 9_pt},
                                   .properties = symbol_properties{.style = '*'}};
            auto result = test::mock_surface();
            draw(s, result, {});
            CHECK_EQ(result.draw_arc_counter(), 0);
            CHECK_EQ(result.draw_path_counter(), 3);  // the outline of each star is a single path
        }
    }
}