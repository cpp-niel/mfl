#pragma once

#include "mfl/code_point.hpp"
#include "noad/noad.hpp"

#include <vector>

namespace mfl
{
    struct accent
    {
        font_family family = font_family::roman;
        code_point char_code = 0;
        bool is_wide = false;
        std::vector<noad> noads;
    };

    struct settings;
    struct hlist;

    hlist accent_to_hlist(const settings s, const cramping cramp, const accent& a);
}