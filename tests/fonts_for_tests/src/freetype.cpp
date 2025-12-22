#include "fonts_for_tests/freetype.hpp"

#include "freetype_error.hpp"

#include <filesystem>
#include <format>

namespace mfl::fft
{
    namespace
    {
        FT_Library load_library()
        {
            FT_Library result = nullptr;
            if (const auto err = FT_Init_FreeType(&result); err != 0) throw std::runtime_error(ft_error_string(err));

            return result;
        }

        FT_Face load_font(FT_Library ft_library, const std::string_view name)
        {
            FT_Face result = nullptr;
            const auto file_path = std::filesystem::path(MFL_FONT_DIR) / name;
            if (const auto err = FT_New_Face(ft_library, file_path.string().c_str(), 0, &result); err != 0)
                throw std::invalid_argument(
                    std::format("Could not load tt font {}. {}", file_path.string(), ft_error_string(err)));

            return result;
        }
    }

    freetype::freetype()
        : ft_library_(load_library(), FT_Done_FreeType)
        , italic_face_(load_font(ft_library_.get(), "Stix2Text-Italic.otf"), FT_Done_Face)
        , normal_face_(load_font(ft_library_.get(), "Stix2Math.otf"), FT_Done_Face)
        , bold_face_(load_font(ft_library_.get(), "Stix2Text-Bold.otf"), FT_Done_Face)
        , mono_face_(load_font(ft_library_.get(), "DejaVuSansMono.ttf"), FT_Done_Face)
    {
    }

    FT_Face freetype::face(const font_family family) const
    {
        if (family == font_family::italic) return italic_face_.get();

        if (family == font_family::bold) return bold_face_.get();

        if (family == font_family::mono) return mono_face_.get();

        return normal_face_.get();
    }

    void ft_set_size(FT_Face face, const points size)
    {
        const auto fp_size = static_cast<FT_F26Dot6>(size.value() * 64.0);
        if (const auto err = FT_Set_Char_Size(face, fp_size, fp_size, 0, 0); err != 0)
            throw std::invalid_argument(ft_error_string(err));
    }
}