#include "parser/parse.hpp"

#include "framework/doctest.hpp"
#include "framework/node_types_are.hpp"
#include "noad/big_op.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse big op")
    {
        TEST_CASE("with subscript and superscript")
        {
            const auto [result, error] = parse("\\sum_i^n");
            CHECK(node_types_are<big_op_wrapper>(result));
            const big_op& op = std::get<big_op_wrapper>(result[0]);
            CHECK(op.sub);
            CHECK(op.sup);
        }

        TEST_CASE("with subscript only")
        {
            const auto [result, error] = parse("\\sum_i");
            CHECK(node_types_are<big_op_wrapper>(result));
            const big_op& op = std::get<big_op_wrapper>(result[0]);
            CHECK(op.sub);
            CHECK(!op.sup);
        }

        TEST_CASE("with superscript only")
        {
            const auto [result, error] = parse("\\sum^n");
            CHECK(node_types_are<big_op_wrapper>(result));
            const big_op& op = std::get<big_op_wrapper>(result[0]);
            CHECK(!op.sub);
            CHECK(op.sup);
        }

        TEST_CASE("without subscript and superscript")
        {
            const auto [result, error] = parse("\\sum");
            CHECK(node_types_are<big_op_wrapper>(result));
            const big_op& op = std::get<big_op_wrapper>(result[0]);
            CHECK(!op.sub);
            CHECK(!op.sup);
        }

        TEST_CASE("limits are normal when not specified")
        {
            const auto [result, error] = parse("\\sum^n_i");
            const big_op& op = std::get<big_op_wrapper>(result[0]);
            CHECK(op.limits == big_op_limits::normal);
        }

        TEST_CASE("with explicit limits")
        {
            const auto [result, error] = parse("\\sum\\limits^n_i");
            const big_op& op = std::get<big_op_wrapper>(result[0]);
            CHECK(op.limits == big_op_limits::yes);
        }

        TEST_CASE("with explicit nolimits")
        {
            const auto [result, error] = parse("\\sum\\nolimits^n_i");
            const big_op& op = std::get<big_op_wrapper>(result[0]);
            CHECK(op.limits == big_op_limits::no);
        }
    }
}
