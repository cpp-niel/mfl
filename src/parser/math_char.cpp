#include "parser/math_char.hpp"

#include "parser/parser_state.hpp"
#include "parser/symbols/arrows.hpp"
#include "parser/symbols/binary_operators.hpp"
#include <parser/symbols/delimiters.hpp>
#include <parser/symbols/punctuation.hpp>
#include "parser/symbols/relational_operators.hpp"

#include <range/v3/algorithm/contains.hpp>
#include <range/v3/view/map.hpp>

#include <array>

namespace mfl::parser
{
    namespace
    {
        using namespace std::string_literals;
        namespace rv = ranges::views;

        item_kind char_kind(const std::string_view s)
        {
            if (s.length() == 1)
            {
                const auto c = s[0];
                if ((c == '(') or (c == '[') or (c == '{')) return item_kind::open;
                if ((c == ')') or (c == ']') or (c == '}')) return item_kind::close;
                if ((c == ':') or (c == '=') or (c == '<') or (c == '>')) return item_kind::rel;
                if ((c == '?') or (c == '!') or (c == ';') or (c == ',')) return item_kind::punct;
                if ((c == '+') or (c == '-') or (c == '*')) return item_kind::bin;
            }

            return item_kind::ord;
        }

        item_kind symbol_kind(const std::string_view symbol_name)
        {
            if (symbol_name.length() == 1) return char_kind(symbol_name);

            if (symbol_name.starts_with("\\"))
            {
                const auto name = symbol_name.substr(1);
                if (ranges::contains(rv::keys(binary_operators), name)
                    or ranges::contains(rv::keys(additional_binary_operators), name))
                    return item_kind::bin;

                if (ranges::contains(rv::keys(relational_operators), name)
                    or ranges::contains(rv::keys(additional_relational_operators), name)
                    or ranges::contains(rv::keys(negations), name)
                    or ranges::contains(rv::keys(additional_negations), name)
                    or ranges::contains(rv::keys(arrows), name) or ranges::contains(rv::keys(additional_arrows), name))
                    return item_kind::rel;

                if (ranges::contains(rv::keys(left_delimiters), name)) return item_kind::open;

                if (ranges::contains(rv::keys(right_delimiters), name)) return item_kind::close;

                if (ranges::contains(rv::keys(punctuation_symbols), name)) return item_kind::punct;
            }

            return item_kind::ord;
        }
    }

    math_char create_math_char(const font_choice font, const std::string& symbol_string, parser_state& state)
    {
        auto family = font_family::roman;
        if (font == font_choice::italic)  // TODO where to check if it's a symbol that can be rendered in italics?
            family = font_family::italic;
        else if (font == font_choice::bold)
            family = font_family::bold;

        const auto char_code = math_alpha_index(unicode_index(symbol_string, state), font);
        return {.kind = symbol_kind(symbol_string), .family = family, .char_code = char_code};
    }

    math_char create_math_char(parser_state& state)
    {
        const auto symbol_string = state.consume_lexer_value();
        return create_math_char(state.get_font_choice(), symbol_string, state);
    }
}
