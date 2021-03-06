#include <framework/approval_tests.hpp>

#include <cdv/core/color/categorical_schemes.hpp>
#include <cdv/elem/swatch_legend.hpp>
#include <cdv/fig/frame.hpp>
#include <cdv/fig/render_svg.hpp>
#include <cdv/stdx/date_io.hpp>

#include <array>

namespace cdv::elem
{
    namespace
    {
        using namespace units_literals;
        using namespace std::string_literals;
        using namespace date::literals;
        namespace rv = ::ranges::views;
    }

    TEST_SUITE("swatch legend")
    {
        TEST_CASE("swatches in columns")
        {
            const auto frame = fig::frame(480_px, 240_px);

            // mdinject-begin: swatch-legend-with-columns
            auto scale = scl::ordinal_scale(std::array{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, scheme::accent);
            const auto legend =
                elem::swatch_legend{.scale = scale, .pos = {30_px, 210_px}, .columns = {100_px, 100_px, 100_px}};
            // mdinject-end

            test::approve_svg(fig::render_to_svg_string(frame.dimensions(), legend));
        }

        TEST_CASE("simple swatches")
        {
            const auto scale0 = scl::ordinal_scale(std::array{"dogs", "cats", "hamsters"}, scheme::accent);
            const auto legend0 = elem::swatch_legend{.scale = scale0, .pos = {50_px, 450_px}};

            const auto scale1 = scl::ordinal_scale(
                std::array{"very long text followed by very", "short", "text as swatch labels"}, scheme::dark2);
            const auto legend1 = elem::swatch_legend{.scale = scale1, .pos = {50_px, 400_px}};

            const auto scale2 = scl::ordinal_scale(
                std::array{2020_y / date::August / 10, 2025_y / date::October / 16, 2030_y / date::May / 01},
                scheme::set1);
            const auto legend2 =
                elem::swatch_legend{.scale = scale2, .pos = {50_px, 350_px}, .label_format_specifier = "%d-%b-%Y"};

            const auto scale3 = scl::ordinal_scale(std::array{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, scheme::pastel1);
            const auto legend3 = elem::swatch_legend{.scale = scale3, .pos = {50_px, 300_px}};

            const auto scale4 = scl::ordinal_scale(std::array{"10K", "Half Marathon", "Marathon"}, scheme::pastel2);
            const auto legend4 = elem::swatch_legend{
                .scale = scale4, .pos = {50_px, 250_px}, .block_width = 30_px, .block_height = 10_px, .title = "Races"};

            const auto scale5 = scl::ordinal_scale(std::array{"10K", "Half Marathon", "Marathon"}, scheme::tableau10);
            const auto legend5 = elem::swatch_legend{.scale = scale5,
                                                     .pos = {50_px, 200_px},
                                                     .block_width = 30_px,
                                                     .block_height = 10_px,
                                                     .title = "Races",
                                                     .title_properties = text_properties{.color = css4::darkred, .font_size = 12_pt}, // TODO is this a bug in CTAD for aggregates? can't deduce const char* from the scale when fint size is specified!?!? unless text_properties is explicitly given
                                                     .title_offset = {20_px, 6_px},
                                                     .label_properties = {.color = css4::darkgreen}};

            const auto scale6 = scl::ordinal_scale(std::array{"More", "than", "three", "entries", "which", "allows",
                                                              "columns", "to", "be", "demonstrated"},
                                                   scheme::paired);
            const auto legend6 = elem::swatch_legend{
                .scale = scale6, .pos = {50_px, 150_px}, .block_height = 24_px, .columns = {160_px, 160_px, 160_px}};

            test::approve_svg(
                fig::render_to_svg_string({}, legend0, legend1, legend2, legend3, legend4, legend5, legend6));
        }
    }
}