#pragma once

#include "cdv/stdx/concepts.hpp"

#include <fmt/format.h>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/all.hpp>

#include <vector>

namespace cdv::scl
{
    struct point_scale_properties
    {
        double padding = 0.0;
        double alignment_factor = 0.5;
    };

    template <typename DomainType, typename CodomainType>
    class point_scale
    {
    public:
        using domain_t = DomainType;
        using codomain_t = CodomainType;

        point_scale(const stdx::range_of<domain_t> auto& domain, const codomain_t& codomain_start,
                    const codomain_t& codomain_stop, const point_scale_properties& properties = {})
            : domain_(ranges::to_vector(domain)), codomain_{codomain_start, codomain_stop}, properties_(properties)
        {
            auto start = codomain_.front();
            auto stop = codomain_.back();
            if (stop < start) std::swap(start, stop);

            // total width is the sum of
            // - 2 * padding * step              : the padding at each end
            // - (n - 1) * step                  : the (n - 1) distances between the n points
            // solve for step, you get the formula used here.
            const auto total_width = stop - start;
            const auto n = static_cast<double>(domain_.size());
            const auto denom = std::max(1.0, (2.0 * properties_.padding) + (n - 1.0));
            step_ = total_width / denom;
        }

        [[nodiscard]] codomain_t operator()(const DomainType& x) const
        {
            if (const auto it = ranges::find(domain_, x); it != domain_.end())
            {
                const auto index = std::distance(domain_.begin(), it);
                const auto initial_padding = properties_.padding * 2.0 * properties_.alignment_factor;
                return codomain_.front() + (step_ * (initial_padding + static_cast<double>(index)));
            }

            throw std::domain_error("error in point_scale: input argument is not in the scale's domain");
        }

        [[nodiscard]] auto step() const { return step_; }

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
        codomain_t step_{0};
        point_scale_properties properties_;
    };

    template <typename DomainRange, typename CodomainType>
    point_scale(const DomainRange&, const CodomainType&, const CodomainType&, const point_scale_properties& = {})
        ->point_scale<ranges::range_value_type_t<DomainRange>, CodomainType>;
}