#include <cdv/core/color/single_hue_interpolators.hpp>
#include <cdv/elem/color_legend.hpp>
#include <cdv/scl/ordinal_scale.hpp>
#include <cdv/scl/sequential_scale.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    namespace
    {
        using namespace units_literals;
        namespace rv = ::ranges::views;

        class mock_surface
        {
        public:
            void set_color(const rgba_color) { set_color_counter_++; }
            void draw_path(const stdx::range_of<pixel_pos> auto&) { draw_path_counter_++; }
            void draw_path(const std::initializer_list<pixel_pos> positions) { draw_path(rv::all(positions)); }
            void stroke() { stroke_counter_++; }
            void fill() { fill_counter_++; }
            void set_line_properties(const elem::line_properties&) {}
            void set_font(const fnt::font_properties&) {}
            void set_font_size(const points) {}

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

            [[nodiscard]] size_t set_color_counter() const { return set_color_counter_; }
            [[nodiscard]] size_t draw_text_counter() const { return draw_text_counter_; }
            [[nodiscard]] size_t draw_path_counter() const { return draw_path_counter_; }
            [[nodiscard]] size_t stroke_counter() const { return stroke_counter_; }
            [[nodiscard]] size_t fill_counter() const { return fill_counter_; }
            [[nodiscard]] size_t gradient_fill_counter() const { return gradient_fill_counter_; }

        private:
            size_t set_color_counter_ = 0;
            size_t draw_text_counter_ = 0;
            size_t draw_path_counter_ = 0;
            size_t stroke_counter_ = 0;
            size_t fill_counter_ = 0;
            size_t gradient_fill_counter_ = 0;
        };
    }

    TEST_SUITE("color legend")
    {
        TEST_CASE("with ordinal scale")
        {
            const auto s = scl::ordinal_scale(std::array{1, 2, 3}, std::array{css4::red, css4::green, css4::blue});
            const auto l = elem::color_legend<decltype(s)>{.scale = s};
            auto result = mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.fill_counter(), 3);       // three blocks
            CHECK_EQ(result.draw_text_counter(), 3);  // three labels
        }

        TEST_CASE("with sequential scale")
        {
            const auto s = scl::sequential_scale(0.0, 1.0, interpolator::reds);
            const auto l = elem::color_legend<decltype(s)>{.scale = s};
            auto result = mock_surface();
            draw(l, result, {});
            CHECK_EQ(result.gradient_fill_counter(), 1);  // the gradient filled block
            CHECK_EQ(result.draw_path_counter(), 6);  // six ticks
            CHECK_EQ(result.draw_text_counter(), 6);  // six labels
        }
    }
}