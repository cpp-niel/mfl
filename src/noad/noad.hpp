#pragma once

#include "noad/math_char.hpp"
#include "noad/math_space.hpp"
#include "utils.hpp"

#include <utility>
#include <variant>
#include <vector>

namespace mfl
{
    struct box;
    enum class cramping;
    enum class item_kind;
    struct settings;

    struct radical;
    using radical_wrapper = recursive_wrapper<radical>;

    struct accent;
    using accent_wrapper = recursive_wrapper<accent>;

    struct vcenter;
    using vcenter_wrapper = recursive_wrapper<vcenter>;

    struct overline;
    using overline_wrapper = recursive_wrapper<overline>;

    struct underline;
    using underline_wrapper = recursive_wrapper<underline>;

    struct fraction;
    using fraction_wrapper = recursive_wrapper<fraction>;

    struct left_right;
    using left_right_wrapper = recursive_wrapper<left_right>;

    struct script;
    using script_wrapper = recursive_wrapper<script>;

    struct big_op;
    using big_op_wrapper = recursive_wrapper<big_op>;

    struct mlist;
    using mlist_wrapper = recursive_wrapper<mlist>;

    struct mlist_with_kind;
    using mlist_with_kind_wrapper = recursive_wrapper<mlist_with_kind>;

    using noad = std::variant<math_char, radical_wrapper, accent_wrapper, vcenter_wrapper, overline_wrapper,
                              underline_wrapper, fraction_wrapper, left_right_wrapper, script_wrapper, big_op_wrapper,
                              math_space, mlist_wrapper, mlist_with_kind_wrapper>;

    [[nodiscard]] box clean_box(const settings s, const cramping cramp, const std::vector<noad>& noads);
    [[nodiscard]] hlist to_hlist(const settings s, const cramping cramp, const bool has_penalties,
                                 const std::vector<noad>& noads);
    [[nodiscard]] item_kind kind(const noad& n);
}