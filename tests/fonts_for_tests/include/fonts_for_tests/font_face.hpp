#pragma once

#include "fonts_for_tests/freetype.hpp"

#include "mfl/abstract_font_face.hpp"
#include "mfl/font_family.hpp"
#include "mfl/units.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace mfl::fft
{
    class font_face : public abstract_font_face
    {
    public:
        font_face(const font_family family, const freetype& ft);

        [[nodiscard]] math_constants constants() const override;
        [[nodiscard]] math_glyph_info glyph_info(const size_t glyph_index) const override;
        [[nodiscard]] size_t glyph_index_from_code_point(const code_point char_code, const bool use_large_variant) const override;
        [[nodiscard]] std::vector<size_variant> horizontal_size_variants(const code_point char_code) const override;
        [[nodiscard]] std::vector<size_variant> vertical_size_variants(const code_point char_code) const override;
        void set_size(const points size) override;

    private:
        FT_Face ft_face_ = nullptr;
    };
}