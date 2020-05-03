#include "framework/mock_font_face.hpp"

namespace mfl
{
    math_constants mock_font_face::constants() const
    {
        // These values are taken from an actual font face
        return {.axis_height = 168960,
                .subscript_drop = 104448,
                .subscript_shift_down = 137216,
                .superscript_drop = 150528,
                .superscript_shift_up = 235520,
                .superscript_shift_up_cramped = 164864,
                .minimum_dual_script_gap = 98304,
                .maximum_superscript_bottom_in_dual_script = 248832,
                .space_after_script = 26624,
                .radical_vertical_gap = 55296,
                .radical_rule_thickness = 45056,
                .radical_extra_ascender = 51200,
                .radical_kern_before_degree = 43008,
                .radical_kern_after_degree = -219136,
                .radical_degree_bottom_raise_percent = 55,
                .default_fraction = {.numerator_shift_up = 95744,
                                     .denominator_shift_down = 95744,
                                     .numerator_min_gap = 45056,
                                     .denominator_min_gap = 45056},
                .display_style_fraction = {.numerator_shift_up = 419840,
                                           .denominator_shift_down = 419840,
                                           .numerator_min_gap = 98304,
                                           .denominator_min_gap = 98304},
                .default_atop = {.numerator_shift_up = 308224,
                                 .denominator_shift_down = 251904,
                                 .numerator_min_gap = 98304,
                                 .denominator_min_gap = 0},
                .display_style_atop = {.numerator_shift_up = 510976,
                                       .denominator_shift_down = 452608,
                                       .numerator_min_gap = 196608,
                                       .denominator_min_gap = 0}};
    }

    math_glyph_info mock_font_face::glyph_info(const size_t glyph_index) const
    {
        if (glyph_index == 10)
            return {.glyph_index = glyph_index, .width = 316416, .depth = 310272};

        if (glyph_index == 11)
            return {.glyph_index = glyph_index, .width = 316416, .height = 620272};

        return {.glyph_index = glyph_index, .width = 316416, .height = 310272, .accent_hpos = 42};
    }

    std::vector<size_variant> mock_font_face::horizontal_size_variants(const code_point char_code) const
    {
        if (char_code == 1) return {{3, 12}, {4, 24}};

        if (char_code == 2) return {{5, 12}, {5, 24}};

        return {};
    }

    std::vector<size_variant> mock_font_face::vertical_size_variants(const code_point char_code) const
    {
        if (char_code == 1) return {{3, 12}, {4, 24}};

        if (char_code == 2) return {{5, 12}, {5, 24}};

        return {};
    }

    std::unique_ptr<abstract_font_face> create_mock_font_face(font_family)
    {
        return std::make_unique<mock_font_face>();
    }
}