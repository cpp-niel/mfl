#pragma once

#include "noad/overline.hpp"
#include "noad/underline.hpp"

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] overline create_overline(parser_state& state);
    [[nodiscard]] underline create_underline(parser_state& state);
}