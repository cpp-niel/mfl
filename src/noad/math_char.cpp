#include "noad/math_char.hpp"

#include "font_library.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

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

            auto result = variants.front().glyph_index;
            for (const auto [glyph_index, size] : variants)
            {
                if (size > requested_size) return result;

                result = glyph_index;
            }

            return result;
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

    std::pair<glyph, horizontal_correction> make_auto_height_glyph(const settings s, const font_family family,
                                                                   const code_point char_code,
                                                                   const dist_t requested_height)
    {
        const auto& face = s.fonts->get_face(family, font_size(s));
        const auto glyph_index = find_best_size_glyph_index(face, char_code, requested_height, false);
        return make_glyph(s, family, face, glyph_index);
    }

    hlist math_char_to_hlist(const settings s, const math_char& mc)
    {
        return make_hlist(make_glyph(s, mc.family, mc.char_code, false).first);
    }
}
