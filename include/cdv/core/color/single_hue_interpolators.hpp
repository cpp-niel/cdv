#pragma once

#include <cdv/core/color/interpolator.hpp>

namespace cdv::interpolator
{
    constexpr auto reds =
        detail::color_interpolator(std::array{0xfff5f0ffu, 0xfee0d2ffu, 0xfcbba1ffu, 0xfc9272ffu, 0xfb6a4affu,
                                              0xef3b2cffu, 0xcb181dffu, 0xa50f15ffu, 0x67000dffu});

    constexpr auto greens =
        detail::color_interpolator(std::array{0xf7fcf5ffu, 0xe5f5e0ffu, 0xc7e9c0ffu, 0xa1d99bffu, 0x74c476ffu,
                                              0x41ab5dffu, 0x238b45ffu, 0x006d2cffu, 0x00441bffu});

    constexpr auto blues =
        detail::color_interpolator(std::array{0xf7fbffffu, 0xdeebf7ffu, 0xc6dbefffu, 0x9ecae1ffu, 0x6baed6ffu,
                                              0x4292c6ffu, 0x2171b5ffu, 0x08519cffu, 0x08306bffu});

    constexpr auto grays =
        detail::color_interpolator(std::array{0xffffffffu, 0xf0f0f0ffu, 0xd9d9d9ffu, 0xbdbdbdffu, 0x969696ffu,
                                              0x737373ffu, 0x525252ffu, 0x252525ffu, 0x000000ffu});

    constexpr auto oranges =
        detail::color_interpolator(std::array{0xfff5ebffu, 0xfee6ceffu, 0xfdd0a2ffu, 0xfdae6bffu, 0xfd8d3cffu,
                                              0xf16913ffu, 0xd94801ffu, 0xa63603ffu, 0x7f2704ffu});

    constexpr auto purples =
        detail::color_interpolator(std::array{0xfcfbfdffu, 0xefedf5ffu, 0xdadaebffu, 0xbcbddcffu, 0x9e9ac8ffu,
                                              0x807dbaffu, 0x6a51a3ffu, 0x54278fffu, 0x3f007dffu});
}