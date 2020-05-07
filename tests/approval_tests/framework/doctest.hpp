#pragma once

#include "doctest.v.2.3.4.hpp"

#undef TEST_CASE
#define TEST_CASE(decorators) DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(_DOCTEST_ANON_FUNC_), decorators)  // NOLINT

#undef TEST_SUITE
#define TEST_SUITE(decorators) DOCTEST_TEST_SUITE_IMPL(decorators, DOCTEST_ANONYMOUS(_DOCTEST_ANON_SUITE_)) // NOLINT

