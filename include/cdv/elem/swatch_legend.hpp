#pragma once

#include <cdv/core/rgba_color.hpp>
#include <cdv/core/units.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/detail/draw_rectangle.hpp>
#include <cdv/elem/detail/legend.hpp>
#include <cdv/elem/text_properties.hpp>
#include <cdv/scl/ordinal_scale.hpp>

#include <range/v3/view/cycle.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/zip.hpp>

#include <string>

namespace cdv::elem
{
    template <typename Domain>
    struct swatch_legend
    {
        scl::ordinal_scale<Domain, rgba_color> scale;
        pixel_pos pos;
        pixels block_width{16};
        pixels block_height{16};
        std::string title;
        text_properties title_properties = {.font = {.weight = fnt::font_weights::bold}};
        pixel_pos title_offset = {pixels{0}, pixels{6}};
        text_properties label_properties = {};
        std::string label_format_specifier;
        std::vector<pixels> columns;
    };

    namespace detail
    {
        template <typename Domain, typename Surface>
        pixels draw_swatch(const Domain& value, const pixel_pos pos, const swatch_legend<Domain>& l, Surface& surface)
        {
            using namespace std::string_literals;

            surface.set_color(l.scale(value));
            detail::fill_rectangle(surface, pos, {pos.x + l.block_width, pos.y + l.block_height});

            surface.set_text_properties(l.label_properties);
            const auto specifier = l.label_format_specifier.empty() ? "{}"s : "{:"s + l.label_format_specifier + "}";
            const auto [label_width, h] =
                surface.draw_text(fmt::format(specifier, value),
                                  pos + pixel_pos{l.block_width * 1.2, l.block_height * 0.15}, {0.0, 0.0}, 0.0);

            return label_width + l.block_width * 2.2;
        }
    }

    template <typename Domain, typename Surface>
    void draw(const swatch_legend<Domain>& l, Surface& surface, const pixel_pos)
    {
        if (l.columns.empty())
        {
            auto pos = l.pos;
            for (const auto& value : l.scale.domain())
                pos.x += detail::draw_swatch(value, pos, l, surface);
        }
        else
        {
            namespace rv = ::ranges::views;
            auto pos = l.pos;
            for (const auto& [index, value_and_column] : rv::enumerate(rv::zip(l.scale.domain(), rv::cycle(l.columns))))
            {
                const auto is_first_swatch_on_new_line = ((index > 0) && (index % l.columns.size() == 0));
                if (is_first_swatch_on_new_line) pos = {l.pos.x, pos.y - l.block_height * 1.5};

                const auto& [value, column_width] = value_and_column;
                detail::draw_swatch(value, pos, l, surface);

                pos.x += column_width;
            }
        }

        detail::draw_legend_title(l, surface, {l.pos.x, l.pos.y + l.block_height});
    }
}