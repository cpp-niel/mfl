#include "parser/tokens.hpp"

#include "framework/doctest.hpp"

namespace mfl::parser
{
    TEST_CASE("to string")
    {
        CHECK(to_string(tokens::unknown) == "unknown token");
        CHECK(to_string(tokens::eof) == "end of input");
        CHECK(to_string(tokens::command) == "command token");
        CHECK(to_string(tokens::symbol) == "symbol token");
        CHECK(to_string(tokens::open_brace) == "opening brace");
        CHECK(to_string(tokens::close_brace) == "closing brace");
        CHECK(to_string(tokens::subscript) == "subscript '_'");
        CHECK(to_string(tokens::superscript) == "superscript '^'");
        CHECK(to_string(tokens::prime) == "apostrophe");
    }
}