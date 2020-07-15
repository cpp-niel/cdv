#include <fnt/freetype_error.hpp>

#include <doctest/doctest.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace cdv::fnt
{
    TEST_SUITE("freetype")
    {
        TEST_CASE("error reporting")
        {
            CHECK_EQ(ft_error_string(FT_Err_Bad_Argument), "FreeType Error: bad argument");
        }
    }
}