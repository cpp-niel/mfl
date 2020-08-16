#include "node/glue.hpp"

#include <doctest/doctest.h>

namespace mfl
{
    TEST_SUITE("glue")
    {
        TEST_CASE("with non-zero glue values")
        {
            std::vector<node_variant> nodes;
            nodes.emplace_back(glue_spec{.size = 100,
                                         .stretch = {.value = 42, .order = infinity_order::fill},
                                         .shrink = {.value = 42, .order = infinity_order::fill}});
            nodes.emplace_back(glue_spec{.size = 200,
                                         .stretch = {.value = 10, .order = infinity_order::filll},
                                         .shrink = {.value = 8, .order = infinity_order::fill}});

            SUBCASE("higher order filll is chosen with value of 10 and diff of 100 giving scale of 10")
            {
                const auto stretch = calculate_glue(100, nodes);
                CHECK(stretch.order == infinity_order::filll);
                CHECK(stretch.scale == 10.0);
            }

            SUBCASE("order fill is chosen which has two values, their sum is 50 and diff is -100 giving scale of 2")
            {
                const auto shrink = calculate_glue(-100, nodes);
                CHECK(shrink.order == infinity_order::fill);
                CHECK(shrink.scale == 2.0);
            }

            SUBCASE("glue param is default when width diff is zero")
            {
                const auto shrink = calculate_glue(0, nodes);
                CHECK(shrink.order == infinity_order::normal);
                CHECK(shrink.scale == 0.0);
            }
        }

        TEST_CASE("glue param is default if all glue nodes have value zero")
        {
            // nodes contains a glue node with value zero and a non-glue node
            std::vector<node_variant> nodes;
            nodes.emplace_back(glue_spec{.size = 100,
                                         .stretch = {.value = 0, .order = infinity_order::fill},
                                         .shrink = {.value = 0, .order = infinity_order::fill}});
            nodes.emplace_back(rule{});
            const auto shrink = calculate_glue(100, nodes);
            CHECK(shrink.order == infinity_order::normal);
            CHECK(shrink.scale == 0.0);
        }
    }
}
