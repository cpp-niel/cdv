#include <cdv/elem/pie_slices.hpp>
#include <cdv/stdx/functional.hpp>

#include <doctest/doctest.h>

#include <array>
#include <vector>

namespace cdv::elem
{
    TEST_SUITE("pie slices")
    {
        using namespace units_literals;

        TEST_CASE("no inputs yields no slices")
        {
            auto slices = pie_slices(std::vector<int>(), stdx::identity());
            for ([[maybe_unused]] const auto& s : slices)
            {
                FAIL_CHECK("There should not be any slices");
            }
        }

        TEST_CASE("one item of data takes up entire circle")
        {
            auto slices = pie_slices(std::array{3}, stdx::identity());
            for (const auto& s : slices)
            {
                CHECK_EQ(s.start_angle, 0_rad);
                CHECK_EQ(s.end_angle, radians(stdx::numbers::tau));
            }
        }

        TEST_CASE("two items of equal size each take up half the circle")
        {
            auto slices = pie_slices(std::array{3, 3}, stdx::identity());
            for (const auto& s : slices)
            {
                CHECK_EQ((s.end_angle - s.start_angle).value(), ::doctest::Approx(stdx::numbers::pi));
            }
        }

        TEST_CASE("two eighths, one quarter and one half")
        {
            auto slices = pie_slices(std::array{3, 3, 6, 12}, stdx::identity());
            auto it = slices.begin();
            CHECK_EQ(it->start_angle, 0_rad);
            CHECK_EQ(it->end_angle.value(), stdx::numbers::tau / 8.0);
            ++it;
            CHECK_EQ(it->start_angle.value(), stdx::numbers::tau / 8.0);
            CHECK_EQ(it->end_angle.value(), stdx::numbers::tau / 4.0);
            ++it;
            CHECK_EQ(it->start_angle.value(), stdx::numbers::tau / 4.0);
            CHECK_EQ(it->end_angle.value(), stdx::numbers::tau / 2.0);
            ++it;
            CHECK_EQ(it->start_angle.value(), stdx::numbers::tau / 2.0);
            CHECK_EQ(it->end_angle.value(), stdx::numbers::tau);
        }

        TEST_CASE("two eighths, one quarter and one half in four radians")
        {
            auto slices = pie_slices(std::array{3, 3, 6, 12}, stdx::identity(), {.end_angle = 4_rad});
            auto it = slices.begin();
            CHECK_EQ(it->start_angle, 0_rad);
            CHECK_EQ(it->end_angle, 0.5_rad);
            ++it;
            CHECK_EQ(it->start_angle, 0.5_rad);
            CHECK_EQ(it->end_angle, 1_rad);
            ++it;
            CHECK_EQ(it->start_angle, 1_rad);
            CHECK_EQ(it->end_angle, 2_rad);
            ++it;
            CHECK_EQ(it->start_angle, 2_rad);
            CHECK_EQ(it->end_angle, 4_rad);
        }

        TEST_CASE("slices are clamped to full circle when range is larger than full circle")
        {
            auto slices = pie_slices(std::array{3, 3}, stdx::identity(), {.end_angle = 7_rad});
            auto it = slices.begin();
            CHECK_EQ(it->start_angle, 0_rad);
            CHECK_EQ(it->end_angle.value(), stdx::numbers::pi);
            ++it;
            CHECK_EQ(it->start_angle.value(), stdx::numbers::pi);
            CHECK_EQ(it->end_angle.value(), stdx::numbers::tau);
        }

        TEST_CASE("slices are reversed when range is negative")
        {
            auto slices =
                pie_slices(std::array{3, 3}, stdx::identity(), {.start_angle = 4_rad, .end_angle = 0_rad});
            auto it = slices.begin();
            CHECK_EQ(it->start_angle, 4_rad);
            CHECK_EQ(it->end_angle, 2_rad);
            ++it;
            CHECK_EQ(it->start_angle, 2_rad);
            CHECK_EQ(it->end_angle, 0_rad);
        }

        TEST_CASE("two items with padding")
        {
            auto slices =
                pie_slices(std::array{3, 3}, stdx::identity(), {.end_angle = radians(5.0), .pad_angle = 0.5_rad});
            auto it = slices.begin();
            CHECK_EQ(it->start_angle, 0_rad);
            CHECK_EQ(it->end_angle, 2.5_rad);
            CHECK_EQ(it->pad_angle, 0.5_rad);
            ++it;
            CHECK_EQ(it->start_angle, 2.5_rad);
            CHECK_EQ(it->end_angle, 5_rad);
            CHECK_EQ(it->pad_angle, 0.5_rad);
        }
    }
}