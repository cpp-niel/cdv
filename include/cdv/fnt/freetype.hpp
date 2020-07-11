#pragma once

#include "cdv/core/units.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <mfl/font_family.hpp>

#include <filesystem>
#include <memory>
#include <utility>
#include <vector>

namespace cdv::fnt
{
    class freetype
    {
    public:
        freetype();

        using library_t = std::remove_pointer_t<FT_Library>;
        using face_t = std::remove_pointer_t<FT_Face>;

        [[nodiscard]] face_t& face(const std::filesystem::path& font_path, const int index) const;
        [[nodiscard]] face_t& face(const mfl::font_family family) const;

        using library_ptr = std::unique_ptr<library_t, decltype(&FT_Done_FreeType)>;
        using face_ptr = std::unique_ptr<face_t, decltype(&FT_Done_Face)>;

        using face_cache_entry = std::pair<std::string, face_ptr>;
        using face_cache = std::vector<face_cache_entry>;

        static void set_size(FT_Face ft_face, const points size, const dots_per_inch dpi);

    private:
        library_ptr library_;
        mutable face_cache face_cache_;
        face_ptr mfl_normal_face_;
        face_ptr mfl_italic_face_;
        face_ptr mfl_bold_face_;
        face_ptr mfl_mono_face_;
    };
}
