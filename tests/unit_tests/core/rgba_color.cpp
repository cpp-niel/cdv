#include <cdv/core/rgba_color.hpp>

#include <doctest/doctest.h>

#include <stdexcept>

namespace cdv
{
    TEST_SUITE("rgba_color")
    {
        TEST_CASE("floating point formats")
        {
            CHECK(rgba_color(1.0, 1.0, 1.0).as_uint32() == 0xffffffff);
            CHECK(rgba_color(1.0, 1.0, 1.0, 1.0).as_uint32() == 0xffffffff);
            CHECK(rgba_color(0.0, 0.0, 0.0).as_uint32() == 0x000000ff);
            CHECK(rgba_color(0.0, 0.0, 0.0, 0.0).as_uint32() == 0x00000000);
            CHECK(rgba_color(1.0, 0.5, 0.0).as_uint32() == 0xff7f00ff);
            CHECK(rgba_color(1.0, 0.5, 0.0, 0.25).as_uint32() == 0xff7f003f);
        }

        TEST_CASE("floating point formats throw when values outside range")
        {
            CHECK_THROWS_AS(rgba_color(-0.1, 0.1, 0.1), std::domain_error);
            CHECK_THROWS_AS(rgba_color(0.1, -0.1, 0.1), std::domain_error);
            CHECK_THROWS_AS(rgba_color(0.1, 0.1, -0.1), std::domain_error);
            CHECK_THROWS_AS(rgba_color(0.1, 0.1, 0.1, -0.1), std::domain_error);
            CHECK_THROWS_AS(rgba_color(1.1, 0.1, 0.1), std::domain_error);
            CHECK_THROWS_AS(rgba_color(0.1, 1.1, 0.1), std::domain_error);
            CHECK_THROWS_AS(rgba_color(0.1, 0.1, 1.1), std::domain_error);
            CHECK_THROWS_AS(rgba_color(0.1, 0.1, 0.1, 1.1), std::domain_error);
        }

        TEST_CASE("hex string format lower case")
        {
            CHECK(rgba_color("#ffffff").as_uint32() == 0xffffffff);
            CHECK(rgba_color("#ffffffff").as_uint32() == 0xffffffff);
            CHECK(rgba_color("#000000").as_uint32() == 0x000000ff);
            CHECK(rgba_color("#00000000").as_uint32() == 0x00000000);
            CHECK(rgba_color("#ff7f00").as_uint32() == 0xff7f00ff);
            CHECK(rgba_color("#ff7f003f").as_uint32() == 0xff7f003f);
        }

        TEST_CASE("hex string format upper case")
        {
            CHECK(rgba_color("#FFFFFF").as_uint32() == 0xffffffff);
            CHECK(rgba_color("#FFFFFFFF").as_uint32() == 0xffffffff);
            CHECK(rgba_color("#000000").as_uint32() == 0x000000ff);
            CHECK(rgba_color("#00000000").as_uint32() == 0x00000000);
            CHECK(rgba_color("#FF7F00").as_uint32() == 0xff7f00ff);
            CHECK(rgba_color("#FF7F003F").as_uint32() == 0xff7f003f);
        }

        TEST_CASE("hex string format throws when input does not start with hash")
        {
            CHECK_THROWS(rgba_color("xffffff"));
        }

        TEST_CASE("hex string format throws when input has wrong length")
        {
            CHECK_THROWS(rgba_color("#"));
            CHECK_THROWS(rgba_color("#fffff"));
            CHECK_THROWS(rgba_color("#fffffff"));
            CHECK_THROWS(rgba_color("#fffffffff"));
        }

        TEST_CASE("hex string format throws when input contains invalid characters")
        {
            CHECK_THROWS(rgba_color("#xfffff"));
            CHECK_THROWS(rgba_color("#fffffx"));
            CHECK_THROWS(rgba_color("#fffffg"));
            CHECK_THROWS(rgba_color("#  ffffff"));
        }

        TEST_CASE("valid single character rgba_color strings")
        {
            CHECK(rgba_color("0").as_uint32() == 0x000000ff);
            CHECK(rgba_color("k").as_uint32() == 0x000000ff);
            CHECK(rgba_color("1").as_uint32() == 0xffffffff);
            CHECK(rgba_color("w").as_uint32() == 0xffffffff);
            CHECK(rgba_color("r").as_uint32() == 0xff0000ff);
            CHECK(rgba_color("g").as_uint32() == 0x00ff00ff);
            CHECK(rgba_color("b").as_uint32() == 0x0000ffff);
            CHECK(rgba_color("y").as_uint32() == 0xffff00ff);
            CHECK(rgba_color("m").as_uint32() == 0xff00ffff);
            CHECK(rgba_color("c").as_uint32() == 0x00ffffff);
        }

        TEST_CASE("invalid single character rgba_color strings")
        {
            CHECK_THROWS(rgba_color("q"));
            CHECK_THROWS(rgba_color("2"));
            CHECK_THROWS(rgba_color(" "));
            CHECK_THROWS(rgba_color("!"));
            CHECK_THROWS(rgba_color("x"));
        }

        TEST_CASE("css4 strings")
        {
            CHECK(rgba_color("black").as_uint32() == 0x000000ff);
            CHECK(rgba_color("Black").as_uint32() == 0x000000ff);
            CHECK(rgba_color("powderblue").as_uint32() == 0xb0e0e6ff);
            CHECK(rgba_color("PowderBlue").as_uint32() == 0xb0e0e6ff);
            CHECK(rgba_color("powderblue") == css4::powderblue);
            CHECK(rgba_color("powderblue") != css4::blue);
            CHECK(rgba_color("purple") == css4::purple);
            CHECK(rgba_color("purple") != css4::blue);
        }

        TEST_CASE("tableau strings")
        {
            CHECK(rgba_color("tab:blue").as_uint32() == 0x4e79a7ff);
            CHECK(rgba_color("tab:blue") == tab::blue);
            CHECK(rgba_color("tab:orange") == tab::orange);
            CHECK(rgba_color("tab:Orange") == tab::orange);
            CHECK(rgba_color("tab:green") != tab::orange);
        }

        TEST_CASE("with alpha creates copies with modified alpha")
        {
            CHECK_EQ(css4::black.with_alpha(0.5), rgba_color(0.0, 0.0, 0.0, 0.5));
            CHECK_EQ(css4::black.with_alpha(1.0), rgba_color(0.0, 0.0, 0.0, 1.0));
            CHECK_EQ(css4::black.with_alpha(0.0), rgba_color(0.0, 0.0, 0.0, 0.0));
        }

        TEST_CASE("with alpha creates copies with modified alpha")
        {
            CHECK_THROWS([] { [[maybe_unused]] auto x = css4::black.with_alpha(1.5); }());
        }
    }
}
