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

        struct circle_t
        {
            circle_t(const pixel_pos pos, const pixels radius) : pos(pos), radius(radius) {}

            pixel_pos pos;
            pixels radius;
        };

        bool operator==(const circle_t& lhs, const circle_t& rhs)
        {
            return lhs.pos == rhs.pos && lhs.radius == rhs.radius;
        }

        struct circle_gatherer
        {
            std::vector<circle_t> circles;
        };

        struct mock_back_end
        {
            mock_back_end(circle_gatherer& cg) : circle_gatherer_(std::ref(cg)) {}
            [[nodiscard]] pixels to_pixels(const points p) const { return points_to_pixels(p, 72_dpi); }

            void circle(const pixel_pos pos, const pixels radius) { circle_gatherer_.get().circles.emplace_back(pos, radius); }

            std::reference_wrapper<circle_gatherer> circle_gatherer_;
        };
    }

    TEST_CASE("draw point markers with default attributes")
    {
        circle_gatherer result;
        fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
        draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, std::vector<points>{6_pt},
                         point_marker(surface));
        CHECK(ranges::equal(result.circles, std::vector{circle_t({10_px, 10_px}, 6_px * point_marker_size_factor),
                                                        circle_t({20_px, 20_px}, 6_px * point_marker_size_factor)}));
    }

    TEST_CASE("point markers still drawn when size is zero")
    {
        circle_gatherer result;
        fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
        draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, std::vector<points>{0_pt},
                         point_marker(surface));
        CHECK(result.circles.size() == 2);
    }

    TEST_CASE("point markers still drawn when size is negative")
    {
        circle_gatherer result;
        fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
        draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, std::vector<points>{-10_pt},
                         point_marker(surface));
        CHECK(result.circles.size() == 2);
    }

    TEST_CASE("positive.size is ignored when drawing pixel markers")
    {
        circle_gatherer result;
        fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
        draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, std::vector<points>{10_pt},
                         pixel_marker(surface));
        CHECK(
            ranges::equal(result.circles, std::vector{circle_t({10_px, 10_px}, 1_px), circle_t({20_px, 20_px}, 1_px)}));
    }

    TEST_CASE("pixel markers are always drawn even when.size is zero")
    {
        circle_gatherer result;
        fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
        draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, std::vector<points>{0_pt},
                         pixel_marker(surface));
        CHECK(
            ranges::equal(result.circles, std::vector{circle_t({10_px, 10_px}, 1_px), circle_t({20_px, 20_px}, 1_px)}));
    }

    TEST_CASE("circle markers are drawn with exactly their given.size")
    {
        circle_gatherer result;
        fig::render_surface<mock_back_end> surface(mock_back_end(result), 72_dpi);
        draw_all_markers(surface, path{{10_px, 10_px}, {20_px, 20_px}}, std::vector{4_pt}, circle_marker(surface));
        CHECK(
            ranges::equal(result.circles, std::vector{circle_t({10_px, 10_px}, 2_px), circle_t({20_px, 20_px}, 2_px)}));
    }
}
