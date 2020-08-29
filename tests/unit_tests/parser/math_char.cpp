#include "parser/parse.hpp"

#include "framework/doctest.hpp"
#include "framework/node_types_are.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse math char")
    {
        TEST_CASE("parse a single math_char")
        {
            const auto expected = math_char{.kind = item_kind::bin, .family = font_family::roman, .char_code = 43};
            const auto [noads, error] = parse("+");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(expected.kind == actual.kind);
            CHECK(expected.char_code == actual.char_code);
            CHECK(expected.family == actual.family);
        }

        //TEST_CASE("parse a single multi-byte utf8 math_char")
        //{
        //    const auto expected = math_char{.kind = item_kind::bin, .family = font_family::roman, .char_code = 0x2200};
        //    const auto [noads, error] = parse("∀");
        //    const auto actual = std::get<math_char>(noads[0]);
        //    CHECK(expected.kind == actual.kind);
        //    CHECK(expected.char_code == actual.char_code);
        //    CHECK(expected.family == actual.family);
        //}

        TEST_CASE("parse a single math_char variable")
        {
            const auto italic_x = 0x1d465;
            const auto expected =
                math_char{.kind = item_kind::ord, .family = font_family::roman, .char_code = italic_x};
            const auto [noads, error] = parse("x");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(expected.kind == actual.kind);
            CHECK(expected.char_code == actual.char_code);
            CHECK(expected.family == actual.family);
        }

        TEST_CASE("parse a single math_char in italic context")
        {
            const auto expected = math_char{.kind = item_kind::ord, .family = font_family::italic, .char_code = 120};
            const auto [noads, error] = parse("\\mathit{x}");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(expected.kind == actual.kind);
            CHECK(expected.char_code == actual.char_code);
            CHECK(expected.family == actual.family);
        }

        TEST_CASE("parse a single math_char in bold context")
        {
            const auto expected = math_char{.kind = item_kind::ord, .family = font_family::bold, .char_code = 120};
            const auto [noads, error] = parse("\\mathbf{x}");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(expected.kind == actual.kind);
            CHECK(expected.char_code == actual.char_code);
            CHECK(expected.family == actual.family);
        }

        TEST_CASE("parse a sequence of math_chars")
        {
            const auto [result, error] = parse("a1b2");
            CHECK(node_types_are<math_char, math_char, math_char, math_char>(result));
        }

        TEST_CASE("kind is ord and font family is roman for letters")
        {
            const auto [noads, error] = parse("a");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(actual.kind == item_kind::ord);
            CHECK(actual.family == font_family::roman);
        }

        TEST_CASE("kind is ord and font family is roman for digits")
        {
            const auto [noads, error] = parse("2");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(actual.kind == item_kind::ord);
            CHECK(actual.family == font_family::roman);
        }

        TEST_CASE("kind is bin and font family is roman for binary operators")
        {
            const auto [noads, error] = parse("*");
            const auto actual = std::get<math_char>(noads[0]);
                CHECK(actual.kind == item_kind::bin);
                CHECK(actual.family == font_family::roman);
        }

        TEST_CASE("kind is bin and font family is roman for command binary operators")
        {
            const auto [noads, error] = parse("\\boxplus");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(actual.kind == item_kind::bin);
            CHECK(actual.family == font_family::roman);
        }

        TEST_CASE("kind is rel and font family is roman for arrows")
        {
            const auto [noads, error] = parse("\\leftarrow");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(actual.kind == item_kind::rel);
            CHECK(actual.family == font_family::roman);
        }

        TEST_CASE("kind is open and font family is roman for left delimiters")
        {
            const auto [noads, error] = parse("[");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(actual.kind == item_kind::open);
            CHECK(actual.family == font_family::roman);
        }

        TEST_CASE("kind is open and font family is roman for command left delimiters")
        {
            const auto [noads, error] = parse("\\lfloor");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(actual.kind == item_kind::open);
            CHECK(actual.family == font_family::roman);
        }

        TEST_CASE("kind is close and font family is roman for right delimiters")
        {
            const auto [noads, error] = parse(")");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(actual.kind == item_kind::close);
            CHECK(actual.family == font_family::roman);
        }

        TEST_CASE("kind is close and font family is roman for command right delimiters")
        {
            const auto [noads, error] = parse("\\rceil");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(actual.kind == item_kind::close);
            CHECK(actual.family == font_family::roman);
        }

        TEST_CASE("kind is op and font family is roman for unicode characters")
        {
            const auto [noads, error] = parse("\\pi");
            const auto actual = std::get<math_char>(noads[0]);
            CHECK(actual.kind == item_kind::ord);
            CHECK(actual.family == font_family::roman);
        }
    }
}
