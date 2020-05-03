#pragma once

#include <string_view>

namespace mfl
{
    void approve(const std::string_view text, const bool auto_approve = false);
}