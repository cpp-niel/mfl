#include "noad/radical.hpp"

#include "node/box.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

namespace mfl
{
    hlist radical_to_hlist(const settings s, const cramping cramp, const radical& r)
    {
        constexpr auto radical_char_code = 0x221a;
        if (r.radicand.empty()) return {.nodes = {make_glyph(s, font_family::roman, radical_char_code, false).first}};

        auto degree_box = clean_box({.style = formula_style::script_script, .fonts = s.fonts}, cramping::on,
                                    r.degree ? *r.degree : std::vector<noad>{});

        const auto min_height = (x_height(s) * 7) / 4;
        auto content_box = clean_box(s, cramp, r.radicand);
        const auto vertical_gap = std::max(radical_vertical_gap(s), min_height - content_box.dims.height);
        const auto requested_height = content_box.dims.height + content_box.dims.depth + vertical_gap * 2;

        auto radical_symbol = make_auto_height_glyph(s, font_family::roman, radical_char_code, requested_height).first;
        const auto shift =
            height(radical_symbol) - (content_box.dims.height + vertical_gap + radical_rule_thickness(s));
        constexpr auto percent_divisor = dist_t(100);
        degree_box.shift = -degree_box.dims.depth
                           - (height(radical_symbol) * radical_degree_bottom_raise_percent(s)) / percent_divisor;
        auto radical_box = make_hbox({.nodes = {kern{.size = radical_kern_before_degree(s)}, std::move(degree_box),
                                                kern{.size = radical_kern_after_degree(s)}, radical_symbol}});
        radical_box.shift = shift;
        const auto content_box_width = content_box.dims.width;
        auto radicand_box = make_vbox(content_box_width, std::move(content_box),
                                      {.nodes = {kern{.size = vertical_gap},
                                                 rule{.width = content_box_width, .height = radical_rule_thickness(s)},
                                                 kern{.size = radical_extra_ascender(s)}}},
                                      {});
        return {.nodes = {std::move(radical_box), std::move(radicand_box)}};
    }
}