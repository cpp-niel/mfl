#include "dist.hpp"

#include "framework/doctest.hpp"

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

        TEST_CASE("convert ratio to dist")
        {
            CHECK(ratio_to_dist(0, 1) == 0);
            CHECK(ratio_to_dist(1, 1) == unit_distance);
            CHECK(ratio_to_dist(2, 1) == unit_distance * 2);
            CHECK(ratio_to_dist(-2, 1) == -unit_distance * 2);
        }
    }
}