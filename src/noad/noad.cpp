#include "noad/noad.hpp"

#include "concepts.hpp"
#include "node/hlist.hpp"
#include "mlist_with_kind.hpp"
#include "noad/accent.hpp"
#include "noad/big_op.hpp"
#include "noad/fraction.hpp"
#include "noad/left_right.hpp"
#include "noad/mlist.hpp"
#include "noad/overline.hpp"
#include "noad/radical.hpp"
#include "noad/script.hpp"
#include "noad/underline.hpp"
#include "node/box.hpp"
#include "settings.hpp"

#include <algorithm>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/front.hpp>
#include <range/v3/view/drop.hpp>

namespace mfl
{
    namespace
    {
        using namespace units_literals;

        struct inoad
        {
            hlist translated_noad;
            item_kind kind = item_kind::none;
        };

        using ispace = math_space;
        using intermediate_term = std::variant<inoad, ispace>;

        struct ilist
        {
            std::vector<intermediate_term> terms;
        };

        hlist noad_to_hlist(const settings s, const bool is_cramped, const noad& n)
        {
            return std::visit(
                overload{[&](const math_char& c) { return math_char_to_hlist(s, c); },
                         [&](const radical& r) { return radical_to_hlist(s, is_cramped, r); },
                         [&](const accent& a) { return accent_to_hlist(s, is_cramped, a); },
                         [](const vcenter&) { return hlist{}; },
                         [&](const overline& o) { return overline_to_hlist(s, is_cramped, o); },
                         [&](const underline& u) { return underline_to_hlist(s, is_cramped, u); },
                         [&](const fraction& f) { return fraction_to_hlist(s, is_cramped, f); },
                         [&](const left_right& l) { return left_right_to_hlist(s, is_cramped, l); },
                         [&](const script& sc) { return script_to_hlist(s, is_cramped, sc); },
                         [&](const big_op& b) { return big_op_to_hlist(s, is_cramped, b); },
                         [](const math_space&) { return hlist{}; },
                         [&](const mlist& m) { return to_hlist(s, is_cramped, false, m.noads); },
                         [&](const mlist_with_kind& m) { return to_hlist(s, is_cramped, false, m.noads); }
                         },
                n);
        }

        ilist noads_to_intermediate_terms(const settings s, const bool is_cramped, const std::vector<noad>& noads)
        {
            ilist result;
            for (const auto& n : noads)
            {
                if (std::holds_alternative<math_space>(n)) { result.terms.emplace_back(std::get<math_space>(n)); }
                else
                {
                    result.terms.emplace_back(
                        inoad{.translated_noad = noad_to_hlist(s, is_cramped, n), .kind = kind(n)});
                }
            }

            return result;
        }

        std::optional<ispace> math_spacing(const item_kind left, const item_kind right)
        {
            constexpr ispace thin_skip_always = {
                .space = glue_spec{.size = points_to_dist(3_pt), .stretch = {.value = 0}, .shrink = {.value = 0}}};
            constexpr ispace thin_skip = {
                .space = glue_spec{.size = points_to_dist(3_pt), .stretch = {.value = 0}, .shrink = {.value = 0}},
                .is_non_script_only = true};
            constexpr ispace medium_skip = {
                .space = glue_spec{.size = points_to_dist(4_pt), .stretch = {.value = 2}, .shrink = {.value = 4}},
                .is_non_script_only = true};
            constexpr ispace thick_skip = {
                .space = glue_spec{.size = points_to_dist(5_pt), .stretch = {.value = 5}, .shrink = {.value = 0}},
                .is_non_script_only = true};

            struct table_entry
            {
                item_kind left;
                item_kind right;
                std::optional<ispace> space;
            };

            constexpr std::array<table_entry, 15> table = {
                table_entry{.left = item_kind::none, .right = item_kind::any, .space = std::nullopt},
                {.left = item_kind::any, .right = item_kind::none, .space = std::nullopt},
                {.left = item_kind::bin, .right = item_kind::any, .space = medium_skip},
                {.left = item_kind::open, .right = item_kind::any, .space = std::nullopt},
                {.left = item_kind::punct, .right = item_kind::any, .space = thin_skip},
                {.left = item_kind::any, .right = item_kind::bin, .space = medium_skip},
                {.left = item_kind::any, .right = item_kind::close, .space = std::nullopt},
                {.left = item_kind::rel, .right = item_kind::rel, .space = std::nullopt},
                {.left = item_kind::rel, .right = item_kind::punct, .space = std::nullopt},
                {.left = item_kind::rel, .right = item_kind::any, .space = thick_skip},
                {.left = item_kind::any, .right = item_kind::rel, .space = thick_skip},
                {.left = item_kind::any, .right = item_kind::op, .space = thin_skip_always},
                {.left = item_kind::any, .right = item_kind::inner, .space = thin_skip},
                {.left = item_kind::inner, .right = item_kind::any, .space = thin_skip},
                {.left = item_kind::op, .right = item_kind::ord, .space = thin_skip_always}};

            const auto is_entry_for_items = [&](const table_entry& e) {
                return (((e.left == left) or (e.left == item_kind::any)) and
                        ((e.right == right) or (e.right == item_kind::any)));
            };

            if (const auto it = ranges::find_if(table, is_entry_for_items); it != table.end())
                return it->space;

            return std::nullopt;
        }

