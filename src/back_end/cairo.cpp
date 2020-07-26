#include <cdv/back_end/cairo.hpp>

#include <cdv/elem/line_properties.hpp>
#include <cdv/fnt/text_shaper.hpp>
#include <cdv/stdx/numbers.hpp>

#include <cairo-ft.h>
#include <cairo-svg.h>
#include <cairo.h>
#include <range/v3/algorithm/transform.hpp>
#include <range/v3/view/enumerate.hpp>

#include <iostream>
#include <sstream>

namespace cdv::back_end
{
    namespace
    {
        cairo_surface_t* create_svg_surface(const pixels width, const pixels height, std::ostream& os)
        {
            const auto callback = [](void* closure, const unsigned char* data, unsigned int size) {
                std::ostringstream& os = *reinterpret_cast<std::ostringstream*>(closure);
                const auto* chars = reinterpret_cast<const std::ostringstream::char_type*>(data);
                os.write(chars, size);  // TODO error handling
                return CAIRO_STATUS_SUCCESS;
            };

            auto* result = cairo_svg_surface_create_for_stream(callback, &os, width.value(), height.value());
            cairo_svg_surface_set_document_unit(result, CAIRO_SVG_UNIT_PX);
            return result;
        }

        cairo_line_join_t get_cairo_line_join(const elem::join_style join)
        {
            if (join == elem::join_style::bevel) return CAIRO_LINE_JOIN_BEVEL;
            if (join == elem::join_style::round) return CAIRO_LINE_JOIN_ROUND;
            return CAIRO_LINE_JOIN_MITER;
        }

        cairo_line_cap_t get_cairo_line_cap(const elem::cap_style cap)
        {
            if (cap == elem::cap_style::square) return CAIRO_LINE_CAP_SQUARE;
            if (cap == elem::cap_style::round) return CAIRO_LINE_CAP_ROUND;
            return CAIRO_LINE_CAP_BUTT;
        }

        std::vector<double> dash_sequence(const elem::line_properties& attributes)
        {
            auto dashes = attributes.style.dash_sequence;
            if (attributes.cap != elem::cap_style::butt)
            {
                // in cairo the dash and gap lengths (which in plt are provided as a multiple of the line width) exclude
                // the cap sizes (which have a radius of 1 as a multiple of the line width), so e.g. if you have
                // dashes and gaps of length 4, then the two caps neighbouring a gap will fill in half of the gap and
                // visually you end up with dashes of length 6 and gaps of length 2. To prevent that we subtract the
                // space that the line caps occupy from the dashes and add it to the gaps
                ranges::transform(ranges::views::enumerate(dashes), dashes.begin(), [&](const auto p) {
                    const auto index = std::get<0>(p);
                    auto result = std::get<1>(p);
                    constexpr auto cap_radius = 1.0;
                    return ((index % 2) == 0) ? std::max(0.0001, result - cap_radius) : result + cap_radius;
                });
            }

            return dashes;
        }
    }

    cairo_static_data::~cairo_static_data() { cairo_debug_reset_static_data(); }

    cairo::cairo(const pixels width, const pixels height)
        : cairo(height, cairo_image_surface_create(CAIRO_FORMAT_ARGB32, static_cast<int>(width.value()),
                                                   static_cast<int>(height.value())))
    {
    }

    cairo::cairo(const pixels width, const pixels height, std::ostream& os)
        : cairo(height, create_svg_surface(width, height, os))
    {
    }

    cairo::cairo(const pixels height, cairo_surface_t* surface)
        : surface_(surface, cairo_surface_destroy)
        , cr_(cairo_create(surface_.get()), cairo_destroy)
        , font_face_(nullptr, cairo_font_face_destroy)
    {
        // this default transformation accounts for the fact that cairo views y = 0 to be at
        // the top of the page with y increasing downwards whereas in cdv it's the other way round
        cairo_translate(cr_.get(), 0.0, height.value());
        cairo_scale(cr_.get(), 1.0, -1.0);
    }

    void cairo::set_line_width(const pixels width) { cairo_set_line_width(cr_.get(), width.value()); }

    void cairo::set_line_properties(const elem::line_properties& attributes)
    {
        cairo_set_line_cap(cr_.get(), get_cairo_line_cap(attributes.cap));
        cairo_set_line_join(cr_.get(), get_cairo_line_join(attributes.join));
        auto dashes = dash_sequence(attributes);
        ranges::transform(dashes, dashes.begin(), [&](const auto x) { return x * attributes.width.value(); });
        cairo_set_dash(cr_.get(), dashes.data(), int(dashes.size()), 0.0);
    }

