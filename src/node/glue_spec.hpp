#pragma once

#include "dist.hpp"

namespace mfl
{
    enum class infinity_order
    {
        normal,
        fil,
        fill,
        filll
    };

    struct glue_scale
    {
        dist_t value = 0;
        infinity_order order = infinity_order::normal;
    };

    struct glue_param
    {
        double scale = 0.0;
        infinity_order order = infinity_order::normal;
    };

    struct glue_spec
    {
        dist_t size = 0;
        glue_scale stretch;
        glue_scale shrink;
    };
}