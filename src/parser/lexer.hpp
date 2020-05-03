#pragma once

#include "parser/scanner.hpp"
#include "parser/tokens.hpp"

#include <iosfwd>
#include <string>

namespace mfl::parser
{
    class lexer
    {
    public:
        explicit lexer(std::istream& is);

        void move_to_next_token();
        [[nodiscard]] tokens token() const { return token_; }
        [[nodiscard]] std::string value() const { return value_; }

        [[nodiscard]] stream_location token_start() const { return token_start_; }
        [[nodiscard]] stream_location token_end() const { return token_end_; }
        [[nodiscard]] stream_location prev_token_end() const { return prev_token_end_; }

    private:
        scanner scanner_;
        stream_location token_start_;
        stream_location token_end_;
        stream_location prev_token_end_;
        tokens token_ = tokens::unknown;
        std::string value_;
    };
}
