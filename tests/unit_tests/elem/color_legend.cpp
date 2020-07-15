#include <test/mock_surface.hpp>

#include <cdv/core/color/single_hue_interpolators.hpp>
#include <cdv/elem/color_legend.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    using namespace units_literals;

    TEST_SUITE("color legend")
    {
        TEST_CASE("with ordinal scale")
        {
            const auto s = scl::ordinal_scale(std::array{1, 2, 3}, std::array{css4::red, css4::green, css4::blue});
            const auto l = elem::color_legend<decltype(s)>{.scale = s};
            auto result = test::mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.fill_counter(), 3);       // three blocks
            CHECK_EQ(result.draw_text_counter(), 3);  // three labels
        }

        TEST_CASE("with sequential scale")
        {
            const auto s = scl::sequential_scale(0.0, 1.0, interpolator::reds);
            const auto l = elem::color_legend<decltype(s)>{.scale = s};
            auto result = test::mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.gradient_fill_counter(), 1);  // the gradient filled block
            CHECK_EQ(result.draw_path_counter(), 6);  // six ticks
            CHECK_EQ(result.draw_text_counter(), 6);  // six labels
        }
    }
}