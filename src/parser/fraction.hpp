#pragma once

#include "noad/fraction.hpp"

namespace mfl::parser
{
    class parser_state;

    [[nodiscard]] fraction create_binom(parser_state& state);
    [[nodiscard]] fraction create_fraction(parser_state& state);
    [[nodiscard]] fraction create_generalized_fraction(parser_state& state);

}