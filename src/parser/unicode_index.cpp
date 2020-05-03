#include "parser/unicode_index.hpp"

#include "concepts.hpp"
#include "parser/parser_state.hpp"
#include "parser/symbols/accents.hpp"
#include "parser/symbols/arrows.hpp"
#include "parser/symbols/big_operators.hpp"
#include "parser/symbols/binary_operators.hpp"
#include "parser/symbols/delimiters.hpp"
#include "parser/symbols/dots.hpp"
#include "parser/symbols/greek_alphabet.hpp"
#include "parser/symbols/letterlike.hpp"
#include "parser/symbols/punctuation.hpp"
#include "parser/symbols/relational_operators.hpp"
#include "parser/utf8.hpp"

#include <fmt/format.h>
#include <range/v3/algorithm/contains.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/view/map.hpp>

#include <array>
#include <tuple>
#include <utility>
#include <vector>

namespace mfl::parser
{
    namespace
    {
        using symbol_pair = std::pair<const char*, code_point>;

        constexpr auto escaped_symbols = std::array<symbol_pair, 8>{{
            {"%", 0x0025},
            {"$", 0x0024},
            {"{", 0x007b},
            {"}", 0x007d},
            {"_", 0x005f},
            {"#", 0x0023},
            {"\\", 0x005c},
            {"|", 0x2016},
        }};

        code_point check(const range_of<symbol_pair> auto& symbols, const std::string_view name)
        {
            const auto it = ranges::find_if(symbols, [&](const auto entry) { return name == entry.first; });
            return (it == symbols.end()) ? 0 : it->second;
        }

        code_point tex_code_point(const std::string_view tex_name, parser_state& state)
        {
            if (const auto result = check(escaped_symbols, tex_name)) return result;
            if (const auto result = check(accents, tex_name)) return result;
            if (const auto result = check(additional_accents, tex_name)) return result;
            if (const auto result = check(combining_symbols, tex_name)) return result;
            if (const auto result = check(arrows, tex_name)) return result;
            if (const auto result = check(additional_arrows, tex_name)) return result;
            if (const auto result = check(big_op_symbols, tex_name)) return result;
            if (const auto result = check(integral_symbols, tex_name)) return result;
            if (const auto result = check(binary_operators, tex_name)) return result;
            if (const auto result = check(additional_binary_operators, tex_name)) return result;
            if (const auto result = check(left_delimiters, tex_name)) return result;
            if (const auto result = check(right_delimiters, tex_name)) return result;
            if (const auto result = check(dots, tex_name)) return result;
            if (const auto result = check(greek_alphabet_lowercase, tex_name)) return result;
            if (const auto result = check(greek_alphabet_uppercase, tex_name)) return result;
            if (const auto result = check(letterlike_symbols, tex_name)) return result;
            if (const auto result = check(punctuation_symbols, tex_name)) return result;
            if (const auto result = check(relational_operators, tex_name)) return result;
            if (const auto result = check(additional_relational_operators, tex_name)) return result;
            if (const auto result = check(negations, tex_name)) return result;
            if (const auto result = check(additional_negations, tex_name)) return result;

            state.set_error(fmt::format("Unknown symbol name '\\{}'.", tex_name));
            return 0;
        }

        enum class script_case
        {
            none,
            roman_upper,
            roman_lower,
            greek_upper,
            greek_lower,
            digit
        };

        [[nodiscard]] std::pair<script_case, uint32_t> alpha_info(const code_point code)
        {
            if ((code >= 0x0041) && (code <= 0x005a)) return {script_case::roman_upper, code - 0x0041};

            if ((code >= 0x0061) && (code <= 0x007a)) return {script_case::roman_lower, code - 0x0061};

            if ((code >= 0x0391) && (code <= 0x03a9)) return {script_case::greek_upper, code - 0x0391};

            if ((code >= 0x03b1) && (code <= 0x03c9)) return {script_case::greek_lower, code - 0x03b1};

            if ((code >= 0x0030) && (code <= 0x0039)) return {script_case::digit, code - 0x0030};

            return {script_case::none, 0};
        }

