#include <framework/approval_tests.hpp>

#include <cdv/core/color/single_hue_interpolators.hpp>
#include <cdv/core/color/perceptually_uniform_interpolators.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/axis.hpp>
#include <cdv/elem/color_legend.hpp>
#include <cdv/elem/rectangle.hpp>
#include <cdv/elem/text.hpp>
#include <cdv/fig/frame.hpp>
#include <cdv/fig/render_svg.hpp>
#include <cdv/scl/linear_scale.hpp>
#include <cdv/scl/ordinal_scale.hpp>

#include <range/v3/view/generate_n.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/transform.hpp>

#include <array>
#include <random>
#include <range/v3/view/cartesian_product.hpp>

namespace cdv
{
    TEST_CASE("heatmaps")
    {
        namespace rv = ranges::views;
        using namespace units_literals;

        SUBCASE("simple heatmap")
        {
            constexpr auto frame = fig::frame();

            const auto num_rows = 10;
            const auto num_cols = 12;

            const auto xs = rv::iota(0, num_cols);
            const auto ys = rv::iota(0, num_rows);

            const auto x = scl::band_scale(xs, frame.x0(), frame.x1());
            const auto y = scl::band_scale(ys, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto color = scl::sequential_scale(0.0, 10.0, interpolator::blues);

            std::uniform_real_distribution<double> dist(0.0, 10.0);
            std::mt19937 rng(42);
            const auto rand = [&] { return dist(rng); };

            const auto values = rv::generate_n(rand, num_rows * num_cols) | ranges::to_vector;

            const auto cells = rv::cartesian_product(xs, ys) | rv::transform([&](const auto keys) {
                                   const auto [xkey, ykey] = keys;
                                   const auto value = values[size_t(ykey * num_cols + xkey)];
                                   return elem::rectangle{.min = {x.min(xkey), y.min(ykey)},
                                                          .max = {x.max(xkey), y.max(ykey)},
                                                          .fill = {.color = color(value)}};
                               });

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), cells, x_axis, y_axis));
        }

        SUBCASE("inferno heatmap")
        {
            constexpr auto frame = fig::frame();

            const auto num_rows = 8;
            const auto num_cols = 10;

            const auto xs = rv::iota(0, num_cols);
            const auto ys = rv::iota(0, num_rows);

            const auto x = scl::band_scale(xs, frame.x0(), frame.x1(), {.inner_padding = 0.08, .outer_padding = 0.08});
            const auto y = scl::band_scale(ys, frame.y0(), frame.y1(), {.inner_padding = 0.08, .outer_padding = 0.08});

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto color = scl::sequential_scale(0.0, 10.0, interpolator::inferno);

            std::uniform_real_distribution<double> dist(0.0, 10.0);
            std::mt19937 rng(42);
            const auto rand = [&] { return dist(rng); };

            const auto values = rv::generate_n(rand, num_rows * num_cols) | ranges::to_vector;

            const auto cells = rv::cartesian_product(xs, ys) | rv::transform([&](const auto keys) {
                                   const auto [xkey, ykey] = keys;
                                   const auto value = values[size_t(ykey * num_cols + xkey)];
                                   return elem::rectangle{.min = {x.min(xkey), y.min(ykey)},
                                                          .max = {x.max(xkey), y.max(ykey)},
                                                          .corner_radius = 4_px,
                                                          .fill = {.color = color(value)}};
                               });

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), cells, x_axis, y_axis));
        }
    }
}