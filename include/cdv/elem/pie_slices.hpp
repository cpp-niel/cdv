#pragma once

#include <cdv/core/units.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/stdx/concepts.hpp>
#include <cdv/stdx/generator.hpp>
#include <cdv/stdx/numbers.hpp>

#include <range/v3/distance.hpp>
#include <range/v3/numeric/accumulate.hpp>

namespace cdv::elem
{
    template <typename Data>
    struct pie_slice
    {
        const Data& data;
        radians start_angle;
        radians end_angle;
        radians pad_angle;
    };

    template <typename Data>
    auto centroid(const pie_slice<Data>& slice, const pixel_pos center, const pixels radius)
    {
        const auto angle = std::lerp(slice.start_angle.value(), slice.end_angle.value(), 0.5);
        return center + pixel_pos(cos(angle) * radius, sin(angle) * radius);
    }

    template <typename Data>
    auto label_angle(const pie_slice<Data>& slice)
    {
        const auto angle = (slice.start_angle + slice.end_angle) * 0.5;
        const auto is_in_right_half =
            (angle < radians(stdx::numbers::pi * 0.5)) or (angle > radians(stdx::numbers::pi * 1.5));
        return angle + (is_in_right_half ? radians(0.0) : radians(stdx::numbers::pi));
    }

    struct pie_geometry
    {
        radians start_angle{0.0};
        radians end_angle{stdx::numbers::tau};
        radians pad_angle{0.0};
    };

    template <::ranges::range DataRange, typename Data = ::ranges::range_value_type_t<DataRange>>
    cppcoro::generator<pie_slice<Data>> pie_slices(const DataRange inputs,
                                                   const ranges::invocable<Data> auto get_value,
                                                   const pie_geometry geometry = {})
    {
        using value_type = decltype(get_value(std::declval<Data>()));
        static_assert(std::is_convertible_v<value_type, double>,
                      "The values used to generate pie slices must be convertible to double");

        const auto num_inputs = static_cast<double>(ranges::distance(inputs));

        if (num_inputs > 0.0)
        {
            const auto sum = ranges::accumulate(
                inputs, 0.0, [&](const double acc, const Data& x) { return acc + static_cast<double>(get_value(x)); });

            constexpr auto full_circle = radians(stdx::numbers::tau);
            const auto total_range = std::clamp(geometry.end_angle - geometry.start_angle, -full_circle, full_circle);
            const auto padding = radians(std::min(std::abs(total_range.value()) / num_inputs, geometry.pad_angle.value()));
            const auto unit_range = total_range / sum;


            auto start_angle = geometry.start_angle;
            for (const auto& input : inputs)
            {
                const auto end_angle = start_angle + static_cast<double>(get_value(input)) * unit_range;
                co_yield pie_slice<Data>{input, start_angle, end_angle, padding};
                start_angle = end_angle;
            }
        }
    }
}