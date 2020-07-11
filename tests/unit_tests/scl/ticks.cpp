#include "cdv/scl/ticks.hpp"
#include "test/util.hpp"

#include <doctest/doctest.h>
#include <range/v3/algorithm/equal.hpp>
#include <range/v3/view/drop.hpp>

namespace doctest
{
    template <typename T>
    struct StringMaker<std::vector<T>>
    {
        static String convert(const std::vector<T>& v)
        {
            auto result = String("std::vector{") + toString(v[0]);
            for (auto x : v | ranges::views::drop(1))
                result += String(", ") + toString(x);

            return result + "}";
        }
    };
}

namespace cdv::scl
{
    TEST_SUITE("ticks")
    {
        TEST_CASE("tick increment")
        {
            CHECK_EQ(tick_increment(-1.0, 1.0, 10), -5);
            CHECK_EQ(tick_increment(0.0, 10.0, 10), 1);
            CHECK_EQ(tick_increment(0.0, 10.0, 100000), -10000);
            CHECK_EQ(tick_increment(0.0001, 0.01, 10), -1000);
            CHECK(std::isinf(tick_increment(1.0, 1.0, 10)));
        }

        TEST_CASE("ascending ticks")
        {
            CHECK_EQ(linear_ticks(0.0, 4.0, 4), std::vector{0.0, 1.0, 2.0, 3.0, 4.0});
            CHECK_EQ(linear_ticks(0.0, 4.0, 2), std::vector{0.0, 2.0, 4.0});
            CHECK_EQ(linear_ticks(0.0, 10.0, 3), std::vector{0.0, 5.0, 10.0});
            CHECK_EQ(linear_ticks(0.0, 4.0, 5), std::vector{0.0, 1.0, 2.0, 3.0, 4.0});
            CHECK_EQ(linear_ticks(0.0f, 1.0f, 5), std::vector{0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 4), std::vector{-10.0, -5.0, 0.0, 5.0, 10.0});
            CHECK_EQ(linear_ticks(-0.5, 4.5, 6), std::vector{0.0, 1.0, 2.0, 3.0, 4.0});
        }

        TEST_CASE("descending ticks")
        {
            CHECK_EQ(linear_ticks(4.0, 0.0, 4), std::vector{4.0, 3.0, 2.0, 1.0, 0.0});
            CHECK_EQ(linear_ticks(4.0, 0.0, 2), std::vector{4.0, 2.0, 0.0});
            CHECK_EQ(linear_ticks(10.0, 0.0, 3), std::vector{10.0, 5.0, 0.0});
            CHECK_EQ(linear_ticks(4.0, 0.0, 5), std::vector{4.0, 3.0, 2.0, 1.0, 0.0});
            CHECK_EQ(linear_ticks(1.0f, 0.0f, 5), std::vector{1.0f, 0.8f, 0.6f, 0.4f, 0.2f, 0.0f});
            CHECK_EQ(linear_ticks(10.0, -10.0, 4), std::vector{10.0, 5.0, 0.0, -5.0, -10.0});
        }

        TEST_CASE("plausible ticks as in d3 tests")
        {
            CHECK(test::almost_equal(linear_ticks(0.0, 1.0, 10), std::vector{0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0},
                               1e-6));
            CHECK(test::almost_equal(linear_ticks(0.0, 1.0, 9), std::vector{0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0},
                               1e-6));
            CHECK(test::almost_equal(linear_ticks(0.0, 1.0, 8), std::vector{0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0},
                               1e-6));
            CHECK_EQ(linear_ticks(0.0f, 1.0f, 7), std::vector{0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f});
            CHECK_EQ(linear_ticks(0.0f, 1.0f, 6), std::vector{0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f});
            CHECK_EQ(linear_ticks(0.0f, 1.0f, 5), std::vector{0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f});
            CHECK_EQ(linear_ticks(0.0f, 1.0f, 4), std::vector{0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f});
            CHECK_EQ(linear_ticks(0.0f, 1.0f, 3), std::vector{0.0f, 0.5f, 1.0f});
            CHECK_EQ(linear_ticks(0.0f, 1.0f, 2), std::vector{0.0f, 0.5f, 1.0f});
            CHECK_EQ(linear_ticks(0.0f, 1.0f, 1), std::vector{0.0f, 1.0f});
            CHECK_EQ(linear_ticks(0.0, 10.0, 10), std::vector{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0});
            CHECK_EQ(linear_ticks(0.0, 10.0, 9), std::vector{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0});
            CHECK_EQ(linear_ticks(0.0, 10.0, 8), std::vector{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0});
            CHECK_EQ(linear_ticks(0.0, 10.0, 7), std::vector{0.0, 2.0, 4.0, 6.0, 8.0, 10.0});
            CHECK_EQ(linear_ticks(0.0, 10.0, 6), std::vector{0.0, 2.0, 4.0, 6.0, 8.0, 10.0});
            CHECK_EQ(linear_ticks(0.0, 10.0, 5), std::vector{0.0, 2.0, 4.0, 6.0, 8.0, 10.0});
            CHECK_EQ(linear_ticks(0.0, 10.0, 4), std::vector{0.0, 2.0, 4.0, 6.0, 8.0, 10.0});
            CHECK_EQ(linear_ticks(0.0, 10.0, 3), std::vector{0.0, 5.0, 10.0});
            CHECK_EQ(linear_ticks(0.0, 10.0, 2), std::vector{0.0, 5.0, 10.0});
            CHECK_EQ(linear_ticks(0.0, 10.0, 1), std::vector{0.0, 10.0});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 10), std::vector{-10.0, -8.0, -6.0, -4.0, -2.0, 0.0, 2.0, 4.0, 6.0, 8.0, 10.0});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 9), std::vector{-10.0, -8.0, -6.0, -4.0, -2.0, 0.0, 2.0, 4.0, 6.0, 8.0, 10.0});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 8), std::vector{-10.0, -8.0, -6.0, -4.0, -2.0, 0.0, 2.0, 4.0, 6.0, 8.0, 10.0});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 7), std::vector{-10.0, -8.0, -6.0, -4.0, -2.0, 0.0, 2.0, 4.0, 6.0, 8.0, 10.0});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 6), std::vector{-10.0, -5.0, 0.0, 5.0, 10.0});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 5), std::vector{-10.0, -5.0, 0.0, 5.0, 10.0});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 4), std::vector{-10.0, -5.0, 0.0, 5.0, 10.0});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 3), std::vector{-10.0, -5.0, 0.0, 5.0, 10.0});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 2), std::vector{-10.0, 0.0, 10.0});
            CHECK_EQ(linear_ticks(-10.0, 10.0, 1), std::vector{0.0});
        }

        TEST_CASE("plausible tick step values as in d3 tests")
        {
            CHECK_EQ(tick_step(0.0, 1.0, 10), 0.1);
            CHECK_EQ(tick_step(0.0, 1.0, 9), 0.1);
            CHECK_EQ(tick_step(0.0, 1.0, 8), 0.1);
            CHECK_EQ(tick_step(0.0, 1.0, 7), 0.2);
            CHECK_EQ(tick_step(0.0, 1.0, 6), 0.2);
            CHECK_EQ(tick_step(0.0, 1.0, 5), 0.2);
            CHECK_EQ(tick_step(0.0, 1.0, 4), 0.2);
            CHECK_EQ(tick_step(0.0, 1.0, 3), 0.5);
            CHECK_EQ(tick_step(0.0, 1.0, 2), 0.5);
            CHECK_EQ(tick_step(0.0, 1.0, 1), 1.0);
            CHECK_EQ(tick_step(0.0, 10.0, 10), 1.0);
            CHECK_EQ(tick_step(0.0, 10.0, 9), 1.0);
            CHECK_EQ(tick_step(0.0, 10.0, 8), 1.0);
            CHECK_EQ(tick_step(0.0, 10.0, 7), 2.0);
            CHECK_EQ(tick_step(0.0, 10.0, 6), 2.0);
            CHECK_EQ(tick_step(0.0, 10.0, 5), 2.0);
            CHECK_EQ(tick_step(0.0, 10.0, 4), 2.0);
            CHECK_EQ(tick_step(0.0, 10.0, 3), 5.0);
            CHECK_EQ(tick_step(0.0, 10.0, 2), 5.0);
            CHECK_EQ(tick_step(0.0, 10.0, 1), 10.0);
            CHECK_EQ(tick_step(-10.0, 10.0, 10), 2.0);
            CHECK_EQ(tick_step(-10.0, 10.0, 9), 2.0);
            CHECK_EQ(tick_step(-10.0, 10.0, 8), 2.0);
            CHECK_EQ(tick_step(-10.0, 10.0, 7), 2.0);
            CHECK_EQ(tick_step(-10.0, 10.0, 6), 5.0);
            CHECK_EQ(tick_step(-10.0, 10.0, 5), 5.0);
            CHECK_EQ(tick_step(-10.0, 10.0, 4), 5.0);
            CHECK_EQ(tick_step(-10.0, 10.0, 3), 5.0);
            CHECK_EQ(tick_step(-10.0, 10.0, 2), 10.0);
            CHECK_EQ(tick_step(-10.0, 10.0, 1), 20.0);
        }

        TEST_CASE("tick step sign changes when limits are switched")
        {
            CHECK_EQ(tick_step(0.0, 1.0, 10), -tick_step(1.0, 0.0, 10));
            CHECK_EQ(tick_step(0.0, 1.0, 9), -tick_step(1.0, 0.0, 9));
            CHECK_EQ(tick_step(0.0, 1.0, 8), -tick_step(1.0, 0.0, 8));
            CHECK_EQ(tick_step(0.0, 1.0, 7), -tick_step(1.0, 0.0, 7));
            CHECK_EQ(tick_step(0.0, 1.0, 6), -tick_step(1.0, 0.0, 6));
            CHECK_EQ(tick_step(0.0, 1.0, 5), -tick_step(1.0, 0.0, 5));
            CHECK_EQ(tick_step(0.0, 1.0, 4), -tick_step(1.0, 0.0, 4));
            CHECK_EQ(tick_step(0.0, 1.0, 3), -tick_step(1.0, 0.0, 3));
            CHECK_EQ(tick_step(0.0, 1.0, 2), -tick_step(1.0, 0.0, 2));
            CHECK_EQ(tick_step(0.0, 1.0, 1), -tick_step(1.0, 0.0, 1));
            CHECK_EQ(tick_step(0.0, 10.0, 10), -tick_step(10.0, 0.0, 10));
            CHECK_EQ(tick_step(0.0, 10.0, 9), -tick_step(10.0, 0.0, 9));
            CHECK_EQ(tick_step(0.0, 10.0, 8), -tick_step(10.0, 0.0, 8));
            CHECK_EQ(tick_step(0.0, 10.0, 7), -tick_step(10.0, 0.0, 7));
            CHECK_EQ(tick_step(0.0, 10.0, 6), -tick_step(10.0, 0.0, 6));
            CHECK_EQ(tick_step(0.0, 10.0, 5), -tick_step(10.0, 0.0, 5));
            CHECK_EQ(tick_step(0.0, 10.0, 4), -tick_step(10.0, 0.0, 4));
            CHECK_EQ(tick_step(0.0, 10.0, 3), -tick_step(10.0, 0.0, 3));
            CHECK_EQ(tick_step(0.0, 10.0, 2), -tick_step(10.0, 0.0, 2));
            CHECK_EQ(tick_step(0.0, 10.0, 1), -tick_step(10.0, 0.0, 1));
            CHECK_EQ(tick_step(-10.0, 10.0, 10), -tick_step(10.0, -10.0, 10));
            CHECK_EQ(tick_step(-10.0, 10.0, 9), -tick_step(10.0, -10.0, 9));
            CHECK_EQ(tick_step(-10.0, 10.0, 8), -tick_step(10.0, -10.0, 8));
            CHECK_EQ(tick_step(-10.0, 10.0, 7), -tick_step(10.0, -10.0, 7));
            CHECK_EQ(tick_step(-10.0, 10.0, 6), -tick_step(10.0, -10.0, 6));
            CHECK_EQ(tick_step(-10.0, 10.0, 5), -tick_step(10.0, -10.0, 5));
            CHECK_EQ(tick_step(-10.0, 10.0, 4), -tick_step(10.0, -10.0, 4));
            CHECK_EQ(tick_step(-10.0, 10.0, 3), -tick_step(10.0, -10.0, 3));
            CHECK_EQ(tick_step(-10.0, 10.0, 2), -tick_step(10.0, -10.0, 2));
            CHECK_EQ(tick_step(-10.0, 10.0, 1), -tick_step(10.0, -10.0, 1));
        }

        TEST_CASE("tick format precision")
        {
            CHECK_EQ(linear_tick_format_precision(0.0, 1.0, 5), 1);
            CHECK_EQ(linear_tick_format_precision(0.0, 1.0, 15), 2);
            CHECK_EQ(linear_tick_format_precision(0.0, 1.0, 150), 3);
            CHECK_EQ(linear_tick_format_precision(0.0, 10.0, 5), 0);
            CHECK_EQ(linear_tick_format_precision(0.0, 100.0, 10000), 2);
        }
    }
}
