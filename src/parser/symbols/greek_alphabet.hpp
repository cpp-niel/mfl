#pragma once

#include "mfl/code_point.hpp"

#include <array>
#include <utility>

namespace mfl::parser
{
    constexpr auto greek_alphabet_lowercase = std::array<std::pair<const char*, code_point>, 29>{{
        {"alpha", 0x03b1},      {"beta", 0x03b2},     {"gamma", 0x03b3},  {"delta", 0x03b4},   {"epsilon", 0x03b5},
        {"varepsilon", 0x03b5}, {"zeta", 0x03b6},     {"eta", 0x03b7},    {"theta", 0x03b8},   {"vartheta", 0x03d1},
        {"iota", 0x03b9},       {"kappa", 0x03ba},    {"lambda", 0x03bb}, {"mu", 0x03bc},      {"nu", 0x03bd},
        {"xi", 0x03be},         {"pi", 0x03c0},       {"varpi", 0x03d6},  {"rho", 0x03c1},     {"varrho", 0x03f1},
        {"sigma", 0x03c3},      {"varsigma", 0x03c2}, {"tau", 0x03c4},    {"upsilon", 0x03c5}, {"phi", 0x03d5},
        {"varphi", 0x03c6},     {"chi", 0x03c7},      {"psi", 0x03c8},    {"omega", 0x03c9},
    }};

    constexpr auto greek_alphabet_uppercase = std::array<std::pair<const char*, code_point>, 10>{{
        {"Gamma", 0x0393},
        {"Delta", 0x0394},
        {"Theta", 0x0398},
        {"Lambda", 0x039b},
        {"Xi", 0x039e},
        {"Pi", 0x03a0},
        {"Sigma", 0x03a3},
        {"Phi", 0x03a6},
        {"Psi", 0x03a8},
        {"Omega", 0x03a9},
    }};
}