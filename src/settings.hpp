#pragma once

#include "dist.hpp"

namespace mfl
{
    class font_library;

    enum class formula_style
    {
        display,
        text,
        script,
        script_script
    };

    struct settings
    {
        formula_style style;
        const font_library* fonts;
    };

    [[nodiscard]] dist_t x_height(const settings s);
    [[nodiscard]] dist_t quad(const settings s);
    [[nodiscard]] dist_t math_unit(const settings s);
    [[nodiscard]] dist_t axis_height(const settings s);
    [[nodiscard]] dist_t rule_thickness(const settings s);
    [[nodiscard]] dist_t atop_numerator_shift(const settings s);
    [[nodiscard]] dist_t atop_min_gap(const settings s);
    [[nodiscard]] dist_t frac_numerator_shift(const settings s);
    [[nodiscard]] dist_t frac_numerator_min_gap(const settings s);
    [[nodiscard]] dist_t atop_denominator_shift(const settings s);
    [[nodiscard]] dist_t frac_denominator_shift(const settings s);
    [[nodiscard]] dist_t frac_denominator_min_gap(const settings s);
    [[nodiscard]] dist_t superscript_shift(const settings s, const bool is_cramped);
    [[nodiscard]] dist_t subscript_shift(const settings s);
    [[nodiscard]] dist_t minimum_dual_script_gap(const settings s);
    [[nodiscard]] dist_t maximum_superscript_bottom_in_dual_script(const settings s);
    [[nodiscard]] dist_t superscript_drop(const settings s);
    [[nodiscard]] dist_t subscript_drop(const settings s);
    [[nodiscard]] dist_t script_space(const settings s);
    [[nodiscard]] dist_t big_op_superscript_clearance(const settings s);
    [[nodiscard]] dist_t big_op_subscript_clearance(const settings s);
    [[nodiscard]] dist_t big_op_padding();
    [[nodiscard]] dist_t radical_vertical_gap(const settings s);
    [[nodiscard]] dist_t radical_rule_thickness(const settings s);
    [[nodiscard]] dist_t radical_extra_ascender(const settings s);
    [[nodiscard]] dist_t radical_kern_before_degree(const settings s);
    [[nodiscard]] dist_t radical_kern_after_degree(const settings s);
    [[nodiscard]] dist_t radical_degree_bottom_raise_percent(const settings s);
    [[nodiscard]] dist_t null_delimiter_space();
    [[nodiscard]] dist_t overline_gap(const settings s);
    [[nodiscard]] dist_t overline_thickness(const settings s);
    [[nodiscard]] dist_t overline_padding(const settings s);
    [[nodiscard]] dist_t underline_gap(const settings s);
    [[nodiscard]] dist_t underline_thickness(const settings s);
    [[nodiscard]] dist_t underline_padding(const settings s);
    [[nodiscard]] points font_size(const settings s);
}