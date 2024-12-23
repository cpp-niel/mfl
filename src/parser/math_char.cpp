#include "parser/math_char.hpp"

#include "parser/parser_state.hpp"
#include "parser/symbols/arrows.hpp"
#include "parser/symbols/binary_operators.hpp"
#include "parser/symbols/relational_operators.hpp"
#include <parser/symbols/delimiters.hpp>
#include <parser/symbols/punctuation.hpp>

#include <algorithm>
#include <ranges>

namespace mfl::parser
{
    namespace
    {
        using namespace std::string_literals;

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
                if (std::ranges::contains(binary_operators | std::views::keys, name)
                    || std::ranges::contains(additional_binary_operators | std::views::keys, name))
                    return item_kind::bin;

                if (std::ranges::contains(relational_operators | std::views::keys, name)
                    || std::ranges::contains(additional_relational_operators | std::views::keys, name)
                    || std::ranges::contains(negations | std::views::keys, name)
                    || std::ranges::contains(additional_negations | std::views::keys, name)
                    || std::ranges::contains(arrows | std::views::keys, name)
                    || std::ranges::contains(additional_arrows | std::views::keys, name))
                    return item_kind::rel;

                if (std::ranges::contains(left_delimiters | std::views::keys, name)) return item_kind::open;

                if (std::ranges::contains(right_delimiters | std::views::keys, name)) return item_kind::close;

                if (std::ranges::contains(punctuation_symbols | std::views::keys, name)) return item_kind::punct;
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
