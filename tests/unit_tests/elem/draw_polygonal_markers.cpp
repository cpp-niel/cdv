#include "cdv/core/vec2.hpp"
#include "cdv/elem/detail/draw_markers.hpp"
#include "cdv/elem/line_properties.hpp"
#include "cdv/fig/render_surface.hpp"

#include <doctest/doctest.h>
#include <range/v3/algorithm/equal.hpp>

namespace cdv::elem::detail
{
    using namespace units_literals;

    namespace
    {
        using path = std::vector<pixel_pos>;

        struct polyline_gatherer
        {
            std::vector<std::vector<pixel_pos>> polylines;
        };

        void round_polyline_coords(std::vector<std::vector<pixel_pos>>& polylines)
        {
            for (auto& pl : polylines)
            {
                for (auto& p : pl)
                {
                    p.x = pixels{std::round(p.x.value())};
                    p.y = pixels{std::round(p.y.value())};
                }
            }
        }

        class mock_back_end
        {
        public:
            mock_back_end(polyline_gatherer& p) : polyline_gatherer_(std::ref(p)) {}
            [[nodiscard]] pixels to_pixels(const points p) const { return mfl::points_to_pixels(p, 72_dpi); }

            void move_to(const pixel_pos pos)
            {
                add_polyline();
                positions.emplace_back(pos);
            }

            void set_color(double, double, double, double) {}
            void set_line_width(pixels) {}
            void set_line_properties(const line_properties&) {}
            void line_to(const pixel_pos pos) { positions.emplace_back(pos); }

            void fill() { add_polyline(); }

        private:
            void add_polyline()
            {
                if (!positions.empty())
                {
                    polyline_gatherer_.get().polylines.push_back(positions);
                    positions.clear();
                }
            }

            std::vector<pixel_pos> positions;
            std::reference_wrapper<polyline_gatherer> polyline_gatherer_;
        };

        const auto sizes = std::vector<points>();
    }

