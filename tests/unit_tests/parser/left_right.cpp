#include "parser/parse.hpp"

#include <doctest/doctest.h>
#include "framework/node_types_are.hpp"
#include "noad/left_right.hpp"

namespace mfl::parser
{
    TEST_SUITE("left_right")
    {
        TEST_CASE("empty fully delimited group")
        {
            const auto [noads, error] = parse(R"(\left[ \right\rceil)");
            const left_right& result = std::get<left_right_wrapper>(noads[0]);
            CHECK(result.left_delim_code == '[');
            CHECK(result.right_delim_code == 8969);
            CHECK(result.noads.empty());
        }

        TEST_CASE("empty fully delimited group with delimiters in braces")
        {
            const auto [noads, error] = parse(R"(\left{[} \right{\rceil})");
            const left_right& result = std::get<left_right_wrapper>(noads[0]);
            CHECK(result.left_delim_code == '[');
            CHECK(result.right_delim_code == 8969);
            CHECK(result.noads.empty());
        }

        TEST_CASE("no left limit using dot")
        {
            const auto [noads, error] = parse(R"(\left. \right\rceil)");
            const left_right& result = std::get<left_right_wrapper>(noads[0]);
            CHECK(result.left_delim_code == 0);
            CHECK(result.right_delim_code == 8969);
            CHECK(result.noads.empty());
        }

        TEST_CASE("no left limit using empty braces")
        {
            const auto [noads, error] = parse(R"(\left{} \right\rceil)");
            const left_right& result = std::get<left_right_wrapper>(noads[0]);
            CHECK(result.left_delim_code == 0);
            CHECK(result.right_delim_code == 8969);
            CHECK(result.noads.empty());
        }

        TEST_CASE("no left limit using dot in empty braces")
        {
            const auto [noads, error] = parse(R"(\left{.} \right\rceil)");
            const left_right& result = std::get<left_right_wrapper>(noads[0]);
            CHECK(result.left_delim_code == 0);
            CHECK(result.right_delim_code == 8969);
            CHECK(result.noads.empty());
        }

        TEST_CASE("invalid delimiter")
        {
            const auto [noads, error] = parse(R"(\left<)");
            CHECK(*error == "Syntax error: '<' is not a valid delimiter.");
        }

        TEST_CASE("invalid delimiter in braces")
        {
            const auto [noads, error] = parse(R"(\left{<})");
            CHECK(*error == "Syntax error: '<' is not a valid delimiter.");
        }

        TEST_CASE("missing closing brace in left delimiter")
        {
            const auto [noads, error] = parse(R"(\left{[ \right{\rceil})");
            CHECK(*error == "Syntax error: unexpected command token while expecting closing brace.");
        }

        TEST_CASE("missing closing brace in right delimiter")
        {
            const auto [noads, error] = parse(R"(\left{[ \right{\rceil)");
            CHECK(*error == "Syntax error: unexpected command token while expecting closing brace.");
        }
    }
}
