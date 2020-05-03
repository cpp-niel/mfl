#pragma once

#include "node/node.hpp"

#include <vector>

namespace mfl
{
    glue_param calculate_glue(const dist_t width_diff, const std::vector<node_variant>& nodes);
}