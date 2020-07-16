#pragma once

#include <cdv/core/rgba_color.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/elem/line_properties.hpp>
#include <cdv/elem/text_properties.hpp>
#include <cdv/fnt/font_properties.hpp>
#include <cdv/stdx/concepts.hpp>

#include <range/v3/view/all.hpp>

#include <initializer_list>
#include <string>

namespace cdv::test
{
    class mock_surface
    {
    public:
        void set_color(const rgba_color) { set_color_counter_++; }
        void draw_path(const stdx::range_of<pixel_pos> auto&) { draw_path_counter_++; }
        void draw_path(const std::initializer_list<pixel_pos> positions) { draw_path(::ranges::views::all(positions)); }
        void circle(const pixel_pos, const pixels ) {draw_circle_counter_++;}
        void stroke() { stroke_counter_++; }
        void fill() { fill_counter_++; }
        void set_line_properties(const elem::line_properties&) {}
        void set_text_properties(const elem::text_properties&) {}

        void gradient_fill_rectangle(const pixel_pos, const pixel_pos,
                                     const stdx::range_of<std::pair<rgba_color, double>> auto&)
        {
            gradient_fill_counter_++;
        }

        pixel_pos draw_text(const std::string&, const pixel_pos, const vec2<double>, const double)
        {
            draw_text_counter_++;
            return {};
        }

        [[nodiscard]] pixels to_pixels(const points p) const { return mfl::points_to_pixels(p, dots_per_inch{100.0}); }

        [[nodiscard]] size_t set_color_counter() const { return set_color_counter_; }
        [[nodiscard]] size_t draw_text_counter() const { return draw_text_counter_; }
        [[nodiscard]] size_t draw_circle_counter() const { return draw_circle_counter_; }
        [[nodiscard]] size_t draw_path_counter() const { return draw_path_counter_; }
        [[nodiscard]] size_t stroke_counter() const { return stroke_counter_; }
        [[nodiscard]] size_t fill_counter() const { return fill_counter_; }
        [[nodiscard]] size_t gradient_fill_counter() const { return gradient_fill_counter_; }

    private:
        size_t set_color_counter_ = 0;
        size_t draw_text_counter_ = 0;
        size_t draw_circle_counter_ = 0;
        size_t draw_path_counter_ = 0;
        size_t stroke_counter_ = 0;
        size_t fill_counter_ = 0;
        size_t gradient_fill_counter_ = 0;
    };
}