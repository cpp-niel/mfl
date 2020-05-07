#pragma once

#include "mfl/units.hpp"

#include <cstdint>

namespace mfl
{
    using dist_t = std::int64_t;

    constexpr auto unit_distance = dist_t(65536);

    [[nodiscard]] constexpr dist_t points_to_dist(const points p)
    {
        return static_cast<dist_t>(p.value() * static_cast<double>(unit_distance));
    }
}