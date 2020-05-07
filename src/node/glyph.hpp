#pragma once

#include "dist.hpp"
#include "mfl/font_family.hpp"
#include "mfl/units.hpp"

namespace mfl
{
    struct glyph
    {
        font_family family = font_family::roman;
        size_t index = 0;
        points size{0};
        dist_t width = 0;
        dist_t height = 0;
        dist_t depth = 0;
    };
}