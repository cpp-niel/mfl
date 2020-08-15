#include "parser/parse.hpp"

#include <doctest/doctest.h>
#include "framework/node_types_are.hpp"
#include "noad/radical.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse radical")
    {
        TEST_CASE("without degree")
        {
            const auto [noads, error] = parse("\\sqrt{x}");
            const radical result = std::get<radical_wrapper>(noads[0]);
            CHECK(!result.degree);
            CHECK(node_types_are<math_char>(result.radicand));
        }

        TEST_CASE("with degree")
        {
            const auto [noads, error] = parse("\\sqrt[n]{x}");
            const radical result = std::get<radical_wrapper>(noads[0]);
            CHECK(node_types_are<math_char>(*result.degree));
            CHECK(node_types_are<math_char>(result.radicand));
        }

        TEST_CASE("with invalid degree")
        {
            const auto [noads, error] = parse("\\sqrt[n{x}");
            CHECK(*error == "Syntax error: unexpected end of input.");
        }
    }
}
