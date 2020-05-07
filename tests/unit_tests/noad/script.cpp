#include "noad/script.hpp"

#include "font_library.hpp"
#include "node/hlist.hpp"
#include "noad/noad.hpp"
#include "node/box.hpp"
#include "settings.hpp"

#include "framework/code_points.hpp"
#include "framework/doctest.hpp"
#include "framework/mock_font_face.hpp"
#include "framework/node_types_are.hpp"

namespace mfl
{
    TEST_CASE("script noad")
    {
        const auto fonts = font_library(create_mock_font_face);
        const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};
        const auto text_style = settings{.style = formula_style::text, .fonts = &fonts};
        const auto script_style = settings{.style = formula_style::script, .fonts = &fonts};
        const auto script_script_style = settings{.style = formula_style::script_script, .fonts = &fonts};
        const noad x_noad = math_char{.char_code = lowercase_x};

        SUBCASE("nucleus with no scripts translates to just the nucleus")
        {
            const auto result = script_to_hlist(display_style, false, script{.nucleus = {x_noad}});
            CHECK(node_types_are<glyph>(result.nodes));
        }

        SUBCASE("nucleus with subscript has subscript box shifted down")
        {
            const auto result =
                script_to_hlist(display_style, false, script{.nucleus = {x_noad}, .sub = std::vector{x_noad}});
            CHECK(node_types_are<glyph, box, kern>(result.nodes));
            const box& b = std::get<wrapped_box>(result.nodes[1]);
            CHECK(b.shift == 137216);
        }

        SUBCASE("nucleus with superscript has superscript box shifted up")
        {
            const auto result =
                script_to_hlist(display_style, false, script{.nucleus = {x_noad}, .sup = std::vector{x_noad}});
            CHECK(node_types_are<glyph, box, kern>(result.nodes));
            const box& b = std::get<wrapped_box>(result.nodes[1]);
            CHECK(b.shift == -235520);
        }

        SUBCASE("dual scripts translate to vbox with scripts separated by kern")
        {
            const auto result =
                script_to_hlist(display_style, false,
                                script{.nucleus = {x_noad}, .sub = std::vector{x_noad}, .sup = std::vector{x_noad}});
            CHECK(node_types_are<glyph, box, kern>(result.nodes));
            const box& b = std::get<wrapped_box>(result.nodes[1]);
            CHECK(b.kind == box_kind::vbox);
            CHECK(node_types_are<box, kern, box>(b.nodes));
            const kern& k = std::get<kern>(b.nodes[1]);
            CHECK(k.size == 98304);
        }

        SUBCASE("text style")
        {
            const auto result =
                script_to_hlist(text_style, false, script{.nucleus = {x_noad}, .sub = std::vector{x_noad}});
            const box& b = std::get<wrapped_box>(result.nodes[1]);
            CHECK(b.shift == 137216);
        }

        SUBCASE("script style")
        {
            const auto result =
                script_to_hlist(script_style, false, script{.nucleus = {x_noad}, .sub = std::vector{x_noad}});
            const box& b = std::get<wrapped_box>(result.nodes[1]);
            CHECK(b.shift == 137216);
        }

        SUBCASE("script script style")
        {
            const auto result =
                script_to_hlist(script_script_style, false, script{.nucleus = {x_noad}, .sub = std::vector{x_noad}});
            const box& b = std::get<wrapped_box>(result.nodes[1]);
            CHECK(b.shift == 137216);
        }

        SUBCASE("nucleus with multiple noads and subscript has subscript box shifted down")
        {
            const auto result =
                script_to_hlist(display_style, false, script{.nucleus = {x_noad, x_noad}, .sub = std::vector{x_noad}});
            CHECK(node_types_are<box, box, kern>(result.nodes));
            const box& b = std::get<wrapped_box>(result.nodes[1]);
            CHECK(b.shift == 137216);
        }
    }
}
