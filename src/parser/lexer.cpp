#include "parser/lexer.hpp"

#include "parser/utf8.hpp"

#include <istream>
#include <string_view>

namespace mfl::parser
{
    namespace
    {
        using namespace std::string_view_literals;

        bool is_escaped_symbol(const char c) { return "#$&~_^%{}"sv.find(c) != std::string::npos; }

        bool is_command_symbol(const char c) { return ",/>:;!| "sv.find(c) != std::string::npos; }

        std::pair<bool, tokens> is_punctuator(const char c)
        {
            if (c == '{') return {true, tokens::open_brace};

            if (c == '}') return {true, tokens::close_brace};

            if (c == '_') return {true, tokens::subscript};

            if (c == '^') return {true, tokens::superscript};

            if (c == '\'') return {true, tokens::prime};

            return {false, tokens::unknown};
        }

        void skip(scanner& s)
        {
            while (!s.is_at_end())
            {
                if (isspace(s.current_char()) != 0)
                    s.skip_whitespace();
                else
                    break;
            }
        }
    }

    lexer::lexer(std::istream& is) : scanner_(is) {}

    void lexer::move_to_next_token()
    {
        value_.clear();

        skip(scanner_);

        token_start_ = scanner_.location();

        if (scanner_.current_char() == '\\')
        {
            scanner_.skip_char();
            if (is_escaped_symbol(scanner_.current_char()))
            {
                value_.push_back(scanner_.current_char());
                token_ = tokens::symbol;
                scanner_.skip_char();
            }
            else if (is_command_symbol(scanner_.current_char()))
            {
                value_.push_back(scanner_.current_char());
                token_ = tokens::command;
                scanner_.skip_char();
            }
            else
            {
                value_ = scanner_.take_while(isalpha);
                token_ = tokens::command;
                if (value_ == "not")
                {
                    if ("<>="sv.find(scanner_.current_char()) != std::string::npos)
                    {
                        value_.push_back(scanner_.current_char());
                        scanner_.skip_char();
                    }
                    else if (scanner_.current_char() == '\\')
                    {
                        scanner_.skip_char();
                        value_ += "\\" + scanner_.take_while(isalpha);
                    }
                }
            }
        }
        else if (auto [result, tok] = is_punctuator(scanner_.current_char()); result)
        {
            value_.push_back(scanner_.current_char());
            token_ = tok;
            scanner_.skip_char();
        }
        else if (!scanner_.is_at_end())
        {
            code_point result_code_point = 0;
            std::uint32_t state = 0;

            const auto decode_utf8 = [&](const char c) {
                return utf8::decode(&state, &result_code_point, c) != utf8::accept_utf8_decoding;
            };

            if (decode_utf8(scanner_.current_char())) value_ = scanner_.take_while(decode_utf8);

            value_.push_back(scanner_.current_char());
            scanner_.skip_char();

            token_ = (state == utf8::accept_utf8_decoding) ? tokens::symbol : tokens::unknown;
        }
        else
        {
            value_.push_back(scanner_.current_char());
            token_ = tokens::eof;
        }

        prev_token_end_ = token_end_;
        token_end_ = scanner_.location();
    }
}
