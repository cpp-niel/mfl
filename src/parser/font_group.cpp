#include "parser/font_group.hpp"

#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"
#include "parser/unicode_index.hpp"

#include <array>
#include <string_view>

namespace mfl::parser
{
    namespace
    {
        using namespace std::string_view_literals;

        const auto font_commands = std::array<std::pair<std::string_view, font_choice>, 9>{{
            {"mathcal"sv, font_choice::calligraphic},
            {"mathnormal"sv, font_choice::normal},
            {"mathbf"sv, font_choice::bold},
            {"mathit"sv, font_choice::italic},
            {"mathrm"sv, font_choice::roman},
            {"mathsf"sv, font_choice::sans},
            {"mathtt"sv, font_choice::mono},
            {"mathbb"sv, font_choice::blackboard},
            {"mathfrak"sv, font_choice::fraktur},
        }};
    }
    bool is_font_choice(const std::string& name)
    {
        return (std::ranges::find_if(font_commands, [&](const auto c) { return c.first == name; }) != font_commands.end());
    }

    std::vector<noad> create_font_group(parser_state& state)
    {
        const auto name = state.consume_lexer_value();
        const auto it = std::ranges::find_if(font_commands, [&](const auto c) { return c.first == name; });
        scoped_state s(state, {.font = it->second});

        return parse_expression(state);
    }

}
