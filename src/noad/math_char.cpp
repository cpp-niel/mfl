#include "noad/math_char.hpp"

#include "font_library.hpp"
#include "node/box.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

#include <algorithm>
#include <ranges>

namespace mfl
{
    namespace
    {
        std::size_t find_best_size_glyph_index(const abstract_font_face& face, const code_point char_code,
                                               const dist_t requested_size, const bool is_horizontal)
        {
            using namespace units_literals;
            const auto variants =
                is_horizontal ? face.horizontal_size_variants(char_code) : face.vertical_size_variants(char_code);

            if (variants.empty()) return face.glyph_index_from_code_point(char_code, false);

            const auto it =
                std::ranges::find_if(variants, [&](const size_variant& v) { return v.size > requested_size; });

            return (it == variants.end()) ? variants.front().glyph_index : it->glyph_index;
        }

        std::pair<glyph, horizontal_correction> make_glyph(const settings s, const font_family family,
                                                           const abstract_font_face& face, const size_t glyph_index)
        {
            using namespace units_literals;
            const auto size = font_size(s);
            auto glyph_info = face.glyph_info(glyph_index);
            return {glyph{.family = family,
                          .index = glyph_info.glyph_index,
                          .size = size,
                          .width = glyph_info.width,
                          .height = glyph_info.height,
                          .depth = glyph_info.depth},
                    {glyph_info.accent_hpos, glyph_info.italic_correction}};
        }

        box boxed_glyph(const settings s, const font_family family, const abstract_font_face& face,
                        const size_t glyph_index)
        { return make_hbox(hlist{.nodes = {make_glyph(s, family, face, glyph_index).first}}); }

        box assemble_vertical_glyph(const settings s, const font_family family, const abstract_font_face& face,
                                    const glyph_assembly& assembly, const dist_t requested_height)
        {
            const auto is_extender = [](const glyph_part& p) { return p.is_extender; };
            const auto extender_it = std::ranges::find_if(assembly.parts, is_extender);
            if (extender_it == assembly.parts.end()) throw;  // todo!!! What to do here???

            const auto& extender = *extender_it;

            const auto& front_part = assembly.parts.front();
            const auto fixed_top = front_part.is_extender ? std::nullopt : std::optional{front_part};

            const auto& back_part = assembly.parts.back();
            const auto fixed_bottom = back_part.is_extender ? std::nullopt : std::optional{back_part};

            auto fixed_middle = std::optional<glyph_part>{};
            if (assembly.parts.size() > 2)
            {
                // a fixed middle is defined as a non-extender that is neither the first nor the last part
                const auto parts_to_search = assembly.parts                                  //
                                             | std::views::drop(1)                           //
                                             | std::views::take(assembly.parts.size() - 2);  //
                const auto middle_it = std::ranges::find_if(parts_to_search, std::not_fn(is_extender));
                if (middle_it != parts_to_search.end()) fixed_middle = *middle_it;
            }

            const auto fixed_height = (fixed_top ? fixed_top->full_advance : 0)
                                      + (fixed_middle ? fixed_middle->full_advance : 0)
                                      + (fixed_bottom ? fixed_bottom->full_advance : 0);

            const auto half_extension_height = std::max(requested_height - fixed_height, dist_t{0}) / 2;

            const auto num_top_extenders = (half_extension_height + extender.full_advance - 1) / extender.full_advance;
            const auto extender_height = (half_extension_height + 1) / num_top_extenders;

            auto glyph_box0 = boxed_glyph(s, family, face, fixed_top ? fixed_top->glyph_index : extender.glyph_index);
            auto vbox_width = glyph_box0.dims.width;
            auto glyph_boxes = vlist{};
            for (auto i = fixed_top ? 0 : 1; i < num_top_extenders; ++i)
            {
                auto glyph_box = boxed_glyph(s, family, face, extender.glyph_index);
                glyph_box.dims.height = extender_height;
                vbox_width = std::max(vbox_width, glyph_box.dims.width);
                glyph_boxes.nodes.emplace_back(std::move(glyph_box));
            }

            if (fixed_middle.has_value())
            {
                auto glyph_box = boxed_glyph(s, family, face, fixed_middle->glyph_index);
                glyph_boxes.nodes.emplace_back(std::move(glyph_box));
                vbox_width = std::max(vbox_width, glyph_box.dims.width);
            }

            for (auto i = 0; i < num_top_extenders; ++i)
            {
                auto glyph_box = boxed_glyph(s, family, face, extender.glyph_index);
                glyph_box.dims.height = extender_height;
                vbox_width = std::max(vbox_width, glyph_box.dims.width);
                glyph_boxes.nodes.emplace_back(std::move(glyph_box));
            }

            if (fixed_bottom.has_value())
            {
                auto glyph_box = boxed_glyph(s, family, face, fixed_bottom->glyph_index);
                glyph_boxes.nodes.emplace_back(std::move(glyph_box));
                vbox_width = std::max(vbox_width, glyph_box.dims.width);
            }

            return center_on_axis(s, make_down_vbox(vbox_width, std::move(glyph_box0), std::move(glyph_boxes)));
        }
    }

    std::pair<glyph, horizontal_correction> make_glyph(const settings s, const font_family family,
                                                       const code_point char_code, const bool use_large_variant)
    {
        const auto& face = s.fonts->get_face(family, font_size(s));
        const auto glyph_index = face.glyph_index_from_code_point(char_code, use_large_variant);
        return make_glyph(s, family, face, glyph_index);
    }

    std::pair<glyph, horizontal_correction> make_auto_width_glyph(const settings s, const font_family family,
                                                                  const code_point char_code,
                                                                  const dist_t requested_width)
    {
        const auto& face = s.fonts->get_face(family, font_size(s));
        const auto glyph_index = find_best_size_glyph_index(face, char_code, requested_width, true);
        return make_glyph(s, family, face, glyph_index);
    }

    std::pair<node_variant, horizontal_correction> make_auto_height_glyph(const settings s, const font_family family,
                                                                          const code_point char_code,
                                                                          const dist_t requested_height)
    {
        const auto& face = s.fonts->get_face(family, font_size(s));
        const auto assembly = face.vertical_assembly(char_code);
        const auto glyph_index = find_best_size_glyph_index(face, char_code, requested_height, false);
        auto [glyph, correction] = make_glyph(s, family, face, glyph_index);
        if (((glyph.height + glyph.depth) > requested_height) || !assembly.has_value()) return {glyph, correction};

        return {assemble_vertical_glyph(s, family, face, assembly.value(), requested_height), horizontal_correction{}};
    }

    hlist math_char_to_hlist(const settings s, const math_char& mc)
    { return make_hlist(make_glyph(s, mc.family, mc.char_code, false).first); }
}
