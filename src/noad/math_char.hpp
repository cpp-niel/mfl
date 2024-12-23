#pragma once

#include "mfl/code_point.hpp"
#include "mfl/font_family.hpp"
#include "noad/item_kind.hpp"
#include "dist.hpp"

#include <utility>

namespace mfl
{
    struct math_char
    {
        item_kind kind = item_kind::none;
        font_family family = font_family::roman;
        code_point char_code = 0;
    };

    struct horizontal_correction
    {
        dist_t accent_pos = 0;
        dist_t italic_correction = 0;
    };

    struct settings;
    struct glyph;
    struct hlist;

    std::pair<glyph, horizontal_correction> make_glyph(const settings s, const font_family family,
                                                       const code_point char_code, const bool use_large_variant);

    std::pair<glyph, horizontal_correction> make_auto_width_glyph(const settings s, const font_family family,
                                                                  const code_point char_code, const dist_t requested_width);

    std::pair<glyph, horizontal_correction> make_auto_height_glyph(const settings s, const font_family family,
                                                                   const code_point char_code, const dist_t requested_height);

    hlist math_char_to_hlist(const settings s, const math_char& mc);
}