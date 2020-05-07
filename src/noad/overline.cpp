#include "noad/overline.hpp"

#include "noad/line.hpp"
#include "node/box.hpp"
#include "settings.hpp"

namespace mfl
{
    hlist overline_to_hlist(const settings s, const bool is_cramped, const overline& ol)
    {
        return make_line(s, is_cramped, ol,
                         {.gap = overline_gap(s), .padding = overline_padding(s), .thickness = overline_thickness(s)},
                         make_up_vbox);
    }
}
