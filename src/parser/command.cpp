#include "parser/command.hpp"

#include "parser/accent.hpp"
#include "parser/big_op.hpp"
#include "parser/font_group.hpp"
#include "parser/fraction.hpp"
#include "parser/function.hpp"
#include "parser/left_right.hpp"
#include "parser/line.hpp"
#include "parser/math_char.hpp"
#include "parser/math_space.hpp"
#include "parser/parser_state.hpp"
#include "parser/radical.hpp"

namespace mfl::parser
{
    std::vector<noad> create_command(parser_state& state)
    {
        if (state.lexer_value() == "binom") return {create_binom(state)};

        if (state.lexer_value() == "frac") return {create_fraction(state)};

        if (state.lexer_value() == "genfrac") return {create_generalized_fraction(state)};

        if (state.lexer_value() == "hspace") return {create_hspace(state)};

        if (state.lexer_value() == "left") return {create_left_right(state)};

        if (state.lexer_value() == "sqrt") return {create_radical(state)};

        if (state.lexer_value() == "operatorname") return {create_operatorname(state)};

        if (state.lexer_value() == "overline") return {create_overline(state)};

        if (state.lexer_value() == "underline") return {create_underline(state)};

        if (is_font_choice(state.lexer_value())) return {create_font_group(state)};

        if (is_accent(state.lexer_value())) return {create_accent(state)};

        if (is_big_op(state.lexer_value())) return {create_big_op(state)};

        if (is_function(state.lexer_value())) return {create_function(state)};

        if (is_explicit_space(state.lexer_value())) return {create_explicit_space(state)};

        return {create_math_char(state.get_font_choice(), "\\" + state.consume_lexer_value(), state)};
    }

}
