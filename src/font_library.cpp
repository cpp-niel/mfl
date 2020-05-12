#include "font_library.hpp"

#include <range/v3/algorithm.hpp>
#include <utility>

namespace mfl
{
    namespace
    {
        font_parameters make_font_parameters(const points size, const font_library& fonts)
        {
            const auto& face = fonts.get_face(font_family::roman, size);

            const auto x_index = face.glyph_index_from_code_point('x', false);
            const auto x_height = face.glyph_info(x_index).height;

            const auto capital_m_index = face.glyph_index_from_code_point('M', false);
            const auto capital_m_width = face.glyph_info(capital_m_index).width;

            return {.size = size, .x_height = x_height, .capital_m_width = capital_m_width, .math_info = face.constants()};
        }
    }

    font_library::font_library(const points font_size, font_face_creator create_face)
        : create_face_(std::move(create_face))
        , normal_parameters_(make_font_parameters(font_size, *this))
        , small_parameters_(make_font_parameters(font_size * 0.7, *this))
        , tiny_parameters_(make_font_parameters(font_size * 0.5, *this))
    {
    }

    const abstract_font_face& font_library::get_face(const font_family family, const points size) const
    {
        auto it = ranges::find_if(cached_faces_, [&](const auto& entry) { return entry.first == family; });
        if (it == cached_faces_.end())
        {
            cached_faces_.emplace_back(family, create_face_(family));
            it = cached_faces_.end() - 1;
        }

        auto& face = *it->second;
        face.set_size(size);
        return face;
    }
}