#pragma once

#include "noad/noad.hpp"

#include <vector>

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] std::vector<noad> create_command(parser_state& state);
}