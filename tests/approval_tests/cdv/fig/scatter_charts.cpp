#include <framework/approval_tests.hpp>

#include <cdv/core/color/perceptually_uniform_interpolators.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/axis.hpp>
#include <cdv/elem/color_legend.hpp>
#include <cdv/elem/line.hpp>
#include <cdv/elem/scatter.hpp>
#include <cdv/elem/symbol.hpp>
#include <cdv/fig/frame.hpp>
#include <cdv/fig/render_svg.hpp>
#include <cdv/scl/linear_scale.hpp>
#include <cdv/scl/sequential_scale.hpp>

#include <range/v3/view/all.hpp>
#include <range/v3/view/generate_n.hpp>
#include <range/v3/view/linear_distribute.hpp>
#include <range/v3/view/transform.hpp>

#include <array>
#include <random>

namespace cdv
{
    TEST_CASE("scatter charts")
    {
        namespace rv = ranges::views;
        using namespace units_literals;

        SUBCASE("simple random scatter")
        {
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(-0.1, 4.1, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(-0.1, 4.1, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            std::uniform_real_distribution<double> dist(0.0, 4.0);
            std::mt19937 rng(42);
            const auto rand = [&] { return dist(rng); };

            auto xs = rv::generate_n(rand, 50) | ranges::to_vector;
            auto ys = rv::generate_n(rand, 50) | ranges::to_vector;

            const auto scatter =
                elem::scatter(xs | rv::transform(x), ys | rv::transform(y), {.color = tab::blue, .style = 'X'});

            test::approve_svg(fig::render_to_svg_string({}, x_axis, y_axis, scatter));
        }

        SUBCASE("with alpha and sizes")
        {
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(-0.05, 1.05, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(-0.05, 1.05, frame.y0(), frame.y1());

            const auto x_axis =
                elem::bottom_axis(x, frame.y0(), {.grid_length = frame.inner_height()});
            const auto y_axis = elem::left_axis(y, frame.x0(), {.grid_length = frame.inner_width()});

            std::uniform_real_distribution<double> dist(0.0, 1.0);
            std::mt19937 rng(42);
            const auto rand = [&] { return dist(rng); };

            const auto create_scatter = [&](const auto color) {
                auto xs = rv::generate_n(rand, 150) | rv::transform(x) | ranges::to_vector;
                auto ys = rv::generate_n(rand, 150) | rv::transform(y) | ranges::to_vector;
                auto sizes = rv::generate_n(rand, 150) | rv::transform([](const double x) { return x * 24_pt; })
                             | ranges::to_vector;
                return elem::scatter(xs, ys, sizes, {.color = rgba_color(color).with_alpha(0.3)});
            };

            const auto colors = std::vector{"tab:blue", "tab:orange", "tab:green"};
            const auto scatters = colors | rv::transform(create_scatter);

            test::approve_svg(fig::render_to_svg_string({}, x_axis, y_axis, scatters));
        }

        SUBCASE("with varying colors")
        {
            // mdinject-begin: example-scatter-plot
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(-0.1, 4.1, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(-0.1, 4.1, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            std::uniform_real_distribution<double> dist(0.0, 4.0);
            std::mt19937 rng(42);
            const auto rand = [&] { return dist(rng); };

            const auto xs = rv::generate_n(rand, 180) | ranges::to_vector;
            const auto ys = rv::generate_n(rand, 180) | ranges::to_vector;

            const auto color = scl::sequential_scale(0.0, 3.5, interpolator::plasma);

            const auto scatter = rv::zip_with(
                [&](const auto a, const auto b) {
                    const auto c = std::sqrt((a - 3.0) * (a - 3.0) + (b - 2.0) * (b - 2.0));
                    return elem::symbol{.position = pixel_pos{x(a), y(b)},
                                        .properties = {.color = color(c), .style = 'P'}};
                },
                xs, ys);

            const auto legend = elem::color_legend<decltype(color)>{
                .scale = color, .pos = {220_px, 450_px}, .width = 400_px, .height = 24_px, .block_height = 12_px};

            const auto svg = fig::render_to_svg_string({}, x_axis, y_axis, scatter, legend);
            // mdinject-end

            test::approve_svg(svg);
        }

        SUBCASE("stem plot")
        {
            // mdinject-begin: example-stem-plot
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 2.0 * stdx::numbers::pi, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(0.0, 2.75, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto xs = rv::linear_distribute(0.1, x.domain()[1], 41);
            const auto ys = xs | rv::transform([](const auto value) { return std::exp(std::sin(value)); });

            const auto stems = rv::zip_with(
                [&](const auto a, const auto b) {
                    const auto pos = pixel_pos{x(a), y(b)};
                    return std::pair{elem::symbol{.position = pos, .properties = {.color = tab::blue}},
                               elem::line(std::array{pos.x, pos.x}, std::array{y(0.0), pos.y}, {.color = tab::blue})};
                },
                xs, ys);

            const auto svg = fig::render_to_svg_string({}, stems, x_axis, y_axis);
            // mdinject-end

            test::approve_svg(svg);
        }
    }
}