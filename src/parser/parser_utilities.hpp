#pragma once

#include "noad/mlist.hpp"
#include "noad/noad.hpp"
#include "parser/tokens.hpp"

#include <optional>
#include <utility>
#include <vector>

namespace mfl::parser
{
    class parser_state;

    using optional_noads = std::optional<std::vector<noad>>;

    [[nodiscard]] mlist parse_until_token(parser_state& state, const tokens end_token);
    [[nodiscard]] std::vector<noad> parse_until_token_and_value(parser_state& state, const tokens& end_token,
                                                  const std::string& value);
    [[nodiscard]] std::vector<noad> parse_required_group(parser_state& state);
    [[nodiscard]] std::pair<optional_noads, optional_noads> parse_sub_sup(parser_state& state);
    [[nodiscard]] std::vector<noad> parse_item(parser_state& state);
    [[nodiscard]] std::vector<noad> parse_expression(parser_state& state);
    [[nodiscard]] double braced_value(parser_state& state);
}