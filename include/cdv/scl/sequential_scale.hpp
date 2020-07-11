#pragma once

#include <cdv/scl/ticks.hpp>
#include <cdv/stdx/concepts.hpp>

#include <array>

namespace cdv::scl
{
    template <stdx::floating_point DomainType, ranges::invocable<DomainType> Interpolator>
    class sequential_scale
    {
    public:
        using domain_t = DomainType;
        using codomain_t = std::invoke_result_t<Interpolator, DomainType>;
        using interpolator_t = Interpolator;

        constexpr sequential_scale(const domain_t start, const domain_t stop, const interpolator_t& interpolator)
            : domain_({start, stop})
            , interpolator_(interpolator)
        {
        }

        [[nodiscard]] constexpr auto operator() (const domain_t x) const
        {
            return interpolator_((x - domain_[0]) / (domain_[1] - domain_[0]));
        }

        std::vector<domain_t> ticks(const size_t num_ticks_hint = 8) const
        {
            return linear_ticks(domain_.front(), domain_.back(), num_ticks_hint);
        }

        auto tick_formatter(const auto num_ticks_hint = 8) const
        {
            const auto precision = linear_tick_format_precision(domain_.front(), domain_.back(), num_ticks_hint);
            return [=](const domain_t x) { return fmt::format("{:.{}f}", x, precision); };
        }

        [[nodiscard]] constexpr auto domain() const { return domain_; }

    private:
        std::array<domain_t, 2> domain_;
        interpolator_t interpolator_;
    };
}