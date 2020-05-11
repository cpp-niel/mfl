#pragma once

#include "noad/math_char.hpp"
#include "node/node.hpp"
#include "settings.hpp"

#include <optional>
#include <vector>

namespace mfl
{
    struct nucleus_result
    {
        node_variant nucleus_node;
        horizontal_correction corrections;
        bool is_single_character = false;
    };

    struct big_op;
    struct hlist;
    struct script;

    template <typename ScriptNoad>
    concept gen_script = std::is_same_v<ScriptNoad, big_op> || std::is_same_v<ScriptNoad, script>;

    template<gen_script Script>
    [[nodiscard]] hlist gen_script_to_hlist(const settings s, const bool is_cramped, const bool use_limit_pos,
                                            const Script& n);
}