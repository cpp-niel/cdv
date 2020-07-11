#include "cdv/scl/band_scale.hpp"

#include <cdv/core/rgba_color.hpp>
#include <cdv/core/units.hpp>

#include <doctest/doctest.h>

namespace cdv::scl
{
    TEST_SUITE("band scale")
    {
        TEST_CASE("band width is total width when there are no bands")
        {
            const auto s = band_scale(std::vector<int>{}, 100.0, 200.0);
            CHECK_EQ(s.band_width(), 100.0);
        }

        TEST_CASE("band width is total width when only one band and no padding")
        {
            const auto s = band_scale(std::vector{0}, 100.0, 200.0);
            CHECK_EQ(s.band_width(), 100.0);
        }

        TEST_CASE("band width is total width minus outer padding when only one band")
        {
            const auto s = band_scale(std::vector{0}, 0.0, 100.0, {.outer_padding = 0.5});
            CHECK_EQ(s.band_width(), 50.0);
        }

        TEST_CASE("band width is ten when codomain is 30 long and there are 3 elements in the domain")
        {
            const auto s = band_scale(std::vector{0, 1, 2}, 10.0, 40.0);
            CHECK_EQ(s.band_width(), 10.0);
        }

        TEST_CASE("applying the scale returns the start of the bands")
        {
            const auto s = band_scale(std::vector{'a', 'b', 'c'}, 10.0, 40.0);
            CHECK_EQ(s('a'), 15.0);
            CHECK_EQ(s('b'), 25.0);
            CHECK_EQ(s('c'), 35.0);
        }

        TEST_CASE("applying the scale respects padding")
        {
            const auto s =
                band_scale(std::vector{'a', 'b', 'c'}, 0.0, 120.0, {.inner_padding = 0.2, .outer_padding = 0.2});
            CHECK_EQ(s.band_width(), 30.0);
            CHECK_EQ(s('a'), ::doctest::Approx(22.5));
            CHECK_EQ(s('b'), ::doctest::Approx(60.0));
            CHECK_EQ(s('c'), ::doctest::Approx(97.5));
        }

        TEST_CASE("applying the scale respects different inner and outer padding")
        {
            const auto s =
                band_scale(std::vector{'a', 'b', 'c'}, 0.0, 600.0, {.inner_padding = 0.2, .outer_padding = 0.1});
            CHECK_EQ(s.band_width(), 160.0);
            CHECK_EQ(s('a'), ::doctest::Approx(100.0));
            CHECK_EQ(s('b'), ::doctest::Approx(300.0));
            CHECK_EQ(s('c'), ::doctest::Approx(500.0));
        }

        TEST_CASE("alignment < 0.5 shifts starting points left")
        {
            const auto s = band_scale(std::vector{'a', 'b', 'c'}, 0.0, 600.0,
                                      {.inner_padding = 0.2, .outer_padding = 0.1, .alignment_factor = 0.25});
            CHECK_EQ(s('a'), ::doctest::Approx(90.0));
            CHECK_EQ(s('b'), ::doctest::Approx(290.0));
            CHECK_EQ(s('c'), ::doctest::Approx(490.0));
        }

        TEST_CASE("alignment > 0.5 shifts starting points right")
        {
            const auto s = band_scale(std::vector{'a', 'b', 'c'}, 0.0, 600.0,
                                      {.inner_padding = 0.2, .outer_padding = 0.1, .alignment_factor = 0.75});
            CHECK_EQ(s('a'), ::doctest::Approx(110.0));
            CHECK_EQ(s('b'), ::doctest::Approx(310.0));
            CHECK_EQ(s('c'), ::doctest::Approx(510.0));
        }

        TEST_CASE("applying the scale to values that are not in the domain throws")
        {
            const auto s = band_scale(std::vector{'a', 'b', 'c'}, 10.0, 40.0);
            CHECK(s('a'));
            CHECK_THROWS([[maybe_unused]] auto x = s('d'));
        }

        TEST_CASE("band scale domain can be of non-numeric type")
        {
            const auto s = band_scale(std::vector{tab::green, tab::orange, tab::blue}, 0.0, 120.0);
            CHECK_EQ(s(tab::green), 20.0);
            CHECK_EQ(s(tab::orange), 60.0);
            CHECK_EQ(s(tab::blue), 100.0);
            CHECK_THROWS([[maybe_unused]] auto x = s(tab::brown));
        }

        TEST_CASE("band scale codomain can be unit type")
        {
            using namespace units_literals;
            const auto s = band_scale(std::vector{'a', 'b', 'c'}, 10_px, 40_px);
            CHECK_EQ(s.band_width(), 10_px);
            CHECK_EQ(s('a'), 15_px);
            CHECK_EQ(s('b'), 25_px);
            CHECK_EQ(s('c'), 35_px);
        }
    }
}
