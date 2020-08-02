#include <framework/approval_tests.hpp>

#include <cdv/core/color/categorical_schemes.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/arc.hpp>
#include <cdv/elem/pie_slices.hpp>
#include <cdv/elem/text.hpp>
#include <cdv/fig/frame.hpp>
#include <cdv/fig/render_svg.hpp>
#include <cdv/scl/ordinal_scale.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/transform.hpp>

#include <array>
#include <cmath>
#include <functional>
#include <random>
#include <vector>

namespace cdv
{
    namespace
    {
        using namespace units_literals;
        namespace rv = ::ranges::views;

        struct node
        {
            std::string name;
            int value = 0;
            std::vector<node> subnodes;
        };

        const auto hierarchical_data =
            std::vector{
                node{.name = "back_end", .subnodes = std::vector{node{.name = "cairo", .value = 85}}},
                node{.name = "core",
                     .subnodes =
                         std::vector{
                             node{.name = "color",
                                  .subnodes = std::vector{node{.name = "interpolator", .value = 71},
                                                          node{.name = "schemes", .value = 59}}},
                             node{.name = "rgba_color", .value = 142},
                             node{.name = "units", .value = 30},
                             node{.name = "vec2", .value = 97},
                         }},
                node{.name = "elem",
                     .subnodes =
                         std::vector{
                             node{.name = "detail",
                                  .subnodes = std::vector{node{.name = "line_markers", .value = 103},
                                                          node{.name = "markers", .value = 127}}},
                             node{.name = "arc", .value = 48},
                             node{.name = "area", .value = 56},
                             node{.name = "axis", .value = 149},
                             node{.name = "color_legend", .value = 101},
                             node{.name = "fill_properties", .value = 16},
                             node{.name = "line", .value = 44},
                             node{.name = "line_properties", .value = 59},
                             node{.name = "pie_slices", .value = 68},
                             node{.name = "range_stack", .value = 103},
                             node{.name = "rectangle", .value = 30},
                             node{.name = "scatter", .value = 35},
                             node{.name = "swatch_legend", .value = 81},
                             node{.name = "symbol", .value = 22},
                             node{.name = "text", .value = 27},
                         }},
                node{.name = "fig",
                     .subnodes =
                         std::vector{
                             node{.name = "draw", .value = 22},
                             node{.name = "frame", .value = 63},
                             node{.name = "frame_dimensions", .value = 22},
                             node{.name = "render_surface", .value = 227},
                         }},
                node{.name = "scl",
                     .subnodes =
                         std::vector{
                             node{.name = "band", .value = 96},
                             node{.name = "linear", .value = 151},
                             node{.name = "ordinal", .value = 87},
                             node{.name = "point", .value = 79},
                             node{.name = "sequential", .value = 46},
                             node{.name = "ticks", .value = 127},
                             node{.name = "time", .value = 264},
                         }},
                node{.name = "stdx",
                     .subnodes =
                         std::vector{
                             node{.name = "concepts", .value = 25},
                             node{.name = "functional", .value = 27},
                             node{.name = "numbers", .value = 20},
                         }},
            };

        int get_value(const node& n)
        {
            return n.value ? n.value : ranges::accumulate(n.subnodes, 0, [](const int acc, const node& subnode) {
                return acc + get_value(subnode);
            });
        }

        void add_arcs(const std::vector<node>& nodes, const pixel_pos center, const radians start_angle,
                      const radians end_angle, const int depth, const rgba_color color, std::vector<elem::arc>& arcs,
                      std::vector<elem::text>& labels)
        {
            constexpr auto ring_radius = 90_px;
            auto slices = elem::pie_slices(nodes, get_value,
                                           {.start_angle = start_angle, .end_angle = end_angle, .pad_angle = 0.01_rad});
            auto color_scale = scl::make_ordinal_scale<std::string>(scheme::set2);
            for (auto&& slice : slices)
            {
                const auto col = (depth == 0) ? color_scale(slice.data.name) : color;
                arcs.push_back(elem::arc{.center = center,
                                         .outer_radius = (depth + 2) * ring_radius,
                                         .inner_radius = 2_px + (depth + 1) * ring_radius,
                                         .start_angle = slice.start_angle,
                                         .end_angle = slice.end_angle,
                                         .pad_angle = slice.pad_angle,
                                         .fill = {.color = col}});

                if (get_value(slice.data) > 24)
                {
                    labels.push_back(elem::text{.string = slice.data.name,
                                                .pos = centroid(slice, center, (depth + 1.5) * ring_radius),
                                                .rotation = label_angle(slice)});
                }

                add_arcs(slice.data.subnodes, center, slice.start_angle, slice.end_angle, depth + 1, col, arcs, labels);
            }
        }
    }

