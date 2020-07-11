#pragma once

#include <cdv/stdx/concepts.hpp>

#include <fmt/format.h>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/front.hpp>
#include <range/v3/range/concepts.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/repeat.hpp>
#include <range/v3/view/zip_with.hpp>

#include <type_traits>
#include <utility>
#include <vector>

namespace cdv::elem
{
    namespace detail
    {
        template <ranges::range RngOfRngs, ranges::range LayerRange>
        void add_layers(const RngOfRngs& inputs, LayerRange& layers)
        {
            for (auto&& x : inputs)
            {
                layers.emplace_back(
                    ranges::views::zip_with([](auto a, auto b) { return std::pair(a.second, a.second + b); },
                                            layers.back(), x)
                    | ranges::to_vector);
            }
        }
    }

    template <typename Key, typename Value>
    class range_stack
    {
    public:
        using layer_t = std::vector<std::pair<Value, Value>>;

        template <ranges::range RngOfRngs>
        range_stack(const RngOfRngs& range_of_ranges, const stdx::range_of<Key> auto& keys)
            : keys_(keys | ranges::to_vector)
        {
            if (ranges::distance(range_of_ranges) < 2)
                throw std::invalid_argument("A stack with a base value needs at least two input ranges");

            if ((ranges::distance(keys) + 1) != ranges::distance(range_of_ranges))
                throw std::invalid_argument(fmt::format("Cannot create a range stack with {} layers and {} keys. The "
                                                        "number of keys and layers must be equal",
                                                        ranges::distance(range_of_ranges) - 1, ranges::distance(keys)));

            namespace rv = ::ranges::views;
            layers_.emplace_back(rv::zip_with([](auto a, auto b) { return std::pair(a, a + b); },
                                              ranges::front(range_of_ranges),
                                              *ranges::next(ranges::begin(range_of_ranges)))
                                 | ranges::to_vector);
            detail::add_layers(range_of_ranges | rv::drop(2), layers_);
        }

        template <ranges::range RngOfRngs, typename InnerType = ranges::range_value_t<ranges::range_value_t<RngOfRngs>>>
        requires std::is_convertible_v<InnerType, Value>
        range_stack(const RngOfRngs& range_of_ranges, const stdx::range_of<Key> auto& keys, const InnerType base_value)
            : keys_(keys | ranges::to_vector)
        {
            if (ranges::distance(range_of_ranges) < 1)
                throw std::invalid_argument("A range stack with a base value needs at least one input range");

            if (ranges::distance(keys) != ranges::distance(range_of_ranges))
                throw std::invalid_argument(fmt::format("Cannot create a range stack with {} layers and {} keys. The "
                                                        "number of keys and layers must be equal",
                                                        ranges::distance(range_of_ranges), ranges::distance(keys)));

            namespace rv = ::ranges::views;
            layers_.emplace_back(rv::zip_with([](auto a, auto b) { return std::pair(a, b); }, rv::repeat(base_value),
                                              ranges::front(range_of_ranges))
                                 | ranges::to_vector);
            detail::add_layers(range_of_ranges | rv::drop(1), layers_);
        }

        const layer_t& layer(const Key& key) const
        {
            const auto it = ranges::find(keys_, key);
            if (it == keys_.end()) throw std::invalid_argument(fmt::format("'{}' is not a valid key", key));

            const auto index = static_cast<size_t>(ranges::distance(keys_.begin(), it));
            return layers_[index];
        }

    private:
        std::vector<Key> keys_;
        std::vector<layer_t> layers_;
    };

    template <ranges::range RngOfRngs, ranges::range KeyRange>
    range_stack(const RngOfRngs& range_of_ranges, const KeyRange& keys)
        ->range_stack<ranges::range_value_type_t<KeyRange>, ranges::range_value_t<ranges::range_value_t<RngOfRngs>>>;

    template <ranges::range RngOfRngs, ranges::range KeyRange,
              typename InnerType = ranges::range_value_t<ranges::range_value_t<RngOfRngs>>>
    range_stack(const RngOfRngs& range_of_ranges, const KeyRange& keys, const InnerType base_value)
        ->range_stack<ranges::range_value_type_t<KeyRange>, InnerType>;
}
