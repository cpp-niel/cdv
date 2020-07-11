#include "cdv/scl/log_scale.hpp"
#include "test/util.hpp"

#include <doctest/doctest.h>
#include <range/v3/algorithm/equal.hpp>

namespace cdv::scl
{
    TEST_SUITE("log scale")
    {
        const auto scale = [](const double a, const double b) { return log_scale(std::vector{a, b}, std::vector{0.0, 1.0});};

        TEST_CASE("two point scale base 10")
        {
            const auto s = log_scale(std::vector{1.0, 10.0}, std::vector{0.0, 1.0});
            CHECK(s(-1.0) == 0.0);
            CHECK(s(-0.5) == doctest::Approx(0.3010299957));
            CHECK(s(1.0) == 0.0);
            CHECK(s(0.5) == doctest::Approx(-0.3010299957));
            CHECK(s(5.0) == doctest::Approx(0.6989700043));
            CHECK(s(15.0) == doctest::Approx(1.1760912591));
        }

        TEST_CASE("two point scale base 10 non unit domain")
        {
            const auto s = log_scale(std::vector{1.0, 2.0}, std::vector{0.0, 1.0});
            CHECK(s(0.5) == doctest::Approx(-1.0));
            CHECK(s(1.0) == 0.0);
            CHECK(s(2.0) == doctest::Approx(1.0));
            CHECK(s(2.5) == doctest::Approx(1.3219281));
        }

        TEST_CASE("two point scale base e")
        {
            const auto s = log_scale(std::vector{1.0, 10.0}, std::vector{0.0, 1.0}, {.base = stdx::numbers::e});
            CHECK(s(1.0) == 0.0);
            CHECK(s(0.5) == doctest::Approx(-0.30102999566398114));
            CHECK(s(5.0) == doctest::Approx(0.6989700043360187));
            CHECK(s(15.0) == doctest::Approx(1.1760912590556811));
        }

        TEST_CASE("two point scale base 2")
        {
            const auto s = log_scale(std::vector{1.0, 32.0}, std::vector{0.0, 1.0}, {.base = 2.0});
            CHECK(s(0.5) == doctest::Approx(-0.2));
            CHECK(s(1.0) == 0.0);
            CHECK(s(5.0) == doctest::Approx(0.46438561897747244));
            CHECK(s(15.0) == doctest::Approx(0.7813781191217037));
        }

        TEST_CASE("two point scale base 3 with non unit range")
        {
            const auto s = log_scale(std::vector{1.0, 81.0}, std::vector{10.0, 20.0}, {.base = 3.0});
            CHECK(s(0.5) == doctest::Approx(8.422675616071357));
            CHECK(s(1.0) == 10.0);
            CHECK(s(5.0) == doctest::Approx(13.662433801794817));
            CHECK(s(15.0) == doctest::Approx(16.162433801794815));
            CHECK(s(81.0) == doctest::Approx(20.0));
            CHECK(s(100.0) == doctest::Approx(20.479516371446923));
        }

        TEST_CASE("snapped to grid")
        {
            const auto s = log_scale(std::vector{1.1, 10.9}, std::vector{0.0, 1.0});
            const auto snapped = s.snapped_to_grid();
            CHECK(ranges::equal(snapped.domain(), std::vector{1.0, 100.0}));
        }

        TEST_CASE("snapped to grid with large numbers")
        {
            const auto s = log_scale(std::vector{4.5, 1234.5}, std::vector{0.0, 1.0});
            const auto snapped = s.snapped_to_grid();
            CHECK(ranges::equal(snapped.domain(), std::vector{1.0, 10000.0}));
        }

        TEST_CASE("snapped to grid with small numbers")
        {
            const auto s = log_scale(std::vector{0.001, 0.45}, std::vector{0.0, 1.0});
            const auto snapped = s.snapped_to_grid();
            CHECK(ranges::equal(snapped.domain(), std::vector{0.001, 1.0}));
        }

        TEST_CASE("snapped to grid descending")
        {
            const auto s = log_scale(std::vector{10.9, 1.1}, std::vector{0.0, 1.0});
            const auto snapped = s.snapped_to_grid();
            CHECK(ranges::equal(snapped.domain(), std::vector{100.0, 1.0}));
        }

        TEST_CASE("snapped to grid with three points")
        {
            const auto s = log_scale(std::vector{1.1, 1.5, 10.9}, std::vector{0.0, 0.5, 1.0});
            const auto snapped = s.snapped_to_grid();
            CHECK(ranges::equal(snapped.domain(), std::vector{1.0, 1.5, 100.0}));
        }

        TEST_CASE("snapped to grid with three descending points")
        {
            const auto s = log_scale(std::vector{1234.5, 10.5, 1.9}, std::vector{0.0, 0.5, 1.0});
            const auto snapped = s.snapped_to_grid();
            CHECK(ranges::equal(snapped.domain(), std::vector{10000.0, 10.5, 1.0}));
        }

        TEST_CASE("plausible ascending ticks")
        {
            CHECK(test::almost_equal(scale(1e-1, 1e1).ticks(10), std::vector{0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0}, 1e-6));
            CHECK(test::almost_equal(scale(1e-1, 1e0).ticks(10), std::vector{0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0}, 1e-6));
            CHECK(test::almost_equal(scale(-1e0, -1e-1).ticks(10), std::vector{-1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1}, 1e-6));
        }

        TEST_CASE("plausible descending ticks")
        {
            CHECK(test::almost_equal(scale(-1e-1, -1e1).ticks(10), std::vector{-0.1, -0.2, -0.3, -0.4, -0.5, -0.6, -0.7, -0.8, -0.9, -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0, -10.0}, 1e-6));
            CHECK(test::almost_equal(scale(-1e-1, -1e0).ticks(10), std::vector{-0.1, -0.2, -0.3, -0.4, -0.5, -0.6, -0.7, -0.8, -0.9, -1.0}, 1e-6));
            CHECK(test::almost_equal(scale(1e0, 1e-1).ticks(10), std::vector{1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1}, 1e-6));
        }

        TEST_CASE("plausible ticks on small domains")
        {
            CHECK(test::almost_equal(scale(1, 5).ticks(), std::vector{1.0, 2.0, 3.0, 4.0, 5.0}, 1e-6));
            CHECK(test::almost_equal(scale(5, 1).ticks(), std::vector{5.0, 4.0, 3.0, 2.0, 1.0}, 1e-6));
            CHECK(test::almost_equal(scale(-1, -5).ticks(), std::vector{-1.0, -2.0, -3.0, -4.0, -5.0}, 1e-6));
            CHECK(test::almost_equal(scale(-5, -1).ticks(), std::vector{-5.0, -4.0, -3.0, -2.0, -1.0}, 1e-6));
            CHECK(test::almost_equal(scale(286.9252014, 329.4978332).ticks(1), std::vector{300.0}, 1e-6));
            CHECK(test::almost_equal(scale(286.9252014, 329.4978332).ticks(2), std::vector{300.0}, 1e-6));
            CHECK(test::almost_equal(scale(286.9252014, 329.4978332).ticks(3), std::vector{300.0, 320.0}, 1e-6));
            CHECK(test::almost_equal(scale(286.9252014, 329.4978332).ticks(4), std::vector{290.0, 300.0, 310.0, 320.0}, 1e-6));
            CHECK(test::almost_equal(scale(286.9252014, 329.4978332).ticks(),  std::vector{290.0, 295.0, 300.0, 305.0, 310.0, 315.0, 320.0, 325.0}, 1e-6));
        }

        TEST_CASE("ticks are linear when domain is fully inside an order of magnitude")
        {
            CHECK(test::almost_equal(scale(41, 42).ticks(), std::vector{41.0, 41.1, 41.2, 41.3, 41.4, 41.5, 41.6, 41.7, 41.8, 41.9, 42.0}, 1e-6));
            CHECK(test::almost_equal(scale(42, 41).ticks(), std::vector{42.0, 41.9, 41.8, 41.7, 41.6, 41.5, 41.4, 41.3, 41.2, 41.1, 41.0}, 1e-6));
            CHECK(test::almost_equal(scale(1600, 1400).ticks(), std::vector{1600.0, 1580.0, 1560.0, 1540.0, 1520.0, 1500.0, 1480.0, 1460.0, 1440.0, 1420.0, 1400.0}, 1e-6));
        }

        TEST_CASE("ticks with non integer base")
        {
            const auto s = log_scale(std::vector{0.1, 100.0}, std::vector{0.0, 1.0}, {.base = stdx::numbers::e});
            CHECK(test::almost_equal(s.ticks(), std::vector{0.135335283237, 0.367879441171, 1.0, 2.718281828459, 7.389056098931, 20.085536923188, 54.598150033144}, 1e-6));
        }
    }
}