#include "parser/lexer.hpp"

#include "framework/doctest.hpp"

#include <sstream>

namespace mfl::parser
{
    TEST_SUITE("lexer")
    {
        TEST_CASE("eof is only token when input string is empty")
        {
            std::istringstream is("");
            lexer lx(is);
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::eof);
        }

        TEST_CASE("utf8 symbol")
        {
            std::istringstream is("∀");
            lexer lx(is);
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::symbol);
            CHECK(lx.value()[0] == char(0xe2));
            CHECK(lx.value()[1] == char(0x88));
            CHECK(lx.value()[2] == char(0x80));  // 0xe28880 is the utf8 encoding of 0x2200, the "for all" symbol: ∀
        }

        TEST_CASE("all tokens")
        {
            std::istringstream is(R"(\abc  { b ∀   \$ } \frac \not< \not\eq \, _ ^ ')");
            lexer lx(is);
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::command);
            CHECK(lx.value() == "abc");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::open_brace);
            CHECK(lx.value() == "{");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::symbol);
            CHECK(lx.value() == "b");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::symbol);
            CHECK(lx.value()[0] == char(0xe2));
            CHECK(lx.value()[1] == char(0x88));
            CHECK(lx.value()[2] == char(0x80));  // 0xe28880 is the utf8 encoding of 0x2200, the "for all" symbol: ∀
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::symbol);
            CHECK(lx.value() == "$");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::close_brace);
            CHECK(lx.value() == "}");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::command);
            CHECK(lx.value() == "frac");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::command);
            CHECK(lx.value() == "not<");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::command);
            CHECK(lx.value() == "not\\eq");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::command);
            CHECK(lx.value() == ",");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::subscript);
            CHECK(lx.value() == "_");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::superscript);
            CHECK(lx.value() == "^");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::prime);
            CHECK(lx.value() == "'");
            lx.move_to_next_token();
            CHECK(lx.token() == tokens::eof);
        }
    }
}