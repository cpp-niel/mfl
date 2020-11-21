#pragma once

#include "noad/noad.hpp"

#include <optional>
#include <vector>

namespace mfl
{
    struct radical
    {
        std::optional<std::vector<noad>> degree;
        std::vector<noad> radicand;
    };

    struct settings;
    struct hlist;

    hlist radical_to_hlist(const settings s, const cramping cramp, const radical& r);
}