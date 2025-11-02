#include "parser/script.hpp"

#include "noad/script.hpp"
#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"

#include <algorithm>

namespace mfl::parser
{
    namespace
    {
        int parse_primes(parser_state& state)
        {
            auto result = 0;
            while (state.lexer_token() == tokens::prime)
            {
                ++result;
                state.consume_token(tokens::prime);
            }

            return result;
        }

        std::vector<noad> get_nodes_for_primes(const int num_primes, parser_state& state)
        {
            const auto prime_char_code = unicode_index("\\prime", state);
            const auto [num_triple_primes, final_num_primes] = std::div(num_primes, 3);

            std::vector<noad> result;
            result.reserve(static_cast<std::size_t>(num_triple_primes) + ((final_num_primes > 0) ? 1U : 0U));
            for (auto i = 0; i < num_triple_primes; ++i)
            {
                result.emplace_back(math_char{.kind = item_kind::ord, .char_code = prime_char_code + 2});
            }

            if (final_num_primes > 0)
            {
                const auto final_char_code = prime_char_code + static_cast<code_point>(final_num_primes - 1);
                result.emplace_back(math_char{.kind = item_kind::ord, .char_code = final_char_code});
            }

            return result;
        }
    }

    std::vector<noad> create_script(parser_state& state)
    {
        std::vector<noad> nucleus;
        auto noads = (state.lexer_token() == tokens::open_brace) ? parse_required_group(state) : parse_item(state);
        std::ranges::move(noads, std::back_inserter(nucleus));

        auto num_primes = parse_primes(state);
        auto [sub0, sup0] = parse_sub_sup(state);
        if (!sup0) num_primes += parse_primes(state);

        auto [sub1, sup1] = parse_sub_sup(state);

        if (sub0 && sub1) state.set_error("Ambiguous subscripts. Use braces to group the subscripts unambiguously.");

        if (sup0 && sup1)
            state.set_error("Ambiguous superscripts. Use braces to group the superscripts unambiguously.");

        const optional_noads sub = sub0 ? sub0 : sub1;

        optional_noads sup;
        if (num_primes > 0)
        {
            sup = get_nodes_for_primes(num_primes, state);
            if (sup0)
                std::ranges::move(*sup0, std::back_inserter(*sup));
            else if (sup1)
                std::ranges::move(*sup1, std::back_inserter(*sup));
        }
        else { sup = sup0 ? sup0 : sup1; }

        if (sub || sup) return {script{.nucleus = nucleus, .sub = sub, .sup = sup}};

        return nucleus;
    }
}
