#pragma once

#include "mfl/code_point.hpp"

#include <array>
#include <utility>

namespace mfl::parser
{
    constexpr auto accents = std::array<std::pair<const char*, code_point>, 12>{{
        {"acute", 0x0301},
        {"bar", 0x0304},
        {"breve", 0x0306},
        {"check", 0x030c},
        {"ddot", 0x0308},
        {"dot", 0x0307},
        {"grave", 0x0300},
        {"hat", 0x0302},
        {"tilde", 0x0303},
        {"vec", 0x20d7},
        {"widehat", 0x0302},
        {"widetilde", 0x0303},
    }};

    constexpr auto additional_accents = std::array<std::pair<const char*, code_point>, 25>{{
        {"rightharpoonaccent", 0x20d1},
        {"leftharpoonaccent", 0x20d0},
        {"overarc", 0x0311},
        {"candra", 0x0310},
        {"ocirc", 0x030a},
        {"ovhook", 0x0309},
        {"oturnedcomma", 0x0312},
        {"ocommatopright", 0x0315},
        {"droang", 0x031a},
        {"widebridgeabove", 0x20e9},
        {"asteraccent", 0x20f0},
        {"mathring", 0x030a},
        {"circumflexaccent", 0x0302},
        {"combiningbreve", 0x0306},
        {"combiningoverline", 0x0304},
        {"combininggraveaccent", 0x0300},
        {"combiningacuteaccent", 0x0301},
        {"combiningdiaeresis", 0x0308},
        {"combiningtilde", 0x0303},
        {"combiningrightarrowabove", 0x20d7},
        {"combiningdotabove", 0x0307},
        {"overleftarrow", 0x20d6},
        {"overleftrightarrow", 0x20e1},
        {"dddot", 0x20db},
        {"ddddot", 0x20dc},
    }};

    constexpr auto combining_symbols = std::array<std::pair<const char*, code_point>, 6>{{
        {"H", 0x030b},
        {"c", 0x0327},
        {"d", 0x0323},
        {"k", 0x0328},
        {"ring", 0x02da},
        {"t", 0x0361},
    }};
}
