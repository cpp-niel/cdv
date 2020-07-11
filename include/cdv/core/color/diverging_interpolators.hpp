#pragma once

#include "cdv/core/color/interpolator.hpp"

namespace cdv::interpolator
{
    constexpr auto brown_blue_green = detail::color_interpolator(std::array{
        0x543005ffu,
        0x8c510affu,
        0xbf812dffu,
        0xdfc27dffu,
        0xf6e8c3ffu,
        0xf5f5f5ffu,
        0xc7eae5ffu,
        0x80cdc1ffu,
        0x35978fffu,
        0x01665effu,
        0x003c30ffu,
    });

    constexpr auto purple_red_green = detail::color_interpolator(std::array{
        0x40004bffu,
        0x762a83ffu,
        0x9970abffu,
        0xc2a5cfffu,
        0xe7d4e8ffu,
        0xf7f7f7ffu,
        0xd9f0d3ffu,
        0xa6dba0ffu,
        0x5aae61ffu,
        0x1b7837ffu,
        0x00441bffu,
    });

    constexpr auto pink_yellow_green = detail::color_interpolator(std::array{
        0x8e0152ffu,
        0xc51b7dffu,
        0xde77aeffu,
        0xf1b6daffu,
        0xfde0efffu,
        0xf7f7f7ffu,
        0xe6f5d0ffu,
        0xb8e186ffu,
        0x7fbc41ffu,
        0x4d9221ffu,
        0x276419ffu,
    });

    constexpr auto purple_orange = detail::color_interpolator(std::array{
        0x2d004bffu,
        0x542788ffu,
        0x8073acffu,
        0xb2abd2ffu,
        0xd8daebffu,
        0xf7f7f7ffu,
        0xfee0b6ffu,
        0xfdb863ffu,
        0xe08214ffu,
        0xb35806ffu,
        0x7f3b08ffu,
    });

    constexpr auto red_blue = detail::color_interpolator(std::array{
        0x67001fffu,
        0xb2182bffu,
        0xd6604dffu,
        0xf4a582ffu,
        0xfddbc7ffu,
        0xf7f7f7ffu,
        0xd1e5f0ffu,
        0x92c5deffu,
        0x4393c3ffu,
        0x2166acffu,
        0x053061ffu,
    });

    constexpr auto red_gray = detail::color_interpolator(std::array{
        0x67001fffu,
        0xb2182bffu,
        0xd6604dffu,
        0xf4a582ffu,
        0xfddbc7ffu,
        0xffffffffu,
        0xe0e0e0ffu,
        0xbababaffu,
        0x878787ffu,
        0x4d4d4dffu,
        0x1a1a1affu,
    });

    constexpr auto red_yellow_blue = detail::color_interpolator(std::array{
        0xa50026ffu,
        0xd73027ffu,
        0xf46d43ffu,
        0xfdae61ffu,
        0xfee090ffu,
        0xffffbfffu,
        0xe0f3f8ffu,
        0xabd9e9ffu,
        0x74add1ffu,
        0x4575b4ffu,
        0x313695ffu,
    });

    constexpr auto red_yellow_green = detail::color_interpolator(std::array{
        0xa50026ffu,
        0xd73027ffu,
        0xf46d43ffu,
        0xfdae61ffu,
        0xfee08bffu,
        0xffffbfffu,
        0xd9ef8bffu,
        0xa6d96affu,
        0x66bd63ffu,
        0x1a9850ffu,
        0x006837ffu,
    });

    constexpr auto spectral = detail::color_interpolator(std::array{
        0x9e0142ffu,
        0xd53e4fffu,
        0xf46d43ffu,
        0xfdae61ffu,
        0xfee08bffu,
        0xffffbfffu,
        0xe6f598ffu,
        0xabdda4ffu,
        0x66c2a5ffu,
        0x3288bdffu,
        0x5e4fa2ffu,
    });
}