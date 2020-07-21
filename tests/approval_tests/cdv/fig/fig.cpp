#include <framework/approval_tests.hpp>

#include <cdv/core/rgba_color.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/arc.hpp>
#include <cdv/elem/area.hpp>
#include <cdv/elem/axis.hpp>
#include <cdv/elem/line.hpp>
#include <cdv/elem/rectangle.hpp>
#include <cdv/elem/text.hpp>
#include <cdv/fig/frame.hpp>
#include <cdv/fig/render_svg.hpp>
#include <cdv/scl/band_scale.hpp>
#include <cdv/scl/linear_scale.hpp>
#include <cdv/scl/ordinal_scale.hpp>
#include <cdv/scl/point_scale.hpp>
#include <cdv/scl/time_scale.hpp>

#include <array>

#include <cdv/elem/symbol.hpp>
#include <cdv/stdx/date.hpp>
#include <range/v3/utility/concepts.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/cycle.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

namespace cdv
{
    using namespace units_literals;
    namespace rv = ::ranges::views;

    namespace tst
    {
        struct cross
        {
            pixel_pos min;
            pixel_pos max;
        };

        template <typename Surface>
        void draw(const cross& c, Surface& surface, const pixel_pos&)
        {
            surface.draw_path(std::array{c.min, c.max});
            surface.stroke();
            auto [min, max] = c;
            std::swap(min.x, max.x);
            surface.draw_path(std::array{min, max});
            surface.stroke();
        }
    }

    TEST_CASE("render arbitrary element")
    {
        const auto cross0 = tst::cross{{10_px, 10_px}, {100_px, 100_px}};
        const auto cross1 = tst::cross{{10_px, 100_px}, {20_px, 110_px}};
        const auto cross2 = tst::cross{{100_px, 100_px}, {200_px, 300_px}};
        test::approve_svg(fig::render_to_svg_string({}, cross0, cross1, cross2));
    }

    TEST_CASE("time line")
    {
        // mdinject-begin: example-time-line
        using namespace std::chrono_literals;
        using namespace date::literals;
        const auto events = std::array{
            std::pair{"1st known case", 2019_y / date::November / 17},
            std::pair{"WHO reports pneumonia cluster", 2019_y / date::December / 31},
            std::pair{"1st case outside China", 2020_y / date::January / 13},
            std::pair{"Virus named COVID-19", 2020_y / date::February / 11},
            std::pair{"Classification as pandemic", 2020_y / date::March / 11},
            std::pair{"1 million cases", 2020_y / date::April / 02},
            std::pair{"5 million cases", 2020_y / date::May / 21},
        };

        const auto x0 = 60_px;
        const auto x1 = 560_px;
        const auto y0 = 250_px;
        const auto y_offsets = std::array{50_px, -50_px, 100_px, -100_px};
        const auto x = scl::time_scale(2019_y / date::November / 01, 2020_y / date::June / 01, x0, x1);
        const auto x_axis = elem::bottom_axis(x, 50_px);

        const auto line = elem::hline(x0, x1, y0);

        const auto ticks =
            rv::zip(events, rv::cycle(y_offsets)) | rv::transform([&](const auto& data_and_offsets) {
                const auto& [label_and_date, y_offset] = data_and_offsets;
                const auto& [label, date] = label_and_date;
                const auto tick_x = x(date::sys_days(date));
                return std::tuple{
                    elem::symbol{.position = {tick_x, y0}, .properties = {.color = tab::red}},
                    elem::line(std::array{tick_x, tick_x}, std::array{y0, y0 + y_offset}, {.color = tab::red}),
                    elem::text{.string = label, .pos = {tick_x, y0 + (y_offset * 1.15)}}};
            });

        const auto svg = fig::render_to_svg_string({}, x_axis, line, ticks);
        // mdinject-end

        test::approve_svg(svg);
    }

