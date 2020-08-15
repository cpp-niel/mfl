#include "noad/left_right.hpp"

#include "font_library.hpp"
#include "noad/noad.hpp"
#include "node/box.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

#include "framework/code_points.hpp"
#include <doctest/doctest.h>
#include "framework/mock_font_face.hpp"
#include "framework/node_types_are.hpp"

namespace mfl
{
    TEST_CASE("left_right noad")
    {
        using namespace units_literals;
        const auto fonts = font_library(10_pt, create_mock_font_face);
        const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};
        const noad x_noad = math_char{.char_code = lowercase_x};

        SUBCASE("becomes a content box with a delimiter glyph on either side")
        {
            const auto result = left_right_to_hlist(
                display_style, false, left_right{.left_delim_code = 10, .noads = {x_noad}, .right_delim_code = 20});
                CHECK(node_types_are<glyph, box, glyph>(result.nodes));
        }

        SUBCASE("becomes a content box with a delimiter glyph on right side when left delimiter is zero")
        {
            const auto result = left_right_to_hlist(
                display_style, false, left_right{.left_delim_code = 0, .noads = {x_noad}, .right_delim_code = 20});
                CHECK(node_types_are<box, glyph>(result.nodes));
        }

        SUBCASE("becomes a content box with a delimiter glyph on left side when right delimiter is zero")
        {
            const auto result = left_right_to_hlist(
                display_style, false, left_right{.left_delim_code = 10, .noads = {x_noad}, .right_delim_code = 0});
            CHECK(node_types_are<glyph, box>(result.nodes));
        }
    }
}
