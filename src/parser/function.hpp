#pragma once

#include "noad/mlist_with_kind.hpp"

#include <string>

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] mlist_with_kind create_operatorname(parser_state& state);
    [[nodiscard]] bool is_function(const std::string& name);
    [[nodiscard]] mlist_with_kind create_function(parser_state& state);
}