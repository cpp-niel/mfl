#include "fonts_for_tests/font_face.hpp"

#include "fonts_for_tests/freetype.hpp"

#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb-ot.h>
#include <range/v3/algorithm/contains.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>

namespace mfl::fft
{
    namespace
    {
        using namespace units_literals;

        constexpr int font_unit_factor = 1024;

        int32_t font_units_to_dist(const int u) { return u * font_unit_factor; }

        std::vector<size_variant> get_size_variants(hb_font_t* font, const size_t glyph_index, const hb_direction_t dir)
        {
            const auto max_number_of_variants = 20;
            auto variants = std::array<hb_ot_math_glyph_variant_t, max_number_of_variants>{};
            uint32_t num_variants = max_number_of_variants;
            const auto glyph_codepoint = static_cast<hb_codepoint_t>(glyph_index);
            hb_ot_math_get_glyph_variants(font, glyph_codepoint, dir, 0, &num_variants, variants.data());

            if (num_variants == 0)
            {
                num_variants = 1;
                variants[0].glyph = glyph_codepoint;
            }

            const auto to_size_variant = [&](const hb_ot_math_glyph_variant_t& v) {
                hb_glyph_extents_t extents;
                hb_font_get_glyph_extents_for_origin(font, v.glyph, HB_DIRECTION_LTR, &extents);
                const auto size = (dir == HB_DIRECTION_LTR) ? extents.width : extents.height;
                return size_variant{.glyph_index = v.glyph, .size = font_units_to_dist(std::abs(size))};
            };

            namespace rv = ranges::views;
            return variants | rv::take(num_variants) | rv::transform(to_size_variant) | ranges::to_vector;
        }
    }

    font_face::font_face(const font_family family, const freetype& ft) : ft_face_(ft.face(family)) {}

    math_constants font_face::constants() const
    {
        std::unique_ptr<hb_font_t, decltype(&hb_font_destroy)> hb_font(hb_ft_font_create(ft_face_, nullptr),
                                                                       hb_font_destroy);
        const auto get_constant = [&](const hb_ot_math_constant_t c) {
            return font_units_to_dist(hb_ot_math_get_constant(hb_font.get(), c));
        };

        // The standard values for non-display style fraction shifts are huge so we divide them
        // by four here to get a more typical non-display style tex aesthetic
        const auto default_fraction_params =
            fraction_constants{get_constant(HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP) / 4,
                               get_constant(HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN) / 4,
                               get_constant(HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN),
                               get_constant(HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN)};

        const auto display_style_fraction_params =
            fraction_constants{get_constant(HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP),
                               get_constant(HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN),
                               get_constant(HB_OT_MATH_CONSTANT_FRACTION_NUM_DISPLAY_STYLE_GAP_MIN),
                               get_constant(HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN)};

        const auto default_atop_params = fraction_constants{get_constant(HB_OT_MATH_CONSTANT_STACK_TOP_SHIFT_UP),
                                                            get_constant(HB_OT_MATH_CONSTANT_STACK_BOTTOM_SHIFT_DOWN),
                                                            get_constant(HB_OT_MATH_CONSTANT_STACK_GAP_MIN)};

        const auto display_style_atop_params =
            fraction_constants{get_constant(HB_OT_MATH_CONSTANT_STACK_TOP_DISPLAY_STYLE_SHIFT_UP),
                               get_constant(HB_OT_MATH_CONSTANT_STACK_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN),
                               get_constant(HB_OT_MATH_CONSTANT_STACK_DISPLAY_STYLE_GAP_MIN)};

        return {.axis_height = get_constant(HB_OT_MATH_CONSTANT_AXIS_HEIGHT),
                .fraction_rule_thickness = get_constant(HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS),
                .subscript_drop = get_constant(HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN),
                .subscript_shift_down = get_constant(HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN),
                .superscript_drop = get_constant(HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX),
                .superscript_shift_up = get_constant(HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP),
                .superscript_shift_up_cramped = get_constant(HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED),
                .minimum_dual_script_gap = get_constant(HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN),
                .maximum_superscript_bottom_in_dual_script =
                    get_constant(HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT),
                .space_after_script = get_constant(HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT),
                .radical_vertical_gap = get_constant(HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP),
                .radical_rule_thickness = get_constant(HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS),
                .radical_extra_ascender = get_constant(HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER),
                .radical_kern_before_degree = get_constant(HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE),
                .radical_kern_after_degree = get_constant(HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE),
                .radical_degree_bottom_raise_percent =
                    hb_ot_math_get_constant(hb_font.get(), HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT),
                .overline_gap = get_constant(HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP),
                .overline_padding = get_constant(HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER),
                .overline_thickness = get_constant(HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS),
                .underline_gap = get_constant(HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP),
                .underline_padding = get_constant(HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER),
                .underline_thickness = get_constant(HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS),
                .lower_limit_min_gap = get_constant(HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN),
                .upper_limit_min_gap = get_constant(HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN),
                .default_fraction = default_fraction_params,
                .display_style_fraction = display_style_fraction_params,
                .default_atop = default_atop_params,
                .display_style_atop = display_style_atop_params};
    }

