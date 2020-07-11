#pragma once

#include "cdv/core/date.hpp"

#include <fmt/chrono.h>

namespace fmt
{
    template <>
    struct formatter<cdv::date::year_month_day> {
        formatter<std::tm, char> tm_formatter;

        constexpr auto parse(format_parse_context& ctx) {
            return tm_formatter.parse(ctx);
        }

        template <typename FormatContext>
        auto format(const cdv::date::year_month_day& ymd, FormatContext& ctx) {
            const auto t = std::chrono::system_clock::to_time_t(cdv::date::sys_days(ymd));
            const auto* tm = std::gmtime(&t);
            return tm_formatter.format(*tm, ctx);
        }
    };
}