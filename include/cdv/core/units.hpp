#pragma once

#include <mfl/units.hpp>

namespace cdv
{
    using inches = mfl::inches;
    using pixels = mfl::pixels;
    using points = mfl::points;
    using dots_per_inch = mfl::dots_per_inch;

    using radians = mfl::detail::quantity<struct radians_tag>;

    namespace units_literals
    {
        using mfl::units_literals::operator""_in;
        using mfl::units_literals::operator""_px;
        using mfl::units_literals::operator""_pt;
        using mfl::units_literals::operator""_dpi;

        constexpr radians operator""_rad(long double x) { return radians{static_cast<double>(x)}; }
        constexpr radians operator""_rad(unsigned long long x) { return radians{static_cast<double>(x)}; }
    }
}

namespace fmt
{
    template <>
    struct formatter<cdv::radians> : mfl::unit_formatter<cdv::radians, 'r', 'a', 'd'> {};
}
