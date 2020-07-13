#pragma once

#include <cdv/core/rgba_color.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/detail/legend.hpp>
#include <cdv/elem/line_properties.hpp>
#include <cdv/elem/text_properties.hpp>
#include <cdv/fig/render_surface.hpp>
#include <cdv/fnt/font_properties.hpp>
#include <cdv/scl/band_scale.hpp>
#include <cdv/scl/linear_scale.hpp>
#include <cdv/scl/ordinal_scale.hpp>
#include <cdv/scl/sequential_scale.hpp>

#include <range/v3/view/linear_distribute.hpp>
#include <range/v3/view/transform.hpp>

namespace cdv::elem
{
    template <typename Scale>
    struct color_legend
    {
        Scale scale;
        pixel_pos pos;
        pixels width;
        pixels height;
        pixels block_height;
        std::string title;
        text_properties title_properties = {.font = {.weight = fnt::font_weights::bold}};
        pixel_pos title_offset = {pixels{0}, pixels{6}};
        double padding = 0.05;
        size_t num_ticks_hint = 6;
        line_properties tick_line_properties = {.width = points{0.75}};
        text_properties tick_label_properties = {};
        pixels tick_length{6};
    };

    namespace detail
    {
        template <typename Scale, typename Surface>
        requires std::is_same_v<std::remove_cvref_t<Scale>, scl::ordinal_scale<typename Scale::domain_t, rgba_color>>
        void draw_color_legend(const color_legend<Scale>& l, Surface& surface)
        {
            const auto bands =
                scl::band_scale(l.scale.domain(), l.pos.x, l.pos.x + l.width, {.inner_padding = l.padding});
            for (const auto& d : l.scale.domain())
            {
                surface.set_color(l.scale(d));
                const auto mid_x = bands(d);
                const auto min_x = mid_x - (bands.band_width() * 0.5);
                const auto max_x = min_x + bands.band_width();
                fig::fill_rectangle(surface, {min_x, l.pos.y + l.height - l.block_height}, {max_x, l.pos.y + l.height});

                surface.set_font_size(l.tick_label_properties.font_size);
                surface.set_color(l.tick_label_properties.color);
                surface.set_font(l.tick_label_properties.font);
                surface.draw_text(fmt::format("{}", d), {mid_x, l.pos.y}, {0.5, 0.0}, 0.0);
            }
        }

        template <typename Scale, typename Surface>
        requires std::is_same_v<std::remove_cvref_t<Scale>, scl::sequential_scale<typename Scale::domain_t, typename Scale::interpolator_t>>
        void draw_color_legend(const color_legend<Scale>& l, Surface& surface)
        {
            namespace rv = ::ranges::views;
            const auto stops = rv::linear_distribute(0.0, 1.0, 50) | rv::transform([&](const auto offset) {
                                   const auto [start, stop] = l.scale.domain();
                                   const auto x = start + (offset * (stop - start));
                                   return std::pair(l.scale(x), offset);
                               });

            surface.gradient_fill_rectangle({l.pos.x, l.pos.y + l.height - l.block_height},
                                            {l.pos.x + l.width, l.pos.y + l.height}, stops);

            surface.set_line_properties(l.tick_line_properties);
            const auto x = scl::linear_scale(l.scale.domain(), std::array{l.pos.x, l.pos.x + l.width});
            const auto ticks = l.scale.ticks(l.num_ticks_hint);
            for (const auto t : ticks)
            {
                const auto px = x(t);
                const auto py = l.pos.y + l.height;
                surface.draw_path({{px, py}, {px, py - l.block_height - l.tick_length}});
            }

            surface.stroke();

            surface.set_font_size(l.tick_label_properties.font_size);
            surface.set_color(l.tick_label_properties.color);
            surface.set_font(l.tick_label_properties.font);
            const auto label_y = l.pos.y + l.height - l.block_height - (2.0 * l.tick_length);
            const auto format = l.scale.tick_formatter(l.num_ticks_hint);
            for (const auto t : ticks)
            {
                surface.draw_text(format(t), {x(t), label_y}, {0.5, 1.0}, 0.0);
            }
        }
    }

    template <typename Scale, typename Surface>
    void draw(const color_legend<Scale>& l, Surface& surface, const pixel_pos)
    {
        detail::draw_color_legend(l, surface);
        detail::draw_legend_title(l, surface, {l.pos.x, l.pos.y + l.height});
    }
}