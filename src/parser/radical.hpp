#pragma once

#include "noad/radical.hpp"

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] radical create_radical(parser_state& state);
}
