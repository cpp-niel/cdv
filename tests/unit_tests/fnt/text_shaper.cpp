#include <cdv/fnt/text_shaper.hpp>

#include <doctest/doctest.h>

namespace cdv::fnt
{
    using namespace units_literals;

    TEST_SUITE("text shaper")
    {
        TEST_CASE("simple text")
        {
            const auto s = text_shaper();
            const auto result = s.shape("shape me", 12_pt, 100_dpi).runs();
            CHECK_EQ(result.size(), 1);
            CHECK_EQ(result[0].font_size, 12_pt);
            CHECK_EQ(result[0].glyphs.size(), 8);
            CHECK(result[0].lines.empty());
        }

        TEST_CASE("simple text extents")
        {
            const auto s = text_shaper();
            const auto result = s.shape("X", 12_pt, 100_dpi);
            CHECK_EQ(result.extents().x.value(), ::doctest::Approx(11.421875));
            CHECK_EQ(result.extents().y, 12_px);
        }

        TEST_CASE("with different font faces")
        {
            auto s = text_shaper();
            s.select_face({.family = "times"}, 20_pt);
            const auto result0 = s.shape("X", 20_pt, 100_dpi).runs();
            CHECK_EQ(result0.front().glyphs.size(), 1);
            CHECK_EQ(result0.front().font_size, 20_pt);
            s.select_face({.family = "sans-serif"}, 12_pt);
            const auto result1 = s.shape("X", 12_pt, 100_dpi).runs();
            CHECK_EQ(result1.front().glyphs.size(), 1);
            CHECK_EQ(result1.front().font_size, 12_pt);
        }

        TEST_CASE("mathematical formula text")
        {
            const auto s = text_shaper();
            const auto result = s.shape(R"($\frac{1}{x^2}$)", 12_pt, 100_dpi).runs();
            CHECK_EQ(result.size(), 3);
            CHECK_EQ(result[0].font_size, 12_pt);
            CHECK_EQ(result[0].glyphs.size(), 1);
            CHECK_EQ(result[0].lines.size(), 1);
            CHECK_EQ(result[1].glyphs.size(), 1);
            CHECK(result[1].lines.empty());
            CHECK_EQ(result[2].glyphs.size(), 1);
            CHECK(result[2].lines.empty());
        }

        TEST_CASE("mathematical formula text with size variants")
        {
            const auto s = text_shaper();
            const auto result = s.shape(R"($\binom{n}{k}$)", 12_pt, 100_dpi).runs();
            CHECK_EQ(result.size(), 4);
            for (const auto& r : result)
            {
                CHECK_EQ(r.font_size, 12_pt);
                CHECK_EQ(r.glyphs.size(), 1);
                CHECK_EQ(r.lines.size(), 0);
            }
        }

        TEST_CASE("mixed text")
        {
            const auto s = text_shaper();
            const auto result = s.shape("a$x$b", 12_pt, 100_dpi).runs();
            CHECK_EQ(result.size(), 3);
            CHECK_EQ(result[0].font_size, 12_pt);
            CHECK_EQ(result[0].glyphs.size(), 1);
            CHECK(result[0].lines.empty());
            CHECK_EQ(result[1].glyphs.size(), 1);
            CHECK(result[1].lines.empty());
            CHECK_EQ(result[2].glyphs.size(), 1);
            CHECK(result[2].lines.empty());
        }
    }
}