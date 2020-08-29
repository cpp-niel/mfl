#include "node/vlist.hpp"

#include "framework/doctest.hpp"
#include "node/box.hpp"
#include "node/hlist.hpp"

namespace mfl
{
    TEST_SUITE("vlist")
    {
        TEST_CASE("size is the sum of all node heights and depths")
        {
            const auto l =
                make_vlist(glyph{.height = 20, .depth = 22}, rule{.height = 2, .depth = 3},
                           make_hbox({.height = 11, .depth = 12}, {}), glue_spec{.size = 10}, kern{.size = 10});
            CHECK(vlist_size(l) == 90);
        }
    }
}