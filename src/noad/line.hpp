#pragma once

#include "node/hlist.hpp"
#include "settings.hpp"
#include "node/vlist.hpp"

namespace mfl
{
    template <typename Line, typename MakeVBox>
    hlist make_line(const settings s, const bool is_cramped, const Line& line, const MakeVBox& make_box)
    {
        if (line.noads.empty()) return {};

        auto content = clean_box(s, is_cramped, line.noads);
        const auto w = content.dims.width;

        auto l = make_vlist(kern{.size = line_gap(s)}, rule{.width = w, .height = line_thickness(s), .depth = 0},
                            kern{.size = line_padding(s)});
        return make_hlist(make_box(w, std::move(content), std::move(l)));
    }
}