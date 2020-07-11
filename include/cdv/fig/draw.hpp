#pragma once

#include <cdv/core/vec2.hpp>
#include <cdv/stdx/concepts.hpp>

#include <tuple>

namespace cdv
{
    template <typename Surface>
    void draw(const stdx::tuple_like auto& t, Surface& surface, const pixel_pos& extents)
    {
        std::apply([&](const auto&... elements) { (draw(elements, surface, extents), ...); }, t);
    }

    template <typename Surface>
    void draw(const ranges::range auto& elements, Surface& surface, const pixel_pos& extents)
    {
        for (const auto& element : elements)
            draw(element, surface, extents);
    }
}
