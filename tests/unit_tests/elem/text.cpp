#include <test/mock_surface.hpp>

#include <cdv/elem/text.hpp>

#include <doctest/doctest.h>

namespace cdv::elem
{
    using namespace units_literals;

    TEST_SUITE("text")
    {
        TEST_CASE("draw with default properties")
        {
            const auto s = text("text", {1_px, 2_px});
            auto result = test::mock_surface();
            draw(s, result, {});
            CHECK_EQ(result.draw_text_counter(), 1);
            CHECK_EQ(result.set_color_counter(), 1);
        }
    }
}