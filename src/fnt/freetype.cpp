#include <cdv/fnt/freetype.hpp>

#include <fnt/freetype_error.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <range/v3/algorithm/find_if.hpp>

#include <array>
#include <string>

namespace cdv::fnt
{
    namespace
    {
        FT_Library create_freetype_library()
        {
            FT_Library result = nullptr;
            if (const auto err = FT_Init_FreeType(&result); err != 0) throw std::runtime_error(ft_error_string(err));

            return result;
        }

        FT_Face create_freetype_face(freetype::library_t& ft_lib, const std::filesystem::path& font_path,
                                     const int index)
        {
            FT_Face result = nullptr;
            if (const auto err = FT_New_Face(&ft_lib, font_path.string().c_str(), FT_Long(index), &result); err != 0)
                throw std::runtime_error(ft_error_string(err));

            return result;
        }

        FT_Face create_mfl_face(freetype::library_t& ft_lib, const std::string& file_name)
        {
            const auto data_dir = std::filesystem::path(CDV_DATA_DIR);
            return create_freetype_face(ft_lib, data_dir / "fonts" / "ttf" / file_name, 0);
        }
    }

    freetype::freetype()
        : library_(create_freetype_library(), FT_Done_FreeType)
        , mfl_normal_face_(create_mfl_face(*library_, "Stix2Math.otf"), FT_Done_Face)
        , mfl_italic_face_(create_mfl_face(*library_, "Stix2Text-Italic.otf"), FT_Done_Face)
        , mfl_bold_face_(create_mfl_face(*library_, "Stix2Text-Bold.otf"), FT_Done_Face)
        , mfl_mono_face_(create_mfl_face(*library_, "DejaVuSansMono.ttf"), FT_Done_Face)
    {
    }

    freetype::face_t& freetype::face(const std::filesystem::path& font_path, const int index) const
    {
        const auto path_name = font_path.string();
        auto it = ranges::find_if(face_cache_, [&](const face_cache_entry& f) { return f.first == path_name; });
        if (it == face_cache_.end())
        {
            const auto& entry = face_cache_.emplace_back(
                path_name, face_ptr(create_freetype_face(*library_, font_path, index), FT_Done_Face));
            return *entry.second;
        }

        return *it->second;
    }

    freetype::face_t& freetype::face(const mfl::font_family family) const
    {
        if (family == mfl::font_family::italic) return *mfl_italic_face_;

        if (family == mfl::font_family::bold) return *mfl_bold_face_;

        if (family == mfl::font_family::mono) return *mfl_mono_face_;

        return *mfl_normal_face_;
    }

    void freetype::set_size(FT_Face ft_face, const points size, const dots_per_inch dpi)
    {
        const auto dpi_value = static_cast<FT_UInt>(dpi.value());
        const auto size_value = static_cast<FT_F26Dot6>(size.value() * 64);
        if (const auto err = FT_Set_Char_Size(ft_face, 0, size_value, dpi_value, dpi_value); err != 0)
            throw std::invalid_argument(ft_error_string(err));
    }
}
