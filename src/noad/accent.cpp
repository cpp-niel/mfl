#include "noad/accent.hpp"

#include "noad/math_char.hpp"
#include "noad/noad.hpp"
#include "node/box.hpp"
#include "node/node.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"
#include "node/vlist.hpp"

namespace mfl
{
    namespace
    {
        bool is_accent_over_single_math_char(const accent& a)
        {
            return ((a.noads.size() == 1) && (std::holds_alternative<math_char>(a.noads.front())));
        }

        hlist accent_hlist(const settings s, const dist_t shift, box&& content_box, glyph&& accent_glyph)
        {
            auto accent_box = make_hbox(make_hlist(std::forward<glyph>(accent_glyph)));
            accent_box.shift = shift;
            const auto width = content_box.dims.width;
            auto gap = kern{.size = rule_thickness(s)};
            return make_hlist(make_up_vbox(width, std::forward<box>(content_box), {.nodes = {std::move(gap), std::move(accent_box)}}));
        }

        // position the accent over an empty box which has the width and height of the x-height
        // in the current style.
        hlist accent_over_empty_box(const settings s, const accent& a)
        {
            const auto box_size = x_height(s);
            return accent_hlist(s, box_size,
                                box{.dims = {.width = box_size, .height = box_size}},
                                make_glyph(s, a.family, a.char_code, false).first);
        }

        // when we're positioning the accent over a single math char, we use the font's correction
        // information to tell us where to place the accent
        hlist accent_over_math_char(const settings s, const accent& a)
        {
            const auto& accentee = std::get<math_char>(a.noads.front());
            auto [accentee_glyph, accentee_correction] = make_glyph(s, accentee.family, accentee.char_code, false);
            auto [accenter_glyph, accenter_correction] = make_auto_width_glyph(s, a.family, a.char_code, 0);
            const auto shift = accentee_correction.accent_pos - accenter_correction.accent_pos;
            return accent_hlist(s, shift,
                    make_hbox(make_hlist(std::move(accentee_glyph))),
                    std::move(accenter_glyph));
        }

        // when positioning over complex content we may have a wide accent so we make an auto
        // width glyph and position it horizontally in the center of the content box
        hlist accent_over_complex_content(const settings s, const bool is_cramped, const accent& a)
        {
            auto content_box = clean_box(s, is_cramped, a.noads);
            const auto w = content_box.dims.width;
            auto accent_glyph = make_auto_width_glyph(s, a.family, a.char_code, a.is_wide ? w : 0).first;
            const auto shift = (w / 2) - (width(accent_glyph) / 2);
            return accent_hlist(s, shift, std::move(content_box), std::move(accent_glyph));
        }
    }

    hlist accent_to_hlist(const settings s, const bool is_cramped, const accent& a)
    {
        if (a.noads.empty())
            return accent_over_empty_box(s, a);

        if (is_accent_over_single_math_char(a))
           return accent_over_math_char(s, a);

        return accent_over_complex_content(s, is_cramped, a);
    }
}
