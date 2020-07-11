#include <cdv/elem/range_stack.hpp>

#include <doctest/doctest.h>

#include <array>
#include <initializer_list>

namespace cdv::elem
{
    namespace
    {
        template <typename T>
        auto layer(std::initializer_list<std::initializer_list<T>> data)
        {
            std::vector<std::pair<T, T>> result;
            for (const auto row : data)
            {
                result.push_back(std::pair(*row.begin(), *std::next(row.begin())));
            }

            return result;
        }
    }

    TEST_SUITE("range stack")
    {
        const auto r0 = std::array{1, 2, 3, 4};
        const auto r1 = std::array{5, 2, 8, 3};
        const auto r2 = std::array{8, 3, 5, 3};

        TEST_CASE("stacks values without base value correctly")
        {
            const auto stack = range_stack(std::array{r0, r1, r2}, std::array{"A", "B"});
            CHECK(stack.layer("A") == layer({{1, 6}, {2, 4}, {3, 11}, {4, 7}}));
            CHECK(stack.layer("B") == layer({{6, 14}, {4, 7}, {11, 16}, {7, 10}}));
        }

        TEST_CASE("stacks values with base value correctly")
        {
            const auto stack = range_stack(std::array{r0, r1, r2}, std::array{0, 1, 2}, 0);
            CHECK(stack.layer(0) == layer({{0, 1}, {0, 2}, {0, 3}, {0, 4}}));
            CHECK(stack.layer(1) == layer({{1, 6}, {2, 4}, {3, 11}, {4, 7}}));
            CHECK(stack.layer(2) == layer({{6, 14}, {4, 7}, {11, 16}, {7, 10}}));
        }

        TEST_CASE("exception when creating stack with too few input ranges")
        {
            CHECK_THROWS(range_stack(std::vector<std::vector<int>>{}, std::vector<int>{}, 0));
            CHECK_THROWS(range_stack(std::vector<std::vector<int>>{}, std::vector<int>{}));
            CHECK_THROWS(range_stack(std::vector{r0}, std::vector<int>{}));
        }

        TEST_CASE("exception when number of keys does not match the number of layers")
        {
            CHECK_THROWS(range_stack(std::array{r0, r1, r2}, std::array{"A", "B"}, 0));
            CHECK_THROWS(range_stack(std::array{r0, r1, r2}, std::array{"A", "B", "C", "D"}, 0));
            CHECK_THROWS(range_stack(std::array{r0, r1, r2}, std::array{"A"}));
            CHECK_THROWS(range_stack(std::array{r0, r1, r2}, std::array{"A", "B", "C"}));
        }
    }
}