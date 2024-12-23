#include "settings.hpp"

#include "dist.hpp"
#include "font_library.hpp"

namespace mfl
{
    namespace
    {
        using namespace units_literals;
        constexpr auto math_unit_divisor = 18;  // a math unit is defined as one eighteenth of a quad
        constexpr auto null_delimiter_gap_size = 1.2_pt;

        const auto& params(const settings s)
        {
            if (s.style == formula_style::script) return s.fonts->small_parameters();

            if (s.style == formula_style::script_script) return s.fonts->tiny_parameters();

            return s.fonts->normal_parameters();
        }

        settings script_base_style(const settings s)
        {
            if (s.style == formula_style::script_script) return {.style = formula_style::script, .fonts = s.fonts};

            return {.style = formula_style::display, .fonts = s.fonts};
        }

        fraction_constants atop_params(const settings s)
        {
            const auto& math_info = params(s).math_info;
            return (s.style == formula_style::display) ? math_info.display_style_atop : math_info.default_atop;
        }

        fraction_constants frac_params(const settings s)
        {
            const auto& math_info = params(s).math_info;
            return (s.style == formula_style::display) ? math_info.display_style_fraction : math_info.default_fraction;
        }
    }

    points font_size(const settings s) { return params(s).size; }

    dist_t x_height(const settings s) { return params(s).x_height; }

    dist_t quad(const settings s) { return params(s).capital_m_width; }

    dist_t math_unit(const settings s) { return quad(s) / math_unit_divisor; }

    dist_t axis_height(const settings s) { return params(s).math_info.axis_height; }

    dist_t rule_thickness(const settings s) { return params(s).math_info.fraction_rule_thickness; }

    dist_t overline_gap(const settings s) { return params(s).math_info.overline_gap; }

    dist_t overline_thickness(const settings s) { return params(s).math_info.overline_thickness; }

    dist_t overline_padding(const settings s) { return params(s).math_info.overline_padding; }

    dist_t underline_gap(const settings s) { return params(s).math_info.underline_gap; }

    dist_t underline_thickness(const settings s) { return params(s).math_info.underline_thickness; }

    dist_t underline_padding(const settings s) { return params(s).math_info.underline_padding; }

    dist_t null_delimiter_space() { return points_to_dist(null_delimiter_gap_size); }

    dist_t atop_numerator_shift(const settings s) { return atop_params(s).numerator_shift_up; }

    dist_t atop_min_gap(const settings s) { return atop_params(s).numerator_min_gap; }

    dist_t frac_numerator_shift(const settings s) { return frac_params(s).numerator_shift_up; }

    dist_t frac_numerator_min_gap(const settings s) { return frac_params(s).numerator_min_gap; }

    dist_t atop_denominator_shift(const settings s) { return atop_params(s).denominator_shift_down; }

    dist_t frac_denominator_shift(const settings s) { return frac_params(s).denominator_shift_down; }

    dist_t frac_denominator_min_gap(const settings s) { return frac_params(s).denominator_min_gap; }

    dist_t superscript_shift(const settings s, const cramping cramp)
    {
        const auto& info = params(script_base_style(s)).math_info;
        return (cramp == cramping::on) ? info.superscript_shift_up_cramped : info.superscript_shift_up;
    }

    dist_t subscript_shift(const settings s) { return params(script_base_style(s)).math_info.subscript_shift_down; }

    dist_t minimum_dual_script_gap(const settings s)
    {
        return params(script_base_style(s)).math_info.minimum_dual_script_gap;
    }

    dist_t maximum_superscript_bottom_in_dual_script(const settings s)
    {
        return params(script_base_style(s)).math_info.maximum_superscript_bottom_in_dual_script;
    }

    dist_t subscript_drop(const settings s) { return params(script_base_style(s)).math_info.subscript_drop; }

    dist_t superscript_drop(const settings s) { return params(script_base_style(s)).math_info.superscript_drop; }

    dist_t script_space(const settings s) { return params(script_base_style(s)).math_info.space_after_script; }

    dist_t big_op_superscript_clearance(const settings s) { return params(s).math_info.upper_limit_min_gap; }

    dist_t big_op_subscript_clearance(const settings s) { return params(s).math_info.lower_limit_min_gap; }

    dist_t big_op_padding() { return unit_distance; }

    dist_t radical_vertical_gap(const settings s) { return params(s).math_info.radical_vertical_gap; }

    dist_t radical_rule_thickness(const settings s) { return params(s).math_info.radical_rule_thickness; }

    dist_t radical_extra_ascender(const settings s) { return params(s).math_info.radical_extra_ascender; }

    dist_t radical_kern_before_degree(const settings s) { return params(s).math_info.radical_kern_before_degree; }

    dist_t radical_kern_after_degree(const settings s) { return params(s).math_info.radical_kern_after_degree; }

    dist_t radical_degree_bottom_raise_percent(const settings s)
    {
        return params(s).math_info.radical_degree_bottom_raise_percent;
    }
}
