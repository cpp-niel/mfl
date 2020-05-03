#include "parser/unicode_index.hpp"

#include "framework/doctest.hpp"
#include "parser/parser_state.hpp"

namespace mfl::parser
{
    namespace
    {
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
    }
}