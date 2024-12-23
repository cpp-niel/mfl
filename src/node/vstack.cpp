#include "node/vstack.hpp"

#include "node/box.hpp"

#include <vector>

namespace mfl
{
    node_variant vstack(node_variant&& node0, node_variant&& node1, const vstack_distances distances)
    {
        const auto dims = box_dims{.width = std::max(width(node0), width(node1)),
            .height = vsize(node0) + distances.node0_to_baseline,
            .depth = vsize(node1) + (distances.node0_to_node1 - distances.node0_to_baseline)};

        std::vector<node_variant> nodes;
        nodes.emplace_back(std::move(node0));
        nodes.emplace_back(kern{.size = distances.node0_to_node1});
        nodes.emplace_back(std::move(node1));

        return box{.kind = box_kind::vbox, .dims = dims, .nodes = std::move(nodes)};
    }
}