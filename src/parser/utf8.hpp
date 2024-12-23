#pragma once

#include "mfl/code_point.hpp"

#include <cstdint>
#include <optional>
#include <string_view>

namespace mfl::parser::utf8
{
    constexpr std::uint32_t accept_utf8_decoding = 0;

    std::uint32_t decode(std::uint32_t* state, std::uint32_t* codep, const char c);

    std::optional<code_point> to_ucs4(const std::string_view s);
}