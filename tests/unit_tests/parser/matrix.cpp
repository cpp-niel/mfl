#include "parser/parse.hpp"

#include "framework/doctest.hpp"
#include "framework/node_types_are.hpp"
#include "noad/matrix.hpp"

namespace mfl::parser
{
    TEST_SUITE("parse matrix")
    {
        TEST_CASE("empty matrix has no rows")
        {
            const auto [result, error] = parse("\\matrix{}");
            CHECK(node_types_are<matrix_wrapper>(result));
            const matrix& m = std::get<matrix_wrapper>(result[0]);
            CHECK(m.rows.empty());
        }

        TEST_CASE("simple 2x2 matrix contains the correct math chars in the correct cells")
        {
            const auto [result, error] = parse("\\matrix{a & b \\cr c & d }");
            CHECK(node_types_are<matrix_wrapper>(result));
            const matrix& m = std::get<matrix_wrapper>(result[0]);
            CHECK(node_types_are<math_char>(m.rows[0][0]));
            CHECK(std::get<math_char>(m.rows[0][0][0]).family == font_family::roman);
            CHECK(std::get<math_char>(m.rows[0][0][0]).char_code == 119886);

            CHECK(node_types_are<math_char>(m.rows[0][1]));
            CHECK(std::get<math_char>(m.rows[0][1][0]).char_code == 119887);

            CHECK(node_types_are<math_char>(m.rows[1][0]));
            CHECK(std::get<math_char>(m.rows[1][0][0]).char_code == 119888);

            CHECK(node_types_are<math_char>(m.rows[1][1]));
            CHECK(std::get<math_char>(m.rows[1][1][0]).char_code == 119889);
        }

        TEST_CASE("jacobian matrix contains the correct number of rows and columns")
        {
            const auto [result, error] = parse(R"(\matrix{
                \frac{\partial f_1}{\partial x_1} & \frac{\partial f_1}{\partial x_2} & \cdots & \frac{\partial f_1}{\partial x_n} \cr
                \frac{\partial f_2}{\partial x_1} & \frac{\partial f_2}{\partial x_2} & \cdots & \frac{\partial f_2}{\partial x_n} \cr
                \vdots                            & \vdots                            & \ddots & \vdots                            \cr
                \frac{\partial f_m}{\partial x_1} & \frac{\partial f_m}{\partial x_2} & \cdots & \frac{\partial f_m}{\partial x_n}
            })");
            CHECK(node_types_are<matrix_wrapper>(result));
            const matrix& m = std::get<matrix_wrapper>(result[0]);
            CHECK(m.rows.size() == 4);
            CHECK(m.rows[0].size() == 4);
        }

        TEST_CASE("matrix without closing brace")
        {
            const auto [result, error] = parse("\\matrix{a & b \\cr c & d");
            CHECK(*error
                  == "Syntax error: Input stream ends inside matrix definition. The closing brace may be missing.");
        }
    }
}