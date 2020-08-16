#include "noad/big_op.hpp"

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
    TEST_CASE("big_op noad")
    {
        using namespace units_literals;
        const auto fonts = font_library(10_pt, create_mock_font_face);
        const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};
        const noad x_noad = math_char{.char_code = lowercase_x};
        const noad prod_noad = math_char{.char_code = product_big_op};

        SUBCASE("with no scripts translates to box with glue inside vbox")
        {
            const auto result = big_op_to_hlist(display_style, false, big_op{.nucleus = {prod_noad}});
            CHECK(node_types_are<box>(result.nodes));
            const box& vb = std::get<wrapped_box>(result.nodes[0]);
            CHECK(vb.kind == box_kind::vbox);
            CHECK(node_types_are<box>(vb.nodes));
            const box& hb = std::get<wrapped_box>(vb.nodes[0]);
            CHECK(hb.kind == box_kind::hbox);
            CHECK(node_types_are<glue_spec, box, glue_spec>(hb.nodes));
        }

        SUBCASE("with subscript in limit mode translates to vbox with op, gap, limit, and spacing")
        {
            const auto result =
                big_op_to_hlist(display_style, false, big_op{.nucleus = {prod_noad}, .sub = std::vector{x_noad}});
            const box& vb = std::get<wrapped_box>(result.nodes[0]);
            CHECK(node_types_are<box, kern, box, kern>(vb.nodes));  // big op, gap, sub limit, spacing
        }

        SUBCASE("with superscript in limit mode translates to vbox with spacing, limit, gap, and big op")
        {
            const auto result =
                big_op_to_hlist(display_style, false, big_op{.nucleus = {prod_noad}, .sup = std::vector{x_noad}});
            const box& vb = std::get<wrapped_box>(result.nodes[0]);
            CHECK(node_types_are<kern, box, kern, box>(vb.nodes));  // spacing, sup limit, gap, big op
        }

        SUBCASE("with dual scripts in limit mode translates to vbox with both limits")
        {
            const auto result =
                big_op_to_hlist(display_style, false,
                                big_op{.nucleus = {prod_noad}, .sub = std::vector{x_noad}, .sup = std::vector{x_noad}});
            const box& vb = std::get<wrapped_box>(result.nodes[0]);
            CHECK(node_types_are<kern, box, kern, box, kern, box, kern>(
                vb.nodes));  // spacing, sup limit, gap, big op, gap, sub limit, spacing
        }

        SUBCASE("with subscript in non-limit mode translates to nucleus, down-shifted script, and kern")
        {
            const auto result = big_op_to_hlist(
                display_style, false,
                big_op{.limits = big_op_limits::no, .nucleus = {prod_noad}, .sub = std::vector{x_noad}});
            CHECK(node_types_are<box, box, kern>(result.nodes));
            const box& sub_box = std::get<wrapped_box>(result.nodes[1]);
            CHECK(sub_box.shift == 137216);
        }

        SUBCASE("with superscript in non-limit mode translates to nucleus, up-shifted script, and kern")
        {
            const auto result = big_op_to_hlist(
                display_style, false,
                big_op{.limits = big_op_limits::no, .nucleus = {prod_noad}, .sup = std::vector{x_noad}});
            CHECK(node_types_are<box, box, kern>(result.nodes));
            const box& sup_box = std::get<wrapped_box>(result.nodes[1]);
            CHECK(sup_box.shift == -235520);
        }

        SUBCASE("with dual scripts in non-limit mode translates to nucleus, vbox with scripts, and kern")
        {
            const auto result = big_op_to_hlist(display_style, false,
                                                big_op{.limits = big_op_limits::no,
                                                       .nucleus = {prod_noad},
                                                       .sub = std::vector{x_noad},
                                                       .sup = std::vector{x_noad}});
            CHECK(node_types_are<box, box, kern>(result.nodes));
            const box& b = std::get<wrapped_box>(result.nodes[1]);
            CHECK(b.kind == box_kind::vbox);
            CHECK(node_types_are<box, kern, box>(b.nodes));
            const kern& k = std::get<kern>(b.nodes[1]);
            CHECK(k.size == 98304);
        }
    }
}
