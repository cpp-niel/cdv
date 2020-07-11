#include "framework/approval_tests.hpp"

#include "cdv/core/color/categorical_schemes.hpp"
#include "cdv/core/color/perceptually_uniform_interpolators.hpp"
#include "cdv/core/rgba_color.hpp"
#include "cdv/core/vec2.hpp"
#include "cdv/elem/axis.hpp"
#include "cdv/elem/line.hpp"
#include "cdv/elem/rectangle.hpp"
// mdinject-begin: tutorial-hello-world-include
#include "cdv/elem/text.hpp"
#include "cdv/fig/frame.hpp"
#include "cdv/fig/render_svg.hpp"
// mdinject-end
#include "cdv/scl/band_scale.hpp"
#include "cdv/scl/linear_scale.hpp"
#include "cdv/scl/ordinal_scale.hpp"
#include "cdv/scl/sequential_scale.hpp"

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

TEST_CASE("hello world")
{
    // mdinject-begin: tutorial-hello-world
    constexpr auto frame = cdv::fig::frame();
    const auto message = cdv::elem::text("Hello, world", frame.center(), {.font_size = cdv::points{36}});
    const auto svg = cdv::fig::render_to_svg_string(frame.dimensions(), message);
    // mdinject-end

    cdv::test::approve_svg(svg);
}

TEST_CASE("hello world units")
{
    // mdinject-begin: tutorial-hello-world-units
    using namespace cdv::units_literals;
    constexpr auto frame = cdv::fig::frame();
    const auto message = cdv::elem::text("Hello, units", {200_px, 300_px}, {.font_size = 36_pt});
    const auto svg = cdv::fig::render_to_svg_string(frame.dimensions(), message);
    // mdinject-end

    cdv::test::approve_svg(svg);
}

TEST_CASE("scales and axes")
{
    using namespace cdv::units_literals;
    constexpr auto frame = cdv::fig::frame();

    // mdinject-begin: tutorial-create-scales
    const auto x = cdv::scl::linear_scale(0.0, 10.0, frame.x0(), frame.x1());
    const auto y = cdv::scl::linear_scale(0.0, 10.0, frame.y0(), frame.y1());
    // mdinject-end

    // mdinject-begin: tutorial-create-axes
    const auto x_axis = cdv::elem::bottom_axis(x, frame.y0());
    const auto y_axis = cdv::elem::left_axis(y, frame.x0());
    // mdinject-end

    // mdinject-begin: tutorial-scales-and-axes
    const auto message = cdv::elem::text("Scales & Axes", {x(4.0), y(7.0)}, {.font_size = 36_pt});
    const auto svg = cdv::fig::render_to_svg_string(frame.dimensions(), message, x_axis, y_axis);
    // mdinject-end

    cdv::test::approve_svg(svg);
}

TEST_CASE("line plot")
{
    using namespace cdv::units_literals;
    constexpr auto frame = cdv::fig::frame();

    // mdinject-begin: tutorial-line-plot-define-data
    const auto data = std::vector{4.0, 7.0, 3.2, 5.9, 9.7, 5.0, 8.2, 9.1, 7.0, 4.3, 6.5};
    // mdinject-end

    // mdinject-begin: tutorial-line-plot-scales-and-axes
    const auto x = cdv::scl::linear_scale(0.0, 10.0, frame.x0(), frame.x1());
    const auto y = cdv::scl::linear_scale(0.0, 10.0, frame.y0(), frame.y1());
    const auto x_axis = cdv::elem::bottom_axis(x, frame.y0());
    const auto y_axis = cdv::elem::left_axis(y, frame.x0());
    // mdinject-end

    // mdinject-begin: tutorial-line-plot
    namespace rv = ::ranges::views;
    const auto line = cdv::elem::line(rv::iota(0, 11) | rv::transform(x), data | rv::transform(y));
    const auto svg = cdv::fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, line);
    // mdinject-end

    cdv::test::approve_svg(svg);
}

TEST_CASE("line plot colors")
{
    // mdinject-begin: tutorial-colors
    using namespace cdv::units_literals;
    constexpr auto frame = cdv::fig::frame();

    const auto data = std::vector{4.0, 7.0, 3.2, 5.9, 9.7, 5.0, 8.2, 9.1, 7.0, 4.3, 6.5};

    const auto x = cdv::scl::linear_scale(0.0, 10.0, frame.x0(), frame.x1());
    const auto y = cdv::scl::linear_scale(0.0, 10.0, frame.y0(), frame.y1());
    const auto x_axis = cdv::elem::bottom_axis(x, frame.y0());
    const auto y_axis = cdv::elem::left_axis(y, frame.x0());

    namespace rv = ::ranges::views;
    const auto line = cdv::elem::line(rv::iota(0, 11) | rv::transform(x), data | rv::transform(y),
                                      {.color = cdv::css4::dodgerblue, .width = 2.5_pt, .style = "--"});
    const auto svg = cdv::fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, line);
    // mdinject-end

    cdv::test::approve_svg(svg);
}

