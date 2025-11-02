#include "parser/tokens.hpp"

namespace mfl::parser
{
    std::string to_string(const tokens t)
    {
        switch (t)
        {
        case tokens::unknown:
            return "unknown token";
        case tokens::eof:
            return "end of input";
        case tokens::command:
            return "command token";
        case tokens::symbol:
            return "symbol token";
        case tokens::open_brace:
            return "opening brace";
        case tokens::close_brace:
            return "closing brace";
        case tokens::subscript:
            return "subscript '_'";
        case tokens::superscript:
            return "superscript '^'";
        case tokens::prime:
            return "apostrophe";
        }

        return "invalid token";
    }
}