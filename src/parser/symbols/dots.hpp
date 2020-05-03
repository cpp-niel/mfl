#pragma once

#include "mfl/code_point.hpp"

#include <array>
#include <utility>

namespace mfl::parser
{
    constexpr auto dots = std::array<std::pair<const char*, code_point>, 6>{{
        {"dots", 0x2026},
        {"ldots", 0x2026},
        {"vdots", 0x22ee},
        {"adots", 0x22f0},
        {"ddots", 0x22f1},
        {"cdots", 0x22ef},
    }};
}
