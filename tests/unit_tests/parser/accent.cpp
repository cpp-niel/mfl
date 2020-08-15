#include "parser/parse.hpp"

#include <doctest/doctest.h>
#include "framework/node_types_are.hpp"
#include "noad/accent.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse accent")
    {
        TEST_CASE("normal accent over no chars")
        {
            const auto [result, error] = parse("\\hat{}");
            CHECK(node_types_are<accent_wrapper>(result));
            const accent& a = std::get<accent_wrapper>(result[0]);
            CHECK(!a.is_wide);
            CHECK(a.noads.empty());
        }

        TEST_CASE("normal accent over single char")
        {
            const auto [result, error] = parse("\\hat{x}");
            CHECK(node_types_are<accent_wrapper>(result));
            const accent& a = std::get<accent_wrapper>(result[0]);
            CHECK(!a.is_wide);
            CHECK(node_types_are<math_char>(a.noads));
        }

        TEST_CASE("normal accent over multiple chars")
        {
            const auto [result, error] = parse("\\hat{xyz}");
            CHECK(node_types_are<accent_wrapper>(result));
            const accent& a = std::get<accent_wrapper>(result[0]);
            CHECK(!a.is_wide);
            CHECK(node_types_are<math_char, math_char, math_char>(a.noads));
        }

        TEST_CASE("wide accent over no chars")
        {
            const auto [result, error] = parse("\\widehat{}");
            CHECK(node_types_are<accent_wrapper>(result));
            const accent& a = std::get<accent_wrapper>(result[0]);
            CHECK(a.is_wide);
            CHECK(a.noads.empty());
        }

        TEST_CASE("wide accent over single char")
        {
            const auto [result, error] = parse("\\widehat{x}");
            CHECK(node_types_are<accent_wrapper>(result));
            const accent& a = std::get<accent_wrapper>(result[0]);
            CHECK(a.is_wide);
            CHECK(node_types_are<math_char>(a.noads));
        }

        TEST_CASE("wide accent over multiple chars")
        {
            const auto [result, error] = parse("\\widehat{xyz}");
            CHECK(node_types_are<accent_wrapper>(result));
            const accent& a = std::get<accent_wrapper>(result[0]);
            CHECK(a.is_wide);
            CHECK(node_types_are<math_char, math_char, math_char>(a.noads));
        }
    }
}
