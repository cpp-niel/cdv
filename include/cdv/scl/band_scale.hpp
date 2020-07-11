#pragma once

#include "cdv/stdx/concepts.hpp"

#include <fmt/format.h>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/all.hpp>

#include <vector>

namespace cdv::scl
{
    struct band_scale_properties
    {
        double inner_padding = 0.0;
        double outer_padding = 0.0;
        double alignment_factor = 0.5;
    };

    template <typename DomainType, typename CodomainType>
    class band_scale
    {
    public:
        using domain_t = DomainType;
        using codomain_t = CodomainType;

        band_scale(const stdx::range_of<domain_t> auto& domain, const codomain_t& codomain_start,
                   const codomain_t& codomain_stop, const band_scale_properties& properties = {})
            : domain_(ranges::to_vector(domain))
            , codomain_{codomain_start, codomain_stop}
            , properties_(properties)
        {
            auto start = codomain_.front();
            auto stop = codomain_.back();
            if (stop < start) std::swap(start, stop);

            // total width is the sum of
            // - outer padding * step            : the initial padding
            // - n * step                        : the width of each band plus its subsequent padding
            // - (outer - inner padding) * step  : the correction for the final padding because the final band's
            //                                     subsequent padding is outer and not inner.
            // solve for step, you get the formula used here.
            const auto total_width = stop - start;
            const auto n = static_cast<double>(domain_.size());
            const auto denom = std::max(1.0, ((2.0 * properties_.outer_padding) + n - properties_.inner_padding));
            const auto step = total_width / denom;
            band_width_ = step * (1.0 - properties.inner_padding);
        }

        [[nodiscard]] codomain_t min(const DomainType& x) const
        {
            if (const auto it = ranges::find(domain_, x); it != domain_.end())
            {
                const auto index = std::distance(domain_.begin(), it);
                const auto step = band_width_ / (1.0 - properties_.inner_padding);
                const auto initial_padding = properties_.outer_padding * 2.0 * properties_.alignment_factor;
                return codomain_.front() + (step * (initial_padding + static_cast<double>(index)));
            }

            throw std::domain_error("error in band_scale: input argument is not in the scale's domain");
        }

        [[nodiscard]] codomain_t operator()(const DomainType& x) const
        {
            return min(x) + (band_width_ * 0.5);
        }

        [[nodiscard]] codomain_t max(const DomainType& x) const
        {
            return min(x) + band_width_;
        }

        [[nodiscard]] codomain_t band_width() const noexcept { return band_width_; }

        [[nodiscard]] auto domain() const { return ranges::views::all(domain_); }
        [[nodiscard]] auto codomain() const { return ranges::views::all(codomain_); }

        [[nodiscard]] auto ticks(const size_t) const { return domain(); }

        auto tick_formatter(const size_t) const
        {
            return [=](const DomainType x) { return fmt::format("{}", x); };
        }

    private:
        std::vector<domain_t> domain_;
        std::array<codomain_t, 2> codomain_;
        codomain_t band_width_{0};
        band_scale_properties properties_;
    };

    template <typename DomainRange, typename CodomainType>
    band_scale(const DomainRange&, const CodomainType&, const CodomainType&, const band_scale_properties& = {})
        ->band_scale<ranges::range_value_type_t<DomainRange>, CodomainType>;
}