#include "parser/unicode_index.hpp"

#include "parser/parser_state.hpp"

#include "framework/doctest.hpp"

#include <sstream>

namespace mfl::parser
{
    namespace
    {
        // This is not officially supported anyway, but the utf8 string literals have to become
        // std::strings in some way.
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        std::string utf8_string(const char8_t* chars) { return std::string(reinterpret_cast<const char*>(chars)); }
    }

    TEST_SUITE("unicode index")
    {
        TEST_CASE("[unicode_index] conversion from bytes to code point")
        {
            std::istringstream is;
            lexer lex(is);
            parser_state s(lex);
            CHECK(unicode_index("A", s) == 0x0041);
            CHECK(unicode_index(utf8_string(u8"∀"), s) == 0x2200);
            CHECK(unicode_index(utf8_string(u8"x"), s) == 0x0078);
            CHECK(unicode_index(utf8_string(u8"∈"), s) == 0x2208);
            CHECK(unicode_index(utf8_string(u8"ℝ"), s) == 0x211d);
        }

        TEST_CASE("[unicode_index] error on conversion from empty utf8 character")
        {
            std::istringstream is;
            lexer lex(is);
            parser_state s(lex);
            [[maybe_unused]] auto i = unicode_index("", s);
            CHECK(*s.error() == "Syntax error: '' does not represent a valid utf8 character.");
        }

        TEST_CASE("[unicode_index] error on conversion from invalid utf8 character")
        {
            std::istringstream is;
            lexer lex(is);
            parser_state s(lex);
            [[maybe_unused]] auto i = unicode_index("nonsense", s);
            CHECK(*s.error() == "Syntax error: 'nonsense' does not represent a valid utf8 character.");
        }

        TEST_CASE("[unicode_index] conversion of special case characters")
        {
            std::istringstream is;
            lexer lex(is);
            parser_state s(lex);
            CHECK(unicode_index("-", s) == 0x2212);   // TeX prefers this character to regular '-' symbol
            CHECK(unicode_index("\\", s) == 0x005c);  // Backslash followed by characters is a TeX command
        }

        TEST_CASE("[unicode_index] conversion from TeX commands to code point")
        {
            std::istringstream is;
            lexer lex(is);
            parser_state s(lex);
            CHECK(unicode_index("\\pi", s) == 0x03c0);
            CHECK(unicode_index("\\forall", s) == 0x2200);
            CHECK(unicode_index("\\in", s) == 0x2208);
            CHECK(unicode_index("\\Sigma", s) == 0x03a3);
        }

        TEST_CASE("[unicode_index] exception on conversion from invalid TeX commands")
        {
            std::istringstream is;
            lexer lex(is);
            parser_state s(lex);
            [[maybe_unused]] auto i = unicode_index("\\nonsense", s);
            CHECK(*s.error() == "Syntax error: Unknown symbol name '\\nonsense'.");
        }

        TEST_CASE("[unicode_index] math alpha index")
        {
            CHECK(math_alpha_index('a', font_choice::normal) == 0x1d44e);
            CHECK(math_alpha_index('h', font_choice::normal) == 0x210e);

            CHECK(math_alpha_index('a', font_choice::roman) == 'a');
            CHECK(math_alpha_index('h', font_choice::roman) == 'h');

            CHECK(math_alpha_index('a', font_choice::italic) == 'a');
            CHECK(math_alpha_index('A', font_choice::italic) == 'A');

            CHECK(math_alpha_index('a', font_choice::bold) == 'a');
            CHECK(math_alpha_index('A', font_choice::bold) == 'A');

            CHECK(math_alpha_index('a', font_choice::sans) == 0x1d5ba);
            CHECK(math_alpha_index('A', font_choice::sans) == 0x1d5a0);

            CHECK(math_alpha_index('a', font_choice::mono) == 0x1d68a);
            CHECK(math_alpha_index('A', font_choice::mono) == 0x1d670);

            CHECK(math_alpha_index('a', font_choice::calligraphic) == 0x1d4b6);
            CHECK(math_alpha_index('A', font_choice::calligraphic) == 0x1d49c);
            CHECK(math_alpha_index('H', font_choice::calligraphic) == 0x210b);

            CHECK(math_alpha_index('a', font_choice::blackboard) == 0x1d552);
            CHECK(math_alpha_index('A', font_choice::blackboard) == 0x1d538);
            CHECK(math_alpha_index('H', font_choice::blackboard) == 0x210d);

            CHECK(math_alpha_index('a', font_choice::fraktur) == 0x1d51e);
            CHECK(math_alpha_index('A', font_choice::fraktur) == 0x1d504);
            CHECK(math_alpha_index('H', font_choice::fraktur) == 0x210c);
        }
    }
}