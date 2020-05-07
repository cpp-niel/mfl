#pragma once

#include "mfl/abstract_font_face.hpp"

#include <memory>
#include <vector>
#include <utility>

namespace mfl
{
    class font_library
    {
    public:
        explicit font_library(font_face_creator  create_face);

        const abstract_font_face& get_face(const font_family family, const points size) const;

    private:
        mutable std::vector<std::pair<font_family, std::unique_ptr<abstract_font_face>>> cached_faces_;
        font_face_creator create_face_;
    };
}