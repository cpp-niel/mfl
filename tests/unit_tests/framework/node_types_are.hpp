#pragma once

#include "node/node.hpp"

#include <vector>

namespace mfl
{
    namespace detail
    {
        template <typename NodeTuple, size_t Index, typename N>
        bool node_type_is(const NodeTuple&, const std::vector<N>& nodes)
        {
            using type = std::tuple_element_t<Index, NodeTuple>;
            using expected_type = std::conditional_t<std::is_same_v<type, box>, wrapped_box, type>;
            return std::holds_alternative<expected_type>(nodes[Index]);
        }

        template <typename NodeTuple, size_t... Indices, typename N>
        bool node_types_are(const NodeTuple& t, const std::vector<N>& nodes, const std::index_sequence<Indices...>)
        {
            return (node_type_is<NodeTuple, Indices>(t, nodes) && ...);
        }

    }

    template <typename... NodeTypes, typename N>
    bool node_types_are(const std::vector<N>& nodes)
    {
        constexpr auto expected_num_nodes = sizeof...(NodeTypes);
        if (nodes.size() != expected_num_nodes) return false;

        return detail::node_types_are(std::tuple<NodeTypes...>{}, nodes, std::make_index_sequence<expected_num_nodes>());
    }
}