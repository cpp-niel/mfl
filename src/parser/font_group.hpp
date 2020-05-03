#pragma once

#include "noad/noad.hpp"

#include <string>
#include <vector>

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] bool is_font_choice(const std::string& name);
    [[nodiscard]] std::vector<noad> create_font_group(parser_state& state);
}