#include "framework/approval_tests.hpp"

#include "cdv/core/color/categorical_schemes.hpp"
#include "cdv/core/vec2.hpp"
#include "cdv/elem/area.hpp"
#include "cdv/elem/axis.hpp"
#include "cdv/elem/color_legend.hpp"
#include "cdv/elem/text.hpp"
#include "cdv/fig/frame.hpp"
#include "cdv/fig/render_svg.hpp"
#include "cdv/scl/linear_scale.hpp"
#include "cdv/scl/ordinal_scale.hpp"

#include <range/v3/view/linear_distribute.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/transform.hpp>

#include <array>
#include <cdv/elem/range_stack.hpp>

namespace cdv
{
    TEST_CASE("area charts")
    {
        namespace rv = ranges::views;
        using namespace units_literals;

        constexpr auto ys = std::array{0.1, 0.7, 0.3, 0.4, 0.9, 0.2, 0.3, 0.5, 0.8};
        constexpr auto xs = rv::linear_distribute(0.0, 8.0, 9);

        SUBCASE("fill between zero")
        {
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 8.0, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(0.0, 1.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto area =
                elem::fill_between(xs | rv::transform(x), ys | rv::transform(y), y(0.0), {.color = tab::green});

            std::ostringstream os;
            fig::render_to_svg_stream(os, {}, area, x_axis, y_axis);
            test::approve_svg(os.str());
        }

        SUBCASE("fill between arbitrary constant")
        {
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 8.0, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(0.0, 1.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto area =
                elem::fill_between(xs | rv::transform(x), ys | rv::transform(y), y(0.4), {.color = tab::pink});

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), area, x_axis, y_axis));
        }

        SUBCASE("fill between data sets")
        {
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 8.0, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(0.0, 1.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            constexpr auto bases = std::array{0.0, 0.2, 0.2, 0.3, 0.6, 0.1, 0.2, 0.3, 0.5};
            const auto area = elem::fill_between(
                xs | rv::transform(x), ys | rv::transform(y), bases | rv::transform(y),
                {.color = tab::blue.with_alpha(0.5), .outline = {.color = tab::blue, .width = 1_pt}});

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), area, x_axis, y_axis));
        }

        SUBCASE("stacked fill between")
        {
            // mdinject-begin: example-stacked-area-chart
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 8.0, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(0.0, 4.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            constexpr auto x_values = rv::linear_distribute(0.0, 8.0, 9);

            constexpr auto keys = std::array{"<10", "10-20", "20-30", "30-40", "40-50"};
            const auto data = elem::range_stack(std::array{std::array{0.1, 0.7, 0.3, 0.4, 0.9, 0.2, 0.3, 0.5, 0.8},
                                                           std::array{0.3, 0.2, 0.5, 0.0, 0.4, 0.5, 0.3, 0.5, 0.4},
                                                           std::array{0.4, 0.2, 0.7, 0.9, 0.5, 0.4, 0.2, 0.9, 0.3},
                                                           std::array{0.5, 0.7, 0.9, 0.3, 0.7, 0.1, 0.7, 0.2, 0.7},
                                                           std::array{0.8, 0.4, 0.4, 0.5, 0.6, 0.6, 0.2, 0.8, 0.2}},
                                                keys, 0.0);

            const auto color = scl::ordinal_scale(keys, scheme::tableau10);
            const auto areas =
                keys | rv::transform([&](const auto& key) {
                    const auto& layer = data.layer(key);
                    return elem::fill_between(x_values | rv::transform(x), rv::keys(layer) | rv::transform(y),
                                              rv::values(layer) | rv::transform(y), {.color = color(key)});
                });

            const auto legend = elem::color_legend<decltype(color)>{
                .scale = color, .pos = {370_px, 420_px}, .width = 250_px, .height = 25_px, .block_height = 10_px};

            const auto svg = fig::render_to_svg_string(frame.dimensions(), areas, x_axis, y_axis, legend);
            // mdinject-end

            test::approve_svg(svg);
        }
    }
}