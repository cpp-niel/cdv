#include <cdv/elem/axis.hpp>
#include <cdv/scl/linear_scale.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    namespace
    {
        using namespace units_literals;

        class mock_surface
        {
        public:
            void set_color(const rgba_color) { set_color_counter_++; }
            void draw_path(const stdx::range_of<pixel_pos> auto&) { draw_path_counter_++; }
            void stroke() { stroke_counter_++; }
            void fill() { fill_counter_++; }
            void set_line_properties(const elem::line_properties&) {}
            void set_font(const fnt::font_properties&) {}
            void set_font_size(const points) {}
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

        private:
            size_t set_color_counter_ = 0;
            size_t draw_text_counter_ = 0;
            size_t draw_path_counter_ = 0;
            size_t stroke_counter_ = 0;
            size_t fill_counter_ = 0;
        };
    }

    TEST_SUITE("axis")
    {
        TEST_CASE("right axis with default properties")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = right_axis(s, 10_px);
            auto result = mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 2);      // one for the spine, one for the ticks
            CHECK_EQ(result.draw_text_counter(), 11);  // labels for 0, 1, 2, ... , 10
        }

        TEST_CASE("left axis with default properties")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = left_axis(s, 10_px);
            auto result = mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 2);      // one for the spine, one for the ticks
            CHECK_EQ(result.draw_text_counter(), 11);  // labels for 0, 1, 2, ... , 10
        }

        TEST_CASE("top axis with default properties")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = top_axis(s, 10_px);
            auto result = mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 2);      // one for the spine, one for the ticks
            CHECK_EQ(result.draw_text_counter(), 11);  // labels for 0, 1, 2, ... , 10
        }

        TEST_CASE("bottom axis with default properties")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = bottom_axis(s, 10_px);
            auto result = mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 2);      // one for the spine, one for the ticks
            CHECK_EQ(result.draw_text_counter(), 11);  // labels for 0, 1, 2, ... , 10
        }

        TEST_CASE("without spine")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = bottom_axis(s, 10_px, {.spine = {.width = 0_pt}});
            auto result = mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 1);  // only one for the ticks
        }

        TEST_CASE("with grid")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = bottom_axis(s, 10_px, {.grid_length = 10_px});
            auto result = mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.stroke_counter(), 3);  // spine, ticks and grid
        }

        TEST_CASE("without ticks")
        {
            const auto s = scl::linear_scale(0.0, 10.0, 0_px, 10_px);
            const auto a = bottom_axis(s, 10_px, {.num_ticks_hint = 0});
            auto result = mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.draw_text_counter(), 0);
        }
    }
}