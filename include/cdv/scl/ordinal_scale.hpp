#pragma once

#include <cdv/stdx/concepts.hpp>

#include <range/v3/algorithm/find.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/all.hpp>

#include <optional>
#include <vector>

namespace cdv::scl
{
    template <typename CodomainType>
    struct ordinal_scale_properties
    {
        std::optional<CodomainType> default_result;
    };

    template <typename DomainType, typename CodomainType>
    class ordinal_scale
    {
    public:
        using domain_t = DomainType;
        using codomain_t = CodomainType;

        ordinal_scale(const stdx::range_of<DomainType> auto& domain, const stdx::range_of<CodomainType> auto& codomain,
                      const ordinal_scale_properties<CodomainType>& properties = {})
            : domain_(ranges::to_vector(domain)), codomain_(ranges::to_vector(codomain)), properties_(properties)
        {
            if (codomain_.empty())
                throw std::invalid_argument(
                    "An ordinal scale must have a valid codomain containing at least one entry.");
        }

        codomain_t operator()(const DomainType& x) const
        {
            const auto it = ranges::find(domain_, x);
            const auto distance = std::distance(domain_.begin(), it);
            if (it == domain_.end())
            {
                if (properties_.default_result) return *properties_.default_result;

                throw std::invalid_argument("const ordinal scale cannot append new domain items");
            }

            const auto index = static_cast<size_t>(distance) % codomain_.size();
            return codomain_[index];
        }

        codomain_t operator()(const DomainType& x)
        {
            const auto it = ranges::find(domain_, x);
            const auto distance = std::distance(domain_.begin(), it);
            if (it == domain_.end())
            {
                if (properties_.default_result) return *properties_.default_result;

                domain_.push_back(x);
            }

            const auto index = static_cast<size_t>(distance) % codomain_.size();
            return codomain_[index];
        }

        [[nodiscard]] auto domain() const { return ranges::views::all(domain_); }
        [[nodiscard]] auto codomain() const { return ranges::views::all(codomain_); }

    private:
        std::vector<domain_t> domain_;
        std::vector<codomain_t> codomain_;
        ordinal_scale_properties<codomain_t> properties_;
    };

    template <typename DomainRange, typename CodomainRange>
    ordinal_scale(const DomainRange& domain, const CodomainRange& codomain,
                  const ordinal_scale_properties<ranges::range_value_type_t<CodomainRange>>& = {})
        ->ordinal_scale<ranges::range_value_type_t<DomainRange>, ranges::range_value_type_t<CodomainRange>>;

    template <typename DomainType, typename CodomainRange>
    auto make_ordinal_scale(const CodomainRange& codomain,
                            const ordinal_scale_properties<ranges::range_value_type_t<CodomainRange>>& properties = {})
    {
        return ordinal_scale<DomainType, ranges::range_value_type_t<CodomainRange>>(std::vector<DomainType>(), codomain,
                                                                              properties);
    }
}