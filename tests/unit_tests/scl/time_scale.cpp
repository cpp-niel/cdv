#include <cdv/scl/time_scale.hpp>
#include <cdv/stdx/date.hpp>

#include <doctest/doctest.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/algorithm/equal.hpp>

#include <iostream>

namespace cdv::scl
{
    using namespace std::literals::chrono_literals;
    using namespace date::literals;

    TEST_SUITE("time scale")
    {
        TEST_CASE("correct interpolation of an hour")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = d0 + 1h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            CHECK_EQ(s(d0), 0.0);
            CHECK_EQ(s(d0 + 15min), 0.25);
            CHECK_EQ(s(d0 + 30min), 0.5);
            CHECK_EQ(s(d0 + 59min), ::doctest::Approx(0.983333));
            CHECK_EQ(s(d1), 1.0);
        }

        TEST_CASE("correct interpolation of multiple hours")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0.0s;
            const auto d1 = d0 + 10.0h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            CHECK_EQ(s(d0), 0.0);
            CHECK_EQ(s(d0 + 2.5h), 0.25);
            CHECK_EQ(s(d0 + 5.0h), 0.5);
            CHECK_EQ(s(d0 + 9.0h + 50.0min), ::doctest::Approx(0.983333));
            CHECK_EQ(s(d1), 1.0);
        }

        TEST_CASE("correct interpolation of a day")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = date::sys_days{2020_y / date::January / 02} + 0s;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            CHECK_EQ(s(d0), 0.0);
            CHECK_EQ(s(d0 + 6h), 0.25);
            CHECK_EQ(s(d0 + 12h), 0.5);
            CHECK_EQ(s(d0 + 23h), ::doctest::Approx(0.9583333));
            CHECK_EQ(s(d1), 1.0);
        }

        TEST_CASE("correct interpolation of multiple days")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 02} + 0s;
            const auto d1 = date::sys_days{2020_y / date::January / 14} + 0s;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            CHECK_EQ(s(d0), 0.0);
            CHECK_EQ(s(d0 + 3 * 24h), 0.25);
            CHECK_EQ(s(d0 + 6 * 24h), 0.5);
            CHECK_EQ(s(d0 + 11 * 24h), ::doctest::Approx(0.91666667));
            CHECK_EQ(s(d1), 1.0);
        }

        TEST_CASE("correct interpolation of multiple weeks")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = date::sys_days{2020_y / date::February / 12} + 0s;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            CHECK_EQ(s(d0), 0.0);
            CHECK_EQ(s(date::sys_days{2020_y / date::January / 11} + 12h), 0.25);
            CHECK_EQ(s(date::sys_days{2020_y / date::January / 22}), 0.5);
            CHECK_EQ(s(date::sys_days{2020_y / date::February / 11}), ::doctest::Approx(0.976190476));
            CHECK_EQ(s(d1), 1.0);
        }

        TEST_CASE("correct interpolation of multiple years")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = date::sys_days{2036_y / date::January / 01} + 0s;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            CHECK_EQ(s(d0), 0.0);
            CHECK_EQ(s(date::sys_days{2024_y / date::January / 01}), 0.25);
            CHECK_EQ(s(date::sys_days{2028_y / date::January / 01}), 0.5);
            CHECK_EQ(s(date::sys_days{2035_y / date::December / 31}), ::doctest::Approx(0.9998288843));
            CHECK_EQ(s(d1), 1.0);
        }

        TEST_CASE("interpolation is correct when start time is after end time")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = d0 - 1h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            CHECK_EQ(s(d0), 0.0);
            CHECK_EQ(s(d1 + 15min), 0.75);
            CHECK_EQ(s(d1 + 30min), 0.5);
            CHECK_EQ(s(d1 + 59min), ::doctest::Approx(0.01666667));
            CHECK_EQ(s(d1), 1.0);
        }

        TEST_CASE("extrapolates when inputs lie outside defining range")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = d0 + 1h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            CHECK_EQ(s(d0 - 15min), -0.25);
            CHECK_EQ(s(d0 - 30min), -0.5);
            CHECK_EQ(s(d1 + 15min), 1.25);
            CHECK_EQ(s(d1 + 30min), 1.5);
            CHECK_EQ(s(d1 + 60min), 2.0);
        }

        TEST_CASE("ticks in 15 min intervals over one hour")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = d0 + 1h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            const auto ticks = s.ticks(4);
            CHECK_EQ(ticks.size(), 5);
            CHECK(ranges::equal(ticks, std::array{d0, d0 + 15min, d0 + 30min, d0 + 45min, d1}));
        }

        TEST_CASE("ticks in 15 min intervals over one hour and ten minutes")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = d0 + 1h + 10min;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            const auto ticks = s.ticks(6);
            CHECK_EQ(ticks.size(), 5);
            CHECK(ranges::equal(ticks, std::array{d0, d0 + 15min, d0 + 30min, d0 + 45min, d0 + 60min}));
        }

        TEST_CASE("ticks in 12 hour intervals over three days")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = d0 + 72h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            const auto ticks = s.ticks(6);
            CHECK_EQ(ticks.size(), 7);
            CHECK(ranges::equal(ticks, std::array{d0, d0 + 12h, d0 + 24h, d0 + 36h, d0 + 48h, d0 + 60h, d1}));
        }

        TEST_CASE("ticks in 6 hour intervals over three days")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = d0 + 72h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            const auto ticks = s.ticks(14);
            CHECK_EQ(ticks.size(), 13);
            CHECK(ranges::equal(ticks, std::array{d0, d0 + 6h, d0 + 12h, d0 + 18h, d0 + 24h, d0 + 30h, d0 + 36h,
                                                  d0 + 42h, d0 + 48h, d0 + 54h, d0 + 60h, d0 + 66h, d1}));
        }

        TEST_CASE("ticks in 1 week intervals over two months")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0s; };
            const auto d0 = d(2020_y / date::January / 01);
            const auto d1 = d(2020_y / date::March / 01);
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            const auto ticks = s.ticks(8);
            CHECK_EQ(ticks.size(), 9);
            CHECK(ranges::equal(ticks, std::array{d0, d(2020_y / date::January / 8), d(2020_y / date::January / 15),
                                                  d(2020_y / date::January / 22), d(2020_y / date::January / 29),
                                                  d(2020_y / date::February / 5), d(2020_y / date::February / 12),
                                                  d(2020_y / date::February / 19), d(2020_y / date::February / 26)}));
        }

        TEST_CASE("ticks in 1 month intervals over one year")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0s; };
            const auto d0 = d(2020_y / date::January / 01);
            const auto d1 = d(2021_y / date::January / 01);
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            const auto ticks = s.ticks(10);
            CHECK_EQ(ticks.size(), 13);
            CHECK(ranges::equal(ticks, std::array{d0, d(2020_y / date::February / 01), d(2020_y / date::March / 01),
                                                  d(2020_y / date::April / 01), d(2020_y / date::May / 01),
                                                  d(2020_y / date::June / 01), d(2020_y / date::July / 01),
                                                  d(2020_y / date::August / 01), d(2020_y / date::September / 01),
                                                  d(2020_y / date::October / 01), d(2020_y / date::November / 01),
                                                  d(2020_y / date::December / 01), d1}));
        }

        TEST_CASE("ticks in 1 year intervals over five years")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0s; };
            const auto d0 = d(2020_y / date::June / 01);
            const auto d1 = d(2025_y / date::June / 01);
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            const auto ticks = s.ticks(6);
            CHECK_EQ(ticks.size(), 6);
            CHECK(ranges::equal(ticks, std::array{d0, d(2021_y / date::June / 01), d(2022_y / date::June / 01),
                                                  d(2023_y / date::June / 01), d(2024_y / date::June / 01), d1}));
        }

        TEST_CASE("day ticks in descending domain")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 6h; };
            const auto d0 = d(2020_y / date::June / 2);
            const auto d1 = d(2020_y / date::May / 28);
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            const auto ticks = s.ticks(6);
            CHECK_EQ(ticks.size(), 6);
            CHECK(ranges::equal(ticks, std::array{d0, d(2020_y / date::June / 01), d(2020_y / date::May / 31),
                                                  d(2020_y / date::May / 30), d(2020_y / date::May / 29), d1}));
        }

        TEST_CASE("month ticks in descending domain")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0h; };
            const auto d0 = date::sys_days{2020_y / date::November / 2} + 6h;
            const auto d1 = date::sys_days{2020_y / date::July / 28} + 6h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            const auto ticks = s.ticks(4);
            CHECK_EQ(ticks.size(), 4);
            CHECK(ranges::equal(ticks, std::array{d(2020_y / date::November / 02), d(2020_y / date::October / 02),
                                                  d(2020_y / date::September / 02), d(2020_y / date::August / 02)}));
        }

        TEST_CASE("year ticks in descending domain")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0h; };
            const auto d0 = d(2025_y / date::June / 10);
            const auto d1 = d(2022_y / date::April / 10);
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0);
            const auto ticks = s.ticks(4);
            CHECK_EQ(ticks.size(), 4);
            CHECK(ranges::equal(ticks, std::array{d(2025_y / date::June / 10), d(2024_y / date::June / 10),
                                                  d(2023_y / date::June / 10), d(2022_y / date::June / 10)}));
        }

        TEST_CASE("snapped to 5 second grid")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 3s;
            const auto d1 = d0 + 20s;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0).snapped_to_grid(3);
            CHECK_EQ(s.domain()[0], date::sys_days{2020_y / date::January / 01} + 0s);
            CHECK_EQ(s.domain()[1], date::sys_days{2020_y / date::January / 01} + 25s);
        }

        TEST_CASE("snapped to 3 hour grid")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 4h;
            const auto d1 = d0 + 19h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0).snapped_to_grid(6);
            CHECK_EQ(s.domain()[0], date::sys_days{2020_y / date::January / 01} + 3h);
            CHECK_EQ(s.domain()[1], date::sys_days{2020_y / date::January / 02} + 0s);
        }

        TEST_CASE("snapped to 1 month grid")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0s; };
            const auto d0 = d(2020_y / date::March / 15);
            const auto d1 = d(2020_y / date::November / 10);
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0).snapped_to_grid(8);
            CHECK_EQ(s.domain()[0], d(2020_y / date::March / 01));
            CHECK_EQ(s.domain()[1], d(2020_y / date::December / 01));
        }

        TEST_CASE("snapped to 1 year grid descending")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0s; };
            const auto d0 = d(2029_y / date::November / 01);
            const auto d1 = d(2020_y / date::November / 01);
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0).snapped_to_grid(8);
            CHECK_EQ(s.domain()[0], d(2030_y / date::January / 01));
            CHECK_EQ(s.domain()[1], d(2020_y / date::January / 01));
        }

        TEST_CASE("snapped to 1 year grid")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0s; };
            const auto d0 = d(2020_y / date::November / 01);
            const auto d1 = d(2029_y / date::November / 01);
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0).snapped_to_grid(8);
            CHECK_EQ(s.domain()[0], d(2020_y / date::January / 01));
            CHECK_EQ(s.domain()[1], d(2030_y / date::January / 01));
        }

        TEST_CASE("snapped to 3 month grid")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0s; };
            const auto d0 = d(2020_y / date::February / 01);
            const auto d1 = d(2021_y / date::November / 01);
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0).snapped_to_grid(8);
            CHECK_EQ(s.domain()[0], d(2020_y / date::January / 01));
            CHECK_EQ(s.domain()[1], d(2022_y / date::January / 01));
        }

        TEST_CASE("snapped to 3 month grid descending")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0s; };
            const auto d0 = d(2021_y / date::November / 01);
            const auto d1 = d(2020_y / date::February / 01);
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0).snapped_to_grid(8);
            CHECK_EQ(s.domain()[0], d(2022_y / date::January / 01));
            CHECK_EQ(s.domain()[1], d(2020_y / date::January / 01));
        }

        TEST_CASE("snapped to 2 day grid descending")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0h; };
            const auto d0 = d(2020_y / date::June / 8) + 6h;
            const auto d1 = d(2020_y / date::May / 28) + 10h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0).snapped_to_grid(6);
            CHECK_EQ(s.domain()[0], d(2020_y / date::June / 9));
            CHECK_EQ(s.domain()[1], d(2020_y / date::May / 28));
        }

        TEST_CASE("snapped to 1 week grid descending")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0h; };
            const auto d0 = d(2020_y / date::June / 10) + 6h;
            const auto d1 = d(2020_y / date::April / 28) + 10h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0).snapped_to_grid(6);
            CHECK_EQ(s.domain()[0], d(2020_y / date::June / 11));
            CHECK_EQ(s.domain()[1], d(2020_y / date::April / 23));
        }

        TEST_CASE("snapped to 1 month grid descending")
        {
            const auto d = [](const auto x) { return date::sys_days(x) + 0h; };
            const auto d0 = d(2020_y / date::March / 10) + 6h;
            const auto d1 = d(2019_y / date::September / 28) + 10h;
            const auto s = scl::time_scale(d0, d1, 0.0, 1.0).snapped_to_grid(6);
            CHECK_EQ(s.domain()[0], d(2020_y / date::April / 01));
            CHECK_EQ(s.domain()[1], d(2019_y / date::September / 01));
        }

        TEST_CASE("tick formatting seconds")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = d0 + 30s;
            const auto format = scl::time_scale(d0, d1, 0.0, 1.0).tick_formatter(6u);
            CHECK_EQ(format(d0), "00:00");
            CHECK_EQ(format(d0 + 5s), "00:05");
            CHECK_EQ(format(d0 + 15s), "00:15");
            CHECK_EQ(format(d1), "00:30");
            CHECK_EQ(format(d1 + 30s), "01:00");
        }

        TEST_CASE("tick formatting minutes")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = d0 + 1h;
            const auto format = scl::time_scale(d0, d1, 0.0, 1.0).tick_formatter(6u);
            CHECK_EQ(format(d0), "12:00");
            CHECK_EQ(format(d0 + 30min), "12:30");
            CHECK_EQ(format(d1), "01:00");
            CHECK_EQ(format(d1 + 30min), "01:30");
        }

        TEST_CASE("tick formatting hours")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = d0 + 10h;
            const auto format = scl::time_scale(d0, d1, 0.0, 1.0).tick_formatter(8u);
            CHECK_EQ(format(d0), "Wed 01");
            CHECK_EQ(format(d0 + 1h), "01 AM");
            CHECK_EQ(format(d1), "10 AM");
            CHECK_EQ(format(d1 + 12h), "10 PM");
        }

        TEST_CASE("tick formatting days")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = date::sys_days{2020_y / date::January / 10} + 0s;
            const auto format = scl::time_scale(d0, d1, 0.0, 1.0).tick_formatter(8u);
            CHECK_EQ(format(d0), "Wed 01");
            CHECK_EQ(format(d0 + 24h), "Thu 02");
            CHECK_EQ(format(d1), "Fri 10");
            CHECK_EQ(format(d1 + 24h), "Sat 11");
        }

        TEST_CASE("tick formatting weeks")
        {
            const auto d0 = date::sys_days{2020_y / date::January / 01} + 0s;
            const auto d1 = date::sys_days{2020_y / date::February / 15} + 0s;
            const auto format = scl::time_scale(d0, d1, 0.0, 1.0).tick_formatter(6u);
            CHECK_EQ(format(d0), "Jan 01");
            CHECK_EQ(format(d0 + 24h), "Jan 02");
            CHECK_EQ(format(d1), "Feb 15");
            CHECK_EQ(format(d1 + 24h), "Feb 16");
        }

        TEST_CASE("tick formatting months")
        {
            const auto d0 = date::sys_days{2020_y / date::August / 10} + 0s;
            const auto d1 = date::sys_days{2021_y / date::April / 10} + 0s;
            const auto format = scl::time_scale(d0, d1, 0.0, 1.0).tick_formatter(8u);
            CHECK_EQ(format(d0), "August");
            CHECK_EQ(format(d0 + date::months{3}), "November");
            CHECK_EQ(format(d1), "April");
            CHECK_EQ(format(d1 + date::months{1}), "May");
        }

        TEST_CASE("tick formatting years")
        {
            const auto d0 = date::sys_days{2020_y / date::August / 10} + 0s;
            const auto d1 = date::sys_days{2030_y / date::April / 10} + 0s;
            const auto format = scl::time_scale(d0, d1, 0.0, 1.0).tick_formatter(10u);
            CHECK_EQ(format(d0), "2020");
            CHECK_EQ(format(d0 + date::months{3}), "2020");
            CHECK_EQ(format(d0 + date::years{3}), "2023");
            CHECK_EQ(format(d1), "2030");
            CHECK_EQ(format(d1 + date::years{1}), "2031");
        }
    }
}