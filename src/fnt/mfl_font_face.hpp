#pragma once

#include <cdv/fnt/freetype.hpp>

#include <mfl/abstract_font_face.hpp>
#include <mfl/font_family.hpp>
#include <mfl/units.hpp>

#include FT_FREETYPE_H
#include <ft2build.h>

namespace cdv::fnt
{
    class mfl_font_face : public mfl::abstract_font_face
    {
    public:
        mfl_font_face(const mfl::font_family family, const freetype& ft);

        [[nodiscard]] mfl::math_constants constants() const override;
        [[nodiscard]] mfl::math_glyph_info glyph_info(const size_t glyph_index) const override;
        [[nodiscard]] size_t glyph_index_from_code_point(const mfl::code_point char_code, const bool use_large_variant) const override;
        [[nodiscard]] std::vector<mfl::size_variant> horizontal_size_variants(const mfl::code_point char_code) const override;
        [[nodiscard]] std::vector<mfl::size_variant> vertical_size_variants(const mfl::code_point char_code) const override;
        void set_size(const mfl::points size) override;

    private:
        FT_Face ft_face_ = nullptr;
    };
}