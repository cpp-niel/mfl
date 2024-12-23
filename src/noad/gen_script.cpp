#include "noad/gen_script.hpp"

#include "noad/big_op.hpp"
#include "noad/script.hpp"
#include "node/box.hpp"
#include "node/hlist.hpp"
#include "node/vstack.hpp"

#include <algorithm>

namespace mfl
{
    namespace
    {
        nucleus_result make_nucleus(const settings s, const cramping cramp, const bool is_big_op,
                                    const std::vector<noad>& noads)
        {
            if ((noads.size() == 1) && (std::holds_alternative<math_char>(noads.front())))
            {
                const auto& mc = std::get<math_char>(noads.front());
                const auto enlarge = ((s.style == formula_style::display) && is_big_op);
                auto [glyph_node, corrections] = make_glyph(s, mc.family, mc.char_code, enlarge);
                if (is_big_op) return {center_on_axis(s, make_hbox(make_hlist(glyph_node))), corrections, false};

                return {glyph_node, corrections, true};
            }

            return {clean_box(s, cramp, noads), {}, false};
        }

        vlist limit_sup_list(const settings s, const dist_t total_width, const dist_t shift,
                             std::optional<node_variant>&& sup_node)
        {
            if (!sup_node) return {};

            box b = std::get<wrapped_box>(sup_node.value());
            auto sup_box = rebox(total_width, std::move(b));
            sup_box.shift = shift;
            return make_vlist(kern{.size = big_op_superscript_clearance(s)}, std::move(sup_box),
                              kern{.size = big_op_padding()});
        }

        vlist limit_sub_list(const settings s, const dist_t total_width, const dist_t shift,
                             std::optional<node_variant>&& sub_node)
        {
            if (!sub_node) return {};

            box b = std::get<wrapped_box>(sub_node.value());
            auto sub_box = rebox(total_width, std::move(b));
            sub_box.shift = shift;

            return make_vlist(kern{.size = big_op_subscript_clearance(s)}, std::move(sub_box),
                              kern{.size = big_op_padding()});
        }

        hlist make_limit_hlist(const settings s, const cramping cramp, const dist_t italic_correction,
                               node_variant&& nuc, const std::optional<std::vector<noad>>& sub,
                               const std::optional<std::vector<noad>>& sup)
        {
            auto sup_node =
                sup ? std::optional<node_variant>(clean_box(s, cramp, *sup)) : std::optional<node_variant>();
            const auto sup_width = sup_node ? width(*sup_node) : 0;
            auto sub_node =
                sub ? std::optional<node_variant>(clean_box(s, cramp, *sub)) : std::optional<node_variant>();
            const auto sub_width = sub_node ? width(*sub_node) : 0;
            const auto w = std::max(width(nuc), std::max(sup_width, sub_width));
            const auto shift = italic_correction / 2;
            auto nuc_box = rebox(w, make_hbox(make_hlist(std::move(nuc))));
            return make_hlist(make_vbox(w, std::move(nuc_box),
                                        limit_sup_list(s, w + italic_correction, shift, std::move(sup_node)),
                                        limit_sub_list(s, w + italic_correction, -shift, std::move(sub_node))));
        }

        dist_t almost_x_height(const settings s)
        {
            // four fifths of the xheight is what was used in TeX
            constexpr auto factor = dist_t(4);
            constexpr auto divisor = dist_t(5);
            return (x_height(s) * factor) / divisor;
        }

        dist_t sub_pos0(const settings s, const bool is_nucleus_single_character, const dist_t nuc_depth)
        {
            return is_nucleus_single_character ? 0 : nuc_depth + subscript_drop(s);
        }

        dist_t sup_pos0(const settings s, const bool is_nucleus_single_character, const dist_t nuc_height)
        {
            return is_nucleus_single_character ? 0 : nuc_height - superscript_drop(s);
        }

        dist_t sup_pos(const settings s, const cramping cramp, const bool is_nucleus_single_character,
                       const dist_t nuc_height, const dist_t sup_depth)
        {
            const auto candidate_positions =
                std::array{sup_pos0(s, is_nucleus_single_character, nuc_height), superscript_shift(s, cramp),
                           sup_depth + (x_height(s) / 4)};
            return std::ranges::max(candidate_positions);
        }

        dist_t subscript_alone_pos(const settings s, const bool is_nucleus_single_character, const dist_t nuc_depth,
                                   const dist_t sub_height)
        {
            const auto candidate_positions = std::array{sub_pos0(s, is_nucleus_single_character, nuc_depth),
                                                        subscript_shift(s), sub_height - almost_x_height(s)};
            return std::ranges::max(candidate_positions);
        }

