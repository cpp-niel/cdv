#pragma once

#include <cdv/stdx/concepts.hpp>

namespace cdv::stdx
{
    namespace numbers
    {
        template<floating_point T> inline constexpr T e_v = static_cast<T>(2.718281828459045235360287471352662497);
        inline constexpr double e = e_v<double>;

        template <floating_point T>
        inline constexpr T pi_v = static_cast<T>(3.141592653589793238462643383279502884);
        inline constexpr double pi = pi_v<double>;

        template <floating_point T>
        inline constexpr T tau_v = static_cast<T>(6.283185307179586476925286766559005768);
        inline constexpr double tau = tau_v<double>;
    }
}