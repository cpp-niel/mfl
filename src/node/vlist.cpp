#include "node/vlist.hpp"

#include "node/node.hpp"

#include <range/v3/numeric.hpp>

namespace mfl
{
    dist_t vlist_size(const vlist& l)
    {
        return ranges::accumulate(l.nodes, dist_t(0), [&](const dist_t acc, const node_variant& n) { return acc + vsize(n); });
    }
}