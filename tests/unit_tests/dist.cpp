#include "dist.hpp"

#include <doctest/doctest.h>

namespace mfl
{
    using namespace units_literals;

    TEST_SUITE("dist_t")
    {
        TEST_CASE("convert points to dist")
        {
            CHECK(points_to_dist({}) == 0);
            CHECK(points_to_dist(0_pt) == 0);
            CHECK(points_to_dist(42_pt) == 42 * unit_distance);
            CHECK(points_to_dist(-42_pt) == -42 * unit_distance);
        }
    }
}