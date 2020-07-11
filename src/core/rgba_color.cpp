#include "cdv/core/rgba_color.hpp"

#include <iostream>

namespace cdv::detail
{
    namespace
    {
        std::string ascii_to_lower(std::string_view s)
        {
            const auto tolower = [](char c) { return (c <= 'Z' && c >= 'A') ? c - ('Z' - 'z') : c; };
            std::string result{s};
            ranges::transform(result.begin(), result.end(), result.begin(), tolower);
            return result;
        }
    }

    std::optional<std::uint32_t> css4_string_to_uint32(std::string_view s)
    {
#define CDV_DEFINE_COLOR(name, value) \
    if (detail::ascii_to_lower(s) == #name) return value;
#include "cdv/core/color/css4.hpp"
#undef CDV_DEFINE_COLOR
        return std::nullopt;
    }

    std::optional<std::uint32_t> tableau_string_to_uint32(std::string_view s)
    {
#define CDV_DEFINE_COLOR(name, value) \
    if (detail::ascii_to_lower(s) == "tab:" #name) return value;
#include "cdv/core/color/tableau.hpp"
#undef CDV_DEFINE_COLOR
        return std::nullopt;
    }

    std::uint32_t hex_string_to_uint32(std::string_view s)
    {
        if ((s.size() != 7) && (s.size() != 9))
            throw std::invalid_argument(
                fmt::format("The hex string '{}' does not have six (rgb) or eight (rgba) hexadecimal digits", s));

        std::uint32_t result = 0;
        const auto* first = s.data() + 1;
        const auto* last = s.data() + s.size();
        const auto [p, ec] = std::from_chars(first, last, result, 16);
        if ((ec == std::errc::invalid_argument) || (p != last) || std::isspace(s[1]))
            throw std::invalid_argument(fmt::format("'{}' does not represent a color in hex string format", s));

        if (ec == std::errc::result_out_of_range)
            throw std::range_error(fmt::format("The conversion of the hex string '{}' to a color is out of range", s));

        if (ec != std::errc())
            throw std::runtime_error(fmt::format("The hex string '{}' could not be converted to a color", s));

        return (s.size() == 7) ? ((result << 8) | 0xff) : result;
    }

    std::uint32_t to_uint32(std::string_view s)
    {
        if (s.empty()) throw std::invalid_argument("An empty string does not represent a valid color");

        if (s[0] == '#') return hex_string_to_uint32(s);

        if (s.size() == 1) return char_to_uint32(s[0]);

        if (const auto result = css4_string_to_uint32(s); result) return *result;

        if (const auto result = tableau_string_to_uint32(s); result) return *result;

        throw std::invalid_argument(fmt::format("The string '{}' does not represent a valid color", s));
    }
}

namespace cdv
{
    std::ostream& operator<<(std::ostream& os, const rgba_color c)
    {
        os << fmt::format("{:#010x}", c.as_uint32());
        return os;
    }
}