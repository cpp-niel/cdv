#include <test/mock_surface.hpp>

#include <cdv/core/color/single_hue_interpolators.hpp>
#include <cdv/elem/swatch_legend.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    using namespace units_literals;

    TEST_SUITE("swatch legend")
    {
        TEST_CASE("with columns")
        {
            const auto s = scl::ordinal_scale(std::array{1, 2, 3}, std::array{css4::red, css4::green, css4::blue});
            const auto l = elem::swatch_legend<int>{.scale = s, .columns = {10_px, 10_px}};
            auto result = test::mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.fill_counter(), 3);       // three swatches
            CHECK_EQ(result.draw_text_counter(), 3);  // three labels
        }

        TEST_CASE("without columns")
        {
            const auto s = scl::ordinal_scale(std::array{1, 2, 3}, std::array{css4::red, css4::green, css4::blue});
            const auto l = elem::swatch_legend<int>{.scale = s};
            auto result = test::mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.fill_counter(), 3);       // three swatches
            CHECK_EQ(result.draw_text_counter(), 3);  // three labels
        }
    }
}