    TEST_CASE("frame terminology")
    {
        const auto frame = fig::frame(640_px, 480_px, 70_px, 30_px, 30_px, 30_px, 100_dpi);
        const auto x = scl::point_scale(std::array{"x0", "x_center", "x1"}, frame.x0(), frame.x1());
        const auto y = scl::point_scale(std::array{"y0", "y_center", "y1"}, frame.y0(), frame.y1());
        const auto x_axis = elem::bottom_axis(x, frame.y0(), {.grid_length = frame.inner_height() * 0.5});
        const auto y_axis = elem::left_axis(y, frame.x0(), {.grid_length = frame.inner_width() * 0.5});
        const auto top_line = elem::hline(frame.x0(), frame.x1(), frame.y1());
        const auto right_line = elem::vline(frame.x1(), frame.y0(), frame.y1());

        const auto center_marker = elem::symbol{.position = frame.center(), .properties = {.style = 'x'}};
        const auto center_text = elem::text{.string = "center", .pos = {frame.x_center(), frame.height() * 0.525}};

        const auto inner_hline = elem::hline(frame.x0(), frame.x1(), frame.height() * 0.75, {.color = tab::blue});
        const auto inner_hline_text = elem::text{.string = "inner_width",
                                                 .pos = {frame.x_center(), frame.height() * 0.77},
                                                 .properties = {.color = tab::blue}};

        const auto inner_vline = elem::vline(frame.width() * 0.75, frame.y0(), frame.y1(), {.color = tab::blue});
        const auto inner_vline_text = elem::text{.string = "inner_height",
                                                 .pos = {frame.width() * 0.81, frame.y_center()},
                                                 .properties = {.color = tab::blue}};

        const auto width_hline = elem::hline(0_px, frame.width(), frame.height() * 0.25, {.color = tab::red});
        const auto width_hline_text = elem::text{
            .string = "width", .pos = {frame.x_center(), frame.height() * 0.27}, .properties = {.color = tab::red}};

        const auto height_vline = elem::vline(frame.width() * 0.25, 0_px, frame.height(), {.color = tab::red});
        const auto height_vline_text = elem::text{
            .string = "height", .pos = {frame.width() * 0.28, frame.y_center()}, .properties = {.color = tab::red}};

        test::approve_svg(fig::render_to_svg_string(frame.dimensions(), x_axis, y_axis, top_line, right_line,
                                                    center_marker, center_text, inner_hline, inner_hline_text,
                                                    inner_vline, inner_vline_text, width_hline, width_hline_text,
                                                    height_vline, height_vline_text));
    }

    TEST_CASE("band scale terminology")
    {
        const auto frame = fig::frame();
        const auto keys = std::array{"band 1", "band 2", "band 3"};
        const auto s =
            scl::band_scale(keys, frame.x0() + 50_px, frame.x1() - 50_px, {.inner_padding = 0.2, .outer_padding = 0.2});

        const auto axis = elem::bottom_axis(s, frame.y_center());
        const auto rects = keys | rv::transform([&](const auto label) {
                               return elem::rectangle{.min = {s.min(label), frame.y_center()},
                                                      .max = {s.max(label), frame.y_center() + 100_px},
                                                      .fill = {.outline = {.width = 1.5_pt}}};
                           });

        auto previous_x = s(keys[0]);
        const auto inner_xs = keys | rv::drop(1) | rv::transform([&](const auto label) {
                                  const auto x = previous_x + (s(label) - previous_x) * 0.5;
                                  previous_x = s(label);
                                  return x;
                              })
                              | ranges::to_vector;

        const auto outer_xs = std::array{
            s.codomain()[0] + (s.min(keys.front()) - s.codomain()[0]) * 0.5,
            s.max(keys.back()) + (s.codomain()[1] - s.max(keys.back())) * 0.5,
        };

        const auto create_label = [&](const auto txt) {
            return [=](const auto x) {
                const auto y = frame.y_center() + 50_px;
                return std::tuple{elem::symbol{.position = {x, y}},
                                  elem::line(std::array{x, x}, std::array{y, y + 100_px}),
                                  elem::text{.string = std::string(txt) + " padding x step", .pos = {x, y + 110_px}}};
            };
        };

        const auto inner_padding_labels = inner_xs | rv::transform(create_label("inner"));
        const auto outer_padding_labels = outer_xs | rv::transform(create_label("outer"));

        const auto label_y = frame.y_center() - 50_px;
        const auto step_label = std::pair{
            elem::line(std::array{s(keys[1]), s(keys[2])}, std::array{label_y, label_y}),
            elem::text{.string = "step", .pos = {s(keys[1]) + ((s(keys[2]) - s(keys[1])) * 0.5), label_y - 15_px}}};

        const auto band_width_label =
            std::pair{elem::line(std::array{s.min(keys.front()), s.max(keys.front())}, std::array{label_y, label_y}),
                      elem::text{.string = "band_width", .pos = {s(keys.front()), label_y - 15_px}}};

        test::approve_svg(fig::render_to_svg_string(frame.dimensions(), axis, rects, inner_padding_labels,
                                                    outer_padding_labels, step_label, band_width_label));
    }

