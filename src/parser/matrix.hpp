#pragma once

#include "noad/matrix.hpp"

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] matrix create_matrix(parser_state& state);
}
