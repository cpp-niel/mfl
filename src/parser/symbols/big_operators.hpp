#pragma once

#include "mfl/code_point.hpp"

#include <array>
#include <utility>

namespace mfl::parser
{
    constexpr auto big_op_symbols = std::array<std::pair<const char*, code_point>, 15>{{
        {"sum", 0x2211},
        {"int", 0x222b},
        {"oint", 0x222e},
        {"prod", 0x220f},
        {"coprod", 0x2210},
        {"bigcap", 0x22c2},
        {"bigcup", 0x22c3},
        {"bigsqcap", 0x2a05},
        {"bigsqcup", 0x2a06},
        {"bigwedge", 0x22c0},
        {"bigvee", 0x22c1},
        {"bigodot", 0x2a00},
        {"bigotimes", 0x2a02},
        {"bigoplus", 0x2a01},
        {"biguplus", 0x2a04},
    }};

    constexpr auto integral_symbols = std::array<std::pair<const char*, code_point>, 6>{{
        {"int", 0x222b},
        {"oint", 0x222e},
        {"iint", 0x222c},
        {"oiint", 0x222f},
        {"iiint", 0x222d},
        {"oiiint", 0x2230},
    }};
}
