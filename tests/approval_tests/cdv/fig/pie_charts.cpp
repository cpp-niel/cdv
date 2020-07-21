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

namespace cdv
{
    TEST_CASE("pie charts")
    {
        using namespace units_literals;
        namespace rv = ::ranges::views;

        SUBCASE("manual arcs")
        {
            const auto frame = fig::frame();
            const auto arc00 = elem::arc{.center = frame.center(),
                                         .outer_radius = 150_px,
                                         .inner_radius = 110_px,
                                         .start_angle = 0_rad,
                                         .end_angle = 2_rad,
                                         .fill = {.color = tab::blue}};
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

        SUBCASE("donut chart with padding")
        {
            const auto frame = fig::frame();
            const auto data = std::array{std::pair{400, "flour"}, std::pair{15, "yeast"}, std::pair{200, "soy milk"},
                                         std::pair{60, "margarine"}, std::pair{90, "sugar"}};
            const auto color = scl::ordinal_scale(data | rv::values, scheme::set2);
            auto slices = elem::pie_slices<std::pair<int, const char*>>(
                data, [](const auto p) { return p.first; },
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
    }
}