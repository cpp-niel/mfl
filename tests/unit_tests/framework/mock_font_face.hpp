#pragma once

#include "mfl/abstract_font_face.hpp"

#include <vector>

namespace mfl
{
    struct mock_font_face : abstract_font_face
    {
        [[nodiscard]] math_constants constants() const override;
        [[nodiscard]] math_glyph_info glyph_info(const size_t glyph_index) const override;
        [[nodiscard]] size_t glyph_index_from_code_point(const code_point char_code, const bool) const override
        { return size_t(char_code); }
        [[nodiscard]] std::vector<size_variant> horizontal_size_variants(const code_point char_code) const override;
        [[nodiscard]] std::vector<size_variant> vertical_size_variants(const code_point char_code) const override;
        [[nodiscard]] std::optional<glyph_assembly> horizontal_assembly(const code_point char_code) const override;
        [[nodiscard]] std::optional<glyph_assembly> vertical_assembly(const code_point char_code) const override;
        void set_size(const points) override {}
    };

    std::unique_ptr<abstract_font_face> create_mock_font_face(font_family);
}