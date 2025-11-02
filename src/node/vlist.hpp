#pragma once

#include "node/node.hpp"

#include <utility>
#include <vector>

namespace mfl
{
    struct vlist
    {
        std::vector<node_variant> nodes;
    };

    [[nodiscard]] dist_t vlist_size(const vlist& l);

    template <node... Node>
    [[nodiscard]] vlist make_vlist(Node&&... nodes)
    {
        vlist result;
        (result.nodes.emplace_back(std::forward<decltype(nodes)>(nodes)), ...);
        return result;
    }
}