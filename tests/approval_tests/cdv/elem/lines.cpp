#include <framework/approval_tests.hpp>

#include <cdv/elem/line.hpp>
#include <cdv/elem/text.hpp>
#include <cdv/fig/frame.hpp>
#include <cdv/fig/render_svg.hpp>

namespace cdv::elem
{
    namespace
    {
        using namespace ::cdv::units_literals;
        using namespace ::std::string_literals;
        namespace rv = ::ranges::views;

        auto line_and_label(const std::string& label, const stdx::range_of<pixels> auto& xs,
                            const stdx::range_of<pixels> auto& ys, const line_properties& properties)
        {
            const auto label_pos = pixel_pos{::ranges::back(xs) + 24_px, ::ranges::back(ys)};
            return std::pair{line(xs, ys, properties), text{.string = label,
                                                            .pos = label_pos,
                                                            .x_anchor = horizontal_anchor::left,
                                                            .properties = {.font_size = 12_pt}}};
        };
    }

    TEST_CASE("line caps")
    {
        const auto frame = fig::frame(320_px, 100_px);

        const auto butt = line_and_label("cap_style::butt", std::array{20_px, 100_px}, std::array{80_px, 80_px},
                                         {.color = cdv_blue, .width = 7_pt, .cap = cap_style::butt});
        const auto square = line_and_label("cap_style::square", std::array{20_px, 100_px}, std::array{50_px, 50_px},
                                           {.color = cdv_blue, .width = 7_pt, .cap = cap_style::square});
        const auto round = line_and_label("cap_style::round", std::array{20_px, 100_px}, std::array{20_px, 20_px},
                                          {.color = cdv_blue, .width = 7_pt, .cap = cap_style::round});

        test::approve_svg(fig::render_to_svg_string(frame.dimensions(), butt, square, round));
    }

    TEST_CASE("line joins")
    {
        const auto frame = fig::frame(320_px, 120_px);

        const auto miter = line_and_label("join_style::miter", std::array{20_px, 50_px, 100_px}, std::array{110_px, 70_px, 90_px},
                                         {.color = cdv_blue, .width = 8_pt, .join = join_style::miter});
        const auto round = line_and_label("join_style::round", std::array{20_px, 50_px, 100_px}, std::array{80_px, 40_px, 60_px},
                                          {.color = cdv_blue, .width = 8_pt, .join = join_style::round});
        const auto bevel = line_and_label("join_style::bevel", std::array{20_px, 50_px, 100_px}, std::array{50_px, 10_px, 30_px},
                                           {.color = cdv_blue, .width = 8_pt, .join = join_style::bevel});

        test::approve_svg(fig::render_to_svg_string(frame.dimensions(), miter, round, bevel));
    }
}
