#pragma once

#include <cdv/scl/linear_scale.hpp>
#include <cdv/stdx/concepts.hpp>
#include <cdv/stdx/numbers.hpp>

#include <range/v3/algorithm/reverse.hpp>
#include <range/v3/algorithm/transform.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/take.hpp>

#include <cmath>

namespace cdv::scl
{
    template <stdx::floating_point T>
    T symlog(const T base, const T x)
    {
        const auto sgn = std::signbit(x) ? T(-1.0) : T(1.0);
        if (base == 10.0) return sgn * std::log10(sgn * x);
        if (base == stdx::numbers::e_v<T>) return sgn * std::log(sgn * x);
        if (base == 2.0) return sgn * std::log2(sgn * x);

        return sgn * std::log(sgn * x) / std::log(base);
    }

    template <typename CodomainType>
    struct log_scale_properties
    {
        double base = 10.0;
        bool clamp = false;
        linear_interpolator<CodomainType> interpolate{};
    };

    template <stdx::floating_point DomainType, typename CodomainType>
    class log_scale
    {
    public:
        using domain_t = DomainType;
        using codomain_t = CodomainType;

        log_scale(const stdx::range_of<DomainType> auto& domain, const stdx::range_of<CodomainType> auto& codomain,
                  const log_scale_properties<CodomainType>& properties = {})
            : domain_(ranges::to_vector(domain)), codomain_(ranges::to_vector(codomain)), properties_(properties)
        {
            assert(domain_.size() > 1);
            assert(domain_.size() == codomain_.size());
        }

        log_scale(const DomainType x0, const DomainType x1, const CodomainType y0, const CodomainType y1,
                  const log_scale_properties<CodomainType>& properties = {})
            : domain_(std::vector{x0, x1}), codomain_(std::vector{y0, y1}), properties_(properties)
        {
        }

        [[nodiscard]] CodomainType operator()(const DomainType& x) const noexcept
        {
            const auto transform = [&](const auto v) { return (v > 0) ? s_log(v) : -s_log(-v); };
            return detail::apply_continuous_scaling(domain_, codomain_, properties_.interpolate, transform,
                                                    properties_.clamp, x);
        }

        [[nodiscard]] log_scale snapped_to_grid([[maybe_unused]] const size_t num_ticks_hint = 8) const
        {
            const auto [start, stop] = detail::ascending_limits(domain_);

            auto x0 = std::pow(properties_.base, std::floor(s_log(start)));
            auto x1 = std::pow(properties_.base, std::ceil(s_log(stop)));
            if (domain_.front() > domain_.back()) std::swap(x0, x1);

            const auto snapped_domain = detail::replaced_front_and_back(x0, domain_, x1);
            return log_scale(snapped_domain, codomain_, properties_);
        }

        std::vector<DomainType> ticks(const size_t num_ticks_hint = 8) const
        {
            std::vector<DomainType> result;
            const auto [start, stop] = detail::ascending_limits(domain_);
            const auto low_order = s_log(start);
            const auto high_order = s_log(stop);
            const auto is_base_whole_number = (std::floor(properties_.base) == properties_.base);
            if (is_base_whole_number and ((high_order - low_order) < DomainType(num_ticks_hint)))
            {
                namespace rv = ::ranges::views;
                const auto inner_indices = rv::iota(1, static_cast<int>(properties_.base));
                result = (start > 0)
                             ? ticks_by_order(start, stop, low_order, high_order, inner_indices, 1.0)
                             : ticks_by_order(start, stop, low_order, high_order, inner_indices | rv::reverse, -1.0);

                if (result.size() * 2 < num_ticks_hint) result = linear_ticks(start, stop, num_ticks_hint);
            }
            else
            {
                const auto num_ticks = std::min(static_cast<size_t>(high_order - low_order), num_ticks_hint);
                result = linear_ticks(low_order, high_order, num_ticks);
                ranges::transform(result, result.begin(), [&](const auto x) { return std::pow(properties_.base, x); });
            }

            if (domain_.front() > domain_.back()) ranges::reverse(result);

            return result;
        }

        auto tick_formatter([[maybe_unused]] const auto num_ticks_hint = 8) const
        {
            const auto base = properties_.base;
            return [=](const DomainType x) {
                const auto sign = (x < 0) ? "-" : "";

                const auto unrounded_exponent = scl::symlog(base, x);
                if (!is_integer(unrounded_exponent)) return std::string{};

                const auto exponent = std::round(unrounded_exponent);
                if (is_integer(base)) return fmt::format("${}{:.0f}^{{{:.0f}}}$", sign, std::round(base), exponent);

                if (base == stdx::numbers::e) return fmt::format("${}e^{{{:.0f}}}$", sign, exponent);

                return fmt::format("${}{}^{{{:.0f}}}$", sign, base, exponent);
            };
        }

        [[nodiscard]] auto domain() const { return ranges::views::all(domain_); }
        [[nodiscard]] auto codomain() const { return ranges::views::all(codomain_); }

        [[nodiscard]] double base() const { return properties_.base; }

    private:
        static bool is_integer(const DomainType& value) { return ((value - std::floor(value)) < DomainType(1e-10)); }

        DomainType s_log(const DomainType x) const { return symlog(DomainType(properties_.base), x); }

        std::vector<DomainType> ticks_by_order(const DomainType start, const DomainType stop,
                                               const DomainType low_order, const DomainType high_order,
                                               const stdx::range_of<int> auto& inner_indices, const double sgn) const
        {
            const auto min_order = static_cast<int>(std::floor(low_order));
            const auto max_order = static_cast<int>(std::ceil(high_order));
            std::vector<DomainType> result;
            for (auto i = min_order; i <= max_order; ++i)
            {
                for (auto k : inner_indices)
                {
                    const auto tick = static_cast<DomainType>(sgn * std::pow(properties_.base, sgn * i) * k);
                    if ((tick >= start) && (tick <= stop)) result.push_back(tick);
                }
            }

            return result;
        }

        std::vector<DomainType> domain_;      // TODO should be some kind of small_vector, size can be ...
        std::vector<CodomainType> codomain_;  // ... arbitrarily large, but is almost always 2
        log_scale_properties<CodomainType> properties_;
    };

    template <typename DomainRange, typename CodomainRange>
    log_scale(DomainRange&&, CodomainRange&&, const log_scale_properties<ranges::range_value_type_t<CodomainRange>>& = {})
        ->log_scale<ranges::range_value_type_t<DomainRange>, ranges::range_value_type_t<CodomainRange>>;
}