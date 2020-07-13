#include <cdv/elem/area.hpp>

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

            [[nodiscard]] size_t set_color_counter() const { return set_color_counter_; }
            [[nodiscard]] size_t draw_path_counter() const { return draw_path_counter_; }
            [[nodiscard]] size_t stroke_counter() const { return stroke_counter_; }
            [[nodiscard]] size_t fill_counter() const { return fill_counter_; }

        private:
            size_t set_color_counter_ = 0;
            size_t draw_path_counter_ = 0;
            size_t stroke_counter_ = 0;
            size_t fill_counter_ = 0;
        };
    }

    TEST_SUITE("area")
    {
        TEST_CASE("draw empty area does not throw")
        {
            const auto a = area(std::vector<pixels>{}, std::vector<pixels>{}, {});
            auto result = mock_surface();
            CHECK_NOTHROW(draw(a, result, {}));
        }

        TEST_CASE("fill between with base value inserts two points to generate the base line")
        {
            const auto a = fill_between(std::array{1_px, 2_px, 3_px}, std::array{4_px, 5_px, 6_px}, 0_px);
            CHECK_EQ(a.xs.size(), 5);
            CHECK_EQ(a.xs[3], 3_px);
            CHECK_EQ(a.xs[4], 1_px);
            CHECK_EQ(a.ys.size(), 5);
            CHECK_EQ(a.ys[3], 0_px);
            CHECK_EQ(a.ys[4], 0_px);
        }

        TEST_CASE("fill between with second line reverses the order of the coordinates in the second line")
        {
            const auto a =
                fill_between(std::array{1_px, 2_px, 3_px}, std::array{4_px, 5_px, 6_px}, std::array{7_px, 8_px, 9_px});
            CHECK_EQ(a.xs.size(), 6);
            CHECK_EQ(a.xs[3], 3_px);
            CHECK_EQ(a.xs[4], 2_px);
            CHECK_EQ(a.xs[5], 1_px);
            CHECK_EQ(a.ys[3], 9_px);
            CHECK_EQ(a.ys[4], 8_px);
            CHECK_EQ(a.ys[5], 7_px);
        }

        TEST_CASE("fill between with second line throws if the number of points in the second and first lines differ")
        {
            CHECK_THROWS([[maybe_unused]] auto a = fill_between(std::array{1_px, 2_px}, std::array{3_px, 4_px},
                                                                std::array{4_px, 5_px, 6_px}));
        }

        TEST_CASE("default area fills but does not draw outline")
        {
            const auto a = fill_between(std::array{1_px, 2_px, 3_px}, std::array{4_px, 5_px, 6_px}, 0_px);
            auto result = mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.fill_counter(), 1);
            CHECK_EQ(result.stroke_counter(), 0);
        }

        TEST_CASE("draws outline when width is set")
        {
            const auto a = fill_between(std::array{1_px, 2_px, 3_px}, std::array{4_px, 5_px, 6_px}, 0_px,
                                        {.outline = {.width = 2_pt}});
            auto result = mock_surface();
            draw(a, result, {});
            CHECK_EQ(result.fill_counter(), 1);
            CHECK_EQ(result.stroke_counter(), 1);
        }
    }
}