#pragma once

#include <cdv/core/units.hpp>
#include <utility>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/line_properties.hpp>

#include <range/v3/back.hpp>
#include <range/v3/front.hpp>
#include <range/v3/view/zip_with.hpp>
#include <utility>

namespace cdv::elem
{
    template <stdx::range_of<pixels> XRange, stdx::range_of<pixels> YRange>
    struct line
    {
        XRange xs;
        YRange ys;
        line_properties properties;
    };

    template <typename XRange, typename YRange>
    line(const XRange&, const YRange&, line_properties = {}) -> line<XRange, YRange>;

    template <ranges::range XRange, ranges::range YRange, typename Surface>
    void draw(const line<XRange, YRange>& ln, Surface& surface, const pixel_pos&)
    {
        surface.set_line_properties(ln.properties);
        surface.draw_path(ranges::views::zip_with(make_pos, ln.xs, ln.ys));
        surface.stroke();
    }

    inline auto hline(const pixels x0, const pixels x1, const pixels y, line_properties properties = {})
    {
        // TODO designated initializer when clang issue is fixed
        return line{std::array{x0, x1}, std::array{y, y}, std::move(properties)};
    }

    inline auto vline(const pixels x, const pixels y0, const pixels y1, line_properties properties = {})
    {
        // TODO designated initializer when clang issue is fixed
        return line{std::array{x, x}, std::array{y0, y1}, std::move(properties)};
    }
}