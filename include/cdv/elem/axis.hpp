#pragma once

#include <cdv/core/rgba_color.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/line_properties.hpp>
#include <cdv/elem/text_properties.hpp>

#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

namespace cdv::elem
{
    enum class axis_orientation
    {
        left,
        right,
        top,
        bottom
    };

    template <typename Codomain>
    struct axis_properties
    {
        line_properties spine;
        size_t num_ticks_hint = 10;
        Codomain tick_length{6.0};
        line_properties ticks;
        Codomain grid_length{0.0};
        line_properties grid = {.color = css4::gray, .width = points{1}, .style = ":"};
        text_properties tick_labels;
        pixel_pos tick_label_offset{};
    };

    template <typename Scale>
    struct axis
    {
        using codomain_t = typename Scale::codomain_t;
        Scale scale;
        codomain_t position;
        axis_orientation orientation = axis_orientation::bottom;
        axis_properties<codomain_t> properties;
    };

    template <typename Scale, typename Codomain = typename Scale::codomain_t>
    auto left_axis(const Scale& scale, const Codomain& position, const axis_properties<Codomain>& properties = {})
    {
        return axis<Scale>{
            .scale = scale, .position = position, .orientation = axis_orientation::left, .properties = properties};
    }

    template <typename Scale, typename Codomain = typename Scale::codomain_t>
    auto right_axis(const Scale& scale, const Codomain& position, const axis_properties<Codomain>& properties = {})
    {
        return axis<Scale>{.scale = scale,
            .position = position,
            .orientation = axis_orientation::right,
            .properties = properties};
    }

    template <typename Scale, typename Codomain = typename Scale::codomain_t>
    auto top_axis(const Scale& scale, const Codomain& position, const axis_properties<Codomain>& properties = {})
    {
        return axis<Scale>{
            .scale = scale, .position = position, .orientation = axis_orientation::top, .properties = properties};
    }

    template <typename Scale, typename Codomain = typename Scale::codomain_t>
    auto bottom_axis(const Scale& scale, const Codomain& position,
                     const axis_properties<Codomain>& properties = {})
    {
        return axis<Scale>{.scale = scale,
            .position = position,
            .orientation = axis_orientation::bottom,
            .properties = properties};
    }

    namespace detail
    {
        template <typename Surface, typename Labels, typename Locations, typename Func>
        pixels draw_tick_labels(Surface& surface, const Labels& labels, const pixels y, const Locations& locations,
                                const Func& xy, const pixels tick_offset, const pixel_pos tick_label_offset)
        {
            using namespace units_literals;
            auto max_label_extent = 0_px;
            for (const auto& [tick_txt, loc] : ranges::views::zip(labels, locations))
            {
                const auto pos = xy(loc, y - (tick_offset * 2.0)) + tick_label_offset;
                const auto align = xy(0.5_px, (tick_offset < 0_px) ? 0_px : 1_px);
                const auto [text_width, text_height] =
                    surface.draw_text(tick_txt, pos, {align.x.value(), align.y.value()}, 0.0);
                const auto extents = xy(text_width, text_height);
                if (extents.y > max_label_extent) max_label_extent = extents.y;
            }

            return max_label_extent;
        }
    }

    template <typename Scale, typename Surface>
    void draw(const axis<Scale>& ax, Surface& surface, const pixel_pos&)
    {
        const auto is_horizontal =
            ((ax.orientation == axis_orientation::top) || (ax.orientation == axis_orientation::bottom));
        const auto sign =
            ((ax.orientation == axis_orientation::left) || (ax.orientation == axis_orientation::bottom)) ? 1.0 : -1.0;

        const auto xy = is_horizontal ? [](const pixels x, const pixels y) { return pixel_pos(x, y); }
                                      : [](const pixels x, const pixels y) { return pixel_pos(y, x); };

        const auto y = ax.position;
        if (ax.properties.spine.width > points{0})
        {
            surface.set_line_properties(ax.properties.spine);
            surface.draw_path(
                std::array{xy(ax.scale.codomain().front(), y), xy(ax.scale.codomain().back(), y)});
            surface.stroke();
        }

        const auto ticks = ax.scale.ticks(ax.properties.num_ticks_hint);
        const auto locations = ticks | ranges::views::transform([&](const auto val) { return ax.scale(val); });

        using namespace units_literals;
        const auto tick_offset = sign * ax.properties.tick_length;
        if (ax.properties.tick_length > 0_px)
        {
            surface.set_line_properties(ax.properties.ticks);
            for (const auto loc : locations)
            {
                surface.draw_path(std::array{xy(loc, y), xy(loc, y - tick_offset)});
            }
            surface.stroke();
        }

        if (ax.properties.grid_length > 0_px)
        {
            surface.set_line_properties(ax.properties.grid);
            const auto grid_offset = -sign * ax.properties.grid_length;
            for (const auto loc : locations)
            {
                surface.draw_path(std::array{xy(loc, y), xy(loc, y - grid_offset)});
            }
            surface.stroke();
        }

        surface.set_font_size(ax.properties.tick_labels.font_size);
        surface.set_color(ax.properties.tick_labels.color);
        surface.set_font(ax.properties.tick_labels.font);
        const auto labels = ticks | ranges::views::transform(ax.scale.tick_formatter(ax.properties.num_ticks_hint));
        detail::draw_tick_labels(surface, labels, y, locations, xy, tick_offset, ax.properties.tick_label_offset);
    }
}
