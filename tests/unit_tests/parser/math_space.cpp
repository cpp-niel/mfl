#include "parser/parse.hpp"

#include <doctest/doctest.h>
#include "framework/node_types_are.hpp"
#include "noad/left_right.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse math space")
    {
        TEST_CASE("built in space")
        {
            const auto [noads, error] = parse("\\,");
            const auto result = std::get<math_space>(noads[0]);
            CHECK(result.is_math_units);
            CHECK(std::get<kern>(result.space).size == 3);
        }

        TEST_CASE("custom space")
        {
            const auto [noads, error] = parse("\\hspace{4}");
            const auto result = std::get<math_space>(noads[0]);
            CHECK(result.is_math_units);
            CHECK(std::get<kern>(result.space).size == 4 * 18);
        }

        TEST_CASE("custom space with invalid number")
        {
            const auto [noads, error] = parse("\\hspace{3.14invalid}");
            CHECK(*error == "Syntax error: '3.14invalid' does not represent a valid floating point value.");
        }
    }
}
