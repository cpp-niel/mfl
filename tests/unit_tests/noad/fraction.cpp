#include "noad/fraction.hpp"

#include "font_library.hpp"
#include "node/hlist.hpp"
#include "noad/noad.hpp"
#include "node/box.hpp"
#include "settings.hpp"

#include "framework/doctest.hpp"
#include "framework/mock_font_face.hpp"
#include "framework/node_types_are.hpp"

namespace mfl
{
    TEST_SUITE("fraction noad")
    {
        const noad x_noad = math_char{.char_code = 0};
        const auto fonts = font_library(create_mock_font_face);
        const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};
        const auto text_style = settings{.style = formula_style::text, .fonts = &fonts};
        const auto script_script_style = settings{.style = formula_style::script_script, .fonts = &fonts};

        TEST_CASE("no nodes created for fraction if no input noads")
        {
            const auto result = fraction_to_hlist(display_style, false, {.numerator = {}, .denominator = {}});
            CHECK(result.nodes.empty());
        }

        TEST_CASE("structure of a default, non-delimited fraction")
        {
            const auto result =
                fraction_to_hlist(display_style, false, {.numerator = {x_noad}, .denominator = {x_noad}});
            CHECK(node_types_are<box>(result.nodes));
            const box& b0 = std::get<wrapped_box>(result.nodes[0]);
            CHECK(
                node_types_are<kern, box, kern>(b0.nodes));  // fractions w/o delimiters have a kern either side of them
            const box& b1 = std::get<wrapped_box>(b0.nodes[1]);
            CHECK(node_types_are<box, kern, rule, kern, box>(b1.nodes));  // the fraction vlist
            const box& num = std::get<wrapped_box>(b1.nodes[0]);
            CHECK(node_types_are<glue_spec, box, glue_spec>(num.nodes));
            const box& den = std::get<wrapped_box>(b1.nodes[4]);
            CHECK(node_types_are<glue_spec, box, glue_spec>(den.nodes));
        }

        TEST_CASE("fraction box is shifted up by the axis height")
        {
            const auto result =
                fraction_to_hlist(display_style, false, {.numerator = {x_noad}, .denominator = {x_noad}});
            const box& b0 =
                std::get<wrapped_box>(result.nodes[0]);  // the hbox containing the delimiters and the fraction
            const box& b1 = std::get<wrapped_box>(b0.nodes[1]);
            CHECK(b1.shift == -axis_height(display_style));
        }

        TEST_CASE("fraction without delimiters has correct kern width on each side")
        {
            const auto result =
                fraction_to_hlist(display_style, false, {.numerator = {x_noad}, .denominator = {x_noad}});
            const box& b = std::get<wrapped_box>(result.nodes[0]);
            const kern& left_kern = std::get<kern>(b.nodes[0]);
            const kern& right_kern = std::get<kern>(b.nodes[2]);
            CHECK(left_kern.size == null_delimiter_space());
            CHECK(left_kern.size == right_kern.size);
        }

        TEST_CASE("fraction with delimiters has correct glyph on each side")
        {
            // we specify that the delimiters are codepoints 1 and 2 and then check that
            // the mock size variants have been chosen (4 and 5)
            const auto result = fraction_to_hlist(
                display_style, false,
                {.left_delim_code = 1, .numerator = {x_noad}, .denominator = {x_noad}, .right_delim_code = 2});
            const box& b = std::get<wrapped_box>(result.nodes[0]);
            CHECK(node_types_are<glyph, box, glyph>(b.nodes));
            const glyph& left_glyph = std::get<glyph>(b.nodes[0]);
            const glyph& right_glyph = std::get<glyph>(b.nodes[2]);
            CHECK(left_glyph.index == 4);
            CHECK(right_glyph.index == 5);
        }

        TEST_CASE("glue is used to center the narrower of numerator and denominator")
        {
            const auto result =
                fraction_to_hlist(display_style, false, {.numerator = {x_noad}, .denominator = {x_noad, x_noad}});
            const box& b0 = std::get<wrapped_box>(result.nodes[0]);
            const box& b1 = std::get<wrapped_box>(b0.nodes[1]);
            const box& num = std::get<wrapped_box>(b1.nodes[0]);
            const box& den = std::get<wrapped_box>(b1.nodes[4]);
            const glue_spec& num_left_glue = std::get<glue_spec>(num.nodes[0]);
            const glue_spec& num_right_glue = std::get<glue_spec>(num.nodes[2]);
            CHECK(num.glue.scale > 2.4);
            CHECK(num_left_glue.stretch.value == 65536);  // unit stretch / shrink values
            CHECK(num_right_glue.stretch.value == 65536);
            CHECK(num_left_glue.shrink.value == 65536);
            CHECK(num_right_glue.shrink.value == 65536);
            CHECK(den.glue.scale == 0.0);
        }