    math_glyph_info font_face::glyph_info(const size_t glyph_index) const
    {
        std::unique_ptr<hb_font_t, decltype(&hb_font_destroy)> hb_font(hb_ft_font_create(ft_face_, nullptr),
                                                                       hb_font_destroy);
        FT_Load_Char(ft_face_, glyph_index, FT_LOAD_DEFAULT);
        const auto& metrics = ft_face_->glyph->metrics;

        const auto glyph_codepoint = static_cast<hb_codepoint_t>(glyph_index);
        const auto italic_correction = hb_ot_math_get_glyph_italics_correction(hb_font.get(), glyph_codepoint);

        // TODO - for the StixFonts, the horizontal advance seems to include the italic correction. This is
        // generally exactly what we want and expect (positioning a tall non-slanted character after a
        // slanted character should take that italic correction into account). We can clearly see that
        // the italic correction is already in the horizontal advance on tall slanted symbols like
        // integrals, but if we subtract the italic correction
        // from the width on all symbols then superscripts are drawn too close to the top right corner of
        // italic base symbols. So we only subtract the italic correction for tall slanted symbols. This
        // appears to means that when taking, say, an italic X, the italic correction is already in the
        // width and then when positioning a superscript on that X the italic correction is applied again,
        // but this prevents superscripts from overlapping with italic symbols in the nucleus.
        const auto integral_indices = std::array{1699, 1705, 1711, 1717, 1723, 1729};
        const auto integral_fix = ranges::contains(integral_indices, glyph_index) ? -italic_correction : 0;

        hb_glyph_extents_t extents;
        hb_font_get_glyph_extents_for_origin(hb_font.get(), glyph_codepoint, HB_DIRECTION_LTR,
                                             &extents);  // TODO what does the boolean return tell us?

        const auto height = metrics.horiBearingY;
        const auto depth =
            metrics.height - height;  // metrics height is total glyph height - our height is height from the baseline
        return {glyph_index,
                font_units_to_dist(static_cast<int>(metrics.horiAdvance + integral_fix)),
                font_units_to_dist(static_cast<int>(height)),
                font_units_to_dist(static_cast<int>(depth)),
                font_units_to_dist(italic_correction),
                font_units_to_dist(hb_ot_math_get_glyph_top_accent_attachment(hb_font.get(), glyph_codepoint))};
    }

    size_t font_face::glyph_index_from_code_point(const code_point char_code, const bool use_large_variant) const
    {
        // hard wired values from the stylistic set 04 in Stix2Math which contains nicer looking primes
        constexpr auto prime_index = 8242U;
        constexpr auto offset_to_better_primes = 6792U;
        if ((char_code >= prime_index) && (char_code < prime_index + 3)) return char_code - offset_to_better_primes;

        std::unique_ptr<hb_font_t, decltype(&hb_font_destroy)> hb_font(hb_ft_font_create(ft_face_, nullptr),
                                                                       hb_font_destroy);
        hb_codepoint_t base_index = 0;
        hb_font_get_glyph(hb_font.get(), char_code, 0, &base_index);

        return base_index + (use_large_variant ? 1 : 0);
    }

    std::vector<size_variant> font_face::horizontal_size_variants(const code_point char_code) const
    {
        std::unique_ptr<hb_font_t, decltype(&hb_font_destroy)> hb_font(hb_ft_font_create(ft_face_, nullptr),
                                                                       hb_font_destroy);
        const auto glyph_index = glyph_index_from_code_point(char_code, false);
        return get_size_variants(hb_font.get(), glyph_index, HB_DIRECTION_LTR);
    }

    std::vector<size_variant> font_face::vertical_size_variants(const code_point char_code) const
    {
        std::unique_ptr<hb_font_t, decltype(&hb_font_destroy)> hb_font(hb_ft_font_create(ft_face_, nullptr),
                                                                       hb_font_destroy);
        const auto glyph_index = glyph_index_from_code_point(char_code, false);
        return get_size_variants(hb_font.get(), glyph_index, HB_DIRECTION_BTT);
    }

    void font_face::set_size(const points size) { ft_set_size(ft_face_, size); }
}