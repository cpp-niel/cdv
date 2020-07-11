#pragma once

#include "cdv/core/color/interpolator.hpp"

namespace cdv::interpolator
{
    constexpr auto blue_green =
        detail::color_interpolator(std::array{0xf7fcfdffu, 0xe5f5f9ffu, 0xccece6ffu, 0x99d8c9ffu, 0x66c2a4ffu,
                                              0x41ae76ffu, 0x238b45ffu, 0x006d2cffu, 0x00441bffu});

    constexpr auto blue_purple =
        detail::color_interpolator(std::array{0xf7fcfdffu, 0xe0ecf4ffu, 0xbfd3e6ffu, 0x9ebcdaffu, 0x8c96c6ffu,
                                              0x8c6bb1ffu, 0x88419dffu, 0x810f7cffu, 0x4d004bffu});

    constexpr auto green_blue =
        detail::color_interpolator(std::array{0xf7fcf0ffu, 0xe0f3dbffu, 0xccebc5ffu, 0xa8ddb5ffu, 0x7bccc4ffu,
                                              0x4eb3d3ffu, 0x2b8cbeffu, 0x0868acffu, 0x084081ffu});

    constexpr auto orange_red =
        detail::color_interpolator(std::array{0xfff7ecffu, 0xfee8c8ffu, 0xfdd49effu, 0xfdbb84ffu, 0xfc8d59ffu,
                                              0xef6548ffu, 0xd7301fffu, 0xb30000ffu, 0x7f0000ffu});

    constexpr auto purple_blue_green =
        detail::color_interpolator(std::array{0xfff7fbffu, 0xece2f0ffu, 0xd0d1e6ffu, 0xa6bddbffu, 0x67a9cfffu,
                                              0x3690c0ffu, 0x02818affu, 0x016c59ffu, 0x014636ffu});

    constexpr auto purple_blue =
        detail::color_interpolator(std::array{0xfff7fbffu, 0xece7f2ffu, 0xd0d1e6ffu, 0xa6bddbffu, 0x74a9cfffu,
                                              0x3690c0ffu, 0x0570b0ffu, 0x045a8dffu, 0x023858ffu});

    constexpr auto purple_red =
        detail::color_interpolator(std::array{0xf7f4f9ffu, 0xe7e1efffu, 0xd4b9daffu, 0xc994c7ffu, 0xdf65b0ffu,
                                              0xe7298affu, 0xce1256ffu, 0x980043ffu, 0x67001fffu});

    constexpr auto red_purple =
        detail::color_interpolator(std::array{0xfff7f3ffu, 0xfde0ddffu, 0xfcc5c0ffu, 0xfa9fb5ffu, 0xf768a1ffu,
                                              0xdd3497ffu, 0xae017effu, 0x7a0177ffu, 0x49006affu});

    constexpr auto yellow_green_blue =
        detail::color_interpolator(std::array{0xffffd9ffu, 0xedf8b1ffu, 0xc7e9b4ffu, 0x7fcdbbffu, 0x41b6c4ffu,
                                              0x1d91c0ffu, 0x225ea8ffu, 0x253494ffu, 0x081d58ffu});

    constexpr auto yellow_green =
        detail::color_interpolator(std::array{0xffffe5ffu, 0xf7fcb9ffu, 0xd9f0a3ffu, 0xaddd8effu, 0x78c679ffu,
                                              0x41ab5dffu, 0x238443ffu, 0x006837ffu, 0x004529ffu});

    constexpr auto yellow_orange_brown =
        detail::color_interpolator(std::array{0xffffe5ffu, 0xfff7bcffu, 0xfee391ffu, 0xfec44fffu, 0xfe9929ffu,
                                              0xec7014ffu, 0xcc4c02ffu, 0x993404ffu, 0x662506ffu});

    constexpr auto yellow_orange_red =
        detail::color_interpolator(std::array{0xffffccffu, 0xffeda0ffu, 0xfed976ffu, 0xfeb24cffu, 0xfd8d3cffu,
                                              0xfc4e2affu, 0xe31a1cffu, 0xbd0026ffu, 0x800026ffu});
}