#pragma once

#include "mfl/abstract_font_face.hpp"
#include "mfl/units.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace mfl
{
    struct shaped_glyph
    {
        font_family family = font_family::roman;
        size_t index = 0;
        points size;
        points x;
        points y;
    };

    struct line
    {
        points x;
        points y;
        points length;
        points thickness;
    };

    struct layout_elements
    {
        std::vector<shaped_glyph> glyphs;
        std::vector<line> lines;
        std::optional<std::string> error;
    };

    layout_elements layout(const std::string_view input, const font_face_creator& create_font_face);
}