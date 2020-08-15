#include "parser/parse.hpp"

#include <doctest/doctest.h>
#include "framework/node_types_are.hpp"
#include "noad/big_op.hpp"
#include "noad/mlist_with_kind.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse function")
    {
        TEST_CASE("becomes math chars in roman font")
        {
            const auto [result, error] = parse("\\ln");
            const mlist_with_kind& group = std::get<mlist_with_kind_wrapper>(result[0]);
            CHECK(group.kind == item_kind::op);
            CHECK(node_types_are<math_char, math_char>(group.noads));
            CHECK(std::get<math_char>(group.noads[0]).family == font_family::roman);
            CHECK(std::get<math_char>(group.noads[1]).family == font_family::roman);
        }

        TEST_CASE("operatorname becomes math chars in roman font")
        {
            const auto [result, error] = parse("\\operatorname{ab}");
            const mlist_with_kind& group = std::get<mlist_with_kind_wrapper>(result[0]);
            CHECK(group.kind == item_kind::op);
            CHECK(node_types_are<math_char, math_char>(group.noads));
            CHECK(std::get<math_char>(group.noads[0]).family == font_family::roman);
            CHECK(std::get<math_char>(group.noads[1]).family == font_family::roman);
        }

        TEST_CASE("with subscript becomes big op")
        {
            const auto [result, error] = parse("\\lim_x");
            const mlist_with_kind& group = std::get<mlist_with_kind_wrapper>(result[0]);
            CHECK(group.kind == item_kind::op);
            CHECK(node_types_are<big_op_wrapper>(group.noads));
        }
    }
}