#include <cdv/fnt/text_shaper.hpp>

#include <fnt/freetype_error.hpp>
#include <fnt/mfl_font_face.hpp>

#include <fontconfig/fontconfig.h>
#include <harfbuzz/hb-ft.h>
#include <mfl/layout.hpp>
#include <range/v3/algorithm/count.hpp>

namespace cdv::fnt
{
    namespace
    {
        using namespace units_literals;

        template <typename T>
        concept font_position = std::is_same_v<T, hb_position_t> || std::is_same_v<T, FT_F26Dot6>;

        pixels fixpoint_26_6_to_pixels(const font_position auto pos) { return pixels(static_cast<double>(pos) / 64.0); }

        using pattern_ptr = std::unique_ptr<FcPattern, decltype(&FcPatternDestroy)>;

        pattern_ptr create_font_config_pattern(const font_properties& properties, const points size)
        {
            // clang-format off
            return pattern_ptr(FcPatternBuild(nullptr,
                                              FC_FAMILY, FcTypeString, properties.family.c_str(),
                                              FC_SLANT, FcTypeInteger, static_cast<int>(properties.slant),
                                              FC_WEIGHT, FcTypeInteger, static_cast<int>(properties.weight),
                                              FC_WIDTH, FcTypeInteger, static_cast<int>(properties.stretch),
                                              FC_SIZE, FcTypeDouble, size.value(),
                                              nullptr), FcPatternDestroy);
            // clang-format on
        }

        void fontconfig_substitute(_FcConfig* fc_config, FcPattern* pattern, const font_properties& properties)
        {
            FcDefaultSubstitute(pattern);
            if (!FcConfigSubstitute(fc_config, pattern, FcMatchPattern))
                throw std::runtime_error(fmt::format(
                    "fontconfig failed to perform substitution due to allocation failure. Requested font was {}",
                    properties));
        }

        std::pair<std::string, int> fontconfig_match(_FcConfig* fc_config, FcPattern* pattern,
                                                     const font_properties& properties)
        {
            FcResult res = FcResultNoMatch;
            const auto font = pattern_ptr(FcFontMatch(fc_config, pattern, &res), FcPatternDestroy);
            if (font)
            {
                FcChar8* file_name = nullptr;
                if (FcPatternGetString(font.get(), FC_FILE, 0, &file_name) != FcResultMatch)
                    throw std::runtime_error(fmt::format("Could not get filename for font {}", properties));

                int face_index = 0;
                if (FcPatternGetInteger(font.get(), FC_INDEX, 0, &face_index) != FcResultMatch)
                {
                    throw std::runtime_error(fmt::format(
                        "Could not get index for font '{}'. The initial request was for {}", file_name, properties));
                }

                return {reinterpret_cast<char*>(file_name), face_index};
            }

            throw std::runtime_error(fmt::format("fontconfig failed to find a match for {}", properties));
        }

        std::pair<std::string, int> fontconfig_select_face(_FcConfig* fc_config, const font_properties& properties,
                                                           const points size)
        {
            const auto pat = create_font_config_pattern(properties, size);
            fontconfig_substitute(fc_config, pat.get(), properties);
            return fontconfig_match(fc_config, pat.get(), properties);
        }

        std::pair<std::vector<shaped_glyph>, pixel_pos>
        shape_normal_text(const std::string& text, const pixels x_offset, freetype::face_t* ft_face)
        {
            using hb_buffer_ptr = std::unique_ptr<hb_buffer_t, decltype(&hb_buffer_destroy)>;
            const auto hb_buffer = hb_buffer_ptr(hb_buffer_create(), hb_buffer_destroy);
            hb_buffer_set_direction(hb_buffer.get(), HB_DIRECTION_LTR);
            hb_buffer_set_script(hb_buffer.get(), HB_SCRIPT_LATIN);
            hb_buffer_set_language(hb_buffer.get(), hb_language_from_string("en", -1));

            hb_buffer_add_utf8(hb_buffer.get(), text.c_str(), static_cast<int>(text.length()), 0,
                               static_cast<int>(text.length()));
            const auto hb_font = std::unique_ptr<hb_font_t, decltype(&hb_font_destroy)>(
                hb_ft_font_create_referenced(ft_face), hb_font_destroy);
            hb_shape(hb_font.get(), hb_buffer.get(), nullptr, 0);

            pixel_pos extents{};

            unsigned int num_glyphs = 0;
            hb_glyph_position_t* glyph_pos = hb_buffer_get_glyph_positions(hb_buffer.get(), &num_glyphs);
            hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(hb_buffer.get(), &num_glyphs);

            auto x = x_offset;
            std::vector<shaped_glyph> glyphs(num_glyphs);
            for (auto i = 0u; i < num_glyphs; ++i)
            {
                glyphs[i].index = glyph_info[i].codepoint;
                glyphs[i].pos.x = x + fixpoint_26_6_to_pixels(glyph_pos[i].x_offset);
                glyphs[i].pos.y = -fixpoint_26_6_to_pixels(glyph_pos[i].y_offset);
                x += fixpoint_26_6_to_pixels(glyph_pos[i].x_advance);
                extents.x += fixpoint_26_6_to_pixels(glyph_pos[i].x_advance);

                FT_Load_Glyph(ft_face, glyph_info[i].codepoint, FT_LOAD_DEFAULT);
                extents.y = std::max(extents.y, fixpoint_26_6_to_pixels(ft_face->glyph->metrics.height));
            }

            return {glyphs, extents};
        }
    }

