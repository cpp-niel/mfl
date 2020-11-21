#pragma once

#include "noad/noad.hpp"

#include <optional>
#include <vector>

namespace mfl
{
    struct script
    {
        std::vector<noad> nucleus;
        std::optional<std::vector<noad>> sub;
        std::optional<std::vector<noad>> sup;
    };

    struct settings;
    struct hlist;

    hlist script_to_hlist(const settings s, const cramping cramp, const script& sc);
}