#pragma once

#include "cdv/core/rgba_color.hpp"

namespace cdv::scheme
{
    constexpr auto tableau10 = std::array{tab::blue,   tab::orange, tab::red,  tab::cyan,  tab::green,
                                          tab::yellow, tab::purple, tab::pink, tab::brown, tab::gray};

    constexpr auto original_tableau10 =
        std::array{rgba_color(0x1f77b4ffu), rgba_color(0xff7f0effu), rgba_color(0x2ca02cffu), rgba_color(0xd62728ffu),
                   rgba_color(0x9467bdffu), rgba_color(0x8c564bffu), rgba_color(0xe377c2ffu), rgba_color(0x7f7f7fffu),
                   rgba_color(0xbcbd22ffu), rgba_color(0x17becfffu)};

    constexpr auto accent = std::array{
        rgba_color(0x7fc97fffu), rgba_color(0xbeaed4ffu), rgba_color(0xfdc086ffu), rgba_color(0xffff99ffu),
        rgba_color(0x386cb0ffu), rgba_color(0xf0027fffu), rgba_color(0xbf5b17ffu), rgba_color(0x666666ffu),
    };

    constexpr auto dark2 = std::array{
        rgba_color(0x1b9e77ffu), rgba_color(0xd95f02ffu), rgba_color(0x7570b3ffu), rgba_color(0xe7298affu),
        rgba_color(0x66a61effu), rgba_color(0xe6ab02ffu), rgba_color(0xa6761dffu), rgba_color(0x666666ffu),
    };

    constexpr auto paired = std::array{
        rgba_color(0xa6cee3ffu), rgba_color(0x1f78b4ffu), rgba_color(0xb2df8affu), rgba_color(0x33a02cffu),
        rgba_color(0xfb9a99ffu), rgba_color(0xe31a1cffu), rgba_color(0xfdbf6fffu), rgba_color(0xff7f00ffu),
        rgba_color(0xcab2d6ffu), rgba_color(0x6a3d9affu), rgba_color(0xffff99ffu), rgba_color(0xb15928ffu),
    };

    constexpr auto pastel1 = std::array{
        rgba_color(0xfbb4aeffu), rgba_color(0xb3cde3ffu), rgba_color(0xccebc5ffu),
        rgba_color(0xdecbe4ffu), rgba_color(0xfed9a6ffu), rgba_color(0xffffccffu),
        rgba_color(0xe5d8bdffu), rgba_color(0xfddaecffu), rgba_color(0xf2f2f2ffu),
    };

    constexpr auto pastel2 = std::array{
        rgba_color(0xb3e2cdffu), rgba_color(0xfdcdacffu), rgba_color(0xcbd5e8ffu), rgba_color(0xf4cae4ffu),
        rgba_color(0xe6f5c9ffu), rgba_color(0xfff2aeffu), rgba_color(0xf1e2ccffu), rgba_color(0xccccccffu),
    };

    constexpr auto set1 = std::array{
        rgba_color(0xe41a1cffu), rgba_color(0x377eb8ffu), rgba_color(0x4daf4affu),
        rgba_color(0x984ea3ffu), rgba_color(0xff7f00ffu), rgba_color(0xffff33ffu),
        rgba_color(0xa65628ffu), rgba_color(0xf781bfffu), rgba_color(0x999999ffu),
    };

    constexpr auto set2 = std::array{
        rgba_color(0x66c2a5ffu), rgba_color(0xfc8d62ffu), rgba_color(0x8da0cbffu), rgba_color(0xe78ac3ffu),
        rgba_color(0xa6d854ffu), rgba_color(0xffd92fffu), rgba_color(0xe5c494ffu), rgba_color(0xb3b3b3ffu),
    };

    constexpr auto set3 = std::array{
        rgba_color(0x8dd3c7ffu), rgba_color(0xffffb3ffu), rgba_color(0xbebadaffu), rgba_color(0xfb8072ffu),
        rgba_color(0x80b1d3ffu), rgba_color(0xfdb462ffu), rgba_color(0xb3de69ffu), rgba_color(0xfccde5ffu),
        rgba_color(0xd9d9d9ffu), rgba_color(0xbc80bdffu), rgba_color(0xccebc5ffu), rgba_color(0xffed6fffu),
    };

}
