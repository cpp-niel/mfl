#pragma once

#include <string>

namespace mfl::parser
{
    enum class tokens
    {
        unknown = 0,
        eof,
        command,
        symbol,
        open_brace,
        close_brace,
        subscript,
        superscript,
        prime,
    };

    [[nodiscard]] std::string to_string(tokens t);
}
