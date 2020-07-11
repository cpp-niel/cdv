#pragma once

#include <range/v3/range/concepts.hpp>

#include <type_traits>

namespace cdv::stdx
{
    template <typename T>
    concept floating_point = std::is_floating_point_v<T>;

    template<typename Rng, typename T>
    concept range_of = ranges::range<Rng> && ranges::concepts::same_as<ranges::range_value_type_t<Rng>, T>;

    template<typename Rng, typename T>
    concept random_access_range_of = ranges::random_access_range<Rng> && ranges::concepts::same_as<ranges::range_value_type_t<Rng>, T>;

    template <typename Rng, typename T>
    concept range_of_floating_point = range_of<Rng, T>&& stdx::floating_point<T>;

    template<typename T>
    concept tuple_like = requires(T t) {
        requires std::tuple_size<T>::value >= 0;
    };
}