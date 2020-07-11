#pragma once

#include <cdv/core/rgba_color.hpp>

namespace cdv::scheme
{
    template <size_t N, typename Interpolator>
    constexpr auto from_interpolator(const Interpolator& interp)
    {
        auto result = std::array<rgba_color, N>{};
        for (size_t i = 0; i < N; ++i)
        {
            result[i] = interp(double(i) / double(N - 1));
        }

        return result;
    }
}
