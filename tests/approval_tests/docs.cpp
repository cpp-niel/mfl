#include "framework/approval_tests.hpp"
#include "renderer/render_formulas.hpp"

#include "parser/parser_state.hpp"
#include "parser/symbols/accents.hpp"
#include "parser/symbols/arrows.hpp"
#include "parser/symbols/big_operators.hpp"
#include "parser/symbols/binary_operators.hpp"
#include "parser/symbols/delimiters.hpp"
#include "parser/symbols/dots.hpp"
#include "parser/symbols/greek_alphabet.hpp"
#include "parser/symbols/letterlike.hpp"
#include "parser/symbols/punctuation.hpp"
#include "parser/symbols/relational_operators.hpp"

#include <doctest/doctest.h>
#include <fmt/format.h>

#include <ranges>

namespace mfl
{
    namespace
    {
        using namespace ::mfl::units_literals;
        using namespace ::std::string_literals;

        struct symbol_render_config
        {
            std::size_t num_columns = 1;
            pixels line_height = 30_px;
            pixels input_offset = 30_px;
        };

        auto render_symbols(const symbol_render_config& config, const std::ranges::range auto& symbols)
        {
            const auto d = std::div(long(std::ranges::distance(symbols)), long(config.num_columns));
            const auto num_rows = size_t(d.quot + ((d.rem > 0) ? 1 : 0));
            const auto width = 720_px;
            const auto height = double(num_rows) * config.line_height + 40_px;
            auto columns = std::vector<column_config>(config.num_columns);
            for (auto&& [i, col] : std::views::enumerate(columns))
            {
                col.x = (double(i) * (width / double(config.num_columns))) + 10_px;
                col.line_height = config.line_height;
                col.num_rows = num_rows;
            }

            return render_formulas({.width = width,
                                    .height = height,
                                    .render_input = true,
                                    .input_offset = config.input_offset,
                                    .columns = columns},
                                   symbols | std::views::transform([](const auto& sym) { return "\\"s + sym; }));
        }
    }

    TEST_CASE("cauchy_integral")
    {
        const auto formula = std::vector<std::string>{R"(f(a) = \frac{1}{2\pi i} \int_\gamma \frac{f(z)}{z-a} \, dz)"};
        const auto result = render_formulas(
            {.width = 340_px, .height = 100_px, .columns = {{.initial_offset = 50_px, .x = 10_px}}}, formula);

        approve_svg(result);
    }

