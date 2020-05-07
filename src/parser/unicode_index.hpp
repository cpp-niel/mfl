#pragma once

#include "mfl/code_point.hpp"

#include <string_view>

namespace mfl::parser
{
    enum class font_choice
    {
        normal,
        roman,
        italic,
        bold,
        sans,
        mono,
        calligraphic,
        blackboard,
        fraktur,

        num_choices
    };

    class parser_state;

    [[nodiscard]] code_point unicode_index(const std::string_view name, parser_state& state);
    [[nodiscard]] code_point math_alpha_index(const code_point code, const font_choice font);
}