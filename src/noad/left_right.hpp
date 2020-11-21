#pragma once

#include "noad/noad.hpp"

#include <vector>

namespace mfl
{
    struct left_right
    {
        code_point left_delim_code = 0;
        std::vector<noad> noads;
        code_point right_delim_code = 0;
    };

    struct settings;
    struct hlist;

    hlist left_right_to_hlist(const settings s, const cramping cramp, const left_right& l);
}