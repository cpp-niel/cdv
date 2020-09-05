#pragma once

#include <cdv/stdx/date.hpp>

#include <range/v3/algorithm/min_element.hpp>
#include <range/v3/view/transform.hpp>

#include <array>
#include <cmath>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <vector>

namespace cdv::scl
{
    namespace detail
    {
        enum class tick_interval_type
        {
            seconds,
            minutes,
            hours,
            days,
            weeks,
            months,
            years
        };

        struct tick_interval
        {
            tick_interval_type type;
            size_t number;
            std::chrono::seconds seconds;
        };

        constexpr auto tick_intervals = std::array{
            tick_interval{tick_interval_type::seconds, 1, std::chrono::seconds(1)},
            tick_interval{tick_interval_type::seconds, 5, std::chrono::seconds(5)},
            tick_interval{tick_interval_type::seconds, 15, std::chrono::seconds(15)},
            tick_interval{tick_interval_type::seconds, 30, std::chrono::seconds(30)},
            tick_interval{tick_interval_type::minutes, 1, std::chrono::minutes(1)},
            tick_interval{tick_interval_type::minutes, 5, std::chrono::minutes(5)},
            tick_interval{tick_interval_type::minutes, 15, std::chrono::minutes(15)},
            tick_interval{tick_interval_type::minutes, 30, std::chrono::minutes(30)},
            tick_interval{tick_interval_type::hours, 1, std::chrono::hours(1)},
            tick_interval{tick_interval_type::hours, 3, std::chrono::hours(3)},
            tick_interval{tick_interval_type::hours, 6, std::chrono::hours(6)},
            tick_interval{tick_interval_type::hours, 12, std::chrono::hours(12)},
            tick_interval{tick_interval_type::days, 1, date::days(1)},
            tick_interval{tick_interval_type::days, 2, date::days(2)},
            tick_interval{tick_interval_type::weeks, 1, date::weeks(1)},
            tick_interval{tick_interval_type::months, 1, date::months(1)},
            tick_interval{tick_interval_type::months, 3, date::months(3)},
            tick_interval{tick_interval_type::years, 1, date::years(1)},
        };
    }

    template <typename Clock, typename Duration, typename Codomain>
    class time_scale
    {
    public:
        using domain_t = std::chrono::time_point<Clock, Duration>;
        using codomain_t = Codomain;

        time_scale(const domain_t& x0, const domain_t& x1, const Codomain& y0, const Codomain& y1)
            : domain_({x0, x1}), codomain_({y0, y1})
        {
        }

        time_scale(const date::year_month_day& x0, const date::year_month_day& x1, const Codomain& y0, const Codomain& y1)
            : domain_({date::sys_days(x0), date::sys_days(x1)}), codomain_({y0, y1})
        {
        }

        Codomain operator()(const domain_t& x) const
        {
            auto [x0, x1] = domain_;
            auto [y0, y1] = codomain_;
            if (x0 > x1)
            {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }

            const auto t = static_cast<double>((x - x0).count()) / static_cast<double>((x1 - x0).count());
            return y0 + (t * (y1 - y0));
        }

        [[nodiscard]] auto domain() const { return domain_; }
        [[nodiscard]] auto codomain() const { return codomain_; }

        [[nodiscard]] time_scale snapped_to_grid(const size_t num_ticks_hint = 8) const
        {
            const auto [type, count, step] = tick_interval(num_ticks_hint);

            // Special case years and months because the number of seconds in a year or month is not constant
            if (type == detail::tick_interval_type::years) return snap_years();
            if (type == detail::tick_interval_type::months) return (count == 3) ? snap_quarters() : snap_months();

            // Seconds, minutes, hours, days and weeks all have a constant number of seconds so the
            // same computation can apply to all using their step which essentially defines the number
            // of seconds in the corresponding tick interval
            return (domain_[0] < domain_[1])
                       ? time_scale{rnd(domain_[0], step, std::floor), rnd(domain_[1], step, std::ceil), codomain_[0],
                                    codomain_[1]}
                       : time_scale{rnd(domain_[0], step, std::ceil), rnd(domain_[1], step, std::floor), codomain_[0],
                                    codomain_[1]};
        }

        [[nodiscard]] std::vector<domain_t> ticks(const size_t num_ticks_hint = 8) const
        {
            const auto [type, count, step_in_seconds] = tick_interval(num_ticks_hint);
            const auto step = std::chrono::duration_cast<Duration>(step_in_seconds);

            if (type == detail::tick_interval_type::years) return year_month_ticks<date::years>(count);

            if (type == detail::tick_interval_type::months) return year_month_ticks<date::months>(count);

            return (domain_[0] < domain_[1]) ? default_ticks(step, std::less_equal<>())
                                             : default_ticks(-step, std::greater_equal<>());
        }

        auto tick_formatter(const auto num_ticks_hint) const
        {
            const auto [type, count, step] = tick_interval(num_ticks_hint);
            return [type = type, count = count](const domain_t x) {
                const auto tm = Clock::to_time_t(x);
                const auto gmt = *std::gmtime(&tm);
                if (type == detail::tick_interval_type::seconds) return fmt::format("{:%M:%S}", gmt);
                if (type == detail::tick_interval_type::minutes) return fmt::format("{:%I:%M}", gmt);
                if (type == detail::tick_interval_type::hours)
                {
                    return (gmt.tm_hour < int(count)) ? fmt::format("{:%a %d}", gmt) : fmt::format("{:%I %p}", gmt);
                }
                if (type == detail::tick_interval_type::days) return fmt::format("{:%a %d}", gmt);
                if (type == detail::tick_interval_type::weeks) return fmt::format("{:%b %d}", gmt);
                if (type == detail::tick_interval_type::months)
                {
                    return (gmt.tm_mon < int(count)) ? fmt::format("{:%Y}", gmt) : fmt::format("{:%B}", gmt);
                }

                return fmt::format("{:%Y}", gmt);
            };
        }

