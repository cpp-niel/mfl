// if the width_diff is positive
//  find highest order for which the sum of all stretch glues in the node list is non-zero
//   return the sum and the corresponding order
//    result is glue_param{.scale = width_diff / sum, .order = order}
//
// if the width_diff is negative, do the same for the shrink glues and return scale reciprocal
//
// if the width_diff is zero, or the node list does not contain any glue, return scale of one and order normal
//
#include "node/glue.hpp"

namespace mfl
{
    namespace
    {
        dist_t sum_of_glue_by_order(const infinity_order order, glue_scale glue_spec::*scale,
                                    const std::vector<node_variant>& nodes)
        {
            dist_t glue_sum = 0;
            for (const auto& n : nodes)
            {
                std::visit(overload{[&](const glue_spec& glue) {
                                        const auto& s = glue.*scale;
                                        if (s.order == order) glue_sum += s.value;
                                    },
                                    [](const auto&) {}},
                           n);
            }

            return glue_sum;
        }

        auto highest_order_total_glue(glue_scale glue_spec::*scale, const std::vector<node_variant>& nodes)
        {
            using enum infinity_order;
            for (const auto order : {filll, fill, fil, normal})
            {
                const auto glue_sum = sum_of_glue_by_order(order, scale, nodes);
                if (glue_sum != 0) return std::pair(order, glue_sum);
            }

            return std::pair(infinity_order::normal, dist_t(0));
        }

        glue_param calculate_glue_param(const dist_t width_diff, const std::vector<node_variant>& nodes,
                                        glue_scale glue_spec::*scale_direction, const double sign)
        {
            const auto [order, sum] = highest_order_total_glue(scale_direction, nodes);
            if (sum == 0) return {};

            const auto scale = sign * static_cast<double>(width_diff) / static_cast<double>(sum);
            return {.scale = scale, .order = order};
        }
    }

    glue_param calculate_glue(const dist_t width_diff, const std::vector<node_variant>& nodes)
    {
        if (width_diff > 0) return calculate_glue_param(width_diff, nodes, &glue_spec::stretch, 1.0);

        if (width_diff < 0) return calculate_glue_param(width_diff, nodes, &glue_spec::shrink, -1.0);

        return {};
    }
}
