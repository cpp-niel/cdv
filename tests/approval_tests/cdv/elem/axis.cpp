#include <framework/approval_tests.hpp>

#include <cdv/core/vec2.hpp>
#include <cdv/elem/axis.hpp>
#include <cdv/elem/line.hpp>
#include <cdv/elem/text.hpp>
#include <cdv/fig/frame.hpp>
#include <cdv/fig/render_svg.hpp>
#include <cdv/scl/band_scale.hpp>
#include <cdv/scl/linear_scale.hpp>
#include <cdv/scl/log_scale.hpp>
#include <cdv/scl/time_scale.hpp>

#include <array>

namespace cdv::elem
{
    using namespace units_literals;

    TEST_SUITE("axis")
    {
        TEST_CASE("all orientations")
        {
            const auto left = left_axis(scl::linear_scale(0.0, 2.0, 150_px, 430_px), 100_px);
            const auto right = right_axis(scl::linear_scale(0.0, 2.0, 150_px, 430_px), 500_px);
            const auto top = top_axis(scl::linear_scale(0.0, 2.0, 120_px, 450_px), 400_px);
            const auto bottom = bottom_axis(scl::linear_scale(0.0, 2.0, 50_px, 510_px), 100_px);
            test::approve_svg(fig::render_to_svg_string({}, left, right, top, bottom));
        }

        TEST_CASE("with grid")
        {
            const auto x_axis =
                bottom_axis(scl::linear_scale(0.0, 2.0, 100_px, 500_px), 150_px, {.grid_length = 300_px});
            const auto y_axis = left_axis(scl::linear_scale(0.0, 2.0, 150_px, 450_px), 100_px,
                                          {.num_ticks_hint = 4, .grid_length = 400_px});
            test::approve_svg(fig::render_to_svg_string({}, x_axis, y_axis));
        }

        TEST_CASE("with log scale")
        {
            const auto x_axis = bottom_axis(scl::log_scale(0.001, 1000.0, 100_px, 500_px), 150_px);
            test::approve_svg(fig::render_to_svg_string({}, x_axis));
        }

        TEST_CASE("with band scale")
        {
            const auto x_axis =
                bottom_axis(scl::band_scale(std::vector{"A", "B", "C"}, 100_px, 500_px), 150_px,
                            {.tick_labels = {.color = css4::firebrick,
                                             .font = {.family = "times", .weight = fnt::font_weights::bold},
                                             .font_size = 24_pt}});
            test::approve_svg(fig::render_to_svg_string({}, x_axis));
        }

        TEST_CASE("with time scale")
        {
            using namespace date::literals;
            using namespace std::chrono_literals;
            const auto year_axis = bottom_axis(
                scl::time_scale(2026_y / date::October / 10, 2034_y / date::June / 10, 40_px, 600_px), 20_px);
            const auto quarter_axis = bottom_axis(
                scl::time_scale(2020_y / date::January / 01, 2023_y / date::January / 01, 40_px, 600_px), 60_px);
            const auto month_axis = bottom_axis(
                scl::time_scale(2020_y / date::October / 10, 2021_y / date::June / 10, 40_px, 600_px), 100_px);
            const auto week_axis = bottom_axis(
                scl::time_scale(2020_y / date::October / 10, 2020_y / date::December / 10, 40_px, 600_px), 140_px);
            const auto two_day_axis = bottom_axis(
                scl::time_scale(2020_y / date::October / 01, 2020_y / date::October / 20, 40_px, 600_px), 180_px);
            const auto day_axis = bottom_axis(
                scl::time_scale(2020_y / date::October / 10, 2020_y / date::October / 20, 40_px, 600_px), 220_px);
            const auto d0 = date::sys_days(2020_y / date::October / 10);
            const auto six_hour_axis = bottom_axis(scl::time_scale(d0 + 0h, d0 + 60h, 40_px, 600_px), 260_px);
            const auto hour_axis = bottom_axis(scl::time_scale(d0 + 0h, d0 + 10h, 40_px, 600_px), 300_px);
            const auto thirty_minute_axis = bottom_axis(scl::time_scale(d0 + 0min, d0 + 240min, 40_px, 600_px), 340_px);
            const auto minute_axis = bottom_axis(scl::time_scale(d0 + 10min, d0 + 20min, 40_px, 600_px), 380_px);
            const auto fifteen_second_axis = bottom_axis(scl::time_scale(d0 + 10s, d0 + 140s, 40_px, 600_px), 420_px);
            const auto second_axis = bottom_axis(scl::time_scale(d0 + 10s, d0 + 20s, 40_px, 600_px), 460_px);
            test::approve_svg(fig::render_to_svg_string(
                {}, year_axis, quarter_axis, month_axis, week_axis, two_day_axis, day_axis, six_hour_axis, hour_axis,
                thirty_minute_axis, minute_axis, fifteen_second_axis, second_axis));
        }

        TEST_CASE("components")
        {
            constexpr auto frame = fig::frame();
            constexpr auto x = frame.x_center();
            constexpr auto dy = frame.inner_height() / 20.0;
            const auto y_axis = left_axis(scl::linear_scale(0.0, 10.0, frame.y0(), frame.y1()), x,
                                          {.num_ticks_hint = 8, .grid_length = frame.x1() - x});

            const auto color = css4::darkred;

            const auto spine_y = frame.y0() + dy;
            const auto spine_line = elem::line{
                .xs = std::array{x, x - 100_px}, .ys = std::array{spine_y, spine_y}, .properties = {.color = color}};
            const auto spine_text = elem::text{.string = "spine",
                                               .pos = {x - 105_px, spine_y},
                                               .x_anchor = horizontal_anchor::right,
                                               .properties = {.color = color}};

            const auto label_y = frame.y0() + 4 * dy;
            const auto label_line = elem::line{.xs = std::array{x - 20_px, x - 100_px},
                                               .ys = std::array{label_y, label_y},
                                               .properties = {.color = color}};
            const auto label_text = elem::text{.string = "label",
                                               .pos = {x - 105_px, label_y},
                                               .x_anchor = horizontal_anchor::right,
                                               .properties = {.color = color}};

            const auto tick_y = frame.y0() + 6 * dy;
            const auto tick_line = elem::line{.xs = std::array{x - 2_px, x - 5_px, x - 100_px},
                                              .ys = std::array{tick_y, tick_y + dy, tick_y + dy},
                                              .properties = {.color = color}};
            const auto tick_text = elem::text{.string = "tick",
                                              .pos = {x - 105_px, tick_y + dy},
                                              .x_anchor = horizontal_anchor::right,
                                              .properties = {.color = color}};

            const auto grid_y = frame.y0() + 10 * dy;
            const auto grid_line = elem::line{.xs = std::array{x + 100_px, x - 5_px, x - 100_px},
                    .ys = std::array{grid_y, grid_y + dy, grid_y + dy},
                    .properties = {.color = color}};
            const auto grid_text = elem::text{.string = "grid",
                    .pos = {x - 105_px, grid_y + dy},
                    .x_anchor = horizontal_anchor::right,
                    .properties = {.color = color}};

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), y_axis, spine_line, spine_text, label_line,
                                                        label_text, tick_line, tick_text, grid_line, grid_text));
        }
    }
}