#pragma once

#include "node/glue_spec.hpp"
#include "node/kern.hpp"

#include <variant>

namespace mfl
{
    struct math_space
    {
        std::variant<kern, glue_spec> space;
        bool is_non_script_only = false;
        bool is_math_units = true;
    };
}