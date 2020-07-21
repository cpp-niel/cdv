#pragma once

#include <cdv/core/rgba_color.hpp>
#include <cdv/elem/line_properties.hpp>
#include <cdv/elem/text_properties.hpp>
#include <cdv/fnt/text_shaper.hpp>
#include <cdv/stdx/numbers.hpp>

#include <range/v3/algorithm/count.hpp>
#include <range/v3/algorithm/transform.hpp>
#include <range/v3/front.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/enumerate.hpp>

namespace cdv::fig
{
    namespace detail
    {
        template <typename BackEnd>
        class scoped_clip_rect_activation
        {
        public:
            scoped_clip_rect_activation(BackEnd& back_end, const pixel_pos origin, const pixel_pos extents)
                : back_end_(back_end)
            {
                back_end_.set_clip_rect(origin, extents);
            }

            ~scoped_clip_rect_activation() { back_end_.unset_clip_rect(); }

        private:
            BackEnd& back_end_;
        };

        template <typename BackEnd>
        class scoped_clip_rect_deactivation
        {
        public:
            explicit scoped_clip_rect_deactivation(BackEnd& back_end) : back_end_(back_end)
            {
                std::tie(origin_, extents_) = back_end_.get_clip_rect();
                back_end_.unset_clip_rect();
            }

            ~scoped_clip_rect_deactivation() { back_end_.set_clip_rect(origin_, extents_); }

        private:
            pixel_pos origin_;
            pixel_pos extents_;
            BackEnd& back_end_;
        };

        template <typename BackEnd>
        class scoped_transformation
        {
        public:
            scoped_transformation(BackEnd& back_end, const pixel_pos translation, const radians rotation_angle,
                                  const vec2<double> scaling)
                : back_end_(back_end)
            {
                back_end_.get().push_transformation(translation, rotation_angle, scaling);
            }

            ~scoped_transformation() { back_end_.get().pop_transformation(); }

        private:
            std::reference_wrapper<BackEnd> back_end_;
        };
    }

    template <typename BackEnd>
    class render_surface
    {
    public:
        render_surface(BackEnd&& back_end, const dots_per_inch dpi) : back_end_(std::move(back_end)), dpi_(dpi) {}

        void set_color(const std::uint32_t hex_value)
        {
            const auto [r, g, b, a] = rgba_color(hex_value).as_doubles();
            back_end_.set_color(r, g, b, a);
        }

        void set_color(const rgba_color color) { set_color(color.as_uint32()); }

        void draw_path(const stdx::range_of<pixel_pos> auto& positions)
        {
            if (ranges::empty(positions)) return;

            move_to(ranges::front(positions));
            for (const auto& pos : ranges::views::drop(positions, 1))
                line_to(pos);
        }

        void draw_path(const std::initializer_list<pixel_pos> positions) { draw_path(ranges::views::all(positions)); }

        void set_color_with_opacity(const rgba_color color, const double opacity)
        {
            const auto c = color.as_uint32();
            const auto o = std::uint32_t(std::lround(opacity * 255.0));
            set_color((c & 0xffffff00) | o);
        }

        void gradient_fill_rectangle(const pixel_pos min, const pixel_pos max,
                                     const stdx::range_of<std::pair<rgba_color, double>> auto& gradient_stops)
        {
            auto gradient = back_end_.create_gradient(min.x, max.x);
            for (const auto& stop : gradient_stops)
            {
                const auto [color, offset] = stop;
                const auto [r, g, b, a] = color.as_doubles();
                back_end_.add_gradient_stop(gradient, r, g, b, a, offset);
            }

            back_end_.set_fill_pattern(gradient);
            draw_path({min, {max.x, min.y}, max, {min.x, max.y}, min});
            fill();
            back_end_.unset_fill_pattern();
        }

        void set_line_width(const points width) { back_end_.set_line_width(to_pixels(width)); }

        void set_line_properties(const elem::line_properties& properties)
        {
            set_color(properties.color);
            set_line_width(properties.width);
            back_end_.set_line_properties(properties);
        }