    void cairo::set_color(const double r, const double g, const double b, const double a)
    {
        cairo_set_source_rgba(cr_.get(), r, g, b, a);
    }

    void cairo::fill_background(const double r, const double g, const double b, const double a)
    {
        set_color(r, g, b, a);
        cairo_paint(cr_.get());
    }

    void cairo::move_to(const pixel_pos pos) { cairo_move_to(cr_.get(), pos.x.value(), pos.y.value()); }

    void cairo::line_to(const pixel_pos pos) { cairo_line_to(cr_.get(), pos.x.value(), pos.y.value()); }

    void cairo::arc(const pixel_pos center, const pixels radius, const radians angle0, const radians angle1)
    {
        if (angle0 < angle1)
            cairo_arc(cr_.get(), center.x.value(), center.y.value(), radius.value(), angle0.value(), angle1.value());
        else
            cairo_arc_negative(cr_.get(), center.x.value(), center.y.value(), radius.value(), angle0.value(), angle1.value());
    }

    void cairo::stroke() { cairo_stroke(cr_.get()); }

    void cairo::fill() { cairo_fill(cr_.get()); }

    cairo::fill_pattern_t cairo::create_gradient(const pixels start, const pixels end)
    {
        return {cairo_pattern_create_linear(start.value(), 0.0, end.value(), 0.0), cairo_pattern_destroy};
    }

    void cairo::add_gradient_stop(fill_pattern_t& gradient, const double r, const double g, const double b,
                                  const double a, const double offset)
    {
        cairo_pattern_add_color_stop_rgba(gradient.get(), offset, r, g, b, a);
    }

    void cairo::set_fill_pattern(fill_pattern_t& pattern) { cairo_set_source(cr_.get(), pattern.get()); }

    void cairo::unset_fill_pattern() { cairo_set_source_rgb(cr_.get(), 0.0, 0.0, 0.0); }

    void cairo::set_clip_rect(const pixel_pos origin, const pixel_pos extents)
    {
        const auto min = origin;
        const auto max = origin + extents;
        move_to(min);
        line_to({max.x, min.y});
        line_to(max);
        line_to({min.x, max.y});
        line_to(min);
        cairo_clip(cr_.get());
    }

    std::pair<pixel_pos, pixel_pos> cairo::get_clip_rect() const
    {
        std::array<double, 4> clip{};
        cairo_clip_extents(cr_.get(), &clip[0], &clip[1], &clip[2], &clip[3]);
        return {{pixels(clip[0]), pixels(clip[1])}, {pixels(clip[2]), pixels(clip[3])}};
    }

    void cairo::unset_clip_rect() { cairo_reset_clip(cr_.get()); }

    void cairo::set_font(FT_Face ft_face, const pixels size)
    {
        font_face_.reset(cairo_ft_font_face_create_for_ft_face(ft_face, 0));
        cairo_set_font_face(cr_.get(), font_face_.get());
        cairo_set_font_size(cr_.get(), size.value());
    }

    void cairo::push_transformation(const pixel_pos translation, const radians rotation_angle, const vec2<double> scale)
    {
        cairo_save(cr_.get());
        cairo_translate(cr_.get(), translation.x.value(), translation.y.value());
        cairo_rotate(cr_.get(), rotation_angle.value());
        cairo_scale(cr_.get(), scale.x, scale.y);
    }

    void cairo::pop_transformation() { cairo_restore(cr_.get()); }

    void cairo::draw_glyphs(const std::vector<fnt::shaped_glyph>& glyphs) const
    {
        std::vector<cairo_glyph_t> cairo_glyphs(glyphs.size());
        ranges::transform(glyphs, cairo_glyphs.begin(), [&](const fnt::shaped_glyph& g) -> cairo_glyph_t {
            return {.index = g.index, .x = g.pos.x.value(), .y = g.pos.y.value()};
        });

        cairo_show_glyphs(cr_.get(), cairo_glyphs.data(), static_cast<int>(cairo_glyphs.size()));
    }

    void cairo::to_png(const std::string& file_name) { cairo_surface_write_to_png(surface_.get(), file_name.c_str()); }

}