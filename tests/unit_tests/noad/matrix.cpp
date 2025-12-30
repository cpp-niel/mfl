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
    TEST_CASE("matrixnoad")
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
    }
}