#include "noad/noad.hpp"

#include "font_library.hpp"
#include "node/hlist.hpp"
#include "noad/accent.hpp"
#include "noad/big_op.hpp"
#include "noad/fraction.hpp"
#include "noad/left_right.hpp"
#include "noad/mlist.hpp"
#include "noad/mlist_with_kind.hpp"
#include "noad/overline.hpp"
#include "noad/radical.hpp"
#include "noad/script.hpp"
#include "noad/underline.hpp"
#include "noad/vcenter.hpp"
#include "node/glyph.hpp"
#include "node/kern.hpp"
#include "settings.hpp"

#include "framework/doctest.hpp"
#include "framework/mock_font_face.hpp"
#include "framework/node_types_are.hpp"

namespace mfl
{
    TEST_SUITE("noad")
    {
        TEST_CASE("kinds are as given in the spec")
        {
            CHECK(kind(math_char{.kind = item_kind::op}) == item_kind::op);
            CHECK(kind(math_char{.kind = item_kind::ord}) == item_kind::ord);
            CHECK(kind(fraction()) == item_kind::inner);
            CHECK(kind(left_right()) == item_kind::inner);
            CHECK(kind(script()) == item_kind::ord);
            CHECK(kind(script{.nucleus = {math_char{.kind = item_kind::op}}}) == item_kind::op);
            CHECK(kind(big_op()) == item_kind::op);
            CHECK(kind(radical()) == item_kind::ord);
            CHECK(kind(accent()) == item_kind::ord);
            CHECK(kind(underline()) == item_kind::ord);
            CHECK(kind(overline()) == item_kind::ord);
            CHECK(kind(vcenter()) == item_kind::ord);
            CHECK(kind(mlist()) == item_kind::ord);
            CHECK(kind(mlist_with_kind{.kind = item_kind::op}) == item_kind::op);
            CHECK(kind(mlist_with_kind{.kind = item_kind::ord}) == item_kind::ord);
        }

        TEST_CASE("to_hlist")
        {
            const auto fonts = font_library(create_mock_font_face);
            const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};

            SUBCASE("in display style")
            {
                SUBCASE("returns empty list when input noads are empty")
                {
                    const auto result = to_hlist(display_style, false, false, {});
                    CHECK(result.nodes.empty());
                }

                SUBCASE("inserts glue on each side of an operator")
                {
                    const auto c = math_char{.kind = item_kind::ord};
                    const auto op = math_char{.kind = item_kind::op};
                    const auto result = to_hlist(display_style, false, false, {c, op, c});
                    CHECK((node_types_are<glyph, glue_spec, glyph, glue_spec, glyph>(result.nodes)));
                }

                SUBCASE("inserts glue on each side of a binary operator")
                {
                    const auto c = math_char{.kind = item_kind::ord};
                    const auto op = math_char{.kind = item_kind::bin};
                    const auto result = to_hlist(display_style, false, false, {c, op, c});
                    CHECK((node_types_are<glyph, glue_spec, glyph, glue_spec, glyph>(result.nodes)));
                }

                SUBCASE("only inserts glue on each side of the first binary operator if two are adjacent")
                {
                    const auto c = math_char{.kind = item_kind::ord};
                    const auto op = math_char{.kind = item_kind::bin};
                    const auto result = to_hlist(display_style, false, false, {c, op, op, c});
                    CHECK((node_types_are<glyph, glue_spec, glyph, glue_spec, glyph, glyph>(result.nodes)));
                }

                SUBCASE("inserts kern if input contains explicit space")
                {
                    const auto c = math_char{.kind = item_kind::ord};
                    const auto space = math_space{.space = kern{.size = 100}};
                    const auto result = to_hlist(display_style, false, false, {c, space, c});
                    CHECK((node_types_are<glyph, kern, glyph>(result.nodes)));
                }

                SUBCASE("inserts kern if input contains explicit space in non math units")
                {
                    const auto c = math_char{.kind = item_kind::ord};
                    const auto space = math_space{.space = kern{.size = 100}, .is_math_units = false};
                    const auto result = to_hlist(display_style, false, false, {c, space, c});
                    CHECK((node_types_are<glyph, kern, glyph>(result.nodes)));
                }
            }

            SUBCASE(
                "[noad] to_hlist does not insert kern if explicit space is non script only and used in script style")
            {
                const auto script_style = settings{.style = formula_style::script, .fonts = &fonts};
                const auto c = math_char{.kind = item_kind::ord};
                const auto space = math_space{.space = kern{.size = 100}, .is_non_script_only = true};
                const auto result = to_hlist(script_style, false, false, {c, space, c});
                CHECK((node_types_are<glyph, glyph>(result.nodes)));
            }
        }
    }
}
