#include <test/mock_surface.hpp>

#include <cdv/elem/arc.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    using namespace units_literals;

    TEST_SUITE("arc")
    {
        TEST_CASE("draw empty arc does not throw")
        {
            const auto a = arc{};
            auto result = test::mock_surface();
            CHECK_NOTHROW(draw(a, result, {}));
        }

        TEST_CASE("draw arc without outline draws inner and outer arc")
        {
            const auto a = arc{.outer_radius = 20_px, .inner_radius = 10_px, .start_angle = 1_rad, .end_angle = 2_rad};
            auto result = test::mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.draw_arc_counter(), 2);
        }

        TEST_CASE("draw arc with outline draws inner and outer arc twice")
        {
            const auto a = arc{.outer_radius = 20_px,
                               .inner_radius = 10_px,
                               .start_angle = 1_rad,
                               .end_angle = 2_rad,
                               .fill = {.outline = {.width = 1_pt}}};
            auto result = test::mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.draw_arc_counter(), 4);
        }
    }
}