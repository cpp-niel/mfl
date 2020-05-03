#pragma once

#include "noad/math_space.hpp"

#include <string>

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] bool is_explicit_space(const std::string& name);
    [[nodiscard]] math_space create_explicit_space(parser_state& state);
    [[nodiscard]] math_space create_hspace(parser_state& state);
}
