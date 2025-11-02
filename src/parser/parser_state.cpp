#include "parser/parser_state.hpp"

#include <fmt/format.h>

namespace mfl::parser
{
    parser_state::parser_state(lexer& lex) : lexer_(lex)
    {
        lexer_.move_to_next_token();
        scopes_.emplace();
    }

    tokens parser_state::lexer_token() const { return lexer_.token(); }

    void parser_state::consume_token(const tokens tok)
    {
        if (lexer_token() == tokens::eof)
            set_error("unexpected end of input.");
        else if (lexer_token() != tok)
            set_error(fmt::format("unexpected {} while expecting {}.", to_string(lexer_token()), to_string(tok)));
        else
            lexer_.move_to_next_token();
    }

    std::string parser_state::lexer_value() const { return lexer_.value(); }

    std::string parser_state::consume_lexer_value() const
    {
        auto movable_result = lexer_.value();
        lexer_.move_to_next_token();
        return movable_result;
    }

    std::pair<tokens, std::string> parser_state::lexer_state() const { return {lexer_.token(), lexer_.value()}; }

    void parser_state::set_font_choice(const font_choice choice) { scopes_.top().font = choice; }

    font_choice parser_state::get_font_choice() const { return scopes_.top().font; }

    void parser_state::push_scope() { scopes_.push({.font = get_font_choice()}); }

    void parser_state::pop_scope() { scopes_.pop(); }

    void parser_state::set_error(const std::string& error_text)
    {
        error_ = fmt::format("Syntax error: {}", error_text);

        while (lexer_token() != tokens::eof)
            lexer_.move_to_next_token();
    }

    std::optional<std::string> parser_state::error() const { return error_; }

    scoped_state::scoped_state(parser_state& state, const scope_settings& settings) : state_(state)
    {
        state_.push_scope();
        state_.set_font_choice(settings.font);
    }

    scoped_state::~scoped_state() { state_.pop_scope(); }
}