        node_variant get_space_node(const settings st, const ispace& s)
        {
            if (std::holds_alternative<kern>(s.space))
            {
                if (s.is_math_units)
                {
                    auto space = std::get<kern>(s.space);
                    space.size *= math_unit(st);
                    return space;
                }

                return std::get<kern>(s.space);
            }

            return std::get<glue_spec>(s.space);
        }

        hlist make_space(const settings s, const std::optional<ispace>& space)
        {
            if (!space) return {};

            if ((s.style == formula_style::display) or (s.style == formula_style::text) or (!space->is_non_script_only))
                return {.nodes = {get_space_node(s, *space)}};

            return {};
        }

        item_kind kind_of_next_inoad(const range_of<intermediate_term> auto& iterms)
        {
            const auto it =
                ranges::find_if(iterms, [](const intermediate_term& t) { return std::holds_alternative<inoad>(t); });

            return it == iterms.end() ? item_kind::none : std::get<inoad>(*it).kind;
        }

        bool check_previous(const item_kind kind)
        {
            constexpr auto kinds = std::array<item_kind, 6>{item_kind::bin,  item_kind::op,    item_kind::rel,
                                                            item_kind::open, item_kind::punct, item_kind::none};
            return ranges::find(kinds, kind) != kinds.end();
        }

        bool check_next(const item_kind kind)
        {
            constexpr auto kinds =
                std::array<item_kind, 4>{item_kind::rel, item_kind::close, item_kind::punct, item_kind::none};
            return ranges::find(kinds, kind) != kinds.end();
        }

        item_kind change_kind(const item_kind prev_kind, const item_kind kind, const range_of<intermediate_term> auto& iterms)
        {
            if (kind != item_kind::bin) return kind;

            return (check_previous(prev_kind) or check_next(kind_of_next_inoad(iterms))) ? item_kind::ord
                                                                                         : item_kind::bin;
        }

        void intermediate_terms_to_hlist(const settings s, const bool has_penalties, const item_kind prev_kind,
                                         const range_of<intermediate_term> auto& iterms, hlist& result)
        {
            if (!ranges::empty(iterms))
            {
                const auto& term = ranges::front(iterms);
                auto tail = ranges::views::drop(iterms, 1);
                item_kind kind = item_kind::none;
                if (std::holds_alternative<inoad>(term))
                {
                    const auto& n = std::get<inoad>(term);
                    kind = change_kind(prev_kind, n.kind, tail);
                    const auto space = make_space(s, math_spacing(prev_kind, n.kind));
                    result.nodes.insert(result.nodes.end(), space.nodes.begin(), space.nodes.end());
                    result.nodes.insert(result.nodes.end(), n.translated_noad.nodes.begin(), n.translated_noad.nodes.end());
                }
                else if (std::holds_alternative<ispace>(term))
                {
                    const auto space = make_space(s, std::get<ispace>(term));
                    result.nodes.insert(result.nodes.end(), space.nodes.begin(), space.nodes.end());
                }

                intermediate_terms_to_hlist(s, has_penalties, kind, tail, result);
            }
        }

        hlist intermediate_terms_to_hlist(const settings s, const bool has_penalties, const ilist& iterms)
        {
            hlist result;
            intermediate_terms_to_hlist(s, has_penalties, item_kind::none, iterms.terms, result);
            return result;
        }

        item_kind nucleus_kind(const script& s)
        {
            if ((s.nucleus.size() == 1) && (std::holds_alternative<math_char>(s.nucleus.front())))
            { return std::get<math_char>(s.nucleus.front()).kind; }

            return item_kind::ord;
        }
    }

    box clean_box(const settings s, const bool is_cramped, const std::vector<noad>& noads)
    {
        return make_hbox(to_hlist(s, is_cramped, false, noads));
    }

    hlist to_hlist(const settings s, const bool is_cramped, const bool has_penalties, const std::vector<noad>& noads)
    {
        const auto iterms = noads_to_intermediate_terms(s, is_cramped, noads);
        return intermediate_terms_to_hlist(s, has_penalties, iterms);
    }

    item_kind kind(const noad& n)
    {
        return std::visit(
            overload{[](const math_char& c) { return c.kind; }, [](const radical&) { return item_kind::ord; },
                     [](const accent&) { return item_kind::ord; }, [](const vcenter&) { return item_kind::ord; },
                     [](const overline&) { return item_kind::ord; }, [](const underline&) { return item_kind::ord; },
                     [](const fraction&) { return item_kind::inner; },
                     [](const left_right&) { return item_kind::inner; },
                     [](const script& s) { return nucleus_kind(s); }, [](const big_op&) { return item_kind::op; },
                     [](const math_space&) { return item_kind::none; }, [](const mlist&) { return item_kind::ord; },
                     [](const mlist_with_kind& m) { return m.kind; }},
            n);
    }
}