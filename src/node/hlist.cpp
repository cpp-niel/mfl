#include "node/hlist.hpp"

#include <algorithm>
#include <numeric>
#include <ranges>

namespace mfl
{
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
        return std::accumulate(l.nodes.begin(), l.nodes.end(), dist_t(0),
                               [&](const dist_t acc, const node_variant& n) { return acc + width(n); });
    }

    dist_t hlist_depth(const hlist& l)
    {
        return l.nodes.empty()
                   ? 0
                   : std::ranges::max(l.nodes | std::views::transform([](const node_variant& n) { return depth(n); }));
    }

    dist_t hlist_height(const hlist& l)
    {
        return l.nodes.empty()
                   ? 0
                   : std::ranges::max(l.nodes | std::views::transform([](const node_variant& n) { return height(n); }));
    }
}