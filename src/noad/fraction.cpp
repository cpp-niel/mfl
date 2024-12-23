#include "noad/fraction.hpp"

#include "node/box.hpp"
#include "node/hlist.hpp"
#include "node/vstack.hpp"
#include "settings.hpp"

namespace mfl
{
    namespace
    {
        bool is_empty(const fraction& f)
        {
            return ((f.left_delim_code == 0) && (f.right_delim_code == 0) && (f.numerator.empty())
                    && f.denominator.empty());
        }

        vlist make_fraction_vlist(const dist_t d, box&& b) { return make_vlist(kern{.size = d}, std::move(b)); }

        node_variant make_fraction(const settings s, const dist_t thickness, const dist_t width, box&& num, box&& den)
        {
            auto stroke = rule{.width = width, .height = thickness / 2, .depth = thickness / 2};

            const auto dist_num_to_axis = frac_numerator_shift(s) - axis_height(s);
            const auto dist_num_base_to_stroke =
                std::max(dist_num_to_axis - (thickness / 2) - num.dims.depth, frac_numerator_min_gap(s));

            const auto dist_den_to_axis = frac_denominator_shift(s) + axis_height(s);
            const auto dist_den_top_to_stroke =
                std::max(dist_den_to_axis - (thickness / 2) - den.dims.height, frac_denominator_min_gap(s));

            auto result = make_vbox(width, stroke, make_fraction_vlist(dist_num_base_to_stroke, std::forward<box>(num)),
                                    make_fraction_vlist(dist_den_top_to_stroke, std::forward<box>(den)));
            result.shift = -axis_height(s);
            return result;
        }

        node_variant make_atop(const settings s, box&& num, box&& den)
        {
            const auto dist_num = atop_numerator_shift(s) - num.dims.depth;
            const auto dist_den = atop_denominator_shift(s) - den.dims.height;
            const auto dist = dist_num + dist_den;
            const auto min_dist = atop_min_gap(s);
            const auto distances = (dist >= min_dist)
                                       ? vstack_distances{.node0_to_baseline = dist_num, .node0_to_node1 = dist}
                                       : vstack_distances{.node0_to_baseline = dist_num + (min_dist - dist) / 2,
                                                          .node0_to_node1 = min_dist};

            return vstack(std::forward<box>(num), std::forward<box>(den), distances);
        }

        node_variant make_delimiter(const settings s, const code_point delimiter_char_code,
                                    const dist_t requested_height)
        {
            if (delimiter_char_code != 0)
                return make_auto_height_glyph(s, font_family::roman, delimiter_char_code, requested_height).first;

            return kern{.size = null_delimiter_space()};
        }

        box make_generalized_fraction(const settings s, const std::optional<dist_t>& optional_rule_thickness,
                                      const code_point left_delimiter_char_code,
                                      const code_point right_delimiter_char_code, box&& numerator, box&& denominator)
        {
            const auto width = std::max(numerator.dims.width, denominator.dims.width);
            auto num = rebox(width, std::move(numerator));
            auto den = rebox(width, std::move(denominator));

            const auto thickness = optional_rule_thickness ? *optional_rule_thickness : rule_thickness(s);

            auto middle = (thickness != 0) ? make_fraction(s, thickness, width, std::move(num), std::move(den))
                                           : make_atop(s, std::move(num), std::move(den));

            const auto sz = vsize(middle);
            return make_hbox(make_hlist(make_delimiter(s, left_delimiter_char_code, sz), std::move(middle),
                                        make_delimiter(s, right_delimiter_char_code, sz)));
        }

        settings fraction_settings(const settings s)
        {
            if (s.style == formula_style::display) return {.style = formula_style::text, .fonts = s.fonts};
            if (s.style == formula_style::text) return {.style = formula_style::script, .fonts = s.fonts};
            return {.style = formula_style::script_script, .fonts = s.fonts};
        }
    }

    hlist fraction_to_hlist(const settings s, const cramping cramp, const fraction& f)
    {
        if (is_empty(f)) return {};

        auto numerator_box = clean_box(fraction_settings(s), cramp, f.numerator);
        auto denominator_box = clean_box(fraction_settings(s), cramping::on, f.denominator);
        return make_hlist(make_generalized_fraction(s, f.thickness, f.left_delim_code, f.right_delim_code,
                                                    std::move(numerator_box), std::move(denominator_box)));
    }
}
