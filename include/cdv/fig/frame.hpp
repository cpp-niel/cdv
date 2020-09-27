#pragma once

#include <cdv/core/units.hpp>
#include <cdv/core/vec2.hpp>
#include <cdv/fig/frame_dimensions.hpp>

#include <variant>

namespace cdv
{
    struct percentage
    {
        double val;
    };

    constexpr double as_fraction(const percentage& pc) { return pc.val * 0.01; }

    namespace units_literals
    {
        constexpr percentage operator""_percent(const long double val)
        {
            return {.val = static_cast<double>(val)};
        }

        constexpr percentage operator""_percent(const unsigned long long val)
        {
            return {.val = static_cast<double>(val)};
        }
    }
}

namespace cdv::fig
{
    class margin
    {
    public:
        constexpr margin(const pixels px) : val_(px) {}
        constexpr margin(const percentage pc) : val_(as_fraction(pc)) {}

        constexpr auto value(const pixels total_size) const
        {
            if (std::holds_alternative<pixels>(val_)) return get<pixels>(val_);

            return get<double>(val_) * total_size;
        }

    private:
        std::variant<pixels, double> val_;
    };

    struct frame
    {
        [[nodiscard]] constexpr auto x0() const noexcept { return left_margin.value(width); }
        [[nodiscard]] constexpr auto x1() const noexcept { return width - right_margin.value(width); }
        [[nodiscard]] constexpr auto y0() const noexcept { return bottom_margin.value(height); }
        [[nodiscard]] constexpr auto y1() const noexcept { return height - top_margin.value(height); }

        [[nodiscard]] constexpr auto x_center() const noexcept { return (x0() + x1()) * 0.5; }
        [[nodiscard]] constexpr auto y_center() const noexcept { return (y0() + y1()) * 0.5; }
        [[nodiscard]] constexpr auto center() const noexcept { return pixel_pos(x_center(), y_center()); }

        [[nodiscard]] constexpr auto inner_width() const noexcept { return x1() - x0(); }
        [[nodiscard]] constexpr auto inner_height() const noexcept { return y1() - y0(); }

        [[nodiscard]] constexpr auto dimensions() const noexcept { return frame_dimensions{width, height, dpi}; }

        pixels width{640.0};
        pixels height{480.0};
        dots_per_inch dpi{100};
        margin left_margin = pixels{40.0};
        margin right_margin = pixels{20.0};
        margin bottom_margin = pixels{30.0};
        margin top_margin = pixels{30.0};
    };
}