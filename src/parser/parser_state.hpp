#pragma once

#include "parser/lexer.hpp"
#include "parser/unicode_index.hpp"

#include <optional>
#include <stack>
#include <string>
#include <utility>

namespace mfl::parser
{
    struct scope_settings
    {
        font_choice font = font_choice::normal;
    };

    class parser_state
    {
    public:
        explicit parser_state(lexer& lex);

        [[nodiscard]] tokens lexer_token() const;
        void consume_token(const tokens tok);
        [[nodiscard]] std::string lexer_value() const;
        [[nodiscard]] std::string consume_lexer_value() const;
        [[nodiscard]] std::pair<tokens, std::string> lexer_state() const;

        void set_font_choice(const font_choice choice);
        [[nodiscard]] font_choice get_font_choice() const;

        void set_error(const std::string& error_text);

        [[nodiscard]] std::optional<std::string> error() const;

        void push_scope();
        void pop_scope();

    private:
        lexer& lexer_;
        std::stack<scope_settings> scopes_;
        std::optional<std::string> error_;
    };

    class scoped_state
    {
    public:
        scoped_state(parser_state& state, const scope_settings& settings);
        ~scoped_state();

        scoped_state(const scoped_state&) = delete;
        scoped_state& operator=(const scoped_state&) = delete;
        scoped_state(scoped_state&&) = delete;
        scoped_state& operator=(scoped_state&&) = delete;
    private:
        parser_state& state_;
    };
}