#include "framework/approval_tests.hpp"

#include "cdv/core/color/categorical_schemes.hpp"
#include "cdv/core/color/cividis_interpolator.hpp"
#include "cdv/core/color/cyclical.hpp"
#include "cdv/core/color/diverging_interpolators.hpp"
#include "cdv/core/color/generated_schemes.hpp"
#include "cdv/core/color/multi_hue_interpolators.hpp"
#include "cdv/core/color/perceptually_uniform_interpolators.hpp"
#include "cdv/core/color/single_hue_interpolators.hpp"
#include "cdv/core/color/turbo_interpolator.hpp"
#include "cdv/elem/color_legend.hpp"
#include "cdv/fig/render_svg.hpp"

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>

#include <array>

namespace cdv::elem
{
    namespace
    {
        using namespace units_literals;
        using namespace std::string_literals;
        namespace rv = ::ranges::views;

        template <typename Interpolators>
        void approve_sequential_scale_legends(const Interpolators& interpolators)
        {
            using func_t = std::function<rgba_color(double)>;
            using scale_t = scl::sequential_scale<double, func_t>;

            const auto legends = interpolators | rv::transform([y = -40_px](const auto& name_and_interp) mutable {
                                     const auto [name, interp] = name_and_interp;
                                     y += 50_px;
                                     return elem::color_legend<scale_t>{.scale = scale_t(0.0, 1.0, func_t(interp)),
                                                                        .pos = {10_px, y},
                                                                        .width = 500_px,
                                                                        .height = 30_px,
                                                                        .block_height = 18_px,
                                                                        .title = "interpolator::"s + name,
                                                                        .num_ticks_hint = 0};
                                 })
                                 | ranges::to_vector;

            test::approve_svg(
                fig::render_to_svg_string({.height = 20_px + double(interpolators.size()) * 50_px}, legends));
        }

        template <typename... NamesAndSchemes>
        void approve_ordinal_scale_legends(const NamesAndSchemes&... names_and_schemes)
        {
            auto to_legend = [y = -40_px](const auto& name_and_scheme) mutable {
                const auto& [name, color_scheme] = name_and_scheme;
                y += 65_px;
                auto ordinal = scl::ordinal_scale(rv::iota(0) | rv::take(color_scheme.size()), color_scheme);
                return elem::color_legend<decltype(ordinal)>{.scale = ordinal,
                                                             .pos = {50_px, y},
                                                             .width = color_scheme.size() * 35_px,
                                                             .height = 35_px,
                                                             .block_height = 35_px,
                                                             .title = name,
                                                             .padding = 0.0,
                                                             .tick_label_properties = {.font_size = 0_pt}};
            };

            std::ostringstream os;
            test::approve_svg(fig::render_to_svg_string(
                {.height = 30_px + double(sizeof...(names_and_schemes)) * 65_px}, to_legend(names_and_schemes)...));
        }
    }

