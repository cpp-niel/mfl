#include "parser/accent.hpp"

#include "parser/math_char.hpp"
#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"
#include "parser/symbols/accents.hpp"
#include "parser/unicode_index.hpp"

#include <range/v3/algorithm/contains.hpp>
#include <range/v3/view/map.hpp>

#include <array>

namespace mfl::parser
{
    bool is_accent(const std::string& name)
    {
        namespace rv = ranges::views;
        return ranges::contains(rv::keys(accents), name) or ranges::contains(rv::keys(additional_accents), name);
    }

    accent create_accent(parser_state& state)
    {
        const auto accent_name = state.consume_lexer_value();
        return {.char_code = unicode_index("\\" + accent_name, state),
                .is_wide = accent_name.starts_with("wide"),
                .noads = (state.lexer_token() == tokens::symbol) ? std::vector<noad>{create_math_char(state)}
                                                                 : parse_required_group(state)};
    }
}