        void fill_background(const std::uint32_t hex_value)
        {
            const auto [r, g, b, a] = rgba_color(hex_value).as_doubles();
            back_end_.fill_background(r, g, b, a);
        }

        void set_text_properties(const elem::text_properties& props)
        {
            set_color(props.color);
            font_size_ = props.font_size;
            text_shaper_.select_face(props.font, font_size_);
        }

        [[nodiscard]] auto push_transformation_in_scope(const pixel_pos translation, const radians rotation_angle,
                                                        const vec2<double> scaling)
        {
            return detail::scoped_transformation<BackEnd>{back_end_, translation, rotation_angle, scaling};
        }

        pixel_pos draw_text(const std::string& text, const pixel_pos pos, const elem::horizontal_anchor x_anchor,
                            const elem::vertical_anchor y_anchor, const radians rotation)
        {
            const auto shaped = text_shaper_.shape(text, font_size_, dpi_);

            const auto vertical_extent = text_shaper_.shape("My", font_size_, dpi_).extents().y;

            const auto anchor_x = static_cast<double>(x_anchor) / static_cast<double>(elem::horizontal_anchor::right);
            const auto anchor_y = static_cast<double>(y_anchor) / static_cast<double>(elem::vertical_anchor::top);
            const auto anchor_offset = pixel_pos{-anchor_x * shaped.extents().x, -anchor_y * vertical_extent};
            const auto xform0 = push_transformation_in_scope(pos, rotation, {1.0, 1.0});
            const auto xform1 = push_transformation_in_scope(anchor_offset, {}, {1.0, -1.0});

            draw_shaped_text(shaped);

            return shaped.extents();
        }

        [[nodiscard]] pixel_pos text_extents(const std::string& text)
        {
            return text_shaper_.shape(text, font_size_, dpi_).extents();
        }

        void move_to(const pixel_pos pos) { back_end_.move_to(pos); }

        void line_to(const pixel_pos pos) { back_end_.line_to(pos); }

        void draw_circle(const pixel_pos center, const pixels radius)
        {
            back_end_.arc(center, radius, radians(0.0), radians(stdx::numbers::tau));
        }

        void draw_arc(const pixel_pos center, const pixels radius, const radians angle0, const radians angle1)
        {
            back_end_.arc(center, radius, angle0, angle1);
        }

        void stroke() { back_end_.stroke(); }

        void fill() { back_end_.fill(); }

        [[nodiscard]] auto activate_clip_rect_in_scope(const pixel_pos origin, const pixel_pos extents)
        {
            return detail::scoped_clip_rect_activation<BackEnd>{back_end_, origin, extents};
        }

        [[nodiscard]] auto deactivate_clip_rect_in_scope()
        {
            return detail::scoped_clip_rect_deactivation<BackEnd>{back_end_};
        }

        [[nodiscard]] std::pair<pixel_pos, pixel_pos> get_clip_rect() const { return back_end_.get_clip_rect(); }

        void to_png(const std::string& file_name) { back_end_.to_png(file_name); }

        [[nodiscard]] pixels to_pixels(const points p) const { return mfl::points_to_pixels(p, dpi_); }

    private:
        void draw_shaped_text(const fnt::shaped_text& shaped)
        {
            for (const auto& run : shaped.runs())
            {
                for (const auto& l : run.lines)
                {
                    draw_path({l.min, {l.max.x, l.min.y}, l.max, {l.min.x, l.max.y}, l.min});
                    fill();
                }

                fnt::freetype::set_size(run.freetype_face, run.font_size, dpi_);
                back_end_.set_font(run.freetype_face, mfl::points_to_pixels(run.font_size, dpi_));
                back_end_.draw_glyphs(run.glyphs);
            }
        }

        fnt::text_shaper text_shaper_;
        BackEnd back_end_;
        dots_per_inch dpi_{100};
        points font_size_{12};
    };
}
