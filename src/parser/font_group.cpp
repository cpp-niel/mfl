#include "parser/font_group.hpp"

#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"
#include "parser/unicode_index.hpp"

#include <range/v3/algorithm/find_if.hpp>

#include <array>

namespace mfl::parser
{
    namespace
    {
        using namespace std::string_literals;

        const std::array<std::pair<std::string, font_choice>, 9> font_commands = {
            std::pair{"mathcal"s, font_choice::calligraphic},
            {"mathnormal"s, font_choice::normal},
            {"mathbf"s, font_choice::bold},
            {"mathit"s, font_choice::italic},
            {"mathrm"s, font_choice::roman},
            {"mathsf"s, font_choice::sans},
            {"mathtt"s, font_choice::mono},
            {"mathbb"s, font_choice::blackboard},
            {"mathfrak"s, font_choice::fraktur},
        };
    }
    bool is_font_choice(const std::string& name)
    {
        return (ranges::find_if(font_commands, [&](const auto c) { return c.first == name; }) != font_commands.end());
    }

    std::vector<noad> create_font_group(parser_state& state)
    {
        const auto name = state.consume_lexer_value();
        const auto it = ranges::find_if(font_commands, [&](const auto c) { return c.first == name; });
        scoped_state s(state, {.font = it->second});

        return parse_expression(state);
    }

}
