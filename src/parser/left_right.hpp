#pragma once

#include "noad/left_right.hpp"

#include <string_view>

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] left_right create_left_right(parser_state& state);
    [[nodiscard]] code_point parse_delimiter(parser_state& state);
}