        using code_map = std::vector<std::pair<code_point, code_point>>;
        const auto special_alpha_codes = std::array<code_map, size_t(font_choice::num_choices)>{
            code_map{{'h', 0x210e}},
            code_map{},
            code_map{{'h', 0x210e}},
            code_map{},
            code_map{},
            code_map{},
            code_map{{'B', 0x212c},
                     {'E', 0x2130},
                     {'F', 0x2131},
                     {'H', 0x210b},
                     {'I', 0x2110},
                     {'L', 0x2112},
                     {'M', 0x2133},
                     {'R', 0x211b},
                     {'e', 0x212f},
                     {'g', 0x210a},
                     {'o', 0x2134}},
            code_map{
                {'C', 0x2102}, {'H', 0x210d}, {'N', 0x2115}, {'P', 0x2119}, {'Q', 0x211a}, {'R', 0x211d}, {'Z', 2124}},
            code_map{{'C', 0x212d}, {'H', 0x210c}, {'I', 0x2111}, {'R', 0x2111c}, {'Z', 0x2128}},
        };

        const auto alpha_start_code_points = std::array<std::tuple<font_choice, script_case, code_point>, 21>{
            std::tuple{font_choice::normal, script_case::roman_upper, 0x1d434},
            std::tuple{font_choice::normal, script_case::roman_lower, 0x1d44e},
            std::tuple{font_choice::normal, script_case::greek_upper, 0x0391},
            std::tuple{font_choice::normal, script_case::greek_lower, 0x1d6fc},

            std::tuple{font_choice::roman, script_case::roman_upper, 0x0041},
            std::tuple{font_choice::roman, script_case::roman_lower, 0x0061},
            std::tuple{font_choice::roman, script_case::greek_upper, 0x0391},
            std::tuple{font_choice::roman, script_case::greek_lower, 0x03b1},

            std::tuple{font_choice::sans, script_case::roman_upper, 0x1d5a0},
            std::tuple{font_choice::sans, script_case::roman_lower, 0x1d5ba},
            std::tuple{font_choice::sans, script_case::digit, 0x1d7e2},

            std::tuple{font_choice::mono, script_case::roman_upper, 0x1d670},
            std::tuple{font_choice::mono, script_case::roman_lower, 0x1d68a},
            std::tuple{font_choice::mono, script_case::digit, 0x1d7f6},

            std::tuple{font_choice::calligraphic, script_case::roman_upper, 0x1d49c},
            std::tuple{font_choice::calligraphic, script_case::roman_lower, 0x1d4b6},

            std::tuple{font_choice::blackboard, script_case::roman_upper, 0x1d538},
            std::tuple{font_choice::blackboard, script_case::roman_lower, 0x1d552},
            std::tuple{font_choice::blackboard, script_case::digit, 0x1d7d8},

            std::tuple{font_choice::fraktur, script_case::roman_upper, 0x1d504},
            std::tuple{font_choice::fraktur, script_case::roman_lower, 0x1d51e},

        };

        code_point alpha_start_code_point(const font_choice fc, const script_case sc)
        {
            for (auto [font, script, code] : alpha_start_code_points)
            {
                if ((font == fc) && (script == sc)) return code;
            }

            return 0;
        }
    }

    code_point unicode_index(const std::string_view sym, parser_state& state)
    {
        if (sym == "-") return 0x2212;

        if ((sym.length() > 1) && (sym[0] == '\\')) return tex_code_point(sym.substr(1, sym.length() - 1), state);

        const auto result = utf8::to_ucs4(sym);
        if (!result)
        {
            state.set_error(fmt::format("'{}' does not represent a valid utf8 character.", sym));
            return 0;
        }

        return *result;
    }

    code_point math_alpha_index(const code_point code, const font_choice font)
    {
        if (auto [script, offset] = alpha_info(code); script != script_case::none)
        {
            const auto mappings = special_alpha_codes[size_t(font)];
            const auto it = ranges::find_if(mappings, [&](const auto& e) { return e.first == code; });

            if (it != mappings.end()) return it->second;

            const auto start = alpha_start_code_point(font, script);
            if (start > 0) return start + offset;
        }

        return code;
    }
}