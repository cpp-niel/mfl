#include "node/vstack.hpp"

#include <doctest/doctest.h>

namespace mfl
{
    TEST_SUITE("vstack")
    {
        TEST_CASE("width is maximum of input widths")
        {
            const auto n = vstack(glyph{.width = 10}, glyph{.width = 20}, {});

            CHECK(width(n) == 20);
        }

        TEST_CASE("vsize is sum of heights, depths and distance between nodes")
        {
            const auto n = vstack(glyph{.height = 10, .depth = 10}, glyph{.height = 20, .depth = 20},
                                  {.node0_to_baseline = 5, .node0_to_node1 = 30});

            CHECK(vsize(n) == 90);
        }

        TEST_CASE("height is the vsize of node0 plus the distance to the baseline")
        {
            const auto n =
                vstack(glyph{.height = 10, .depth = 10}, glyph{}, {.node0_to_baseline = 5, .node0_to_node1 = 5});

            CHECK(height(n) == 25);
        }

        TEST_CASE("depth is vsize of node1 plus the distance between nodes minus node0's distance to the baseline")
        {
            const auto n =
                vstack(glyph{}, glyph{.height = 20, .depth = 20}, {.node0_to_baseline = 5, .node0_to_node1 = 25});

            CHECK(depth(n) == 60);
        }
    }
}