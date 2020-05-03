#include "parser/function.hpp"

#include "parser/big_op.hpp"
#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"
#include "parser/unicode_index.hpp"

#include <range/v3/algorithm/contains.hpp>

#include <array>

namespace mfl::parser
{
    namespace
    {
        using namespace std::string_literals;

        const auto function_names = std::array{
            "arccos"s, "csc"s, "ker"s, "min"s,    "arcsin"s, "deg"s, "lg"s,   "Pr"s,     "arctan"s, "det"s,  "lim"s,
            "sec"s,    "arg"s, "dim"s, "liminf"s, "sin"s,    "cos"s, "exp"s,  "limsup"s, "sinh"s,   "cosh"s, "gcd"s,
            "ln"s,     "sup"s, "cot"s, "hom"s,    "log"s,    "tan"s, "coth"s, "inf"s,    "max"s,    "tanh"s};

        const auto sub_function_names =
            std::array{"det"s, "gcd"s, "inf"s, "lim"s, "liminf"s, "limsup"s, "max"s, "min"s, "Pr"s, "sup"s};

        bool is_sub_function(const std::string& name) { return ranges::contains(sub_function_names, name); }
    }

    mlist_with_kind create_operatorname(parser_state& state)
    {
        state.consume_token(tokens::command);

        state.consume_token(tokens::open_brace);
        scoped_state s(state, {.font = font_choice::roman});

        mlist_with_kind result{.kind = item_kind::op};
        result.noads = parse_until_token(state, tokens::close_brace).noads;

        state.consume_token(tokens::close_brace);

        return result;
    }

    bool is_function(const std::string& name) { return ranges::contains(function_names, name); }

    mlist_with_kind create_function(parser_state& state)
    {
        const auto name = state.consume_lexer_value();

        std::vector<noad> nucleus;
        for (auto c : name)
        {
            nucleus.emplace_back(math_char{.kind = item_kind::ord,
                                        .family = font_family::roman,
                                        .char_code = unicode_index(std::string_view(&c, 1), state)});
        }

        if (is_sub_function(name) && (state.lexer_token() == tokens::subscript))
        {
            auto [sub, sup] = parse_sub_sup(state);
            return {.kind = item_kind::op, .noads = {big_op{.nucleus = std::move(nucleus), .sub = sub}}};
        }

        return {.kind = item_kind::op, .noads = std::move(nucleus)};
    }
}
