#include "noad/math_char.hpp"

#include "font_library.hpp"
#include "framework/code_points.hpp"
#include "framework/doctest.hpp"
#include "framework/mock_font_face.hpp"
#include "framework/node_types_are.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

namespace mfl
{
    TEST_CASE("math_char noad")
    {
        const auto fonts = font_library(create_mock_font_face);
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
            CHECK(result.first.index == lowercase_x);
        }

        SUBCASE("[math_char] find correct height variant of glyph")
        {
            const auto char_index_for_height = [&](const dist_t h)
            {
                return make_auto_height_glyph(display_style, font_family::roman, 1, h).first.index;
            };

            CHECK(char_index_for_height(10) == 3);
            CHECK(char_index_for_height(12) == 3);
            CHECK(char_index_for_height(20) == 3);
            CHECK(char_index_for_height(25) == 4);
            CHECK(char_index_for_height(40) == 4);
        }

        SUBCASE("[math_char] find correct width variant of glyph")
        {
            const auto char_index_for_width = [&](const dist_t w)
            {
                return make_auto_width_glyph(display_style, font_family::roman, 1, w).first.index;
            };

            CHECK(char_index_for_width(10) == 3);
            CHECK(char_index_for_width(12) == 3);
            CHECK(char_index_for_width(20) == 3);
            CHECK(char_index_for_width(25) == 4);
            CHECK(char_index_for_width(40) == 4);
        }
    }
}
