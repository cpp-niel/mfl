#include "node/vlist.hpp"

#include <algorithm>
#include <ranges>

namespace mfl
{
    dist_t vlist_size(const vlist& l)
    {
        return std::ranges::fold_left_first(
                   l.nodes | std::views::transform([](const node_variant& n) { return vsize(n); }), std::plus{})
            .value_or(dist_t{});
    }
}