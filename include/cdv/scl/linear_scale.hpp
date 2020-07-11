#pragma once

#include <cdv/core/units.hpp>
#include <cdv/scl/ticks.hpp>
#include <cdv/stdx/concepts.hpp>

#include <range/v3/algorithm/lower_bound.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>

#include <cmath>

namespace cdv::scl
{
    namespace detail
    {
        template <typename T>
        auto replaced_front_and_back(const T front, const stdx::range_of_floating_point<T> auto& original_range,
                                     const T back)
        {
            namespace rv = ranges::views;
            return rv::concat(rv::single(front), original_range | rv::drop(1) | rv::take(original_range.size() - 2),
                              rv::single(back));
        }

        template <typename T>
        auto interpolation_parameters(const stdx::range_of_floating_point<T> auto& pieces, const T x)
        {
            const auto upper = (pieces.front() < pieces.back()) ? ranges::lower_bound(pieces, x)
                                                                : ranges::lower_bound(pieces, x, std::greater<>());

            const auto i1 = std::clamp(upper, std::next(pieces.begin()), std::prev(pieces.end()));
            const auto i0 = std::prev(i1);
            return std::pair{std::distance(pieces.begin(), i0), (x - *i0) / (*i1 - *i0)};
        }

        template <stdx::floating_point DomainType, typename Interpolator, typename Transformer>
        auto apply_continuous_scaling(const stdx::range_of<DomainType> auto& domain, const ranges::range auto& codomain,
                                      const Interpolator interpolate, const Transformer xform, const bool clamp,
                                      const DomainType x)
        {
            namespace rv = ranges::views;
            const auto [n, t] = detail::interpolation_parameters(domain | rv::transform(xform), xform(x));
            const auto i0 = std::next(codomain.begin(), n);
            const auto i1 = std::next(i0);
            const auto result = interpolate(*i0, *i1, t);
            if (clamp) return std::clamp(result, codomain.front(), codomain.back());

            return result;
        }
    }

    template <typename T>
    struct linear_interpolator;

    template <stdx::floating_point FloatingPointType>
    struct linear_interpolator<FloatingPointType>
    {
        [[nodiscard]] constexpr FloatingPointType operator()(const FloatingPointType a, const FloatingPointType b,
                                                             const FloatingPointType t) const noexcept
        {
            return std::lerp(a, b, t);
        }
    };

    template <>
    struct linear_interpolator<cdv::pixels>
    {
        [[nodiscard]] constexpr cdv::pixels operator()(const cdv::pixels a, const cdv::pixels b, const double t) const
            noexcept
        {
            return cdv::pixels(std::lerp(a.value(), b.value(), t));
        }
    };

    template <typename CodomainType>
    struct linear_scale_properties
    {
        bool clamp = false;
        linear_interpolator<CodomainType> interpolate{};
    };

    template <stdx::floating_point DomainType, typename CodomainType>
    class linear_scale
    {
    public:
        using domain_t = DomainType;
        using codomain_t = CodomainType;

        linear_scale(const stdx::range_of<DomainType> auto& domain, const stdx::range_of<CodomainType> auto& codomain,
                     const linear_scale_properties<CodomainType>& properties = {})
            : domain_(ranges::to_vector(domain)), codomain_(ranges::to_vector(codomain)), properties_(properties)
        {
            assert(domain_.size() > 1);
            assert(domain_.size() == codomain_.size());
        }

        linear_scale(const DomainType x0, const DomainType x1, const CodomainType y0, const CodomainType y1,
                     const linear_scale_properties<CodomainType>& properties = {})
            : domain_(std::vector{x0, x1}), codomain_(std::vector{y0, y1}), properties_(properties)
        {
        }

        [[nodiscard]] CodomainType operator()(const DomainType& x) const noexcept
        {
            return detail::apply_continuous_scaling(
                domain_, codomain_, properties_.interpolate, [](auto x) { return x; }, properties_.clamp, x);
        }

        [[nodiscard]] linear_scale snapped_to_grid(const size_t num_ticks_hint = 8) const
        {
            const auto [start, stop] = detail::ascending_limits(domain_);

            const auto inc = snapped_tick_increment(start, stop, num_ticks_hint);
            auto x0 = (inc > 0) ? std::floor(start / inc) * inc : std::ceil(start * inc) / inc;
            auto x1 = (inc > 0) ? std::ceil(stop / inc) * inc : std::floor(stop * inc) / inc;
            if (domain_.front() > domain_.back()) std::swap(x0, x1);

            const auto snapped_domain = detail::replaced_front_and_back(x0, domain_, x1);
            return linear_scale(snapped_domain, codomain_, properties_);
        }

        std::vector<DomainType> ticks(const size_t num_ticks_hint = 8) const
        {
            return linear_ticks(domain_.front(), domain_.back(), num_ticks_hint);
        }

        auto tick_formatter(const auto num_ticks_hint = 8) const
        {
            const auto precision = linear_tick_format_precision(domain_.front(), domain_.back(), num_ticks_hint);
            return [=](const DomainType x) { return fmt::format("{:.{}f}", x, precision); };
        }

        [[nodiscard]] auto domain() const { return ranges::views::all(domain_); }
        [[nodiscard]] auto codomain() const { return ranges::views::all(codomain_); }

    private:
        std::vector<DomainType> domain_;      // TODO should be some kind of small_vector, size can be ...
        std::vector<CodomainType> codomain_;  // ... arbitrarily large, but is almost always 2
        linear_scale_properties<CodomainType> properties_;
    };

    template <typename DomainRange, typename CodomainRange>
    linear_scale(const DomainRange&, const CodomainRange&,
                 const linear_scale_properties<ranges::range_value_type_t<CodomainRange>>& = {})
        ->linear_scale<ranges::range_value_type_t<DomainRange>, ranges::range_value_type_t<CodomainRange>>;
}