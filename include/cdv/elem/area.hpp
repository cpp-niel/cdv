#pragma once

#include <cdv/elem/fill_properties.hpp>
#include <cdv/core/vec2.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/repeat_n.hpp>
#include <range/v3/view/reverse.hpp>
#include <utility>

namespace cdv::elem
{
    template <stdx::range_of<pixels> XRange, stdx::range_of<pixels> YRange>
    struct area
    {
        area(const XRange& xs, const YRange& ys, fill_properties fill)
            : xs(xs), ys(ys), fill(std::move(fill))
        {
            if (ranges::distance(xs) != ranges::distance(ys))
                throw std::invalid_argument("The number of x and y coordinates that define an area must be equal");
        }
        XRange xs;
        YRange ys;
        fill_properties fill;
    };

    template <ranges::range XRange, ranges::range YRange, typename Surface>
    void draw(const area<XRange, YRange>& a, Surface& surface, const pixel_pos&)
    {
        using namespace units_literals;
        namespace rv = ::ranges::views;
        surface.set_color(a.fill.color);
        surface.draw_path(rv::zip_with(make_pos, a.xs, a.ys) | rv::reverse);
        surface.fill();

        if (a.fill.outline.width > 0_pt)
        {
            surface.set_line_properties(a.fill.outline);
            surface.draw_path(rv::zip_with(make_pos, a.xs, a.ys) | rv::reverse);
            surface.stroke();
        }
    }

    template <ranges::range XRange, ranges::range TopYRange, ranges::range BaseYRange>
    requires std::is_same_v<ranges::range_value_type_t<TopYRange>, ranges::range_value_type_t<BaseYRange>> auto
    fill_between(const XRange& xs, const TopYRange& top_ys, const BaseYRange& base_ys,
                 const fill_properties& fill = {})
    {
        auto all_xs = (xs | ranges::to_vector);
        all_xs.insert(all_xs.end(), all_xs.rbegin(), all_xs.rend());
        auto all_ys = top_ys | ranges::to_vector;
        all_ys.insert(all_ys.end(), ranges::rbegin(base_ys), ranges::rend(base_ys));
        return area(all_xs, all_ys, fill);
    }

    template <ranges::range XRange, ranges::range YRange, typename YType = ranges::range_value_type_t<YRange>>
    auto fill_between(const XRange& xs, const YRange& ys, const YType& y, const fill_properties& fill = {})
    {
        auto all_xs = (xs | ranges::to_vector);
        all_xs.insert(all_xs.end(), {all_xs.back(), all_xs.front()});
        auto all_ys = ys | ranges::to_vector;
        all_ys.insert(all_ys.end(), {YType(y), YType(y)});
        return area(all_xs, all_ys, fill);
    }
}