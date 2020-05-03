#pragma once

#include "noad/item_kind.hpp"
#include "noad/noad.hpp"

#include <vector>

namespace mfl
{
    struct mlist_with_kind
    {
        item_kind kind;
        std::vector<noad> noads;
    };
}
