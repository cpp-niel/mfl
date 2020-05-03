#pragma once

#include "dist.hpp"

namespace mfl
{
    struct rule
    {
        dist_t width = 0;
        dist_t height = 0;
        dist_t depth = 0;
    };
}