TEST_CASE("bar chart")
{
    using namespace cdv::units_literals;
    namespace rv = ::ranges::views;
    constexpr auto frame = cdv::fig::frame();

    // mdinject-begin: tutorial-bar-chart-define-data
    const auto keys = rv::iota(0, 11);
    const auto data = std::vector{4.0, 7.0, 3.2, 5.9, 9.7, 5.0, 8.2, 9.1, 7.0, 4.3, 6.5};
    // mdinject-end

    // mdinject-begin: tutorial-bar-chart-band-scale
    const auto x = cdv::scl::band_scale(keys, frame.x0(), frame.x1());
    // mdinject-end

    // mdinject-begin: tutorial-bar-chart-scale-and-axes
    const auto y = cdv::scl::linear_scale(0.0, 10.0, frame.y0(), frame.y1());
    const auto x_axis = cdv::elem::bottom_axis(x, frame.y0());
    const auto y_axis = cdv::elem::left_axis(y, frame.x0());
    // mdinject-end

    // mdinject-begin: tutorial-bar-chart-create-bars
    const auto bars = rv::zip(keys, data) | rv::transform([&](const auto& key_value_pair) {
                          const auto [key, value] = key_value_pair;
                          return cdv::elem::rectangle{.min = {x.min(key), y(0.0)}, .max = {x.max(key), y(value)}};
                      });
    // mdinject-end

    // mdinject-begin: tutorial-bar-chart-render-bars
    const auto svg = cdv::fig::render_to_svg_string({}, bars, x_axis, y_axis);
    // mdinject-end

    cdv::test::approve_svg(svg);
}

TEST_CASE("bar chart colors")
{
    using namespace cdv::units_literals;
    namespace rv = ::ranges::views;
    constexpr auto frame = cdv::fig::frame();

    const auto keys = rv::iota(0, 11);
    const auto data = std::vector{4.0, 7.0, 3.2, 5.9, 9.7, 5.0, 8.2, 9.1, 7.0, 4.3, 6.5};

    const auto x = cdv::scl::band_scale(keys, frame.x0(), frame.x1());
    const auto y = cdv::scl::linear_scale(0.0, 10.0, frame.y0(), frame.y1());
    const auto x_axis = cdv::elem::bottom_axis(x, frame.y0());
    const auto y_axis = cdv::elem::left_axis(y, frame.x0());

    // mdinject-begin: tutorial-bar-chart-create-color-scheme
    const auto color = cdv::scl::ordinal_scale(keys, cdv::scheme::pastel1);
    // mdinject-end

    // mdinject-begin: tutorial-bar-chart-create-colored-bars
    const auto bars = rv::zip(keys, data) | rv::transform([&](const auto& key_value_pair) {
      const auto [key, value] = key_value_pair;
      return cdv::elem::rectangle{.min = {x.min(key), y(0.0)},
          .max = {x.max(key), y(value)},
          .fill = {.color = color(key)}};
    });
    // mdinject-end

    const auto svg = cdv::fig::render_to_svg_string({}, bars, x_axis, y_axis);

    cdv::test::approve_svg(svg);
}

TEST_CASE("bar chart more colors")
{
    using namespace cdv::units_literals;
    namespace rv = ::ranges::views;
    constexpr auto frame = cdv::fig::frame();

    const auto keys = rv::iota(0, 11);
    const auto data = std::vector{4.0, 7.0, 3.2, 5.9, 9.7, 5.0, 8.2, 9.1, 7.0, 4.3, 6.5};

    const auto x = cdv::scl::band_scale(keys, frame.x0(), frame.x1());
    const auto y = cdv::scl::linear_scale(0.0, 10.0, frame.y0(), frame.y1());
    const auto x_axis = cdv::elem::bottom_axis(x, frame.y0());
    const auto y_axis = cdv::elem::left_axis(y, frame.x0());

    // mdinject-begin: tutorial-bar-chart-create-color-interpolator
    const auto color = cdv::scl::sequential_scale(4.0, 10.0, cdv::interpolator::viridis);
    // mdinject-end

    // mdinject-begin: tutorial-bar-chart-create-more-colored-bars
    const auto bars = rv::zip(keys, data) | rv::transform([&](const auto& key_value_pair) {
                          const auto [key, value] = key_value_pair;
                          return cdv::elem::rectangle{.min = {x.min(key), y(0.0)},
                                                      .max = {x.max(key), y(value)},
                                                      .fill = {.color = color(value)}};
                      });
    // mdinject-end

    const auto svg = cdv::fig::render_to_svg_string({}, bars, x_axis, y_axis);

    cdv::test::approve_svg(svg);
}