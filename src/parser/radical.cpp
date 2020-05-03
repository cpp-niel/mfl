#include "parser/radical.hpp"

#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"

namespace mfl::parser
{
    namespace
    {
        optional_noads parse_radical_degree(parser_state& state)
        {
            if ((state.lexer_token() == tokens::symbol) && (state.lexer_value() == "["))
            {
                state.consume_token(tokens::symbol);
                const auto result = parse_until_token_and_value(state, tokens::symbol, "]");
                return result;
            }

            return std::nullopt;
        }
    }

    radical create_radical(parser_state& state)
    {
        state.consume_token(tokens::command);
        return {.degree = parse_radical_degree(state), .radicand = parse_required_group(state)};
    }
}
