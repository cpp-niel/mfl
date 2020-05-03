#include "parser/parse.hpp"

#include "framework/doctest.hpp"
#include "framework/node_types_are.hpp"
#include "noad/overline.hpp"
#include "noad/underline.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse line")
    {
        TEST_CASE("overline")
        {
            const auto [result, error] = parse("\\overline{xyz}");
            CHECK(node_types_are<overline_wrapper>(result));
            const overline& ol = std::get<overline_wrapper>(result[0]);
            CHECK(node_types_are<math_char, math_char, math_char>(ol.noads));
        }

        TEST_CASE("underline")
        {
            const auto [result, error] = parse("\\underline{xyz}");
            CHECK(node_types_are<underline_wrapper>(result));
            const underline& ul = std::get<underline_wrapper>(result[0]);
            CHECK(node_types_are<math_char, math_char, math_char>(ul.noads));
        }
    }
}
