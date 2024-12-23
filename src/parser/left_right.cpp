#include "parser/left_right.hpp"

#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <string>

namespace mfl::parser
{
    namespace
    {
        using namespace std::string_literals;
        using namespace std::string_view_literals;

        enum class delimiter_type
        {
            none,
            command,
            symbol
        };

        const auto command_delimiter_names =
            std::array{"|"sv,         "lfloor"sv,      "rfloor"sv,      "lceil"sv,   "rceil"sv,
                       "langle"sv,    "rangle"sv,      "uparrow"sv,     "Uparrow"sv, "downarrow"sv,
                       "Downarrow"sv, "updownarrow"sv, "Updownarrow"sv, "vert"sv,    "Vert"sv};

        delimiter_type get_delimiter_type(const std::string_view name)
        {
            if ((name.length() == 1) && (R"(()[]{}|/\)"sv.find(name[0]) != std::string::npos))
                return delimiter_type::symbol;

            if (name.starts_with("\\")
                && (std::ranges::contains(command_delimiter_names, name.substr(1, name.length() - 1))))
                return delimiter_type::command;

            return delimiter_type::none;
        }
    }

    code_point parse_delimiter(parser_state& state)
    {
        bool is_closing_brace_expected = false;
        if (state.lexer_token() == tokens::open_brace)
        {
            is_closing_brace_expected = true;
            state.consume_token(tokens::open_brace);
        }

        auto result = code_point(0);

        const auto tok = state.lexer_token();
        const auto is_empty_braces = (is_closing_brace_expected and (tok == tokens::close_brace));

        if (!is_empty_braces)
        {
            const auto name = state.consume_lexer_value();
            if (!((tok == tokens::symbol) and (name == ".")))
            {
                const auto is_valid_token = (tok == tokens::symbol) or (tok == tokens::command);
                const auto full_name = ((tok == tokens::command) ? "\\"s : ""s) + name;
                const auto type = get_delimiter_type(full_name);
                if (!is_valid_token or (type == delimiter_type::none))
                {
                    state.set_error(fmt::format("'{}' is not a valid delimiter.", full_name));
                    return 0;
                }

                result = unicode_index(full_name, state);
                if (state.error()) return 0;
            }
        }

        if (is_closing_brace_expected) state.consume_token(tokens::close_brace);

        return result;
    }

    left_right create_left_right(parser_state& state)
    {
        state.consume_token(tokens::command);

        const auto left_delim = parse_delimiter(state);
        if (state.error()) return {};

        auto noads = parse_until_token_and_value(state, tokens::command, "right");

        const auto right_delim = state.error() ? 0 : parse_delimiter(state);

        return {.left_delim_code = left_delim, .noads = std::move(noads), .right_delim_code = right_delim};
    }
}
