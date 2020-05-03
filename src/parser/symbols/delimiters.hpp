#pragma once

#include "mfl/code_point.hpp"

#include <array>
#include <utility>

namespace mfl::parser
{
    constexpr auto left_delimiters = std::array<std::pair<const char*, code_point>, 32>{{
        {"langle", 0x27e8},        {"lbrace", 0x007b},    {"lbrack", 0x005b},     {"lceil", 0x2308},
        {"leftangle", 0x27e8},     {"leftbrace", 0x007b}, {"leftparen", 0x0028},  {"lfloor", 0x230a},
        {"llcorner", 0x231e},      {"ulcorner", 0x231c},  {"lmoustache", 0x23b0}, {"lbrbrak", 0x2772},
        {"lbag", 0x027c5},         {"lBrack", 0x27e6},    {"langle", 0x27e8},     {"lAngle", 0x27ea},
        {"Lbrbrak", 0x27ec},       {"lgroup", 0x27ee},    {"lBrace", 0x2983},     {"lParen", 0x2985},
        {"llparenthesis", 0x2987}, {"llangle", 0x2989},   {"lbrackubar", 0x298b}, {"lbrackultick", 0x298d},
        {"lbracklltick", 0x298f},  {"langledot", 0x2991}, {"lparenless", 0x2993}, {"Lparengtr", 0x2995},
        {"lblkbrbrak", 0x2997},    {"lvzigzag", 0x29d8},  {"Lvzigzag", 0x29da},   {"lcurvyangle", 0x29fc},
    }};

    constexpr auto right_delimiters = std::array<std::pair<const char*, code_point>, 32>{{
        {"rangle", 0x27e9},        {"rbrace", 0x007d},     {"rbrack", 0x005d},     {"rceil", 0x2309},
        {"rightangle", 0x27e9},    {"rightbrace", 0x007d}, {"rightparen", 0x0029}, {"rfloor", 0x230b},
        {"lrcorner", 0x231f},      {"urcorner", 0x231d},   {"rmoustache", 0x23b1}, {"rbrbrak", 0x2773},
        {"rbag", 0x27c6},          {"rBrack", 0x27e7},     {"rangle", 0x27e9},     {"rAngle", 0x27eb},
        {"Rbrbrak", 0x27ed},       {"rgroup", 0x27ef},     {"rBrace", 0x2984},     {"rParen", 0x2986},
        {"rrparenthesis", 0x2988}, {"rrangle", 0x298a},    {"rbrackubar", 0x298c}, {"rbrackultick", 0x298e},
        {"rbracklltick", 0x2990},  {"rangledot", 0x2992},  {"rparenless", 0x2994}, {"Rparengtr", 0x2996},
        {"rblkbrbrak", 0x2998},    {"rvzigzag", 0x29d9},   {"Rvzigzag", 0x29db},   {"rcurvyangle", 0x29fd},
    }};
}
