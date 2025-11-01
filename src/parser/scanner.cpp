#include "parser/scanner.hpp"

namespace mfl::parser
{
    scanner::scanner(std::istream& is) : is_(is) { skip_whitespace(); }

    char scanner::current_char() const { return current_char_; }

    void scanner::skip_whitespace()
    {
        bool ok = true;
        char c = ' ';
        while ((std::isspace(c) != 0) && ok)
            ok = get_char(c);

        current_char_ = c;
    }

    void scanner::skip_char() { [[maybe_unused]] const auto unused = get_char(current_char_); }

    bool scanner::is_at_end() const { return is_.eof(); }
}
