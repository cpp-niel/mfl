#include "parser/parse.hpp"

#include <doctest/doctest.h>

namespace mfl
{
    TEST_SUITE("parser")
    {
        TEST_CASE("parse returns no noads when input string is empty")
        {
            CHECK(parse("").first.empty());
        }

        TEST_CASE("parsing until a token that isn't in the input produces an error")
        {
            CHECK(*parse("\\hat{x").second == "Syntax error: unexpected end of input.");
        }

        TEST_CASE("parsing until a value that isn't in the input produces an error")
        {
            CHECK(*parse("\\left[x").second == "Syntax error: expected token not found: 'right'.");
        }
    }
}
