#include "node/node.hpp"

#include "framework/doctest.hpp"
#include "node/box.hpp"
#include "node/hlist.hpp"

namespace mfl
{
    TEST_SUITE("node")
    {
        TEST_CASE("width for all node types")
        {
            CHECK(width(glyph{}) == 0);
            CHECK(width(kern{}) == 0);
            CHECK(width(rule{}) == 0);
            CHECK(width(glue_spec{}) == 0);
            CHECK(width(make_empty_box()) == 0);

            CHECK(width(glyph{.width = 20}) == 20);
            CHECK(width(kern{.size = 20}) == 20);
            CHECK(width(rule{.width = 20}) == 20);
            CHECK(width(glue_spec{.size = 20}) == 20);
            CHECK(width(make_hbox({.width = 20}, {})) == 20);
        }

        TEST_CASE("height for all node types")
        {
            CHECK(height(glyph{}) == 0);
            CHECK(height(kern{}) == 0);
            CHECK(height(rule{}) == 0);
            CHECK(height(glue_spec{}) == 0);
            CHECK(height(make_empty_box()) == 0);

            CHECK(height(glyph{.height = 20}) == 20);
            CHECK(height(kern{.size = 20}) == 0);
            CHECK(height(rule{.height = 20}) == 20);
            CHECK(height(glue_spec{.size = 20}) == 0);
            CHECK(height(make_hbox({.height = 20}, {})) == 20);
        }

        TEST_CASE("depth for all node types")
        {
            CHECK(depth(glyph{}) == 0);
            CHECK(depth(kern{}) == 0);
            CHECK(depth(rule{}) == 0);
            CHECK(depth(glue_spec{}) == 0);
            CHECK(depth(make_empty_box()) == 0);

            CHECK(depth(glyph{.depth = 20}) == 20);
            CHECK(depth(kern{.size = 20}) == 0);
            CHECK(depth(rule{.depth = 20}) == 20);
            CHECK(depth(glue_spec{.size = 20}) == 0);
            CHECK(depth(make_hbox({.depth = 20}, {})) == 20);
        }

        TEST_CASE("vwidth for all node types")
        {
            CHECK(vwidth(glyph{}) == 0);
            CHECK(vwidth(kern{}) == 0);
            CHECK(vwidth(rule{}) == 0);
            CHECK(vwidth(glue_spec{}) == 0);
            CHECK(vwidth(make_empty_box()) == 0);

            CHECK(vwidth(glyph{.width = 20}) == 20);
            CHECK(vwidth(kern{.size = 20}) == 0);
            CHECK(vwidth(rule{.width = 20}) == 20);
            CHECK(vwidth(glue_spec{.size = 20}) == 0);
            CHECK(vwidth(make_hbox({.width = 20}, {})) == 20);
        }

        TEST_CASE("vheight for all node types")
        {
            CHECK(vheight(glyph{}) == 0);
            CHECK(vheight(kern{}) == 0);
            CHECK(vheight(rule{}) == 0);
            CHECK(vheight(glue_spec{}) == 0);
            CHECK(vheight(make_empty_box()) == 0);

            CHECK(vheight(glyph{.height = 20}) == 20);
            CHECK(vheight(kern{.size = 20}) == 20);
            CHECK(vheight(rule{.height = 20}) == 20);
            CHECK(vheight(glue_spec{.size = 20}) == 20);
            CHECK(vheight(make_hbox({.height = 20}, {})) == 20);
        }

        TEST_CASE("vsize for all node types")
        {
            CHECK(vsize(glyph{}) == 0);
            CHECK(vsize(kern{}) == 0);
            CHECK(vsize(rule{}) == 0);
            CHECK(vsize(glue_spec{}) == 0);
            CHECK(vsize(make_empty_box()) == 0);

            CHECK(vsize(glyph{.height = 20, .depth = 20}) == 40);
            CHECK(vsize(kern{.size = 20}) == 20);
            CHECK(vsize(rule{.height = 20, .depth = 20}) == 40);
            CHECK(vsize(glue_spec{.size = 20}) == 20);
            CHECK(vsize(make_hbox({.height = 20, .depth = 20}, {})) == 40);
        }
    }
}