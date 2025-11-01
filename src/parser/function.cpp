#include "parser/function.hpp"

#include "parser/big_op.hpp"
#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"
#include "parser/unicode_index.hpp"

#include <algorithm>
#include <array>
#include <string_view>

namespace mfl::parser
{
    namespace
    {
        using namespace std::string_view_literals;

        const auto function_names =
            std::array{"arccos"sv, "csc"sv, "ker"sv,    "min"sv,  "arcsin"sv, "deg"sv, "lg"sv,     "Pr"sv,
                       "arctan"sv, "det"sv, "lim"sv,    "sec"sv,  "arg"sv,    "dim"sv, "liminf"sv, "sin"sv,
                       "cos"sv,    "exp"sv, "limsup"sv, "sinh"sv, "cosh"sv,   "gcd"sv, "ln"sv,     "sup"sv,
                       "cot"sv,    "hom"sv, "log"sv,    "tan"sv,  "coth"sv,   "inf"sv, "max"sv,    "tanh"sv};

        const auto sub_function_names =
            std::array{"det"sv, "gcd"sv, "inf"sv, "lim"sv, "liminf"sv, "limsup"sv, "max"sv, "min"sv, "Pr"sv, "sup"sv};

        bool is_sub_function(const std::string& name) { return std::ranges::contains(sub_function_names, name); }
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

    bool is_function(const std::string& name) { return std::ranges::contains(function_names, name); }

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
