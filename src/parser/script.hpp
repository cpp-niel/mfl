#pragma once

#include "noad/noad.hpp"

#include <vector>

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] std::vector<noad> create_script(parser_state& state);
}