    private:
        auto tick_interval(const size_t num_ticks_hint) const
        {
            const auto num_hint = std::max(size_t(2), num_ticks_hint) - 1;
            const auto duration = domain_[0] < domain_[1] ? domain_[1] - domain_[0] : domain_[0] - domain_[1];
            const auto naive_interval = std::chrono::duration_cast<std::chrono::seconds>(duration / num_hint);
            return *(::ranges::min_element(detail::tick_intervals, [&](const auto& i0, const auto& i1) {
                return std::chrono::abs(i0.seconds - naive_interval) < std::chrono::abs(i1.seconds - naive_interval);
            }));
        }

        auto snap_months() const
        {
            auto ym0 = date::year_month_day(std::chrono::time_point_cast<date::days>(domain_[0]));
            auto ym1 = date::year_month_day(std::chrono::time_point_cast<date::days>(domain_[1]));
            if (domain_[0] < domain_[1])
                ym1 += date::months{1};
            else
                ym0 += date::months{1};

            const auto d0 = date::year_month_day(ym0.year(), ym0.month(), date::day{1});
            const auto d1 = date::year_month_day(ym1.year(), ym1.month(), date::day{1});
            return time_scale(d0, d1, codomain_[0], codomain_[1]);
        }

        auto snap_quarters() const
        {
            auto ym0 = date::year_month_day(std::chrono::time_point_cast<date::days>(domain_[0]));
            auto ym1 = date::year_month_day(std::chrono::time_point_cast<date::days>(domain_[1]));
            if (domain_[0] < domain_[1])
            {
                ym0 -= date::months{((static_cast<unsigned>(ym0.month()) - 1) % 3)};
                ym1 += date::years{1};
            }
            else
            {
                ym0 += date::years{1};
                ym1 -= date::months{((static_cast<unsigned>(ym1.month()) - 1) % 3)};
            }

            const auto d0 = date::year_month_day(ym0.year(), date::month{1}, date::day{1});
            const auto d1 = date::year_month_day(ym1.year(), date::month{1}, date::day{1});
            return time_scale(d0, d1, codomain_[0], codomain_[1]);
        }

        auto snap_years() const
        {
            auto ym0 = date::year_month_day(std::chrono::time_point_cast<date::days>(domain_[0]));
            auto ym1 = date::year_month_day(std::chrono::time_point_cast<date::days>(domain_[1]));
            if (domain_[0] < domain_[1])
                ym1 += date::years{1};
            else
                ym0 += date::years{1};

            const auto d0 = date::year_month_day(ym0.year(), date::month{1}, date::day{1});
            const auto d1 = date::year_month_day(ym1.year(), date::month{1}, date::day{1});
            return time_scale(d0, d1, codomain_[0], codomain_[1]);
        }

        template <typename TickDuration, typename Comparison>
        auto year_month_ticks(const TickDuration& step, const Comparison compare) const
        {
            auto d = date::year_month_day(date::floor<date::days>(domain_[0]));
            const auto dy1 = date::year_month_day(date::floor<date::days>(domain_[1]));
            std::vector<domain_t> result;
            while (compare(d, dy1))
            {
                result.push_back(date::sys_days(d));
                d += step;
            }

            return result;
        }

        template <typename TickDuration>
        auto year_month_ticks(const size_t count) const
        {
            if (domain_[0] < domain_[1]) return year_month_ticks(TickDuration(count), std::less_equal<>());

            return year_month_ticks(-TickDuration(count), std::greater_equal<>());
        }

        template <typename Comparison>
        auto default_ticks(const Duration step, const Comparison compare) const
        {
            std::vector<domain_t> result;
            auto d = domain_[0];
            while (compare(d, domain_[1]))
            {
                result.push_back(d);
                d += step;
            }

            return result;
        }

        using round_t = double (*)(double);
        static auto rnd(const auto val, const auto interval, const round_t round_func)
        {
            const auto d = std::chrono::time_point_cast<std::chrono::seconds>(val);
            const auto c = static_cast<double>(interval.count());
            const auto t = round_func(static_cast<double>(d.time_since_epoch().count()) / c);
            const auto new_d = std::chrono::time_point<Clock, std::chrono::seconds>(
                std::chrono::seconds(static_cast<long long>(t * c)));
            return std::chrono::time_point_cast<Duration>(new_d);
        }

        std::array<domain_t, 2> domain_;
        std::array<Codomain, 2> codomain_;
    };

    template <typename TimePoint, typename Codomain>
    time_scale(const TimePoint& x0, const TimePoint& x1, const Codomain& y0, const Codomain& y1)
        ->time_scale<typename TimePoint::clock, typename TimePoint::duration, Codomain>;

    template <typename Codomain>
    time_scale(const date::year_month_day& x0, const date::year_month_day& x1, const Codomain& y0, const Codomain& y1)
        ->time_scale<std::chrono::system_clock, std::chrono::system_clock::duration, Codomain>;
}