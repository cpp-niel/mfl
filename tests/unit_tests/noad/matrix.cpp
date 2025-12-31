#include "noad/matrix.hpp"

#include "font_library.hpp"
#include "noad/noad.hpp"
#include "node/box.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

#include "framework/doctest.hpp"
#include "framework/mock_font_face.hpp"
#include "framework/node_types_are.hpp"

namespace mfl
{
    TEST_CASE("matrix noad")
    {
        using namespace units_literals;
        const noad x_noad = math_char{.char_code = 0};
        const auto fonts = font_library(10_pt, create_mock_font_face);
        const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};
        // const auto text_style = settings{.style = formula_style::text, .fonts = &fonts};
        // const auto script_script_style = settings{.style = formula_style::script_script, .fonts = &fonts};

        SUBCASE("no nodes created for matrix if no input noads")
        {
            const auto result = matrix_to_hlist(display_style, cramping::off, {});
            CHECK(result.nodes.empty());
        }

        SUBCASE("matrix with one element contains one glyph noad")
        {
            const auto result =
                matrix_to_hlist(display_style, cramping::off, {.rows = {matrix_row{std::vector{x_noad}}}});
            const box& matrix_box = std::get<wrapped_box>(result.nodes[0]);
            CHECK(node_types_are<box>(matrix_box.nodes));
            const box& row_box = std::get<wrapped_box>(matrix_box.nodes[0]);
            CHECK(node_types_are<glue_spec, box, glue_spec>(row_box.nodes));
            const box& cell_box = std::get<wrapped_box>(row_box.nodes[1]);
            CHECK(node_types_are<glyph>(cell_box.nodes));
        }

        SUBCASE("3x1 matrix contains three rows, each with one glyph noad")
        {
            const auto result =
                matrix_to_hlist(display_style, cramping::off,
                                {.rows = {matrix_row{std::vector{x_noad}}, matrix_row{std::vector{x_noad}},
                                          matrix_row{std::vector{x_noad}}}});
            const box& matrix_box = std::get<wrapped_box>(result.nodes[0]);
            CHECK(node_types_are<box, glue_spec, box, glue_spec, box>(matrix_box.nodes));
            for (size_t i = 0; i < 3; ++i)
            {
                const box& row_box = std::get<wrapped_box>(matrix_box.nodes[i * 2]);
                CHECK(node_types_are<glue_spec, box, glue_spec>(row_box.nodes));
                const box& cell_box = std::get<wrapped_box>(row_box.nodes[1]);
                CHECK(node_types_are<glyph>(cell_box.nodes));
            }
        }

        SUBCASE("1x3 matrix contains one row with three glyph noads")
        {
            const auto result =
                matrix_to_hlist(display_style, cramping::off,
                                {.rows = {matrix_row{std::vector{x_noad}, std::vector{x_noad}, std::vector{x_noad}}}});
            const box& matrix_box = std::get<wrapped_box>(result.nodes[0]);
            CHECK(node_types_are<box>(matrix_box.nodes));
            const box& row_box = std::get<wrapped_box>(matrix_box.nodes[0]);
            CHECK(node_types_are<box, glue_spec, box, glue_spec, box>(row_box.nodes));
            for (size_t i = 0; i < 3; ++i)
            {
                const box& cell_box = std::get<wrapped_box>(row_box.nodes[i * 2]);
                CHECK(node_types_are<glue_spec, box, glue_spec>(cell_box.nodes));
                const box& glyph_box = std::get<wrapped_box>(cell_box.nodes[1]);
                CHECK(node_types_are<glyph>(glyph_box.nodes));
            }
        }
    }
}