#pragma once

#include "mfl/code_point.hpp"
#include "mfl/font_family.hpp"
#include "mfl/units.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace mfl
{
    struct fraction_constants
    {
        std::int32_t numerator_shift_up = 0;
        std::int32_t denominator_shift_down = 0;
        std::int32_t numerator_min_gap = 0;
        std::int32_t denominator_min_gap = 0;
    };

    struct math_constants
    {
        std::int32_t axis_height = 0;
        std::int32_t fraction_rule_thickness = 0;
        std::int32_t subscript_drop = 0;
        std::int32_t subscript_shift_down = 0;
        std::int32_t superscript_drop = 0;
        std::int32_t superscript_shift_up = 0;
        std::int32_t superscript_shift_up_cramped = 0;
        std::int32_t minimum_dual_script_gap = 0;
        std::int32_t maximum_superscript_bottom_in_dual_script = 0;
        std::int32_t space_after_script = 0;
        std::int32_t radical_vertical_gap = 0;
        std::int32_t radical_rule_thickness = 0;
        std::int32_t radical_extra_ascender = 0;
        std::int32_t radical_kern_before_degree = 0;
        std::int32_t radical_kern_after_degree = 0;
        std::int32_t radical_degree_bottom_raise_percent = 0;
        std::int32_t overline_gap = 0;
        std::int32_t overline_padding = 0;
        std::int32_t overline_thickness = 0;
        std::int32_t underline_gap = 0;
        std::int32_t underline_padding = 0;
        std::int32_t underline_thickness = 0;
        std::int32_t lower_limit_min_gap = 0;
        std::int32_t upper_limit_min_gap = 0;
        fraction_constants default_fraction;
        fraction_constants display_style_fraction;
        fraction_constants default_atop;
        fraction_constants display_style_atop;
    };

    struct math_glyph_info
    {
        size_t glyph_index = 0;
        std::int32_t width = 0;
        std::int32_t height = 0;
        std::int32_t depth = 0;
        std::int32_t italic_correction = 0;
        std::int32_t accent_hpos = 0;
    };

    struct size_variant
    {
        size_t glyph_index = 0;
        std::int32_t size = 0;
    };

    struct glyph_part
    {
        size_t glyph_index = 0;
        std::int32_t start_connector_length = 0;
        std::int32_t end_connector_length = 0;
        std::int32_t full_advance = 0;
        bool is_extender;
    };

    struct glyph_assembly
    {
        std::vector<glyph_part> parts;
        bool is_extensible;
    };

    struct abstract_font_face
    {
        virtual ~abstract_font_face() = default;
        [[nodiscard]] virtual math_constants constants() const = 0;
        [[nodiscard]] virtual math_glyph_info glyph_info(const size_t glyph_index) const = 0;
        [[nodiscard]] virtual size_t glyph_index_from_code_point(const code_point char_code,
                                                                 const bool use_large_variant) const = 0;
        [[nodiscard]] virtual std::vector<size_variant> horizontal_size_variants(const code_point char_code) const = 0;
        [[nodiscard]] virtual std::vector<size_variant> vertical_size_variants(const code_point char_code) const = 0;
        [[nodiscard]] virtual std::optional<glyph_assembly> horizontal_assembly(const code_point char_code) const = 0;
        [[nodiscard]] virtual std::optional<glyph_assembly> vertical_assembly(const code_point char_code) const = 0;
        virtual void set_size(const points size) = 0;
    };

    using font_face_creator = std::function<std::unique_ptr<abstract_font_face>(const font_family)>;
}