#pragma once

#include "mfl/code_point.hpp"

#include <array>
#include <utility>

namespace mfl::parser
{
    constexpr auto binary_operators = std::array<std::pair<const char*, code_point>, 32>{{
        {"amalg", 0x2a3f},
        {"ast", 0x2217},
        {"bigcirc", 0x25cb},
        {"bullet", 0x2219},
        {"cap", 0x2229},
        {"cdot", 0x22c5},
        {"circ", 0x2218},
        {"cup", 0x222a},
        {"dagger", 0x2020},
        {"ddagger", 0x2021},
        {"diamond", 0x22c4},
        {"div", 0x00f7},
        {"mp", 0x2213},
        {"odot", 0x2299},
        {"ominus", 0x2296},
        {"oplus", 0x2295},
        {"oslash", 0x2298},
        {"otimes", 0x2297},
        {"pm", 0x00b1},
        {"setminus", 0x005c},
        {"sqcap", 0x2293},
        {"sqcup", 0x2294},
        {"star", 0x22c6},
        {"times", 0x00d7},
        {"uplus", 0x228e},
        {"vee", 0x2228},
        {"wedge", 0x2227},
        {"wr", 0x2240},
        {"triangleleft", 0x25c1},
        {"triangleright", 0x25b7},
        {"bigtriangledown", 0x25bd},
        {"bigtriangleup", 0x25b3},
    }};

    constexpr auto additional_binary_operators = std::array<std::pair<const char*, code_point>, 63>{{
        {"Cap", 0x22d2},
        {"Colon", 0x2237},
        {"Cup", 0x22d3},
        {"Vert", 0x2016},
        {"angle", 0x2220},
        {"asterisk", 0x002a},
        {"barwedge", 0x22bc},
        {"bigstar", 0x2605},
        {"blacktriangledown", 0x25be},
        {"blacktriangleleft", 0x25c2},
        {"blacktriangleright", 0x25b8},
        {"blacktriangleup", 0x25b4},
        {"boxasterisk", 0x29c6},
        {"boxbackslash", 0x29c5},
        {"boxbar", 0x25eb},
        {"boxcirc", 0x29c7},
        {"boxdot", 0x22a1},
        {"boxminus", 0x229f},
        {"boxplus", 0x229e},
        {"boxslash", 0x29c4},
        {"boxtimes", 0x22a0},
        {"boxvoid", 0x25a1},
        {"cdotp", 0x00b7},
        {"centerdot", 0x22c5},
        {"circledast", 0x229b},
        {"circledcirc", 0x229a},
        {"circleddash", 0x229d},
        {"complement", 0x2201},
        {"curlyvee", 0x22ce},
        {"curlywedge", 0x22cf},
        {"dag", 0x2020},
        {"ddag", 0x2021},
        {"divideontimes", 0x22c7},
        {"dotminus", 0x2238},
        {"dotplus", 0x2214},
        {"doublebarwedge", 0x2306},
        {"intercal", 0x22ba},
        {"invnot", 0x2310},
        {"kernelcontraction", 0x223b},
        {"leftthreetimes", 0x22cb},
        {"lhd", 0x22b2},
        {"ltimes", 0x22c9},
        {"measuredangle", 0x2221},
        {"minus", 0x2212},
        {"neg", 0x00ac},
        {"not", 0x0338},
        {"obar", 0x233d},
        {"plus", 0x002b},
        {"rhd", 0x22b3},
        {"rightthreetimes", 0x22cc},
        {"rtimes", 0x22ca},
        {"slash", 0x2215},
        {"smallsetminus", 0x2216},
        {"smalltriangledown", 0x25bf},
        {"smalltriangleleft", 0x25c3},
        {"smalltriangleright", 0x25b9},
        {"smalltriangleup", 0x25b5},
        {"sphericalangle", 0x2222},
        {"turnednot", 0x2319},
        {"unlhd", 0x22b4},
        {"unrhd", 0x22b5},
        {"veebar", 0x22bb},
        {"veedoublebar", 0x2a63},
    }};
}