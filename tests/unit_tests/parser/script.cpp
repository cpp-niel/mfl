#include "parser/parse.hpp"

#include <doctest/doctest.h>
#include "framework/node_types_are.hpp"
#include "noad/script.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse script")
    {
        TEST_CASE("nucleus and both scripts")
        {
            const auto [noads, error] = parse("a_i^x");
            const script result = std::get<script_wrapper>(noads[0]);
            CHECK(!result.nucleus.empty());
            CHECK(result.sub);
            CHECK(result.sup);
        }

        TEST_CASE("nucleus and both scripts with superscript first")
        {
            const auto [noads, error] = parse("a^x_i");
            const script result = std::get<script_wrapper>(noads[0]);
            CHECK(!result.nucleus.empty());
            CHECK(result.sub);
            CHECK(result.sup);
        }

        TEST_CASE("no nucleus and both scripts")
        {
            const auto [noads, error] = parse("{}_i^x");
            const script result = std::get<script_wrapper>(noads[0]);
            CHECK(result.nucleus.empty());
            CHECK(result.sub);
            CHECK(result.sup);
        }

        TEST_CASE("nucleus and subscript only")
        {
            const auto [noads, error] = parse("{a}_i");
            const script result = std::get<script_wrapper>(noads[0]);
            CHECK(!result.nucleus.empty());
            CHECK(result.sub);
            CHECK(!result.sup);
        }

        TEST_CASE("nucleus and superscript only")
        {
            const auto [noads, error] = parse("{a}^x");
            const script result = std::get<script_wrapper>(noads[0]);
            CHECK(!result.nucleus.empty());
            CHECK(!result.sub);
            CHECK(result.sup);
        }

        TEST_CASE("nucleus and superscript only with primes")
        {
            const auto [noads, error] = parse("a'''");
            const script result = std::get<script_wrapper>(noads[0]);
            CHECK(!result.nucleus.empty());
            CHECK(!result.sub);
            CHECK(result.sup);
        }

        TEST_CASE("nucleus and superscript with more primes than in one char")
        {
            const auto [noads, error] = parse("a'''''");
            const script result = std::get<script_wrapper>(noads[0]);
            CHECK(!result.nucleus.empty());
            CHECK(!result.sub);
            CHECK(result.sup);
        }

        TEST_CASE("nucleus and mix of primes and superscripts")
        {
            const auto [noads, error] = parse("a''^x");
            const script result = std::get<script_wrapper>(noads[0]);
            CHECK(!result.nucleus.empty());
            CHECK(!result.sub);
            CHECK(result.sup);
        }

        TEST_CASE("nucleus and mix of all sub and superscripts")
        {
            const auto [noads, error] = parse("a'_i''^x");
            const script result = std::get<script_wrapper>(noads[0]);
            CHECK(!result.nucleus.empty());
            CHECK(result.sub);
            CHECK(result.sup);
        }

        TEST_CASE("error when subscripts are ambiguous")
        {
            const auto [noads, error] = parse("a_i_j");
            CHECK(*error == "Syntax error: Ambiguous subscripts. Use braces to group the subscripts unambiguously.");
        }

        TEST_CASE("error when superscripts are ambiguous")
        {
            const auto [noads, error] = parse("a^x^y");
            CHECK(*error
                  == "Syntax error: Ambiguous superscripts. Use braces to group the superscripts unambiguously.");
        }

        TEST_CASE("throws when primes come after superscript")
        {
            const auto [noads, error] = parse("a^x'_i");
            CHECK(*error == "Syntax error: unexpected token.");
        }
    }
}
