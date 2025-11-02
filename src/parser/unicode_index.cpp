#include "parser/unicode_index.hpp"

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

#include <array>
#include <span>
#include <tuple>
#include <utility>

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

        code_point check(const std::span<const symbol_pair> symbols, const std::string_view name)
        {
            const auto it = std::ranges::find_if(symbols, [&](const auto entry) { return name == entry.first; });
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

        [[nodiscard]] std::pair<script_case, std::uint32_t> alpha_info(const code_point code)
        {
            constexpr auto upper_a = code_point{0x0041};
            constexpr auto upper_z = code_point{0x005a};
            constexpr auto upper_alpha = code_point{0x0391};
            constexpr auto upper_omega = code_point{0x03a9};
            constexpr auto lower_a = code_point{0x0061};
            constexpr auto lower_z = code_point{0x007a};
            constexpr auto lower_alpha = code_point{0x03b1};
            constexpr auto lower_omega = code_point{0x03c9};
            constexpr auto zero = code_point{0x0030};
            constexpr auto nine = code_point{0x0039};

            if ((code >= upper_a) && (code <= upper_z)) return {script_case::roman_upper, code - upper_a};

            if ((code >= lower_a) && (code <= lower_z)) return {script_case::roman_lower, code - lower_a};

            if ((code >= upper_alpha) && (code <= upper_omega)) return {script_case::greek_upper, code - upper_alpha};

            if ((code >= lower_alpha) && (code <= lower_omega)) return {script_case::greek_lower, code - lower_alpha};

            if ((code >= zero) && (code <= nine)) return {script_case::digit, code - zero};

            return {script_case::none, 0};
        }

        template <size_t N>
        std::optional<code_point>
        code_point_from_special_cases(const code_point code,
                                      const std::array<std::pair<code_point, code_point>, N>& mappings)
        {
            const auto it = std::ranges::find_if(mappings, [&](const auto& e) { return e.first == code; });
            if (it != mappings.end()) return it->second;

            return std::nullopt;
        }

        // some unicode code points for certain letters in certain fonts are not in the
        // normal 'a' to 'z' (or 'A' to 'Z') range of code points. They are listed here as explicit
        // special cases
        std::optional<code_point> special_letter_code_point(const code_point code, const font_choice font)
        {

            if (((font == font_choice::normal) || (font == font_choice::italic)) && (code == 'h')) return 0x210e;

            if (font == font_choice::calligraphic)
            {
                return code_point_from_special_cases<11>(code, {{{'B', 0x212c},
                                                                 {'E', 0x2130},
                                                                 {'F', 0x2131},
                                                                 {'H', 0x210b},
                                                                 {'I', 0x2110},
                                                                 {'L', 0x2112},
                                                                 {'M', 0x2133},
                                                                 {'R', 0x211b},
                                                                 {'e', 0x212f},
                                                                 {'g', 0x210a},
                                                                 {'o', 0x2134}}});
            }

            if (font == font_choice::blackboard)
            {
                return code_point_from_special_cases<7>(code, {{{'C', 0x2102},
                                                                {'H', 0x210d},
                                                                {'N', 0x2115},
                                                                {'P', 0x2119},
                                                                {'Q', 0x211a},
                                                                {'R', 0x211d},
                                                                {'Z', 2124}}});
            }

            if (font == font_choice::fraktur)
            {
                return code_point_from_special_cases<5>(
                    code, {{{'C', 0x212d}, {'H', 0x210c}, {'I', 0x2111}, {'R', 0x2111c}, {'Z', 0x2128}}});
            }

            return std::nullopt;
        }

        constexpr auto alpha_start_code_points = std::array<std::tuple<font_choice, script_case, code_point>, 21>{
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

    code_point unicode_index(const std::string_view name, parser_state& state)
    {
        constexpr auto elongated_minus_sign = code_point{0x2212};
        if (name == "-") return elongated_minus_sign;

        if ((name.length() > 1) && (name[0] == '\\')) return tex_code_point(name.substr(1, name.length() - 1), state);

        const auto result = utf8::to_ucs4(name);
        if (!result)
        {
            state.set_error(fmt::format("'{}' does not represent a valid utf8 character.", name));
            return 0;
        }

        return *result;
    }

    code_point math_alpha_index(const code_point code, const font_choice font)
    {
        if (auto [script, offset] = alpha_info(code); script != script_case::none)
        {
            if (const auto special_code = special_letter_code_point(code, font)) return *special_code;

            if (const auto start = alpha_start_code_point(font, script); start > 0) return start + offset;
        }

        return code;
    }
}