#pragma once

#include "node/node.hpp"

#include <utility>
#include <vector>

namespace mfl
{
    struct hlist
    {
        std::vector<node_variant> nodes;
    };

    [[nodiscard]] dist_t hlist_width(const hlist& l);
    [[nodiscard]] dist_t hlist_depth(const hlist& l);
    [[nodiscard]] dist_t hlist_height(const hlist& l);

    [[nodiscard]] hlist extend(node_variant&& n, const dist_t d);
    [[nodiscard]] hlist concat(hlist&& l0, hlist&& l1);

    [[nodiscard]] hlist make_hlist(node auto&&... nodes)
    {
        hlist result;
        (result.nodes.emplace_back(std::forward<decltype(nodes)>(nodes)), ...);
        return result;
    }
}