        hlist make_subscript_hlist(const settings s, nucleus_result&& nuc, box&& sub_box)
        {
            const auto nuc_depth = depth(nuc.nucleus_node);
            const auto sub_height = sub_box.dims.height;
            sub_box.shift = subscript_alone_pos(s, nuc.is_single_character, nuc_depth, sub_height);
            return concat(make_hlist(std::move(nuc.nucleus_node)), extend(std::move(sub_box), script_space(s)));
        }

        hlist make_superscript_hlist(const settings s, const cramping cramp, nucleus_result&& nuc, box&& sup_box)
        {
            const auto nuc_height = height(nuc.nucleus_node);
            const auto sup_depth = sup_box.dims.depth;
            sup_box.shift = -sup_pos(s, cramp, nuc.is_single_character, nuc_height, sup_depth);
            return concat(extend(std::move(nuc.nucleus_node), nuc.corrections.italic_correction),
                          extend(std::move(sup_box), script_space(s)));
        }

        vstack_distances dual_script_vstack_distances(const settings s, const cramping cramp,
                                                      const bool is_nucleus_single_character, const dist_t nuc_depth,
                                                      const dist_t nuc_height, const dist_t sub_height,
                                                      const dist_t sup_depth)
        {
            auto sub_dist = subscript_alone_pos(s, is_nucleus_single_character, nuc_depth, sub_height) - sub_height;
            auto sup_dist = sup_pos(s, cramp, is_nucleus_single_character, nuc_height, sup_depth);
            const auto min_gap = minimum_dual_script_gap(s);
            const auto total_gap = sub_dist + sup_dist;
            if (total_gap < min_gap)
            {
                const auto total_offset = min_gap - total_gap;
                const auto sup_offset = std::min(total_offset, maximum_superscript_bottom_in_dual_script(s));
                const auto sub_offset = (sup_offset < total_offset) ? total_offset - sup_offset : 0;
                sub_dist += sub_offset;
                sup_dist += sup_offset;
            }

            return {.node0_to_baseline = sup_dist, .node0_to_node1 = sup_dist + sub_dist};
        }

        hlist make_dual_scripts_hlist(const settings s, const cramping cramp, nucleus_result&& nuc, box&& sub_box,
                                      box&& sup_box)
        {
            const auto nuc_depth = depth(nuc.nucleus_node);
            const auto nuc_height = height(nuc.nucleus_node);
            const auto sub_height = sub_box.dims.height;
            const auto sup_depth = sup_box.dims.depth;
            const auto distances = dual_script_vstack_distances(s, cramp, nuc.is_single_character, nuc_depth,
                                                                nuc_height, sub_height, sup_depth);
            sup_box.shift = nuc.corrections.italic_correction;
            auto script_node = vstack(std::move(sup_box), std::move(sub_box), distances);
            return concat(make_hlist(std::move(nuc.nucleus_node)), extend(std::move(script_node), script_space(s)));
        }

        hlist make_script_hlist(const settings s, const cramping cramp, nucleus_result&& nucleus,
                                const std::optional<std::vector<noad>>& sub,
                                const std::optional<std::vector<noad>>& sup)
        {
            auto sup_box = sup ? std::make_optional<box>(clean_box(s, cramp, *sup)) : std::optional<box>();
            auto sub_box = sub ? std::make_optional<box>(clean_box(s, cramp, *sub)) : std::optional<box>();

            if (!sub_box && !sup_box)
                return extend(std::move(nucleus.nucleus_node), nucleus.corrections.italic_correction);

            if (sub_box && !sup_box) return make_subscript_hlist(s, std::move(nucleus), std::move(*sub_box));

            if (!sub_box && sup_box)
                return make_superscript_hlist(s, cramp, std::move(nucleus), std::move(*sup_box));

            return make_dual_scripts_hlist(s, cramp, std::move(nucleus), std::move(*sub_box), std::move(*sup_box));
        }

        settings script_settings(const settings s)
        {
            if (s.style == formula_style::display) return {.style = formula_style::script, .fonts = s.fonts};
            if (s.style == formula_style::text) return {.style = formula_style::script, .fonts = s.fonts};
            return {.style = formula_style::script_script, .fonts = s.fonts};
        }
    }

    template<gen_script Script>
    hlist gen_script_to_hlist(const settings s, const cramping cramp, const bool use_limit_pos,
                              const Script& n)
    {
        constexpr auto is_big_op = std::is_same_v<decltype(n), const big_op&>;
        auto nucleus = make_nucleus(s, cramp, is_big_op, n.nucleus);
        const auto st = script_settings(s);

        return use_limit_pos ? make_limit_hlist(st, cramp, nucleus.corrections.italic_correction,
                                                std::move(nucleus.nucleus_node), n.sub, n.sup)
                             : make_script_hlist(st, cramp, std::move(nucleus), n.sub, n.sup);
    }

    template hlist gen_script_to_hlist<script>(const settings, const cramping, const bool, const script&);
    template hlist gen_script_to_hlist<big_op>(const settings, const cramping, const bool, const big_op&);
}