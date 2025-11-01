#include "node/vlist.hpp"

#include <numeric>

namespace mfl
{
    dist_t vlist_size(const vlist& l)
    {
        return std::accumulate(l.nodes.begin(), l.nodes.end(), dist_t(0),
                               [&](const dist_t acc, const node_variant& n) { return acc + vsize(n); });
    }
}