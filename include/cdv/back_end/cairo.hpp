#pragma once

#include <cdv/core/vec2.hpp>

#include <cdv/core/units.hpp>

#include <iosfwd>
#include <string>
#include <utility>

struct _cairo_surface;
struct _cairo;
struct _cairo_font_face;
struct _cairo_pattern;
struct FT_FaceRec_;

namespace cdv
{
    // TODO should avoid depending on these cdv types in the back end
    namespace fnt
    {
        struct shaped_glyph;
    }

    namespace elem
    {
        struct line_properties;
    }
}

namespace cdv::back_end
{
    struct cairo_static_data
    {
        ~cairo_static_data();
    };

    class cairo
    {
    public:
        cairo(const pixels width, const pixels height);
        cairo(const pixels width, const pixels height, std::ostream& os);

        void set_line_width(const pixels width);
        void set_line_properties(const elem::line_properties& properties);

        void set_color(const double r, const double g, const double b, const double a);
        void fill_background(const double r, const double g, const double b, const double a);

        void move_to(const pixel_pos pos);
        void line_to(const pixel_pos pos);

        void arc(const pixel_pos center, const pixels radius, const double angle0, const double angle1);

        void stroke();
        void fill();

        using fill_pattern_t = std::unique_ptr<_cairo_pattern, void (*)(_cairo_pattern*)>;
        fill_pattern_t create_gradient(const pixels start, const pixels end);
        void add_gradient_stop(fill_pattern_t& gradient, const double r, const double g, const double b, const double a, const double offset);
        void set_fill_pattern(fill_pattern_t& pattern);
        void unset_fill_pattern();

        void set_clip_rect(const pixel_pos origin, const pixel_pos extents);
        [[nodiscard]] std::pair<pixel_pos, pixel_pos> get_clip_rect() const;
        void unset_clip_rect();

        void set_font(FT_FaceRec_* ft_face, const pixels size);
        void draw_glyphs(const std::vector<fnt::shaped_glyph>& glyphs) const;

        void push_transformation(const pixel_pos translation, const double rotation_angle_in_radians,
                                 const vec2<double> scale);
        void pop_transformation();

        void to_png(const std::string& file_name);

    private:
        cairo_static_data static_data_;
        std::unique_ptr<_cairo_surface, void (*)(_cairo_surface*)> surface_;
        std::unique_ptr<_cairo, void (*)(_cairo*)> cr_;
        std::unique_ptr<_cairo_font_face, void (*)(_cairo_font_face*)> font_face_;

        cairo(const pixels height, _cairo_surface* surface);
    };
}
