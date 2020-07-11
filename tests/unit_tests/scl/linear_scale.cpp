#include <cdv/scl/linear_scale.hpp>

#include <doctest/doctest.h>
#include <range/v3/algorithm/equal.hpp>
#include <range/v3/back.hpp>

namespace cdv::scl
{
    TEST_SUITE("linear scale")
    {
        TEST_CASE("two point scale")
        {
            const auto s = linear_scale(std::vector{0.0, 1.0}, std::vector{1.0, 2.0});
            CHECK(s(-1.0) == 0.0);
            CHECK(s(0.0) == 1.0);
            CHECK(s(0.5) == 1.5);
            CHECK(s(1.0) == 2.0);
            CHECK(s(2.0) == 3.0);
        }

        TEST_CASE("three point scale")
        {
            const auto s = linear_scale(std::vector{-1.0, 0.0, 1.0}, std::vector{0.0, 1.0, 2.0});
            CHECK(s(-2.0) == -1.0);
            CHECK(s(-1.0) == 0.0);
            CHECK(s(0.0) == 1.0);
            CHECK(s(0.5) == 1.5);
            CHECK(s(1.0) == 2.0);
            CHECK(s(2.0) == 3.0);
        }

        TEST_CASE("clamped two point scale")
        {
            const auto s = linear_scale(std::vector{0.0, 1.0}, std::vector{1.0, 2.0}, {.clamp = true});
            CHECK(s(-1.0) == 1.0);
            CHECK(s(0.0) == 1.0);
            CHECK(s(0.5) == 1.5);
            CHECK(s(1.0) == 2.0);
            CHECK(s(2.0) == 2.0);
        }

        TEST_CASE("clamped three point scale")
        {
            const auto s = linear_scale(std::vector{-1.0, 0.0, 1.0}, std::vector{0.0, 1.0, 2.0}, {.clamp = true});
            CHECK(s(-2.0) == 0.0);
            CHECK(s(-1.0) == 0.0);
            CHECK(s(0.0) == 1.0);
            CHECK(s(0.5) == 1.5);
            CHECK(s(1.0) == 2.0);
            CHECK(s(2.0) == 2.0);
        }

        TEST_CASE("descending domain")
        {
            const auto s = linear_scale(std::vector{5.0, 0.0}, std::vector{0.0, 10.0});
            CHECK(s(10.0) == -10.0);
            CHECK(s(5.0) == 0.0);
            CHECK(s(2.5) == 5.0);
            CHECK(s(0.0) == 10.0);
            CHECK(s(-5.0) == 20.0);
        }

        TEST_CASE("descending codomain")
        {
            const auto s = linear_scale(std::vector{0.0, 5.0}, std::vector{10.0, 0.0});
            CHECK(s(-1.0) == 12.0);
            CHECK(s(0.0) == 10.0);
            CHECK(s(2.5) == 5.0);
            CHECK(s(5.0) == 0.0);
            CHECK(s(6.0) == doctest::Approx(-2.0));
        }

        TEST_CASE("descending three point domain")
        {
            const auto s = linear_scale(std::vector{5.0, 0.0, -10.0}, std::vector{0.0, 10.0, 60.0});
            CHECK(s(10.0) == -10.0);
            CHECK(s(5.0) == 0.0);
            CHECK(s(2.5) == 5.0);
            CHECK(s(0.0) == 10.0);
            CHECK(s(-2.0) == 20.0);
            CHECK(s(-10.0) == 60.0);
            CHECK(s(-20.0) == 110.0);
        }

        TEST_CASE("snapped to grid")
        {
            const auto s = linear_scale(std::vector{0.1, 5.0, 9.99}, std::vector{0.0, 0.5, 1.0});
            const auto snapped = s.snapped_to_grid();
            CHECK(ranges::equal(snapped.domain(), std::vector{0.0, 5.0, 10.0}));
        }

        TEST_CASE("snapped to grid with large numbers")
        {
            const auto s = linear_scale(std::vector{1'000'100.0, 9'900'000.0}, std::vector{0.0, 1.0});
            const auto snapped = s.snapped_to_grid();
            CHECK(ranges::front(snapped.domain()) == 1'000'000.0);
            CHECK(ranges::back(snapped.domain()) == 10'000'000.0);
        }

        TEST_CASE("snapped to grid with small numbers")
        {
            const auto s = linear_scale(std::vector{0.000111, 0.000999}, std::vector{0.0, 1.0});
            const auto snapped = s.snapped_to_grid();
            CHECK(ranges::front(snapped.domain()) == 1e-4);
            CHECK(ranges::back(snapped.domain()) == 1e-3);
        }
    }
}