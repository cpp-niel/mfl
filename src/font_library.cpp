#include "font_library.hpp"

#include <range/v3/algorithm.hpp>
#include <utility>

namespace mfl
{
    font_library::font_library(font_face_creator create_face) : create_face_(std::move(create_face))
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