    TEST_CASE("subscripts_and_superscripts")
    {
        const auto formula = std::vector<std::string>{
            R"(x^2)", R"(M_{i,j})", R"(z_i)", R"({}_{23}^{42} F)", R"(y_i^2)", R"(\mathcal{X}^{z_j^e}_{a_{i,j}^{2x}})",
        };
        const auto result = render_formulas({.width = 800_px,
                                             .height = 100_px,
                                             .render_input = true,
                                             .columns =
                                                 {
                                                     {.x = 10_px, .num_rows = 2},
                                                     {.x = 240_px, .num_rows = 2},
                                                     {.x = 470_px, .num_rows = 2},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("fractions")
    {
        const auto formula = std::vector<std::string>{
            R"(\frac{1}{x})",
            R"(\frac{x^2-y^2}{x+y})",
            R"(\frac{\frac{\alpha}{x+y}+\frac{1}{x}}{1-\frac{x^2}{1-z}})",
        };
        const auto result = render_formulas({.width = 800_px,
                                             .height = 250_px,
                                             .render_input = true,
                                             .input_offset = 100_px,
                                             .columns =
                                                 {
                                                     {.line_height = 80_px, .x = 10_px, .num_rows = 3},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("genfrac")
    {
        const auto formula = std::vector<std::string>{
            R"(\binom{n}{k})",
            R"(\genfrac{}{}{0}{}{0\leq i\leq m}{0<j<n})",
            R"(\genfrac][{4}{}{x}{y})",
        };
        const auto result = render_formulas({.width = 800_px,
                                             .height = 220_px,
                                             .render_input = true,
                                             .input_offset = 150_px,
                                             .columns =
                                                 {
                                                     {.line_height = 70_px, .x = 10_px, .num_rows = 3},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("radicals")
    {
        const auto formula = std::vector<std::string>{
            R"(\sqrt{a+b}=c)",
            R"(\sqrt[n]{\frac{a^2+b^2}{c^n}})",
            R"(\sqrt[3]{-q + \sqrt{q^2 + p^3}})",
        };
        const auto result = render_formulas({.width = 800_px,
                                             .height = 220_px,
                                             .render_input = true,
                                             .input_offset = 200_px,
                                             .columns =
                                                 {
                                                     {.line_height = 70_px, .x = 10_px, .num_rows = 3},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("big_ops")
    {
        const auto result =
            render_symbols({.num_columns = 3, .line_height = 60_px}, parser::big_op_symbols | std::views::keys);

        approve_svg(result);
    }

    TEST_CASE("big_ops_limits")
    {
        const auto formula = std::vector<std::string>{
            R"(\sum_{i=0}^n)",
            R"(\int_0^\infty)",
            R"(\sum\nolimits_{i=0}^n)",
            R"(\int\limits_0^\infty)",
        };
        const auto result = render_formulas({.width = 600_px,
                                             .height = 150_px,
                                             .render_input = true,
                                             .input_offset = 50_px,
                                             .columns =
                                                 {
                                                     {.line_height = 70_px, .x = 10_px, .num_rows = 2},
                                                     {.line_height = 70_px, .x = 310_px, .num_rows = 2},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("big_ops_integrals")
    {
        const auto result = render_symbols({.num_columns = 3, .line_height = 60_px, .input_offset = 60_px},
                                           parser::integral_symbols | std::views::keys);

        approve_svg(result);
    }

    TEST_CASE("accents")
    {
        const auto formulas = parser::accents     //
                              | std::views::keys  //
                              | std::views::transform([](const char* name) { return fmt::format("{}{{a}}", name); });
        const auto result = render_symbols({.num_columns = 5}, formulas);

        approve_svg(result);
    }

    TEST_CASE("wide_accents")
    {
        const auto formula = std::vector<std::string>{
            R"(\widehat{PQR})",
            R"(\widetilde{x^2+1})",
        };
        const auto result = render_formulas({.width = 600_px,
                                             .height = 100_px,
                                             .render_input = true,
                                             .input_offset = 120_px,
                                             .columns =
                                                 {
                                                     {.x = 10_px},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("additional_accents")
    {
        // TODO - some additional accents are set in the wrong place
        const auto formulas = parser::additional_accents  //
                              | std::views::keys          //
                              | std::views::transform([](const char* name) { return fmt::format("{}{{a}}", name); });
        const auto result = render_symbols({.num_columns = 3}, formulas);

        approve_svg(result);
    }

    TEST_CASE("lines")
    {
        const auto formula = std::vector<std::string>{
            R"(\overline{\overline{x}^2 + \underline{z-\overline{y}}})",
        };
        const auto result = render_formulas({.width = 600_px,
                                             .height = 60_px,
                                             .render_input = true,
                                             .input_offset = 150_px,
                                             .columns =
                                                 {
                                                     {.x = 10_px},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("functions")
    {
        const auto formula = std::vector<std::string>{
            R"(\arccos)",
            R"(\arcsin)",
            R"(\arctan)",
            R"(\arg)",
            R"(\cos)",
            R"(\cosh)",
            R"(\cot)",
            R"(\coth)",
            R"(\csc)",
            R"(\deg)",
            R"(\dim)",
            R"(\exp)",
            R"(\hom)",
            R"(\ker)",
            R"(\lg)",
            R"(\ln)",
            R"(\log)",
            R"(\sec)",
            R"(\sin)",
            R"(\sinh)",
            R"(\tan)",
            R"(\tanh)",
            R"(\det_M)",
            R"(\gcd_n)",
            R"(\inf_x)",
            R"(\lim_{x \to \infty})",
            R"(\liminf_{x \to \infty})",
            R"(\limsup_{x \to \infty})",
            R"(\max_{i \in X})",
            R"(\min_{i \in X})",
            R"(\Pr_x)",
            R"(\sup_x)",
        };
        const auto result = render_formulas({.width = 800_px,
                                             .height = 480_px,
                                             .render_input = true,
                                             .input_offset = 80_px,
                                             .columns =
                                                 {
                                                     {.line_height = 40_px, .x = 10_px, .num_rows = 11},
                                                     {.line_height = 40_px, .x = 260_px, .num_rows = 11},
                                                     {.line_height = 40_px, .x = 510_px, .num_rows = 11},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("operatorname")
    {
        const auto formula = std::vector<std::string>{
            R"(x_i = \operatorname{func}(x_{i-1}))",
        };
        const auto result = render_formulas({.width = 600_px,
                                             .height = 60_px,
                                             .render_input = true,
                                             .input_offset = 180_px,
                                             .columns =
                                                 {
                                                     {.x = 10_px},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("delimiters")
    {
        const auto formulas =
            std::views::zip(parser::left_delimiters | std::views::keys, parser::right_delimiters | std::views::keys)
            | std::views::transform(
                [](const auto delims) { return fmt::format("{} x \\{}", std::get<0>(delims), std::get<1>(delims)); });
        const auto result = render_symbols({.num_columns = 2, .input_offset = 40_px}, formulas);

        approve_svg(result);
    }

    TEST_CASE("sized_delimiters")
    {
        const auto formula = std::vector<std::string>{
            R"(\left[ \oint + \oint \right]_0^\infty)",
            R"(\left\langle \frac{1}{x} \right[)",
            R"(x = \left\{ \genfrac{}{}{0}{}{a \; : \; y > 0}{b \; : \; y \le 0} \right.)",
        };
        const auto result = render_formulas({.width = 800_px,
                                             .height = 260_px,
                                             .render_input = true,
                                             .input_offset = 160_px,
                                             .columns =
                                                 {
                                                     {.line_height = 80_px, .x = 10_px},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("fonts")
    {
        const auto formula = std::vector<std::string>{
            R"(\mathrm{abcABC123})", R"(\mathit{abcABC123})", R"(\mathbf{abcABC123})",   R"(\mathsf{abcABC123})",
            R"(\mathtt{abcABC123})", R"(\mathbb{abcABC123})", R"(\mathfrak{abcABC123})", R"(\mathcal{abcABC123})",
        };
        const auto result = render_formulas({.width = 600_px,
                                             .height = 360_px,
                                             .render_input = true,
                                             .input_offset = 200_px,
                                             .columns =
                                                 {
                                                     {.line_height = 40_px, .x = 10_px},
                                                 }},
                                            formula);

        approve_svg(result);
    }

    TEST_CASE("greek_alphabet_lowercase")
    {
        approve_svg(render_symbols({.num_columns = 4}, parser::greek_alphabet_lowercase | std::views::keys));
    }

    TEST_CASE("greek_alphabet_uppercase")
    {
        approve_svg(render_symbols({.num_columns = 4}, parser::greek_alphabet_uppercase | std::views::keys));
    }

    TEST_CASE("binary_operators")
    {
        approve_svg(render_symbols({.num_columns = 4}, parser::binary_operators | std::views::keys));
    }

    TEST_CASE("additional_binary_operators")
    {
        approve_svg(render_symbols({.num_columns = 3}, parser::additional_binary_operators | std::views::keys));
    }

    TEST_CASE("relational_operators")
    {
        approve_svg(render_symbols({.num_columns = 4}, parser::relational_operators | std::views::keys));
    }

    TEST_CASE("additional_relational_operators")
    {
        approve_svg(render_symbols({.num_columns = 3}, parser::additional_relational_operators | std::views::keys));
    }

    TEST_CASE("negations")
    {
        approve_svg(render_symbols({.num_columns = 4}, parser::negations | std::views::keys));
    }

    TEST_CASE("additional_negations")
    {
        approve_svg(render_symbols({.num_columns = 3}, parser::additional_negations | std::views::keys));
    }

    TEST_CASE("arrows")
    {
        approve_svg(render_symbols({.num_columns = 3}, parser::arrows | std::views::keys));
    }

    TEST_CASE("additional_arrows")
    {
        approve_svg(render_symbols({.num_columns = 3}, parser::additional_arrows | std::views::keys));
    }

    TEST_CASE("punctuation")
    {
        approve_svg(render_symbols({.num_columns = 3}, parser::punctuation_symbols | std::views::keys));
    }

    TEST_CASE("letterlike")
    {
        approve_svg(render_symbols({.num_columns = 4}, parser::letterlike_symbols | std::views::keys));
    }

    TEST_CASE("combining_symbols")
    {
        const auto formulas = parser::combining_symbols  //
                              | std::views::keys         //
                              | std::views::transform([](const char* name) { return fmt::format("c \\{}", name); });
        const auto result = render_formulas({.width = 720_px,
                                             .height = 100_px,
                                             .render_input = true,
                                             .input_offset = 30_px,
                                             .columns =
                                                 {
                                                     {.line_height = 30_px, .x = 10_px, .num_rows = 2},
                                                     {.line_height = 30_px, .x = 190_px, .num_rows = 2},
                                                     {.line_height = 30_px, .x = 370_px, .num_rows = 2},
                                                     {.line_height = 30_px, .x = 550_px, .num_rows = 2},
                                                 }},
                                            formulas);

        approve_svg(result);
    }

    TEST_CASE("dots")
    {
        approve_svg(render_symbols({.num_columns = 4}, parser::dots | std::views::keys));
    }

    TEST_CASE("spaces")
    {
        const auto formulas = std::vector<std::string>{
            R"(a \, b)", R"(a \/ b)",       R"(a \> b)",    R"(a \: b)",     R"(a \; b)",
            R"(a \  b)", R"(a \enspace b)", R"(a \quad b)", R"(a \qquad b)", R"(a \! b)",
        };
        const auto result = render_formulas({.width = 800_px,
                                             .height = 150_px,
                                             .render_input = true,
                                             .input_offset = 60_px,
                                             .columns =
                                                 {
                                                     {.x = 10_px, .num_rows = 4},
                                                     {.x = 240_px, .num_rows = 4},
                                                     {.x = 470_px, .num_rows = 4},
                                                 }},
                                            formulas);

        approve_svg(result);
    }
}