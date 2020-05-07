#include "noad/accent.hpp"

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
    TEST_CASE("accent noad")
    {
        const noad x_noad = math_char{.char_code = lowercase_x};
        const auto fonts = font_library(create_mock_font_face);
        const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};

        SUBCASE("accent box over empty content box and kern if there are no noads under the accent")
        {
            const auto result = accent_to_hlist(display_style, false, {.noads = {}});
            CHECK(node_types_are<box>(result.nodes));
            const box& b = std::get<wrapped_box>(result.nodes[0]);
            CHECK(b.kind == box_kind::vbox);
            CHECK(node_types_are<box, kern, box>(b.nodes));
            const box& accent_box = std::get<wrapped_box>(b.nodes[0]);
            CHECK(accent_box.kind == box_kind::hbox);
            CHECK(node_types_are<glyph>(accent_box.nodes));
        }

        SUBCASE("positive shift when there are no noads under the accent")
        {
            const auto result = accent_to_hlist(display_style, false, {.noads = {}});
            const box& b = std::get<wrapped_box>(result.nodes[0]);
            const box& accent_box = std::get<wrapped_box>(b.nodes[0]);
            CHECK(accent_box.shift == 310272);
        }

        SUBCASE("no shift when there are is one noad under the accent")
        {
            // There is no shift in this case because the accent positioning is the difference of the
            // accent positions defined for the content and accent glyphs in the font face. The mock
            // font sets all these positions to the same value so the difference is zero
            const auto result = accent_to_hlist(display_style, false, {.noads = {x_noad}});
            const box& b = std::get<wrapped_box>(result.nodes[0]);
            const box& accent_box = std::get<wrapped_box>(b.nodes[0]);
            CHECK(accent_box.shift == 0);
        }

        SUBCASE("positive shift when there are multiple noads under the accent")
        {
            const auto result = accent_to_hlist(display_style, false, {.noads = {x_noad, x_noad, x_noad}});
            const box& b = std::get<wrapped_box>(result.nodes[0]);
            const box& accent_box = std::get<wrapped_box>(b.nodes[0]);
            CHECK(accent_box.shift == 316416);
        }
    }
}
