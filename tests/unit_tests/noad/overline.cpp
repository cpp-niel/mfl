#include "noad/overline.hpp"

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
    TEST_CASE("overline noad")
    {
        using namespace units_literals;
        const auto fonts = font_library(10_pt, create_mock_font_face);
        const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};
        const noad x_noad = math_char{.char_code = lowercase_x};

        SUBCASE("no nodes created when overline contents have no noads")
        {
            const auto result = overline_to_hlist(display_style, false, {.noads = {}});
            CHECK(result.nodes.empty());
        }

        SUBCASE("becomes box containing kern, rule, kern and box")
        {
            const auto result = overline_to_hlist(display_style, false, {.noads = {x_noad}});
            CHECK(node_types_are<box>(result.nodes));
            const box& b = std::get<wrapped_box>(result.nodes[0]);
            CHECK(node_types_are<kern, rule, kern, box>(b.nodes));
        }

        SUBCASE("width of outer box, rule and inner box are all the same")
        {
            const auto result = overline_to_hlist(display_style, false, {.noads = {x_noad}});
            const box& outer_box = std::get<wrapped_box>(result.nodes[0]);
            const rule& r = std::get<rule>(outer_box.nodes[1]);
            const box& inner_box = std::get<wrapped_box>(outer_box.nodes[3]);
            CHECK(outer_box.dims.width == r.width);
            CHECK(inner_box.dims.width == r.width);
        }
    }
}
