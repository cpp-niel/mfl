#pragma once

#include <string>

using FT_Error = int;

namespace mfl::fft
{
    [[nodiscard]] std::string ft_error_string(const FT_Error error_code);
}