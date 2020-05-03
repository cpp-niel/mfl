#pragma once

#include "mfl/code_point.hpp"

#include <array>
#include <utility>

namespace mfl::parser
{
    constexpr auto punctuation_symbols = std::array<std::pair<const char*, code_point>, 16>{{
        {"vert", 0x007c},
        {"textexclamdown", 0x00a1},
        {"lq", 0x2018},
        {"textquotedblleft", 0x201c},
        {"textquotedblright", 0x201d},
        {"rasp", 0x02bc},
        {"lasp", 0x02bd},
        {"colon", 0x003a},
        {"textquestiondown", 0x00bf},
        {"emdash", 0x2014},
        {"endash", 0x2013},
        {"rq", 0x2019},
        {"guilsinglleft", 0x2039},
        {"guilsinglright", 0x203a},
        {"guillemotleft", 0x00ab},
        {"guillemotright", 0x00bb},
    }};
}
