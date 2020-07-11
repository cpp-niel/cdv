#pragma once

#include <cdv/core/units.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/fnt/font_properties.hpp>
#include <cdv/fnt/freetype.hpp>

#include <harfbuzz/hb.h>
#include <mfl/layout.hpp>

#include <string>

struct _FcConfig;

namespace cdv::fnt
{
    struct shaped_glyph
    {
        size_t index = 0;
        pixel_pos pos{};
    };

    struct shaped_line
    {
        pixel_pos min;
        pixel_pos max;
    };

    struct shaped_text_run
    {
        points font_size;
        freetype::face_t* freetype_face;
        std::vector<shaped_glyph> glyphs;
        std::vector<shaped_line> lines;
    };

    class shaped_text
    {
    public:
        [[nodiscard]] const std::vector<shaped_text_run>& runs() const { return runs_; }
        [[nodiscard]] pixel_pos extents() const { return extents_; }

    private:
        friend class text_shaper;

        std::vector<shaped_text_run> runs_;
        pixel_pos extents_;
    };

    class text_shaper
    {
    public:
        text_shaper();

        void select_face(const font_properties& properties, const points size);

        [[nodiscard]] shaped_text shape(const std::string& text, const points size, const dots_per_inch dpi) const;

    private:
        pixel_pos shape_mathematical_formula(const std::string& text, const points size, const pixels x_offset,
                                             const dots_per_inch dpi, shaped_text& result) const;

        freetype freetype_;
        freetype::face_t* face_ = nullptr;
        _FcConfig* fc_config_ = nullptr;
    };
}