#pragma once

#include "doctest.v.2.3.4.hpp"
#include <sstream>  // release mode on mac os needs this

#undef TEST_CASE
#define TEST_CASE(decorators) DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(_DOCTEST_ANON_FUNC_), decorators)  // NOLINT

#undef TEST_CASE_TEMPLATE
#define TEST_CASE_TEMPLATE(dec, T, ...) DOCTEST_TEST_CASE_TEMPLATE_IMPL(dec, T, DOCTEST_ANONYMOUS(_DOCTEST_ANON_TMP_), __VA_ARGS__)  // NOLINT

#undef TEST_SUITE
#define TEST_SUITE(decorators) DOCTEST_TEST_SUITE_IMPL(decorators, DOCTEST_ANONYMOUS(_DOCTEST_ANON_SUITE_)) // NOLINT

