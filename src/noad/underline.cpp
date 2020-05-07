#include "noad/underline.hpp"

#include "noad/line.hpp"
#include "node/box.hpp"
#include "settings.hpp"

namespace mfl
{
    hlist underline_to_hlist(const settings s, const bool is_cramped, const underline& ul)
    {
        return make_line(
            s, is_cramped, ul,
            {.gap = underline_gap(s), .padding = underline_padding(s), .thickness = underline_thickness(s)},
            make_down_vbox);
    }
}