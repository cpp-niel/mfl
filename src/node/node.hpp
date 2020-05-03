#pragma once

#include "node/glue_spec.hpp"
#include "node/glyph.hpp"
#include "node/kern.hpp"
#include "node/rule.hpp"
#include "utils.hpp"

#include <variant>

namespace mfl
{
    struct box;

    using wrapped_box = recursive_wrapper<box>;

    using node_variant = std::variant<glyph, kern, rule, glue_spec, wrapped_box>;

    [[nodiscard]] dist_t width(const node_variant& n);
    [[nodiscard]] dist_t height(const node_variant& n);
    [[nodiscard]] dist_t depth(const node_variant& n);
    [[nodiscard]] dist_t vwidth(const node_variant& n);
    [[nodiscard]] dist_t vheight(const node_variant& n);
    [[nodiscard]] dist_t vsize(const node_variant& n);

    template<typename N>
    concept node = is_variant_alternative<N, node_variant>::value || std::is_same_v<N, node_variant>;
}