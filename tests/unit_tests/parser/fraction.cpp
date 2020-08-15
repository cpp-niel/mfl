#include "parser/parse.hpp"

#include "framework/code_points.hpp"
#include <doctest/doctest.h>
#include "framework/node_types_are.hpp"
#include "noad/fraction.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse fraction")
    {
        TEST_CASE("standard fraction")
        {
            const auto [result, error] = parse("\\frac{x}{2}");
            CHECK(node_types_are<fraction_wrapper>(result));
            const fraction& f = std::get<fraction_wrapper>(result[0]);
            CHECK(node_types_are<math_char>(f.numerator));
            CHECK(std::get<math_char>(f.numerator[0]).family == font_family::roman);
            CHECK(node_types_are<math_char>(f.denominator));
            CHECK(std::get<math_char>(f.denominator[0]).family == font_family::roman);
        }

        TEST_CASE("nested fraction")
        {
            const auto [result, error] = parse("\\frac{\\frac{y}{4}}{2}");
            CHECK(node_types_are<fraction_wrapper>(result));
            const fraction& f = std::get<fraction_wrapper>(result[0]);
            CHECK(node_types_are<fraction_wrapper>(f.numerator));
            CHECK(node_types_are<math_char>(f.denominator));
        }

        TEST_CASE("binom")
        {
            const auto [result, error] = parse("\\binom{x}{2}");
            CHECK(node_types_are<fraction_wrapper>(result));
            const fraction& f = std::get<fraction_wrapper>(result[0]);
            CHECK(f.thickness.value() == 0);
            CHECK(f.left_delim_code == open_paren);
            CHECK(f.right_delim_code == close_paren);
            CHECK(node_types_are<math_char>(f.numerator));
            CHECK(std::get<math_char>(f.numerator[0]).family == font_family::roman);
            CHECK(node_types_are<math_char>(f.denominator));
            CHECK(std::get<math_char>(f.denominator[0]).family == font_family::roman);
        }

        TEST_CASE("generalized_fraction")
        {
            const auto [result, error] = parse("\\genfrac[){42}{}{x}{y}");
            CHECK(node_types_are<fraction_wrapper>(result));
            const fraction& f = std::get<fraction_wrapper>(result[0]);
            CHECK(f.thickness.value() == 2752512);
            CHECK(f.left_delim_code == open_bracket);
            CHECK(f.right_delim_code == close_paren);
            CHECK(node_types_are<math_char>(f.numerator));
            CHECK(std::get<math_char>(f.numerator[0]).family == font_family::roman);
            CHECK(node_types_are<math_char>(f.denominator));
            CHECK(std::get<math_char>(f.denominator[0]).family == font_family::roman);
        }

        TEST_CASE("generalized_fraction with non-delimiter in the delimiter argument")
        {
            const auto [result, error] = parse("\\genfrac{|}{a}{42}{}{x}{y}");
            CHECK(*error == "Syntax error: 'a' is not a valid delimiter.");
        }
    }
}
