#pragma once

#include "node/hlist.hpp"
#include "node/vlist.hpp"
#include "settings.hpp"

namespace mfl
{
    struct line_sizes
    {
        dist_t gap = 0;
        dist_t padding = 0;
        dist_t thickness = 0;
    };

    template <typename Line, typename MakeVBox>
    hlist make_line(const settings s, const bool is_cramped, const Line& line, const line_sizes& sizes,
                    const MakeVBox& make_box)
    {
        if (line.noads.empty()) return {};

        auto content = clean_box(s, is_cramped, line.noads);
        const auto w = content.dims.width;

        auto l = make_vlist(kern{.size = sizes.gap}, rule{.width = w, .height = sizes.thickness, .depth = 0},
                            kern{.size = sizes.padding});
        return make_hlist(make_box(w, std::move(content), std::move(l)));
    }
}