    TEST_CASE("text positioning")
    {
        const auto frame = fig::frame();
        const auto right_anchored =
            elem::text{.string = "right anchored ", .pos = frame.center(), .x_anchor = elem::horizontal_anchor::right};
        const auto left_anchored =
            elem::text{.string = " left anchored", .pos = frame.center(), .x_anchor = elem::horizontal_anchor::left};
        const auto right_anchored_rotated = elem::text{.string = "right anchored & rotated ",
                                                       .pos = frame.center(),
                                                       .x_anchor = elem::horizontal_anchor::right,
                                                       .rotation = 1_rad};
        const auto left_anchored_rotated = elem::text{.string = " left anchored & rotated",
                                                      .pos = frame.center(),
                                                      .x_anchor = elem::horizontal_anchor::left,
                                                      .rotation = 1_rad};

        const auto top_center = pixel_pos{frame.x_center(), frame.y1()};
        const auto top_anchored =
            elem::text{.string = "top anchored", .pos = top_center, .y_anchor = elem::vertical_anchor::top};
        const auto bottom_anchored =
            elem::text{.string = "bottom anchored", .pos = top_center, .y_anchor = elem::vertical_anchor::bottom};

        const auto middle_left = pixel_pos{frame.x0(), frame.y_center()};
        const auto bottom_to_top = elem::text{.string = "bottom to top",
                                              .pos = middle_left,
                                              .y_anchor = elem::vertical_anchor::bottom,
                                              .rotation = radians(stdx::numbers::pi * 0.5)};
        const auto top_to_bottom = elem::text{.string = "top to bottom",
                                              .pos = middle_left,
                                              .y_anchor = elem::vertical_anchor::bottom,
                                              .rotation = radians(stdx::numbers::pi * 1.5)};

        test::approve_svg(fig::render_to_svg_string(frame.dimensions(), right_anchored, left_anchored,
                                                    right_anchored_rotated, left_anchored_rotated, top_anchored,
                                                    bottom_anchored, bottom_to_top, top_to_bottom));
    }

    TEST_CASE("cdv logo")
    {
        const auto frame = fig::frame();
        const auto radius = frame.inner_height() * 0.5;
        const auto ring_outer_radius = radius * 0.8;
        const auto ring_inner_radius = radius * 0.45;
        const auto blue_disc = elem::arc{.center = frame.center(), .outer_radius = radius};
        const auto white_ring = elem::arc{.center = frame.center(),
                                          .outer_radius = ring_outer_radius,
                                          .inner_radius = ring_inner_radius,
                                          .fill = {.color = css4::white}};

        const auto v_width = radius * 0.5;
        const auto top_y = frame.y_center() + (ring_outer_radius * 1.05);
        const auto v = elem::fill_between(
            std::array{frame.x_center() - v_width, frame.x_center(), frame.x_center() + v_width},
            std::array{top_y, top_y, top_y}, std::array{top_y, frame.y_center() - ring_outer_radius, top_y});

        test::approve_svg(fig::render_to_svg_string(frame.dimensions(), blue_disc, white_ring, v));
    }
}