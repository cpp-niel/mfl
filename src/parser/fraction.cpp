#include "parser/fraction.hpp"

#include "parser/left_right.hpp"
#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"

namespace mfl::parser
{
    fraction create_binom(parser_state& state)
    {
        state.consume_token(tokens::command);
        return {.left_delim_code = unicode_index("(", state),
                .numerator = parse_required_group(state),
                .denominator = parse_required_group(state),
                .thickness = 0,
                .right_delim_code = unicode_index(")", state)};
    }

    fraction create_fraction(parser_state& state)
    {
        state.consume_token(tokens::command);
        return {.numerator = parse_required_group(state), .denominator = parse_required_group(state)};
    }

    fraction create_generalized_fraction(parser_state& state)
    {
        state.consume_token(tokens::command);

        const auto ldelim_code = parse_delimiter(state);
        if (state.error()) return {};

        const auto rdelim_code = parse_delimiter(state);
        if (state.error()) return {};

        const auto thickness = points(braced_value(state));
        [[maybe_unused]] const auto math_style = parse_required_group(state);  // TODO support this??
        if (state.error()) return {};

        const auto numerator = parse_required_group(state);
        if (state.error()) return {};

        return {.left_delim_code = ldelim_code,
                .numerator = std::move(numerator),
                .denominator = parse_required_group(state),
                .thickness = points_to_dist(thickness),
                .right_delim_code = rdelim_code};
    }
}
