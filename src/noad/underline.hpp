#pragma once

#include "noad/noad.hpp"

#include <vector>

namespace mfl
{
    struct underline
    {
        std::vector<noad> noads;
    };

    struct settings;
    struct hlist;

    hlist underline_to_hlist(const settings s, const cramping cramp, const underline& ul);
}