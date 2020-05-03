#pragma once

#include "dist.hpp"
#include "node/node.hpp"

namespace mfl
{
    struct vstack_distances
    {
        dist_t node0_to_baseline = 0;
        dist_t node0_to_node1 = 0;
    };

    [[nodiscard]] node_variant vstack(node_variant&& node0, node_variant&& node1, const vstack_distances distances);
}