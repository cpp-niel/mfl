#include "node/box.hpp"

#include "node/glue.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

namespace mfl
{
    namespace
    {
        box make_hbox_with_glue(const box_dims& dims, const dist_t width_diff, hlist&& l)
        {
            auto result = make_hbox(dims, std::move(l));
            result.glue = calculate_glue(width_diff, result.nodes);
            return result;
        }

        hlist hlist_with_unit_glue(box&& box)
        {
            constexpr auto unit_glue_spec = glue_spec{.size = 0,
                                                      .stretch = {.value = unit_distance, .order = infinity_order::fil},
                                                      .shrink = {.value = unit_distance, .order = infinity_order::fil}};
            return make_hlist(unit_glue_spec, std::move(box), unit_glue_spec);
        }
    }

    box make_hbox(const box_dims& dims, hlist&& l) { return box{.dims = dims, .nodes = std::move(l.nodes)}; }

    box make_hbox(hlist&& l)
    {
        if ((l.nodes.size() == 1) && (std::holds_alternative<wrapped_box>(l.nodes[0])))
            return std::get<wrapped_box>(l.nodes[0]);

        return make_hbox({.width = hlist_width(l), .height = hlist_height(l), .depth = hlist_depth(l)}, std::move(l));
    }

    box make_empty_box() { return make_hbox({}, {}); }

    box rebox(const dist_t new_width, box&& b)
    {
        const auto old_width = b.dims.width;
        const auto dims = box_dims{.width = new_width, .height = b.dims.height, .depth = b.dims.depth};

        const auto shift = b.shift;
        b.shift = 0;
        auto result = make_hbox_with_glue(dims, new_width - old_width, hlist_with_unit_glue(std::move(b)));
        result.shift = shift;
        return result;
    }

    box center_on_axis(const settings s, box&& b)
    {
        const auto axh = axis_height(s);
        const auto h = b.dims.height;
        const auto d = b.dims.depth;
        box result = std::move(b);
        result.shift = ((h - d) / 2) - axh;
        return result;
    }

    box make_vbox(const dist_t width, node_variant&& ref_node, vlist&& up_list, vlist&& down_list)
    {
        auto shift = dist_t{0};
        if (std::holds_alternative<wrapped_box>(ref_node))
        {
            auto& b = static_cast<box&>(std::get<wrapped_box>(ref_node));
            std::swap(shift, b.shift);
        }

        const auto dims = box_dims{.width = width,
                                   .height = vlist_size(up_list) + height(ref_node),
                                   .depth = vlist_size(down_list) + depth(ref_node)};

        std::vector<node_variant> nodes;
        nodes.reserve(up_list.nodes.size() + 1 + down_list.nodes.size());
        std::move(up_list.nodes.rbegin(), up_list.nodes.rend(), std::back_inserter(nodes));
        nodes.emplace_back(std::move(ref_node));
        std::ranges::move(down_list.nodes, std::back_inserter(nodes));

        return box{.kind = box_kind::vbox, .dims = dims, .shift = shift, .nodes = std::move(nodes)};
    }

    box make_up_vbox(const dist_t width, box&& ref_node, vlist&& up_list)
    {
        return make_vbox(width, std::forward<box>(ref_node), std::forward<vlist>(up_list), {});
    }

    box make_down_vbox(const dist_t width, box&& ref_node, vlist&& down_list)
    {
        return make_vbox(width, std::forward<box>(ref_node), {}, std::forward<vlist>(down_list));
    }
}