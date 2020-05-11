#include "parser/math_space.hpp"

#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"

#include <range/v3/algorithm/find_if.hpp>

#include <string_view>

namespace mfl::parser
{
    namespace
    {
        using namespace std::string_view_literals;

        const std::array<std::pair<std::string_view, int>, 10> explicit_spaces = {{
            {","sv, 3},
            {"/"sv, 3},
            {">"sv, 4},
            {":"sv, 4},
            {";"sv, 5},
            {" "sv, 9},
            {"enspace"sv, 9},
            {"quad"sv, 18},
            {"qquad"sv, 36},
            {"!"sv, -3},
        }};
    }
    bool is_explicit_space(const std::string& name)
    {
        return (ranges::find_if(explicit_spaces, [&](const auto& p) { return p.first == name; })
                != explicit_spaces.end());
    }

    math_space create_explicit_space(parser_state& state)
    {
        const auto name = state.consume_lexer_value();

        const auto it = ranges::find_if(explicit_spaces, [&](const auto& p) { return p.first == name; });

        return {.space = kern{.size = it->second}, .is_math_units = true};
    }

    math_space create_hspace(parser_state& state)
    {
        state.consume_token(tokens::command);

        const auto factor = braced_value(state);
        const auto size = static_cast<dist_t>(factor * 18.0);

        return {.space = kern{.size = size}, .is_math_units = true};
    }
}
