#include "noad/overline.hpp"

#include "noad/line.hpp"
#include "node/box.hpp"
#include "settings.hpp"

namespace mfl
{
    hlist overline_to_hlist(const settings s, const bool is_cramped, const overline& ol)
    {
        if (ol.noads.empty()) return {};

        auto content = clean_box(s, is_cramped, ol.noads);
        const auto w = content.dims.width;

        auto l = make_vlist(kern{.size = overline_gap(s)}, rule{.width = w, .height = overline_thickness(s), .depth = 0},
                            kern{.size = overline_padding(s)});
        return make_hlist(make_up_vbox(w, std::move(content), std::move(l)));
    }
}