    TEST_SUITE("color legend")
    {
        TEST_CASE("scale types")
        {
            auto ordinal = scl::ordinal_scale(std::array{1, 2, 3, 4, 5, 6, 7, 8}, scheme::original_tableau10);
            const auto ordinal_legend = elem::color_legend<decltype(ordinal)>{
                .scale = ordinal, .pos = {50_px, 50_px}, .width = 500_px, .height = 30_px, .block_height = 15_px};

            const auto sequential = scl::sequential_scale(0.0, 1.0, interpolator::magma);
            const auto sequential_legend = elem::color_legend<decltype(sequential)>{
                .scale = sequential, .pos = {50_px, 150_px}, .width = 500_px, .height = 30_px, .block_height = 15_px};

            test::approve_svg(fig::render_to_svg_string({}, ordinal_legend, sequential_legend));
        }

        TEST_CASE("categorical schemes")
        {
            approve_ordinal_scale_legends(
                std::pair("scheme::tableau10", scheme::tableau10),
                std::pair("scheme::original_tableau10", scheme::original_tableau10),
                std::pair("scheme::accent", scheme::accent), std::pair("scheme::dark2", scheme::dark2),
                std::pair("scheme::paired", scheme::paired), std::pair("scheme::pastel1", scheme::pastel1),
                std::pair("scheme::pastel2", scheme::pastel2), std::pair("scheme::set1", scheme::set1),
                std::pair("scheme::set2", scheme::set2), std::pair("scheme::set3", scheme::set3));
        }

        TEST_CASE("generated categorical schemes")
        {
            approve_ordinal_scale_legends(
                std::pair("6 greens", scheme::from_interpolator<6>(interpolator::greens)),
                std::pair("12 greens", scheme::from_interpolator<12>(interpolator::greens)),
                std::pair("12 reds", scheme::from_interpolator<12>(interpolator::reds)),
                std::pair("12 viridis", scheme::from_interpolator<12>(interpolator::viridis)),
                std::pair("8 spectral", scheme::from_interpolator<8>(interpolator::spectral)),
                std::pair("16 spectral", scheme::from_interpolator<16>(interpolator::spectral)),
                std::pair("10 yellow_orange_red", scheme::from_interpolator<10>(interpolator::yellow_orange_red)));
        }

        TEST_CASE("diverging scales")
        {
            approve_sequential_scale_legends(std::vector{
                std::pair{"brown_blue_green", interpolator::brown_blue_green},
                std::pair{"purple_red_green", interpolator::purple_red_green},
                std::pair{"pink_yellow_green", interpolator::pink_yellow_green},
                std::pair{"purple_orange", interpolator::purple_orange},
                std::pair{"red_blue", interpolator::red_blue},
                std::pair{"red_gray", interpolator::red_gray},
                std::pair{"red_yellow_blue", interpolator::red_yellow_blue},
                std::pair{"red_yellow_green", interpolator::red_yellow_green},
                std::pair{"spectral", interpolator::spectral},
            });
        }

        TEST_CASE("single hue scales")
        {
            approve_sequential_scale_legends(std::vector{
                std::pair{"reds", interpolator::reds},
                std::pair{"blues", interpolator::blues},
                std::pair{"greens", interpolator::greens},
                std::pair{"purples", interpolator::purples},
                std::pair{"oranges", interpolator::oranges},
                std::pair{"grays", interpolator::grays},
            });
        }

        TEST_CASE("multi hue scales")
        {
            approve_sequential_scale_legends(std::vector{
                std::pair{"blue_green", interpolator::blue_green},
                std::pair{"blue_purple", interpolator::blue_purple},
                std::pair{"green_blue", interpolator::green_blue},
                std::pair{"orange_red", interpolator::orange_red},
                std::pair{"purple_blue_green", interpolator::purple_blue_green},
                std::pair{"purple_blue", interpolator::purple_blue},
                std::pair{"purple_red", interpolator::purple_red},
                std::pair{"red_purple", interpolator::red_purple},
                std::pair{"yellow_green_blue", interpolator::yellow_green_blue},
                std::pair{"yellow_green", interpolator::yellow_green},
                std::pair{"yellow_orange_brown", interpolator::yellow_orange_brown},
                std::pair{"yellow_orange_red", interpolator::yellow_orange_red},
            });
        }

        TEST_CASE("other scales")
        {
            using func_t = std::function<rgba_color(double)>;
            approve_sequential_scale_legends(std::vector{
                std::pair<const char*, func_t>{"sinebow", interpolator::sinebow},
                std::pair<const char*, func_t>{"turbo", interpolator::turbo},
                std::pair<const char*, func_t>{"cividis", interpolator::cividis},
                std::pair<const char*, func_t>{"plasma", interpolator::plasma},
                std::pair<const char*, func_t>{"magma", interpolator::magma},
                std::pair<const char*, func_t>{"inferno", interpolator::inferno},
                std::pair<const char*, func_t>{"viridis", interpolator::viridis},
            });
        }
    }
}