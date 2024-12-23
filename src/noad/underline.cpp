#include "noad/underline.hpp"

#include "node/box.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

namespace mfl
{
    hlist underline_to_hlist(const settings s, const cramping cramp, const underline& ul)
    {
        if (ul.noads.empty()) return {};

        auto content = clean_box(s, cramp, ul.noads);
        const auto w = content.dims.width;

        auto l = make_vlist(kern{.size = underline_gap(s)}, rule{.width = w, .height = underline_thickness(s), .depth = 0},
                            kern{.size = underline_padding(s)});
        return make_hlist(make_down_vbox(w, std::move(content), std::move(l)));
    }
}