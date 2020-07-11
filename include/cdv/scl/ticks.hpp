#pragma once

#include "cdv/stdx/concepts.hpp"

#include <fmt/format.h>
#include <range/v3/range_concepts.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/reverse.hpp>

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

namespace cdv::scl
{
    namespace detail
    {
        template <typename T>
        auto error_factor(const T& error)
        {
            if (error >= std::sqrt(T(50))) return T(10);
            if (error >= std::sqrt(T(10))) return T(5);
            if (error >= std::sqrt(T(2))) return T(2);

            return T(1);
        }

        template <typename T>
        auto ascending_values(const T& v0, const T& v1)  // TODO should be constrained by partially ordered
        {
            return (v0 < v1) ? std::pair{v0, v1} : std::pair{v1, v0};
        }

        auto ascending_limits(const ranges::range auto& rng)  // TODO should be range of partially ordered
        {
            return ascending_values(rng.front(), rng.back());
        }

        template <typename T>
        void fill_tick_values(const T& start, const T& increment, ranges::range auto& rng)
        {
            for (auto&& [index, value] : ranges::views::enumerate(rng))
                value = (start + static_cast<T>(index)) * increment;
        }

        template <typename T>
        std::vector<T> make_tick_vector(const T& start, const T& stop, const T& increment, const bool reverse)
        {
            auto result = std::vector<T>(static_cast<size_t>(std::ceil((stop - start) + 1)));
            if (reverse)
            {
                auto reversed_range = ranges::views::reverse(result);
                fill_tick_values(start, increment, reversed_range);
            }
            else
            {
                fill_tick_values(start, increment, result);
            }

            return result;
        }
    }

    template <typename T>
    auto tick_increment(const T start, const T stop, const size_t num_ticks_hint = 8)
    {
        assert(start <= stop);

        const auto naive_step = (stop - start) / T(std::max(num_ticks_hint, size_t(1)));
        const auto exponent = std::floor(std::log10(naive_step));
        const auto power_of_ten = std::pow(T(10), exponent);
        const auto error = naive_step / power_of_ten;
        return (exponent >= 0) ? detail::error_factor(error) * power_of_ten
                               : -std::pow(T(10), -exponent) / detail::error_factor(error);
    }

    template <typename T>
    auto snapped_tick_increment(const T start, const T stop, const size_t num_ticks_hint = 8)
    {
        auto increment = tick_increment(start, stop, num_ticks_hint);
        if (increment > 0)
        {
            const auto snapped_start = std::floor(start / increment) * increment;
            const auto snapped_stop = std::ceil(stop / increment) * increment;
            return tick_increment(snapped_start, snapped_stop, num_ticks_hint);
        }

        const auto snapped_start = std::ceil(start * increment) / increment;
        const auto snapped_stop = std::floor(stop * increment) / increment;
        return tick_increment(snapped_start, snapped_stop, num_ticks_hint);
    }

    template <typename T>
    auto tick_step(const T start, const T stop, const size_t num_ticks_hint = 8)
    {
        const auto naive_step = std::abs(stop - start) / T(std::max(size_t(1), num_ticks_hint));
        const auto power_of_ten = std::pow(T(10), std::floor(std::log10(naive_step)));
        const auto result = power_of_ten * detail::error_factor(naive_step / power_of_ten);
        return (stop < start) ? -result : result;
    }

    template <typename T>
    std::vector<T> linear_ticks(const T start, const T stop, const size_t num_ticks_hint = 8)
    {
        if (num_ticks_hint == 0) return {};
        if (start == stop) return {start};

        const auto [a, b] = detail::ascending_values(start, stop);
        const auto increment = tick_increment(a, b, num_ticks_hint);
        if (std::isinf(increment) || (increment == T(0))) return {};

        return (increment > 0) ? detail::make_tick_vector(std::ceil(a / increment), std::floor(b / increment),
                                                          increment, start > stop)
                               : detail::make_tick_vector(std::ceil(a * -increment), std::floor(b * -increment),
                                                          T(1) / -increment, start > stop);
    }

    template <typename T>
    size_t linear_tick_format_precision(const T start, const T stop, const size_t num_ticks_hint = 8)
    {
        const auto step = tick_step(start, stop, num_ticks_hint);
        const auto scientific = fmt::format("{:e}", step);
        const auto idx = scientific.find('e');
        return static_cast<size_t>((idx != std::string::npos) ? std::abs(std::stoi(scientific.substr(idx + 1))) : 0);
    }
}