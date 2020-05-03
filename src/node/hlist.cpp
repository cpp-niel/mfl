#include "node/hlist.hpp"

#include "node/node.hpp"

#include <range/v3/algorithm/max.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/view/transform.hpp>

namespace mfl
{
    namespace rv = ranges::views;

    hlist extend(node_variant&& n, const dist_t d)
    {
        auto result = make_hlist(std::move(n));
        if (d != 0) result.nodes.emplace_back(kern{.size = d});
        return result;
    }

    hlist concat(hlist&& l0, hlist&& l1)
    {
        auto result = std::move(l0);
        std::move(l1.nodes.begin(), l1.nodes.end(), std::back_inserter(result.nodes));
        return result;
    }

    dist_t hlist_width(const hlist& l)
    {
        return ranges::accumulate(l.nodes, dist_t(0), [&](const dist_t acc, const node_variant& n) { return acc + width(n); });
    }

    dist_t hlist_depth(const hlist& l)
    {
        return l.nodes.empty() ? 0 : ranges::max(l.nodes | rv::transform([](const node_variant& n) { return depth(n); }));
    }

    dist_t hlist_height(const hlist& l)
    {
        return l.nodes.empty() ? 0 : ranges::max(l.nodes | rv::transform([](const node_variant& n) { return height(n); }));
    }
}