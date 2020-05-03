#include "parser/parser_utilities.hpp"

#include "parser/command.hpp"
#include "parser/math_char.hpp"
#include "parser/parser_state.hpp"
#include "parser/script.hpp"

#include <fmt/format.h>

#include <charconv>
#include <cstdlib>

namespace mfl::parser
{
    mlist parse_until_token(parser_state& state, const tokens end_token)
    {
        mlist result;
        auto tok = state.lexer_token();
        while (tok != end_token)
        {
            if ((state.lexer_token() == tokens::eof) && (end_token != tokens::eof))
            {
                if (!state.error())
                    state.set_error("expected token not found");

                return {};
            }

            auto noads = create_script(state);
            std::move(noads.begin(), noads.end(), std::back_inserter(result.noads));
            tok = state.lexer_token();
        }

        return result;
    }

    std::vector<noad> parse_until_token_and_value(parser_state& state, const tokens& end_token,
                                                  const std::string& value)
    {
        std::vector<noad> result;
        while (state.lexer_state() != std::pair{end_token, value})
        {
            if (state.lexer_token() == tokens::eof)
            {
                state.set_error(fmt::format("expected token not found: '{}'.", value));
                return {};
            }

            auto noads = create_script(state);
            std::move(noads.begin(), noads.end(), std::back_inserter(result));
        }

        state.consume_token(end_token);
        return result;
    }

    std::vector<noad> parse_required_group(parser_state& state)
    {
        state.consume_token(tokens::open_brace);
        scoped_state s(state, {.font = state.get_font_choice()});

        const auto noad_list = parse_until_token(state, tokens::close_brace);

        state.consume_token(tokens::close_brace);

        return noad_list.noads;
    }

    std::pair<optional_noads, optional_noads> parse_sub_sup(parser_state& state)
    {
        if (state.lexer_token() == tokens::superscript)
        {
            state.consume_token(tokens::superscript);
            return {std::nullopt, parse_expression(state)};
        }

        if (state.lexer_token() == tokens::subscript)
        {
            state.consume_token(tokens::subscript);
            return {parse_expression(state), std::nullopt};
        }

        return {};
    }

    std::vector<noad> parse_item(parser_state& state)
    {
        std::vector<noad> result;
        auto tok = state.lexer_token();
        if (tok == tokens::symbol)
            result.push_back(create_math_char(state));
        else if (tok == tokens::command)
        {
            auto noads = create_command(state);
            std::move(noads.begin(), noads.end(), std::back_inserter(result));
        }
        else
            state.set_error("unexpected token.");

        return result;
    }

    std::vector<noad> parse_expression(parser_state& state)
    {
        if (state.lexer_token() == tokens::open_brace)
            return parse_required_group(state);

        return parse_item(state);
    }

    double braced_value(parser_state& state)
    {
        state.consume_token(tokens::open_brace);

        std::string number_string;
        while ((state.lexer_token() != tokens::close_brace) && (state.lexer_token() != tokens::eof))
        {
            number_string.append(state.consume_lexer_value());
        }

        state.consume_token(tokens::close_brace);

        char* str_end = nullptr;
        auto value = std::strtod(number_string.c_str(), &str_end);
        if (str_end != number_string.data() + number_string.size())
            state.set_error(fmt::format("'{}' does not represent a valid floating point value.", number_string));

        // TODO - do this instead when from_chars becomes available ...
        //const auto result = std::from_chars(number_string.data(), number_string.data() + number_string.size(), &value);
        //if (result.ec == std::errc::invalid_argument)
        //    state.set_error(fmt::format("'{}' does not represent a valid floating point value.", number_string));

        return value;
    }
}
