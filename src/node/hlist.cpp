#include "node/hlist.hpp"

#include <algorithm>
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
        std::ranges::move(l1.nodes, std::back_inserter(result.nodes));
        return result;
    }

    dist_t hlist_width(const hlist& l)
    {
        return std::ranges::fold_left_first(
                   l.nodes | std::views::transform([](const node_variant& n) { return width(n); }), std::plus{})
            .value_or({});
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