#include "noad/radical.hpp"

#include "font_library.hpp"
#include "noad/noad.hpp"
#include "node/box.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

#include "framework/code_points.hpp"
#include "framework/doctest.hpp"
#include "framework/mock_font_face.hpp"
#include "framework/node_types_are.hpp"

namespace mfl
{
    TEST_CASE("radical noad")
    {
        using namespace units_literals;
        const auto fonts = font_library(10_pt, create_mock_font_face);
        const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};
        const noad x_noad = math_char{.char_code = lowercase_x};
        const auto degree_noads = std::vector<noad>{x_noad};

        SUBCASE("has an hbox for the radical symbol and a vbox for the overlined radicand")
        {
            const auto result = radical_to_hlist(
                display_style, cramping::off, radical{.degree = degree_noads, .radicand = {x_noad}});
            CHECK(node_types_are<box, box>(result.nodes));
        }

        SUBCASE("the radical symbol box contains the a box for the degree, a kern and the actual symbol")
        {
            const auto result = radical_to_hlist(
                display_style, cramping::off, radical{.degree = degree_noads, .radicand = {x_noad}});

            const box& symbol_box = std::get<wrapped_box>(result.nodes[0]);
            CHECK(symbol_box.kind == box_kind::hbox);
            CHECK(node_types_are<kern, box, kern, glyph>(symbol_box.nodes));
        }

        SUBCASE("kern in the degree box is negative so degree is rendered on top of radical symbol")
        {
            // the negative value is a mathematical font constant which should normally be negative and in our
            // case in this test must be negative because the corresponding property in the mock font face that
            // we are using is hard wired to a negative value
            const auto result = radical_to_hlist(
                display_style, cramping::off, radical{.degree = degree_noads, .radicand = {x_noad}});

            const box& symbol_box = std::get<wrapped_box>(result.nodes[0]);
            const auto& kern_after_degree = std::get<kern>(symbol_box.nodes[2]);
            CHECK(kern_after_degree.size < 0);
        }

        SUBCASE("the radicand box contains the overline between two kerns and the content box")
        {
            const auto result = radical_to_hlist(
                display_style, cramping::off, radical{.degree = degree_noads, .radicand = {x_noad}});

            const box& radicand_box = std::get<wrapped_box>(result.nodes[1]);
            CHECK(radicand_box.kind == box_kind::vbox);
            CHECK(node_types_are<kern, rule, kern, box>(radicand_box.nodes));
        }
    }
}
