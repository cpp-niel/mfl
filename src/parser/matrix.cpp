#include "parser/matrix.hpp"

#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"
#include "script.hpp"

namespace mfl::parser
{
    matrix_row parse_matrix_row(parser_state& state)
    {
        auto row = matrix_row{};
        auto cell = std::vector<noad>{};

        auto is_definition_complete = false;
        while (!is_definition_complete)
        {
            const auto tok = state.lexer_token();
            if (tok == tokens::eof) return {};

            if (tok == tokens::alignment_tab)
            {
                state.consume_token(tokens::alignment_tab);
                row.push_back(cell);
                cell.clear();
            }
            else if ((tok == tokens::command) && ((state.lexer_value() == "cr") || (state.lexer_value() == "\\")))
            {
                state.consume_token(tokens::command);
                row.push_back(cell);
                is_definition_complete = true;
            }
            else if (tok == tokens::close_brace)
            {
                row.push_back(cell);
                is_definition_complete = true;
            }
            else
            {
                cell.append_range(create_script(state));
            }
        }

        return row;
    }

    matrix create_matrix(parser_state& state)
    {
        state.consume_token(tokens::command);
        state.consume_token(tokens::open_brace);
        auto result = matrix{};
        auto tok = state.lexer_token();
        while (tok != tokens::close_brace)
        {
            if (state.lexer_token() == tokens::eof)
            {
                if (!state.error())
                    state.set_error("Input stream ends inside matrix definition. The closing brace may be missing.");

                return {};
            }

            result.rows.push_back(parse_matrix_row(state));
            tok = state.lexer_token();
        }

        state.consume_token(tokens::close_brace);
        return result;
    }
}