    text_shaper::text_shaper() : fc_config_(FcInitLoadConfigAndFonts())
    {
        if (!FcConfigAppFontAddDir(fc_config_, reinterpret_cast<const FcChar8*>(CDV_DATA_DIR "/fonts/ttf")))
            throw std::runtime_error("Failed to add cdv font directory to fontconfig");

        select_face({.family = "sans-serif"}, 12_pt);
    }

    void text_shaper::select_face(const font_properties& properties, const points size)
    {
        auto [file_name, face_index] = fontconfig_select_face(fc_config_, properties, size);
        face_ = &freetype_.face(file_name, face_index);
    }

    shaped_text text_shaper::shape(const std::string& text, const points size, const dots_per_inch dpi) const
    {
        freetype::set_size(face_, size, dpi);

        shaped_text result;
        if (ranges::count(text, '$') < 2)
        {
            auto [glyphs, extents] = shape_normal_text(text, 0_px, face_);
            result.runs_.emplace_back(
                shaped_text_run{.font_size = size, .freetype_face = face_, .glyphs = std::move(glyphs)});
            result.extents_ = extents;
            return result;
        }

        pixel_pos total_extents{};
        auto substring_start = 0ul;

        while (substring_start < text.length())
        {
            auto substring_end = text.find('$', substring_start + 1);
            if ((substring_end != std::string::npos) and (text[substring_start] == '$')
                and (text[substring_end] == '$'))
            {
                substring_end += 1;  // include the closing '$' in the substring
                const auto substring = text.substr(substring_start, substring_end - substring_start);
                const auto extents = shape_mathematical_formula(substring, size, total_extents.x, dpi, result);
                total_extents = {total_extents.x + extents.x, std::max(total_extents.y, extents.y)};
            }
            else
            {
                const auto substring = text.substr(substring_start, substring_end - substring_start);
                auto [glyphs, extents] = shape_normal_text(substring, total_extents.x, face_);
                result.runs_.emplace_back(
                    shaped_text_run{.font_size = size, .freetype_face = face_, .glyphs = std::move(glyphs)});
                total_extents = {total_extents.x + extents.x, std::max(total_extents.y, extents.y)};
            }

            substring_start = substring_end;
        }

        result.extents_ = total_extents;
        return result;
    }

    pixel_pos text_shaper::shape_mathematical_formula(const std::string& text, const points size, const pixels x_offset,
                                                      const dots_per_inch dpi, shaped_text& result) const
    {
        const auto create_font_face = [&](const mfl::font_family family) {
            return std::make_unique<mfl_font_face>(family, freetype_);
        };

        auto elements = mfl::layout(text.substr(1, text.length() - 2), size, create_font_face);
        if (elements.error) throw std::runtime_error(*elements.error);

        for (const auto glyph : elements.glyphs)
        {
            shaped_text_run run;
            if (!elements.lines.empty())
            {
                run.lines.reserve(elements.lines.size());
                for (const auto line : elements.lines)
                {
                    shaped_line l;
                    l.min.x = x_offset + mfl::points_to_pixels(line.x, dpi);
                    l.min.y = -mfl::points_to_pixels(line.y, dpi);
                    l.max.x = l.min.x + mfl::points_to_pixels(line.length, dpi);
                    l.max.y = l.min.y - mfl::points_to_pixels(line.thickness, dpi);
                    run.lines.push_back(l);
                }
                elements.lines.clear();
            }

            run.font_size = glyph.size;
            run.freetype_face = &freetype_.face(glyph.family);
            const auto x = x_offset + mfl::points_to_pixels(glyph.x, dpi);
            const auto y = -mfl::points_to_pixels(glyph.y, dpi);
            run.glyphs.emplace_back(shaped_glyph{.index = glyph.index, .pos = pixel_pos{x, y}});

            result.runs_.emplace_back(std::move(run));
        }

        return {mfl::points_to_pixels(elements.width, dpi), mfl::points_to_pixels(elements.height, dpi)};
    }
}