        TEST_CASE("spacing")
        {
            const auto result =
                fraction_to_hlist(display_style, false, {.numerator = {x_noad}, .denominator = {x_noad}});
            const box& b0 = std::get<wrapped_box>(result.nodes[0]);
            const box& b1 = std::get<wrapped_box>(b0.nodes[1]);
            const kern& num_space = std::get<kern>(b1.nodes[1]);
            const kern& den_space = std::get<kern>(b1.nodes[3]);
            CHECK(num_space.size == 237773);
            CHECK(den_space.size == 265421);
        }

        TEST_CASE("spacing in text style is smaller")
        {
            const auto result = fraction_to_hlist(text_style, false, {.numerator = {x_noad}, .denominator = {x_noad}});
            const box& b0 = std::get<wrapped_box>(result.nodes[0]);
            const box& b1 = std::get<wrapped_box>(b0.nodes[1]);
            const kern& num_space = std::get<kern>(b1.nodes[1]);
            const kern& den_space = std::get<kern>(b1.nodes[3]);
            CHECK(num_space.size == 45056);
            CHECK(den_space.size == 45056);
        }

        TEST_CASE("no nodes created for atop if no input noads")
        {
            const auto result =
                fraction_to_hlist(display_style, false, {.numerator = {}, .denominator = {}, .thickness = 0});
            CHECK(result.nodes.empty());
        }

        TEST_CASE("structure of a default, non-delimited atop")
        {
            const auto result = fraction_to_hlist(display_style, false,
                                                  {.numerator = {x_noad}, .denominator = {x_noad}, .thickness = 0});
            CHECK(node_types_are<box>(result.nodes));
            const box& b0 = std::get<wrapped_box>(result.nodes[0]);
            CHECK(node_types_are<kern, box, kern>(b0.nodes));  // atops w/o delimiters have a kern either side of them
            const box& b1 = std::get<wrapped_box>(b0.nodes[1]);
            CHECK(node_types_are<box, kern, box>(b1.nodes));  // the atop vlist
            const box& num = std::get<wrapped_box>(b1.nodes[0]);
            CHECK(node_types_are<glue_spec, box, glue_spec>(num.nodes));
            const box& den = std::get<wrapped_box>(b1.nodes[2]);
            CHECK(node_types_are<glue_spec, box, glue_spec>(den.nodes));
        }

        TEST_CASE("atop spacing")
        {
            const auto result = fraction_to_hlist(display_style, false,
                                                  {.numerator = {x_noad}, .denominator = {x_noad}, .thickness = 0});
            const box& b0 = std::get<wrapped_box>(result.nodes[0]);
            const box& b1 = std::get<wrapped_box>(b0.nodes[1]);
            const kern& space = std::get<kern>(b1.nodes[1]);
            CHECK(space.size == 653312);
        }

        TEST_CASE("atop spacing in text style is smaller")
        {
            const auto result =
                fraction_to_hlist(text_style, false, {.numerator = {x_noad}, .denominator = {x_noad}, .thickness = 0});
            const box& b0 = std::get<wrapped_box>(result.nodes[0]);
            const box& b1 = std::get<wrapped_box>(b0.nodes[1]);
            const kern& space = std::get<kern>(b1.nodes[1]);
            CHECK(space.size == 249856);
        }

        TEST_CASE("atop spacing in script script style is smallest")
        {
            const auto result = fraction_to_hlist(script_script_style, false,
                                                  {.numerator = {x_noad}, .denominator = {x_noad}, .thickness = 0});
            const box& b0 = std::get<wrapped_box>(result.nodes[0]);
            const box& b1 = std::get<wrapped_box>(b0.nodes[1]);
            const kern& space = std::get<kern>(b1.nodes[1]);
            CHECK(space.size == 249856);
        }

        TEST_CASE("atop with very small spacing")
        {
            const noad deep_noad = math_char{.char_code = 10};
            const noad high_noad = math_char{.char_code = 11};
            const auto result = fraction_to_hlist(
                script_script_style, false, {.numerator = {deep_noad}, .denominator = {high_noad}, .thickness = 0});
            const box& b0 = std::get<wrapped_box>(result.nodes[0]);
            const box& b1 = std::get<wrapped_box>(b0.nodes[1]);
            const kern& space = std::get<kern>(b1.nodes[1]);
            CHECK(space.size == 98304);
        }
    }
}
