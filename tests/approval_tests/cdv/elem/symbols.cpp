#include <framework/approval_tests.hpp>

#include <cdv/elem/symbol.hpp>
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

        auto symbol_and_label(const char style, const pixel_pos pos)
        {
            const auto label = style > 11 ? ("'"s + std::string(&style, 1) + "'"s) : fmt::format("{}", int(style));
            return std::pair{
                symbol{.position = pos, .size = 10_pt, .properties = {.color = cdv_blue, .style = style}},
                text{.string = label, .pos = pos + pixel_pos{24_px, 0_px}, .properties = {.font_size = 12_pt}}};
        };
    }

    TEST_CASE("all symbols")
    {
        const auto frame = fig::frame(480_px, 360_px);
        const auto symbols = std::array<char, 37>{'.', ',', 'o', 'x', '+', '1', '2', '3', '4', 'v', '^', '<',
                                                  '>', '8', 's', 'p', 'P', '*', 'h', 'H', 'X', 'D', 'd', '|',
                                                  '_', 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10, 11};

        const auto symbols_and_labels =
            rv::enumerate(symbols) | rv::transform([&](const auto index_and_symbol) {
                const auto [index, sym] = index_and_symbol;
                const auto [x, y] = std::div(int(index), 10);
                return symbol_and_label(sym, {frame.x0() + double(x) * 120_px, frame.y1() - (double(y) * 35_px)});
            });

        test::approve_svg(fig::render_to_svg_string(frame.dimensions(), symbols_and_labels));
    }
}
