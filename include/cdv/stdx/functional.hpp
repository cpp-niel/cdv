#pragma once

namespace cdv::stdx
{
    struct identity
    {
        template<typename T>
        constexpr auto operator()(T&& x) const noexcept
        {
            return std::forward<T>(x);
        }
    };
}