#pragma once

#include "noad/noad.hpp"

#include <optional>
#include <vector>

namespace mfl
{
    enum class big_op_limits
    {
        normal,
        yes,
        no
    };

    struct big_op
    {
        big_op_limits limits = big_op_limits::normal;
        std::vector<noad> nucleus;
        std::optional<std::vector<noad>> sub;
        std::optional<std::vector<noad>> sup;
    };

    struct settings;
    struct hlist;

    hlist big_op_to_hlist(const settings s, const bool is_cramped, const big_op& op);
}