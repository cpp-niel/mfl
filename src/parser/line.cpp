#include "parser/line.hpp"

#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"

namespace mfl::parser
{
    overline create_overline(parser_state& state)
    {
        state.consume_token(tokens::command);
        return {.noads = parse_required_group(state)};
    }

    underline create_underline(parser_state& state)
    {
        state.consume_token(tokens::command);
        return {.noads = parse_required_group(state)};
    }
}
