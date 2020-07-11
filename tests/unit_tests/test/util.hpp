#pragma once

#include <cdv/stdx/concepts.hpp>

#include <doctest/doctest.h>
#include <range/v3/view/zip.hpp>

#include <cmath>
#include <sstream>  // release mode on mac os needs this

#undef TEST_CASE
#define TEST_CASE(decorators) DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(_DOCTEST_ANON_FUNC_), decorators)  // NOLINT

#undef TEST_CASE_TEMPLATE
#define TEST_CASE_TEMPLATE(dec, T, ...) DOCTEST_TEST_CASE_TEMPLATE_IMPL(dec, T, DOCTEST_ANONYMOUS(_DOCTEST_ANON_TMP_), __VA_ARGS__)  // NOLINT

#undef TEST_SUITE
#define TEST_SUITE(decorators) DOCTEST_TEST_SUITE_IMPL(decorators, DOCTEST_ANONYMOUS(_DOCTEST_ANON_SUITE_))  // NOLINT

namespace cdv::test
{
    template <typename T>
    bool almost_equal(const stdx::range_of<T> auto& r0, const stdx::range_of<T> auto& r1, const T& tolerance)
    {
        for (auto [x, y] : ranges::views::zip(r0, r1))
        {
            if (std::abs(x - y) > tolerance) return false;
        }

        return true;
    }
}