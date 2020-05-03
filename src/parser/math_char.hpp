#pragma once

#include "noad/item_kind.hpp"
#include "noad/math_char.hpp"
#include "parser/unicode_index.hpp"

#include <string>
#include <string_view>

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] math_char create_math_char(const font_choice font, const std::string& symbol_string, parser_state& state);
    [[nodiscard]] math_char create_math_char(parser_state& state);
}