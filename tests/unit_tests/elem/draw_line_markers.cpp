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

        class mock_back_end
        {
        public:
            mock_back_end(polyline_gatherer& p) : polyline_gatherer_(std::ref(p)){}
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

            void stroke() { add_polyline(); }


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

    TEST_SUITE("draw_line_markers")
    {
        TEST_CASE("draw x-markers with default attributes")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, sizes, x_marker(surface));
            CHECK(ranges::equal(result.polylines, std::vector{
                                               std::vector{pixel_pos(7_px, 7_px), pixel_pos(13_px, 13_px)},
                                               std::vector{pixel_pos(13_px, 7_px), pixel_pos(7_px, 13_px)},
                                               std::vector{pixel_pos(17_px, 17_px), pixel_pos(23_px, 23_px)},
                                               std::vector{pixel_pos(23_px, 17_px), pixel_pos(17_px, 23_px)},
                                           }));
        }

        TEST_CASE("x-markers still drawn when size is zero")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, std::vector{0_pt}, x_marker(surface));
            CHECK(result.polylines.size() == 4);
        }

        TEST_CASE("draw plus markers with default attributes")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, sizes, plus_marker(surface));
            CHECK(ranges::equal(result.polylines, std::vector{
                                               std::vector{pixel_pos(7_px, 10_px), pixel_pos(13_px, 10_px)},
                                               std::vector{pixel_pos(10_px, 7_px), pixel_pos(10_px, 13_px)},
                                               std::vector{pixel_pos(17_px, 20_px), pixel_pos(23_px, 20_px)},
                                               std::vector{pixel_pos(20_px, 17_px), pixel_pos(20_px, 23_px)},
                                           }));
        }

        TEST_CASE("draw tri markers with specific size")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, std::vector{10_pt}, tri_down_marker(surface));
            CHECK(ranges::equal(result.polylines, std::vector{
                           std::vector{pixel_pos(5_px, 15_px), pixel_pos(10_px, 10_px), pixel_pos(15_px, 15_px)},
                           std::vector{pixel_pos(10_px, 10_px), pixel_pos(10_px, 5_px)},
                           std::vector{pixel_pos(15_px, 25_px), pixel_pos(20_px, 20_px), pixel_pos(25_px, 25_px)},
                           std::vector{pixel_pos(20_px, 20_px), pixel_pos(20_px, 15_px)},
                       }));
        }

        TEST_CASE("check tri marker directions")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, tri_down_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, tri_up_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, tri_left_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, std::vector{10_pt}, tri_right_marker(surface));
            CHECK(ranges::equal(result.polylines, std::vector{
                           std::vector{pixel_pos(5_px, 15_px), pixel_pos(10_px, 10_px), pixel_pos(15_px, 15_px)},
                           std::vector{pixel_pos(10_px, 10_px), pixel_pos(10_px, 5_px)},
                           std::vector{pixel_pos(5_px, 5_px), pixel_pos(10_px, 10_px), pixel_pos(15_px, 5_px)},
                           std::vector{pixel_pos(10_px, 10_px), pixel_pos(10_px, 15_px)},
                           std::vector{pixel_pos(15_px, 15_px), pixel_pos(10_px, 10_px), pixel_pos(15_px, 5_px)},
                           std::vector{pixel_pos(10_px, 10_px), pixel_pos(5_px, 10_px)},
                           std::vector{pixel_pos(5_px, 15_px), pixel_pos(10_px, 10_px), pixel_pos(5_px, 5_px)},
                           std::vector{pixel_pos(10_px, 10_px), pixel_pos(15_px, 10_px)},
                       }));
        }

        TEST_CASE("hline and vline markers drawn on the same spot are the same as a plus marker")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}}, sizes, hline_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, sizes, vline_marker(surface));
            CHECK(ranges::equal(result.polylines, std::vector{
                                               std::vector{pixel_pos(7_px, 10_px), pixel_pos(13_px, 10_px)},
                                               std::vector{pixel_pos(10_px, 7_px), pixel_pos(10_px, 13_px)},
                                           }));
        }

        TEST_CASE("tick markers all meet in the center")
        {
            polyline_gatherer result;
            fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
            draw_all_markers(surface, path{{10_px, 10_px}}, sizes, tick_left_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, sizes, tick_right_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, sizes, tick_up_marker(surface));
            draw_all_markers(surface, path{{10_px, 10_px}}, sizes, tick_down_marker(surface));
            CHECK(ranges::equal(result.polylines, std::vector{
                                               std::vector{pixel_pos(10_px, 10_px), pixel_pos(7_px, 10_px)},
                                               std::vector{pixel_pos(10_px, 10_px), pixel_pos(13_px, 10_px)},
                                               std::vector{pixel_pos(10_px, 10_px), pixel_pos(10_px, 13_px)},
                                               std::vector{pixel_pos(10_px, 10_px), pixel_pos(10_px, 7_px)},
                                           }));
        }
    }
}
