#include "noad/math_char.hpp"

#include "font_library.hpp"
#include "framework/code_points.hpp"
#include "framework/doctest.hpp"
#include "framework/mock_font_face.hpp"
#include "framework/node_types_are.hpp"
#include "node/box.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

namespace mfl
{
    TEST_CASE("math_char noad")
    {
        using namespace units_literals;
        const auto fonts = font_library(10_pt, create_mock_font_face);
        const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};

        SUBCASE("[math_char] math char translates to glyph")
        {
            const auto result = math_char_to_hlist(display_style, math_char{.char_code = lowercase_x});
            CHECK(node_types_are<glyph>(result.nodes));
        }

        SUBCASE("[math_char] translation finds the correct glyph index")
        {
            const auto result = math_char_to_hlist(display_style, math_char{.char_code = lowercase_x});
            CHECK(std::get<glyph>(result.nodes[0]).index == 120);
        }

        SUBCASE("[math_char] use normal char when no variants exist")
        {
            const auto result = make_auto_height_glyph(display_style, font_family::roman, lowercase_x, 100);
            CHECK(std::get<glyph>(result.first).index == lowercase_x);
        }

        SUBCASE("[math_char] find correct height variant of glyph")
        {
            const auto char_index_for_height = [&](const dist_t h) {
                return std::get<glyph>(make_auto_height_glyph(display_style, font_family::roman, 1, h).first).index;
            };

            CHECK(char_index_for_height(8) == 3);
            CHECK(char_index_for_height(10) == 3);
            CHECK(char_index_for_height(12) == 3);
            CHECK(char_index_for_height(20) == 4);
            CHECK(char_index_for_height(24) == 4);
        }

        SUBCASE("[math_char] find correct width variant of glyph")
        {
            const auto char_index_for_width = [&](const dist_t w) {
                return make_auto_width_glyph(display_style, font_family::roman, 1, w).first.index;
            };

            CHECK(char_index_for_width(8) == 3);
            CHECK(char_index_for_width(10) == 3);
            CHECK(char_index_for_width(12) == 3);
            CHECK(char_index_for_width(20) == 4);
            CHECK(char_index_for_width(24) == 4);
        }

        SUBCASE("[math_char] assemble oversized vertical glyph")
        {
            const box assembly =
                std::get<wrapped_box>(make_auto_height_glyph(display_style, font_family::roman, 41, 1'000'000).first);

            const auto glyph_index_for_node = [&](const int node_index) {
                const box glyph_box = std::get<wrapped_box>(assembly.nodes[static_cast<size_t>(node_index)]);
                return std::get<glyph>(glyph_box.nodes.front()).index;
            };

            // top glyph has index 42, middle glyph 43, extender glyph 44, and bottom glyph 45
            // We want to cover a requested height of 1'000'000 where each glyph has a height of
            // 100'000. So we should end up with 11 nodes: 1 top, 5 extenders, 1 middle, 5 extenders,
            // 1 bottom.
            CHECK(assembly.nodes.size() == 11);
            CHECK(glyph_index_for_node(0) == 42);   // top
            CHECK(glyph_index_for_node(5) == 43);   // middle
            CHECK(glyph_index_for_node(10) == 45);  // bottom
            for (const auto i : {1, 2, 3, 4, 6, 7, 8, 9})
                CHECK(glyph_index_for_node(i) == 44);  // extenders
        }
    }
}
