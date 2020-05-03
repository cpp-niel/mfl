#pragma once

#include "noad/noad.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace mfl
{
    [[nodiscard]] std::pair<std::vector<noad>, std::optional<std::string>> parse(const std::string_view input);
}
