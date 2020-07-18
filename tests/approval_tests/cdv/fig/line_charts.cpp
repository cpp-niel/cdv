#include <framework/approval_tests.hpp>

#include <cdv/core/color/categorical_schemes.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/axis.hpp>
#include <cdv/elem/line.hpp>
#include <cdv/elem/swatch_legend.hpp>
#include <cdv/elem/text.hpp>
#include <cdv/fig/frame.hpp>
#include <cdv/fig/render_svg.hpp>
#include <cdv/scl/linear_scale.hpp>
#include <cdv/scl/log_scale.hpp>
#include <cdv/scl/ordinal_scale.hpp>
#include <cdv/scl/time_scale.hpp>

#include <range/v3/view/generate_n.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/linear_distribute.hpp>
#include <range/v3/view/transform.hpp>

#include <array>
#include <cmath>
#include <functional>
#include <random>

namespace cdv
{
    TEST_CASE("line charts")
    {
        namespace rv = ranges::views;
        using namespace units_literals;

        // mdinject-begin: example-full-chart-no-grid-data
        const auto x1 = rv::linear_distribute(0.0, 2.0, 100);
        const auto x2 = x1 | rv::transform([](auto x) { return x * x; });
        const auto x3 = x1 | rv::transform([](auto x) { return x * x * x; });
        // mdinject-end

        SUBCASE("full chart no grid")
        {
            // mdinject-begin: example-full-chart-no-grid
            constexpr auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 2.0, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(0.0, 8.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto keys = std::array{"$x$", "$x^2$", "$x^3$"};
            const auto color = scl::ordinal_scale(keys, scheme::original_tableau10);
            const auto linear = elem::line(x1 | rv::transform(x), x1 | rv::transform(y), {.color = color(keys[0])});
            const auto quadratic = elem::line(x1 | rv::transform(x), x2 | rv::transform(y), {.color = color(keys[1])});
            const auto cubic = elem::line(x1 | rv::transform(x), x3 | rv::transform(y), {.color = color(keys[2])});

            const auto legend = elem::swatch_legend<const char*>{.scale = color,
                                                                 .pos = {frame.x0() + 20_px, frame.y1() - 20_px},
                                                                 .label_properties = {.font_size = 12_pt},
                                                                 .columns = {200_px}};

            const auto svg =
                fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, linear, quadratic, cubic, legend);
            // mdinject-end

            test::approve_svg(svg);
        }

        SUBCASE("full chart with grid")
        {
            constexpr auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 2.0, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(0.0, 8.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0(), {.grid_length = frame.inner_height()});
            const auto y_axis = elem::left_axis(y, frame.x0(), {.grid_length = frame.inner_width()});

            const auto linear = elem::line(x1 | rv::transform(x), x1 | rv::transform(y), {.color = tab::blue});
            const auto quadratic = elem::line(x1 | rv::transform(x), x2 | rv::transform(y), {.color = tab::orange});
            const auto cubic = elem::line(x1 | rv::transform(x), x3 | rv::transform(y), {.color = tab::green});

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, linear, quadratic, cubic));
        }

        SUBCASE("with horizontal grid")
        {
            constexpr auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 2.0, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(0.0, 8.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(
                x, frame.y0(),
                {.spine = {.width = 0_pt}, .num_ticks_hint = 4, .ticks = {.color = css4::gray, .width = 1_pt}});
            const auto y_axis = elem::left_axis(y, frame.x0(),
                                                {.spine = {.width = 0_pt},
                                                 .tick_length = 0_px,
                                                 .grid_length = frame.inner_width(),
                                                 .tick_label_offset = {10_px, 10_px}});

            const auto linear = elem::line(x1 | rv::transform(x), x1 | rv::transform(y), {.color = tab::blue});
            const auto quadratic = elem::line(x1 | rv::transform(x), x2 | rv::transform(y), {.color = tab::orange});
            const auto cubic = elem::line(x1 | rv::transform(x), x3 | rv::transform(y), {.color = tab::green});

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, linear, quadratic, cubic));
        }

        SUBCASE("logarithmic x axis")
        {
            // mdinject-begin: example-logarithmic-x-axis
            constexpr auto frame = fig::frame();
            const auto x = scl::log_scale(0.01, 100.0, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(-0.05, 1.05, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0(), {.grid_length = frame.inner_height()});
            const auto y_axis =
                elem::left_axis(y, frame.x0(), {.num_ticks_hint = 5, .grid_length = frame.inner_width()});

            const auto xs = rv::linear_distribute(0.1, 10.0, 100) | rv::transform([](auto x) { return x * x; });
            const auto ys = xs | rv::transform([](const double x) { return std::exp(-x / 5.0); });
            const auto line = elem::line(xs | rv::transform(x), ys | rv::transform(y), {.color = cdv_blue});
            const auto text = elem::text(R"($f(x) = e^{\frac{-x}{5}}$)", {450_px, 350_px}, {.font_size = 24_pt});

            const auto svg = fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, line, text);
            // mdinject-end

            test::approve_svg(svg);
        }

        SUBCASE("logarithmic axes with different bases")
        {
            constexpr auto frame = fig::frame();
            const auto x = scl::log_scale(0.01, 100.0, frame.x0(), frame.x1(), {.base = 2.0});
            const auto y = scl::log_scale(0.0001, 100.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0(), {.grid_length = frame.inner_height()});
            const auto y_axis = elem::left_axis(y, frame.x0(), {.grid_length = frame.inner_width()});

            const auto xs = rv::linear_distribute(0.1, 10.0, 100) | rv::transform([](auto x) { return x * x; });
            const auto ys = xs | rv::transform([](const double x) { return 20.0 * std::exp(-x / 10.0); });
            const auto line = elem::line(xs | rv::transform(x), ys | rv::transform(y), {.color = tab::blue});
            const auto text =
                elem::text(R"($f(x) =  20 \times e^{\frac{-x}{10}}$)", {350_px, 350_px}, {.font_size = 24_pt});

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, line, text));
        }

        SUBCASE("with hlines and vlines")
        {
            constexpr auto frame = fig::frame();
            const auto x = scl::linear_scale(-10.0, 10.0, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(-0.05, 1.05, frame.y0(), frame.y1());

            const auto top_line = elem::hline(x.codomain().front(), x.codomain().back(), y(1.0), {.style = "--"});
            const auto mid_line = elem::hline(x.codomain().front(), x.codomain().back(), y(0.5), {.style = ":"});
            const auto bottom_line = elem::hline(x.codomain().front(), x.codomain().back(), y(0.0), {.style = "--"});
            const auto center_line =
                elem::vline(x(0.0), y.codomain().front(), y.codomain().back(), {.color = css4::gray});

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto xs = rv::linear_distribute(x.domain().front(), x.domain().back(), 100);
            const auto ys = xs | rv::transform([](const double x) { return 1.0 / (1.0 + std::exp(-x)); });
            const auto curve = elem::line(xs | rv::transform(x), ys | rv::transform(y),
                                          {.color = scheme::tableau10[0], .width = 3_pt});

            const auto legend = elem::swatch_legend<const char*>{
                .scale = scl::ordinal_scale(std::array{R"($\sigma(x) = \frac{1}{1+e^{-x}}$)"}, scheme::tableau10),
                .pos = {x(4.0), y(0.1)},
                .block_width = 30_px,
                .block_height = 5_px,
                .label_properties = {.font_size = 14_pt}};
            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), top_line, mid_line, bottom_line,
                                                        center_line, curve, x_axis, y_axis, legend));
        }

        SUBCASE("with time axis")
        {
            using namespace date::literals;
            const auto num_months = 27;
            const auto d0 = 2025_y / date::April / 10;
            constexpr auto frame = fig::frame();
            const auto x = scl::time_scale(d0, d0 + date::months{27}, frame.x0(), frame.x1());
            const auto y = scl::linear_scale(0.0, 10.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto xs =
                rv::generate_n([&, i = 0]() mutable { return x.domain()[0] + date::months{i++}; }, num_months)
                | ranges::to_vector;
            std::uniform_real_distribution<double> dist(5.0, 9.0);
            std::mt19937 rng(42);
            const auto rand = [&] { return dist(rng); };
            auto y0s = rv::generate_n(rand, 180) | ranges::to_vector;
            auto y1s = rv::generate_n(rand, 180) | ranges::to_vector;
            auto color = scl::ordinal_scale(std::array{0, 1}, scheme::set1);

            const auto curve = elem::line(xs | rv::transform(x), y0s | rv::transform(y),
                                          {.color = color(0), .width = 1.5_pt, .style = "--"});
            const auto curve1 =
                elem::line(xs | rv::transform(x), y1s | rv::transform(y), {.color = color(1), .width = 3_pt});

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), curve, curve1, x_axis, y_axis));
        }

        SUBCASE("multiple plot with shared x axis")
        {
            // mdinject-begin: example-shared-x-axis
            constexpr auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 2.0, frame.x0(), frame.x1());
            const auto x_axis = elem::bottom_axis(x, frame.y0());

            using function_t = double (*)(double);
            const auto functions = std::array<function_t, 4>{
                [](const auto t) { return sin(2.0 * stdx::numbers::pi * t); }, [](const auto t) { return exp(-t); },
                [](const auto t) { return cos(2.0 * stdx::numbers::pi * t); },
                [](const auto t) { return pow(2.0, t) * 0.25; }};

            const auto plot_height = frame.inner_height() / double(functions.size());
            const auto y_scales = rv::iota(0, std::ssize(functions)) | rv::transform([&](const auto& index) {
                                      const auto miny = frame.y0() + double(index) * plot_height;
                                      const auto minx = (index % 2) ? 0.01 : -0.99;
                                      return scl::linear_scale(minx, 0.99, miny, miny + plot_height);
                                  })
                                  | ranges::to_vector;

            const auto separators =
                y_scales | rv::transform([&](const auto& y) {
                    return elem::hline(x.codomain().front(), x.codomain().back(), y(0.99), {.style = "--"});
                });

            const auto y_axes = y_scales | rv::transform([&](const auto& y) {
                                    return elem::left_axis(y, frame.x0(), {.num_ticks_hint = 4});
                                });

            const auto color = scl::ordinal_scale(functions, scheme::set1);
            const auto xs = rv::linear_distribute(x.domain().front(), x.domain().back(), 60);
            const auto curves = rv::zip(functions, y_scales) | rv::transform([&](const auto& func_and_scale) {
                                    const auto& [f, y] = func_and_scale;
                                    const auto ys = xs | rv::transform(f) | rv::transform(y) | ranges::to_vector;
                                    return elem::line(xs | rv::transform(x), ys, {.color = color(f), .width = 2_pt});
                                });

            const auto svg = fig::render_to_svg_string(frame.dimensions(), x_axis, separators, y_axes, curves);
            // mdinject-end

            test::approve_svg(svg);
        }

        SUBCASE("plot with different y axes")
        {
            constexpr auto frame = fig::frame(640_px, 480_px, 80_px, 50_px, 30_px, 30_px, 100_dpi);
            const auto x = scl::linear_scale(0.0, 10.0, frame.x0(), frame.x1());
            const auto x_axis = elem::bottom_axis(x, frame.y0());

            const auto y0 = scl::linear_scale(0.0, std::exp(x.domain().back()), frame.y0(), frame.y1());
            const auto y1 = scl::linear_scale(-1.0, 1.0, frame.y0(), frame.y1());

            auto color = scl::ordinal_scale(std::array{0, 1}, scheme::dark2);

            const auto y0_axis = elem::left_axis(y0, frame.x0(), {.tick_labels = {.color = color(0)}});
            const auto y1_axis = elem::right_axis(y1, frame.x1(), {.tick_labels = {.color = color(1)}});

            const auto xs = rv::linear_distribute(x.domain().front(), x.domain().back(), 400);
            const auto y0s = xs | rv::transform([](const auto t) { return exp(t); });
            const auto y1s = xs | rv::transform([](const auto t) { return sin(2.0 * stdx::numbers::pi * t); });
            const auto curve0 =
                elem::line(xs | rv::transform(x), y0s | rv::transform(y0), {.color = color(0), .width = 2.5_pt});
            const auto curve1 = elem::line(xs | rv::transform(x), y1s | rv::transform(y1), {.color = color(1)});

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), x_axis, y0_axis, y1_axis, curve0, curve1));
        }

        SUBCASE("getting started")
        {
            // mdinject-begin: getting-started-create-frame
            constexpr auto frame = cdv::fig::frame();
            // mdinject-end

            // mdinject-begin: getting-started-create-scales
            const auto x = cdv::scl::linear_scale(0.0, 2.0, frame.x0(), frame.x1());
            const auto y = cdv::scl::linear_scale(0.0, 4.0, frame.y0(), frame.y1());
            // mdinject-end

            // mdinject-begin: getting-started-create-axes
            const auto x_axis = cdv::elem::bottom_axis(x, frame.y0());
            const auto y_axis = cdv::elem::left_axis(y, frame.x0());
            // mdinject-end

            // mdinject-begin: getting-started-create-line
            namespace rv = ::ranges::views;
            const auto xs = rv::linear_distribute(0.0, 2.0, 100);
            const auto ys = xs | rv::transform([](auto x) { return x * x; });
            const auto curve = cdv::elem::line(xs | rv::transform(x), ys | rv::transform(y));
            // mdinject-end

            // mdinject-begin: getting-started-render-svg
            const auto svg = cdv::fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, curve);
            // mdinject-end

            test::approve_svg(svg);
        }
    }
}