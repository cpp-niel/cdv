#pragma once

#include "cdv/core/units.hpp"
#include "cdv/core/vec2.hpp"
#include "cdv/fig/frame_dimensions.hpp"

namespace cdv::fig
{
    class frame
    {
    public:
        constexpr frame() = default;

        constexpr frame(const pixels width, const pixels height, const pixels margin_left, const pixels margin_right,
                        const pixels margin_top, const pixels margin_bottom, const dots_per_inch dpi)
            : width_(width)
            , height_(height)
            , margin_left_(margin_left)
            , margin_right_(margin_right)
            , margin_top_(margin_top)
            , margin_bottom_(margin_bottom)
            , dpi_(dpi)
        {
        }

        // TODO this should be constexpr - why isn't mfl::inches_to_pixels?
        frame(const inches width, const inches height, const dots_per_inch dpi)
            : width_(mfl::inches_to_pixels(width, dpi))
            , height_(mfl::inches_to_pixels(height, dpi))
            , margin_left_(width_ / 16.0)
            , margin_right_(width_ / 32.0)
            , margin_top_(height_ / 16.0)
            , margin_bottom_(height_ / 16.0)
        {
        }

        [[nodiscard]] constexpr auto x0() const noexcept { return margin_left_; }
        [[nodiscard]] constexpr auto x1() const noexcept { return width_ - margin_right_; }
        [[nodiscard]] constexpr auto y0() const noexcept { return margin_bottom_; }
        [[nodiscard]] constexpr auto y1() const noexcept { return height_ - margin_top_; }

        [[nodiscard]] constexpr auto x_center() const noexcept { return (x0() + x1()) * 0.5; }
        [[nodiscard]] constexpr auto y_center() const noexcept { return (y0() + y1()) * 0.5; }
        [[nodiscard]] constexpr auto center() const noexcept { return pixel_pos(x_center(), y_center()); }

        [[nodiscard]] constexpr auto width() const noexcept { return width_; }
        [[nodiscard]] constexpr auto height() const noexcept { return height_; }
        [[nodiscard]] constexpr auto inner_width() const noexcept { return x1() - x0(); }
        [[nodiscard]] constexpr auto inner_height() const noexcept { return y1() - y0(); }

        [[nodiscard]] constexpr auto dimensions() const noexcept { return frame_dimensions{width_, height_, dpi_}; }

    private:
        pixels width_{640.0};
        pixels height_{480.0};
        pixels margin_left_{40.0};
        pixels margin_right_{20.0};
        pixels margin_top_{30.0};
        pixels margin_bottom_{30.0};
        dots_per_inch dpi_{100};
    };

}