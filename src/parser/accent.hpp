#pragma once

#include "noad/accent.hpp"

#include <string>

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] bool is_accent(const std::string& name);

    [[nodiscard]] accent create_accent(parser_state& state);
}