    TEST_SUITE("draw_polygonal_markers")
    {
        TEST_CASE("draw square markers with default attributes")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, sizes, square_marker(surface));
            CHECK(ranges::equal(result.polylines, std::vector{
                                                      std::vector{pixel_pos(7_px, 13_px), pixel_pos(7_px, 7_px),
                                                                  pixel_pos(13_px, 7_px), pixel_pos(13_px, 13_px)},
                                                      std::vector{pixel_pos(17_px, 23_px), pixel_pos(17_px, 17_px),
                                                                  pixel_pos(23_px, 17_px), pixel_pos(23_px, 23_px)},
                                                  }));
        }

        TEST_CASE("markers still drawn when size is zero")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, std::vector{0_pt}, square_marker(surface));
            CHECK(result.polylines.size() == 2);
        }

        TEST_CASE("draw octagon markers with specific size")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, octagon_marker(surface));
            CHECK(ranges::equal(result.polylines, std::vector<std::vector<pixel_pos>>{
                                                      std::vector{pixel_pos(5_px, 12.5_px), pixel_pos(5_px, 7.5_px),
                                                                  pixel_pos(7.5_px, 5_px), pixel_pos(12.5_px, 5_px),
                                                                  pixel_pos(15_px, 7.5_px), pixel_pos(15_px, 12.5_px),
                                                                  pixel_pos(12.5_px, 15_px), pixel_pos(7.5_px, 15_px)},
                                                  }));
        }

        TEST_CASE("draw pentagon markers with specific size")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{100_px, 100_px}}, std::vector{100_pt}, pentagon_marker(surface));
            CHECK(ranges::equal(
                result.polylines,
                std::vector<std::vector<pixel_pos>>{
                    std::vector{pixel_pos(100_px, 150_px), pixel_pos(52.5_px, 115.5_px), pixel_pos(70.5_px, 59.5_px),
                                pixel_pos(129.5_px, 59.5_px), pixel_pos(147.5_px, 115.5_px)},
                }));
        }

        TEST_CASE("draw plus filled markers with specific size")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{12_pt}, plus_filled_marker(surface));
            round_polyline_coords(result.polylines);
            CHECK(
                ranges::equal(result.polylines,
                              std::vector<std::vector<pixel_pos>>{
                                  std::vector{pixel_pos(4_px, 12_px), pixel_pos(4_px, 8_px), pixel_pos(8_px, 8_px),
                                              pixel_pos(8_px, 4_px), pixel_pos(12_px, 4_px), pixel_pos(12_px, 8_px),
                                              pixel_pos(16_px, 8_px), pixel_pos(16_px, 12_px), pixel_pos(12_px, 12_px),
                                              pixel_pos(12_px, 16_px), pixel_pos(8_px, 16_px), pixel_pos(8_px, 12_px)},
                              }));
        }

        TEST_CASE("draw star markers with specific size")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10000_px, 10000_px}}, std::vector{10000_pt}, star_marker(surface));
            CHECK(ranges::equal(result.polylines,
                                std::vector<std::vector<pixel_pos>>{
                                    std::vector{pixel_pos(10000_px, 15000_px), pixel_pos(9262.5_px, 11012.5_px),
                                                pixel_pos(5250_px, 11550_px), pixel_pos(8812.5_px, 9612.5_px),
                                                pixel_pos(7050_px, 5950_px), pixel_pos(10000_px, 8750_px),
                                                pixel_pos(12950_px, 5950_px), pixel_pos(11187.5_px, 9612.5_px),
                                                pixel_pos(14750_px, 11550_px), pixel_pos(10737.5_px, 11012.5_px)},
                                }));
        }

        TEST_CASE("draw hexagon markers with specific size")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{1000_px, 1000_px}}, std::vector{1200_pt}, hexagon1_marker(surface));
            draw_all_markers(surface, path{{2000_px, 2000_px}}, std::vector{1200_pt}, hexagon2_marker(surface));
            CHECK(ranges::equal(result.polylines,
                                std::vector<std::vector<pixel_pos>>{
                                    std::vector{pixel_pos(1000_px, 1600_px), pixel_pos(480.4_px, 1300_px),
                                                pixel_pos(480.4_px, 700_px), pixel_pos(1000_px, 400_px),
                                                pixel_pos(1519.6_px, 700_px), pixel_pos(1519.6_px, 1300_px)},
                                    std::vector{pixel_pos(1700_px, 2519.6_px), pixel_pos(1400_px, 2000_px),
                                                pixel_pos(1700_px, 1480.4_px), pixel_pos(2300_px, 1480.4_px),
                                                pixel_pos(2600_px, 2000_px), pixel_pos(2300_px, 2519.6_px)},
                                }));
        }

        TEST_CASE("draw x-filled markers with specific size")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, x_filled_marker(surface));
            CHECK(ranges::equal(
                result.polylines,
                std::vector<std::vector<pixel_pos>>{
                    std::vector{pixel_pos(10_px, 12_px), pixel_pos(7_px, 15_px), pixel_pos(5_px, 13_px),
                                pixel_pos(8_px, 10_px), pixel_pos(5_px, 7_px), pixel_pos(7_px, 5_px),
                                pixel_pos(10_px, 8_px), pixel_pos(13_px, 5_px), pixel_pos(15_px, 7_px),
                                pixel_pos(12_px, 10_px), pixel_pos(15_px, 13_px), pixel_pos(13_px, 15_px)},
                }));
        }

        TEST_CASE("check directions of triangle markers")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, triangle_down_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, triangle_up_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, triangle_left_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, triangle_right_marker(surface));
            CHECK(
                ranges::equal(result.polylines,
                              std::vector{
                                  std::vector{pixel_pos(5_px, 15_px), pixel_pos(10_px, 5_px), pixel_pos(15_px, 15_px)},
                                  std::vector{pixel_pos(10_px, 15_px), pixel_pos(5_px, 5_px), pixel_pos(15_px, 5_px)},
                                  std::vector{pixel_pos(5_px, 10_px), pixel_pos(15_px, 5_px), pixel_pos(15_px, 15_px)},
                                  std::vector{pixel_pos(5_px, 15_px), pixel_pos(5_px, 5_px), pixel_pos(15_px, 10_px)},
                              }));
        }

        TEST_CASE("draw diamond markers with specific size")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, diamond_marker(surface));
            draw_all_markers(surface, path{{20_px, 20_px}}, std::vector{10_pt}, thin_diamond_marker(surface));
            CHECK(ranges::equal(result.polylines, std::vector<std::vector<pixel_pos>>{
                                                      std::vector{pixel_pos(10_px, 15_px), pixel_pos(5_px, 10_px),
                                                                  pixel_pos(10_px, 5_px), pixel_pos(15_px, 10_px)},
                                                      std::vector{pixel_pos(20_px, 25_px), pixel_pos(17_px, 20_px),
                                                                  pixel_pos(20_px, 15_px), pixel_pos(23_px, 20_px)},
                                                  }));
        }

        TEST_CASE("check directions of caret markers")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, caret_down_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, caret_up_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, caret_left_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, caret_right_marker(surface));
            CHECK(
                ranges::equal(result.polylines,
                              std::vector{
                                  std::vector{pixel_pos(10_px, 10_px), pixel_pos(15_px, 15_px), pixel_pos(5_px, 15_px)},
                                  std::vector{pixel_pos(10_px, 10_px), pixel_pos(5_px, 5_px), pixel_pos(15_px, 5_px)},
                                  std::vector{pixel_pos(10_px, 10_px), pixel_pos(15_px, 5_px), pixel_pos(15_px, 15_px)},
                                  std::vector{pixel_pos(10_px, 10_px), pixel_pos(5_px, 15_px), pixel_pos(5_px, 5_px)},
                              }));
        }

        TEST_CASE("check directions of caret base markers")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, caret_down_base_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, caret_up_base_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, caret_left_base_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, caret_right_base_marker(surface));
            CHECK(
                ranges::equal(result.polylines,
                              std::vector{
                                  std::vector{pixel_pos(5_px, 10_px), pixel_pos(10_px, 5_px), pixel_pos(15_px, 10_px)},
                                  std::vector{pixel_pos(10_px, 15_px), pixel_pos(5_px, 10_px), pixel_pos(15_px, 10_px)},
                                  std::vector{pixel_pos(10_px, 15_px), pixel_pos(5_px, 10_px), pixel_pos(10_px, 5_px)},
                                  std::vector{pixel_pos(10_px, 15_px), pixel_pos(10_px, 5_px), pixel_pos(15_px, 10_px)},
                              }));
        }
    }
}