    TEST_CASE("pie charts")
    {
        SUBCASE("manual arcs")
        {
            constexpr auto frame = fig::frame();
            // mdinject-begin: example-arc
            const auto arc00 = elem::arc{.center = frame.center(),
                                         .outer_radius = 150_px,
                                         .inner_radius = 110_px,
                                         .start_angle = 0_rad,
                                         .end_angle = 2_rad,
                                         .fill = {.color = tab::blue}};
            // mdinject-end
            const auto arc01 = elem::arc{.center = frame.center(),
                                         .outer_radius = 150_px,
                                         .inner_radius = 110_px,
                                         .start_angle = 2_rad,
                                         .end_angle = 5_rad,
                                         .fill = {.color = tab::orange}};
            const auto arc02 = elem::arc{.center = frame.center(),
                                         .outer_radius = 150_px,
                                         .inner_radius = 110_px,
                                         .start_angle = 5_rad,
                                         .end_angle = radians(stdx::numbers::tau),
                                         .fill = {.color = tab::red}};

            const auto arc10 = elem::arc{.center = frame.center(),
                                         .outer_radius = 100_px,
                                         .start_angle = 0_rad,
                                         .end_angle = 2_rad,
                                         .fill = {.color = tab::cyan}};
            const auto arc11 = elem::arc{.center = frame.center(),
                                         .outer_radius = 100_px,
                                         .start_angle = 2_rad,
                                         .end_angle = 5_rad,
                                         .fill = {.color = tab::green}};
            const auto arc12 = elem::arc{.center = frame.center(),
                                         .outer_radius = 100_px,
                                         .start_angle = 5_rad,
                                         .end_angle = radians(stdx::numbers::tau),
                                         .fill = {.color = tab::yellow}};

            const auto arc20 = elem::arc{.center = frame.center(),
                                         .outer_radius = 230_px,
                                         .inner_radius = 160_px,
                                         .start_angle = 1_rad,
                                         .end_angle = 3_rad,
                                         .pad_angle = 0.05_rad,
                                         .fill = {.color = tab::purple, .outline = {.width = 1.5_pt}}};
            const auto arc21 = elem::arc{.center = frame.center(),
                                         .outer_radius = 230_px,
                                         .inner_radius = 160_px,
                                         .start_angle = 3_rad,
                                         .end_angle = 6_rad,
                                         .pad_angle = 0.05_rad,
                                         .fill = {.color = tab::pink, .outline = {.width = 1.5_pt}}};
            const auto arc22 = elem::arc{.center = frame.center(),
                                         .outer_radius = 230_px,
                                         .inner_radius = 160_px,
                                         .start_angle = 6_rad,
                                         .end_angle = radians(stdx::numbers::tau) + 1_rad,
                                         .pad_angle = 0.05_rad,
                                         .fill = {.color = tab::brown, .outline = {.width = 1.5_pt}}};

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), arc00, arc01, arc02, arc10, arc11, arc12,
                                                        arc20, arc21, arc22));
        }

        SUBCASE("simple pie chart")
        {
            constexpr auto frame = fig::frame();
            // mdinject-begin: pie-slices
            const auto data = std::array{std::pair{400, "flour"}, std::pair{15, "yeast"}, std::pair{200, "soy milk"},
                                         std::pair{60, "margarine"}, std::pair{90, "sugar"}};
            const auto color = scl::ordinal_scale(data | rv::values, scheme::dark2);
            auto slices = elem::pie_slices(data, [](const auto p) { return p.first; });
            const auto arcs = slices | rv::transform([&](const auto& slice) {
                                  return elem::arc{.center = frame.center(),
                                                   .outer_radius = frame.inner_height() / 2.0,
                                                   .start_angle = slice.start_angle,
                                                   .end_angle = slice.end_angle,
                                                   .fill = {.color = color(slice.data.second)}};
                              });
            // mdinject-end

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), arcs));
        }

        SUBCASE("donut chart with padding")
        {
            constexpr auto frame = fig::frame();
            const auto data = std::array{std::pair{400, "flour"}, std::pair{15, "yeast"}, std::pair{200, "soy milk"},
                                         std::pair{60, "margarine"}, std::pair{90, "sugar"}};
            const auto color = scl::ordinal_scale(data | rv::values, scheme::set3);
            auto slices = elem::pie_slices(data, [](const auto p) { return p.first; },
                                           {.start_angle = 2_rad, .end_angle = 10_rad, .pad_angle = 0.01_rad});
            const auto arcs =
                rv::all(slices) | rv::transform([&](const auto& slice) {
                    return std::pair{elem::arc{.center = frame.center(),
                                               .outer_radius = 220_px,
                                               .inner_radius = 100_px,
                                               .start_angle = slice.start_angle,
                                               .end_angle = slice.end_angle,
                                               .pad_angle = slice.pad_angle,
                                               .fill = {.color = color(slice.data.second)}},
                                     elem::text{.string = fmt::format("{}g {}", slice.data.first, slice.data.second),
                                                .pos = centroid(slice, frame.center(), 160_px)}};
                })
                | ranges::to_vector;

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), arcs | rv::keys, arcs | rv::values));
        }

        SUBCASE("sunburst")
        {
            constexpr auto frame = fig::frame(800_px, 800_px);

            std::vector<elem::arc> arcs;
            std::vector<elem::text> labels;
            add_arcs(hierarchical_data, frame.center(), 0_rad, radians(stdx::numbers::tau), 0, {}, arcs, labels);

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), arcs, labels));
        }
    }
}