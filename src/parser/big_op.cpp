#include "parser/big_op.hpp"

#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"
#include "parser/symbols/big_operators.hpp"

#include <range/v3/algorithm/contains.hpp>
#include <range/v3/view/map.hpp>

#include <array>

namespace mfl::parser
{
    namespace rv = ranges::views;

    bool is_big_op(const std::string& name)
    {
        return ranges::contains(rv::keys(big_op_symbols), name) || ranges::contains(rv::keys(integral_symbols), name);
    }

    big_op_limits big_op_limit_style(const std::string& name, parser_state& state)
    {
        if (state.lexer_token() == tokens::command)
        {
            if (state.lexer_value() == "limits")
            {
                state.consume_token(tokens::command);
                return big_op_limits::yes;
            }

            if (state.lexer_value() == "nolimits")
            {
                state.consume_token(tokens::command);
                return big_op_limits::no;
            }
        }

        if (ranges::contains(rv::keys(integral_symbols), name)) return big_op_limits::no;

        return big_op_limits::normal;
    }

    big_op create_big_op(parser_state& state)
    {
        const auto op_name = state.consume_lexer_value();

        const auto limits = big_op_limit_style(op_name, state);

        auto [sub0, sup0] = parse_sub_sup(state);
        auto [sub1, sup1] = parse_sub_sup(state);

        if (sub0 && sub1) state.set_error("multiple subscripts");

        if (sup0 && sup1) state.set_error("multiple superscripts");

        optional_noads sub = sub0 ? sub0 : sub1;
        optional_noads sup = sup0 ? sup0 : sup1;
        return {.limits = limits,
                .nucleus = {math_char{.kind = item_kind::op, .char_code = unicode_index("\\" + op_name, state)}},
                .sub = sub,
                .sup = sup};
    }
}
