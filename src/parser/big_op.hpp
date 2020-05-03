#pragma once

#include "noad/big_op.hpp"

#include <string>

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] bool is_big_op(const std::string& name);

    [[nodiscard]] big_op create_big_op(parser_state& state);
}