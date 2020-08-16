#include "node/hlist.hpp"

#include <doctest/doctest.h>
#include "node/box.hpp"

namespace mfl
{
    TEST_SUITE("hlist")
    {
        TEST_CASE("width is zero when hlist is empty")
        {
            hlist l;
            CHECK(hlist_width(l) == 0);
        }

        TEST_CASE("width is the sum of all node widths")
        {
            const auto l = make_hlist(glyph{.width = 20}, kern{.size = 10}, glue_spec{.size = 12});
            CHECK(hlist_width(l) == 42);
        }

        TEST_CASE("depth is zero when hlist is empty")
        {
            hlist l;
            CHECK(hlist_depth(l) == 0);
        }

        TEST_CASE("depth is the maximum of all node depths")
        {
            const auto l = make_hlist(glyph{.depth = 20}, make_hbox({.depth = 42}, {}), glue_spec{.size = 12});
            CHECK(hlist_depth(l) == 42);
        }

        TEST_CASE("height is zero when hlist is empty")
        {
            hlist l;
            CHECK(hlist_height(l) == 0);
        }

        TEST_CASE("height is the maximum of all node heights")
        {
            const auto l = make_hlist(glyph{.height = 20}, make_hbox({.height = 42}, {}), glue_spec{.size = 12});
            CHECK(hlist_height(l) == 42);
        }

        TEST_CASE("extend by positive width returns hlist with two nodes")
        {
            const auto extended = extend(glyph{.width = 20}, 20);
            CHECK(extended.nodes.size() == 2);
            CHECK(hlist_width(extended) == 40);
        }

        TEST_CASE("extend by zero width returns hlist with single node")
        {
            const auto extended = extend(glyph{.width = 20}, 0);
            CHECK(extended.nodes.size() == 1);
            CHECK(hlist_width(extended) == 20);
        }

        TEST_CASE("is empty when two empty lists are concatenated")
        {
            const auto result = concat({}, {});
            CHECK(result.nodes.empty());
        }

        TEST_CASE("is unchanged when concatenated with empty hlist")
        {
            const auto result = concat(make_hlist(glyph{.height = 20}, glue_spec{.size = 12}), {});
            CHECK(result.nodes.size() == 2);
        }

        TEST_CASE("is as long as both inputs when concatenating two non-empty hlists")
        {
            const auto result =
                concat(make_hlist(glyph{.height = 20}, glue_spec{.size = 12}), make_hlist(kern{.size = 20}));
            CHECK(result.nodes.size() == 3);
        }
    }
}