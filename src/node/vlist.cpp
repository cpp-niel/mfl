#include "node/vlist.hpp"

#include "node/node.hpp"

#include <range/v3/algorithm/max.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/view/transform.hpp>

namespace mfl
{
    namespace rv = ranges::views;

    dist_t vlist_size(const vlist& l)
    {
        return ranges::accumulate(l.nodes, dist_t(0), [&](const dist_t acc, const node_variant& n) { return acc + vsize(n); });
    }
}