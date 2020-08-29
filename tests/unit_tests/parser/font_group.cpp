#include "parser/parse.hpp"

#include "framework/doctest.hpp"
#include "framework/node_types_are.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse font group")
    {
        TEST_CASE("font choice applies to all items in group")
        {
            const auto [result, error] = parse("\\mathbf{ab}");
            CHECK(node_types_are<math_char, math_char>(result));
            CHECK(std::get<math_char>(result[0]).family == font_family::bold);
            CHECK(std::get<math_char>(result[1]).family == font_family::bold);
        }

        TEST_CASE("font choice applies to next item only when not grouped")
        {
            const auto [result, error] = parse("\\mathit ab");
            CHECK(node_types_are<math_char, math_char>(result));
            CHECK(std::get<math_char>(result[0]).family == font_family::italic);
            CHECK(std::get<math_char>(result[1]).family == font_family::roman);
        }
    }
}