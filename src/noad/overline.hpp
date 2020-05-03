#pragma once

#include "noad/noad.hpp"

#include <vector>

namespace mfl
{
    struct overline
    {
        std::vector<noad> noads;
    };

    struct settings;
    struct hlist;

    hlist overline_to_hlist(const settings s, const bool is_cramped, const overline& ol);
}