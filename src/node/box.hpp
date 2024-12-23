#pragma once

#include "dist.hpp"
#include "node/node.hpp"
#include "node/vlist.hpp"

#include <vector>

namespace mfl
{
    struct settings;
    struct hlist;
    struct vlist;

    enum class box_kind
    {
        hbox,
        vbox
    };

    struct box_dims
    {
        dist_t width = 0;
        dist_t height = 0;
        dist_t depth = 0;
    };

    struct box
    {
        box_kind kind = box_kind::hbox;
        box_dims dims;
        dist_t shift = 0;
        std::vector<node_variant> nodes;
        glue_param glue;
    };

    [[nodiscard]] box make_hbox(const box_dims& dims, hlist&& l);
    [[nodiscard]] box make_hbox(hlist&& l);
    [[nodiscard]] box make_empty_box();
    [[nodiscard]] box rebox(const dist_t new_width, box&& b);

    [[nodiscard]] box center_on_axis(const settings s, box&& b);

    [[nodiscard]] box make_vbox(const dist_t width, node_variant&& ref_node, vlist&& up_list, vlist&& down_list);
    [[nodiscard]] box make_up_vbox(const dist_t width, box&& ref_node, vlist&& up_list);
    [[nodiscard]] box make_down_vbox(const dist_t width, box&& ref_node, vlist&& down_list);
}