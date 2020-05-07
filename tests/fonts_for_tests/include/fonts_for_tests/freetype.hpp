#pragma once

#include "mfl/font_family.hpp"
#include "mfl/units.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>

namespace mfl::fft
{
    class freetype
    {
    public:
        freetype();

        [[nodiscard]] FT_Face face(const font_family family) const;

    private:
        using library_ptr = std::unique_ptr<std::remove_pointer_t<FT_Library>, decltype(&FT_Done_FreeType)>;
        using face_ptr = std::unique_ptr<std::remove_pointer_t<FT_Face>, decltype(&FT_Done_Face)>;

        library_ptr ft_library_;
        face_ptr italic_face_;
        face_ptr normal_face_;
        face_ptr bold_face_;
        face_ptr mono_face_;
    };

    void ft_set_size(FT_Face face, const points size);
}