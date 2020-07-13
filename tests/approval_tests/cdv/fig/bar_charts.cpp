#include <framework/approval_tests.hpp>

#include <cdv/core/color/categorical_schemes.hpp>
#include <cdv/core/color/diverging_interpolators.hpp>
#include <cdv/core/color/generated_schemes.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/axis.hpp>
#include <cdv/elem/range_stack.hpp>
#include <cdv/elem/rectangle.hpp>
#include <cdv/elem/swatch_legend.hpp>
#include <cdv/elem/text.hpp>
#include <cdv/fig/frame.hpp>
#include <cdv/fig/render_svg.hpp>
#include <cdv/scl/band_scale.hpp>
#include <cdv/scl/linear_scale.hpp>
#include <cdv/scl/log_scale.hpp>
#include <cdv/scl/ordinal_scale.hpp>

#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/linear_distribute.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

#include <array>

namespace cdv
{
    TEST_CASE("bar charts")
    {
        namespace rv = ranges::views;
        using namespace units_literals;

        constexpr auto domain = std::array{"A", "B", "C", "D", "E", "F", "G", "H"};
        constexpr auto data = std::array{0.1, 0.7, 0.3, 0.4, 0.9, 0.2, 0.3, 0.5};

        // mdinject-begin: example-group-bar-chart-data
        constexpr auto group_domain = std::array{"I", "II", "III", "IV", "V"};
        constexpr auto group_keys = std::array{"A", "B", "C", "D"};
        constexpr auto group_data = std::array{std::pair{group_keys[0], std::array{20, 34, 30, 35, 27}},
                                               std::pair{group_keys[1], std::array{25, 32, 34, 20, 25}},
                                               std::pair{group_keys[2], std::array{17, 26, 24, 27, 32}},
                                               std::pair{group_keys[3], std::array{29, 22, 31, 23, 16}}};
        auto color = scl::ordinal_scale(group_keys, scheme::tableau10);
        // mdinject-end

        SUBCASE("bars")
        {
            const auto frame = fig::frame();
            const auto x =
                scl::band_scale(domain, frame.x0(), frame.x1(), {.inner_padding = 0.1, .outer_padding = 0.1});
            const auto y = scl::linear_scale(0.0, 1.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto bars = rv::zip(domain, data) | rv::transform([&](const auto key_and_value) {
                                  const auto [key, value] = key_and_value;
                                  return elem::rectangle{.min = {x.min(key), y(0.0)}, .max = {x.max(key), y(value)}};
                              });

            test::approve_svg(fig::render_to_svg_string({}, bars, x_axis, y_axis));
        }

        SUBCASE("grouped bars")
        {
            const auto frame = fig::frame();
            const auto x =
                scl::band_scale(group_domain, frame.x0(), frame.x1(), {.inner_padding = 0.2, .outer_padding = 0.1});
            const auto group_x = scl::band_scale(group_keys, 0_px, x.band_width());
            const auto y = scl::linear_scale(0.0, 35.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto bars = rv::cartesian_product(group_domain | rv::enumerate, group_data)
                              | rv::transform([&](auto keys_and_data) {
                                    const auto& [index_and_key, group_key_and_data] = keys_and_data;
                                    const auto& [index, key] = index_and_key;
                                    const auto& [group_key, data] = group_key_and_data;
                                    const auto min = pixel_pos{x.min(key) + group_x.min(group_key), y(0.0)};
                                    const auto max = pixel_pos{min.x + group_x.band_width(), y(data[index])};
                                    return elem::rectangle{.min = min, .max = max, .fill = {.color = color(group_key)}};
                                });

            test::approve_svg(fig::render_to_svg_string({}, bars, x_axis, y_axis));
        }

        SUBCASE("stacked bars")
        {
            const auto frame = fig::frame();
            const auto x =
                scl::band_scale(group_domain, frame.x0(), frame.x1(), {.inner_padding = 0.1, .outer_padding = 0.1});
            const auto y = scl::linear_scale(0.0, 120.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto stacked_data = elem::range_stack(
                group_data | rv::transform([](const auto& p) { return rv::all(p.second); }), group_keys, 0);
            const auto stacks =
                rv::cartesian_product(group_domain | rv::enumerate, group_keys) | rv::transform([&](const auto keys) {
                    const auto [index_and_key, group_key] = keys;
                    const auto [index, key] = index_and_key;
                    const auto [base, top] = stacked_data.layer(group_key)[index];
                    return elem::rectangle{
                        .min = {x.min(key), y(base)}, .max = {x.max(key), y(top)}, .fill = {.color = color(group_key)}};
                });

            test::approve_svg(fig::render_to_svg_string({}, stacks, x_axis, y_axis));
        }

        SUBCASE("hbars")
        {
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 1.0, frame.x0(), frame.x1());
            const auto y =
                scl::band_scale(domain, frame.y0(), frame.y1(), {.inner_padding = 0.1, .outer_padding = 0.1});

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto hbars = rv::zip(domain, data) | rv::transform([&](const auto key_and_value) {
                                   const auto [key, value] = key_and_value;
                                   return elem::rectangle{.min = {x(0.0), y.min(key)}, .max = {x(value), y.max(key)}};
                               });

            test::approve_svg(fig::render_to_svg_string({}, hbars, x_axis, y_axis));
        }

        SUBCASE("grouped hbars")
        {
            // mdinject-begin: example-group-bar-chart
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 35.0, frame.x0(), frame.x1());
            const auto y =
                scl::band_scale(group_domain, frame.y0(), frame.y1(), {.inner_padding = 0.2, .outer_padding = 0.1});
            const auto group_y = scl::band_scale(group_keys | rv::reverse, 0_px, y.band_width());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto hbars = rv::cartesian_product(group_domain | rv::enumerate, group_data)
                               | rv::transform([&](const auto keys_and_data) {
                                     const auto [index_and_key, group_key_and_data] = keys_and_data;
                                     const auto [index, key] = index_and_key;
                                     const auto [group_key, data] = group_key_and_data;
                                     const auto min_y = y.min(key) + group_y.min(group_key);
                                     return elem::rectangle{.min = {x(0.0), min_y},
                                                            .max = {x(data[index]), min_y + group_y.band_width()},
                                                            .fill = {.color = color(group_key)}};
                                 });

            const auto legend = elem::swatch_legend<const char*>{.scale = color, .pos = {frame.x_center(), frame.y1()}};

            const auto svg = fig::render_to_svg_string({}, hbars, x_axis, y_axis, legend);
            // mdinject-end

            test::approve_svg(svg);
        }

        SUBCASE("stacked hbars")
        {
            const auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 120.0, frame.x0(), frame.x1());
            const auto y =
                scl::band_scale(group_domain, frame.y0(), frame.y1(), {.inner_padding = 0.2, .outer_padding = 0.1});

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            const auto stacked_data = elem::range_stack(
                group_data | rv::transform([](const auto& p) { return rv::all(p.second); }), group_keys, 0);
            const auto hbars =
                rv::cartesian_product(group_domain | rv::enumerate, group_keys) | rv::transform([&](const auto keys) {
                    const auto [index_and_key, group_key] = keys;
                    const auto [index, key] = index_and_key;
                    const auto [base, top] = stacked_data.layer(group_key)[index];
                    return elem::rectangle{
                        .min = {x(base), y.min(key)}, .max = {x(top), y.max(key)}, .fill = {.color = color(group_key)}};
                });

            test::approve_svg(fig::render_to_svg_string({}, hbars, x_axis, y_axis));
        }

        SUBCASE("logarithmic bars")
        {
            const auto frame = fig::frame();
            const auto x =
                scl::band_scale(domain, frame.x0(), frame.x1(), {.inner_padding = 0.2, .outer_padding = 0.2});
            const auto y = scl::log_scale(0.001, 1000.0, frame.y0(), frame.y1());

            const auto x_axis = elem::bottom_axis(x, frame.y0());
            const auto y_axis = elem::left_axis(y, frame.x0());

            constexpr auto log_data = std::array{3, 10, 100, 500, 50, 1000, 30, 800};
            const auto bars = rv::zip(domain, log_data) | rv::transform([&](const auto key_and_value) {
                                  const auto [key, value] = key_and_value;
                                  return elem::rectangle{.min = {x.min(key), y(0.001)}, .max = {x.max(key), y(value)}};
                              });

            test::approve_svg(fig::render_to_svg_string({}, bars, x_axis, y_axis));
        }

        SUBCASE("discrete distribution")
        {
            // mdinject-begin: example-bar-chart-distribution
            const auto category_names =
                std::array{"Strongly disagree", "Disagree", "Neither agree nor disagree", "Agree", "Strongly agree"};
            const auto keys = {"Q1", "Q2", "Q3", "Q4", "Q5", "Q6"};
            const auto results =
                elem::range_stack(std::array{std::array{10, 26, 35, 32, 21, 8}, std::array{15, 22, 37, 11, 29, 19},
                                             std::array{17, 29, 7, 9, 5, 5}, std::array{32, 10, 2, 15, 5, 30},
                                             std::array{26, 13, 19, 33, 40, 38}},
                                  category_names, 0);

            const auto frame = fig::frame();
            const auto x = scl::linear_scale(0.0, 100.0, frame.x0(), frame.x1());
            const auto y = scl::band_scale(keys, frame.y0(), frame.y1(), {.inner_padding = 0.2, .outer_padding = 0.2});
            const auto col = scl::ordinal_scale(category_names, scheme::from_interpolator<7>(interpolator::spectral));

            const auto y_axis = elem::left_axis(y, frame.x0(), {.spine = {.width = 0_pt}});

            const auto bars = rv::cartesian_product(category_names, rv::enumerate(keys))
                              | rv::transform([&](const auto categories_and_keys) {
                                    const auto [category, index_and_key] = categories_and_keys;
                                    const auto [index, key] = index_and_key;
                                    const auto [base, top] = results.layer(category)[index];
                                    const auto rgba = col(category).as_doubles();
                                    const auto c = (rgba[0] * rgba[1] * rgba[2]) > 0.4 ? css4::darkgrey : css4::white;
                                    return std::pair{elem::rectangle{.min = {x(base), y.min(key)},
                                                                     .max = {x(top), y.max(key)},
                                                                     .fill = {.color = col(category)}},
                                                     elem::text(fmt::format("{}", top - base),
                                                                {(x(base) + x(top)) * 0.5, y(key)}, {.color = c})};
                                });

            const auto legend = elem::swatch_legend<const char*>{.scale = col, .pos = {frame.x0(), frame.y1()}};

            test::approve_svg(fig::render_to_svg_string({}, bars, y_axis, legend));
            // mdinject-end
        }
    }
}
