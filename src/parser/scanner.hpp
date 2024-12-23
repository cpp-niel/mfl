#pragma once

#include <fmt/format.h>

#include <concepts>
#include <iostream>
#include <istream>
#include <string>

namespace mfl::parser
{
    class stream_location
    {
    public:
        void increment_column() { ++column_; }

        [[nodiscard]] size_t column() const { return column_; }

    private:
        size_t column_ = 1;
    };

    class scanner
    {
    public:
        explicit scanner(std::istream& is);

        [[nodiscard]] bool is_at_end() const;
        [[nodiscard]] char current_char() const;

        void skip_whitespace();
        void skip_char();

        [[nodiscard]] std::string take_while(const std::predicate<char> auto& is_predicate_true)
        {
            std::string result(1, current_char_);
            while (get_char(current_char_) && is_predicate_true(current_char_))
            {
                result.append(&current_char_, 1);
            }

            return result;
        }

        [[nodiscard]] stream_location location() const { return location_; }

    private:
        [[nodiscard]] bool get_char(char& c)
        {
            location_ = next_location_;

            is_.get(c);
            next_location_.increment_column();

            if (!is_) c = ' ';
            return !is_.fail();
        }

        std::istream& is_;
        stream_location location_;
        stream_location next_location_;
        char current_char_ = 0;
    };
}
