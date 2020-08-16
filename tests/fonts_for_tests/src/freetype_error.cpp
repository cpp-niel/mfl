#include "freetype_error.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <range/v3/algorithm/find_if.hpp>

#include <array>
#include <string>

#undef FTERRORS_H_
// NOLINTNEXTLINE
#define FT_ERRORDEF(e, v, s) {e, s},
// NOLINTNEXTLINE
#define FT_ERROR_START_LIST {
// NOLINTNEXTLINE
#define FT_ERROR_END_LIST }

namespace mfl::fft
{
    struct error_definition
    {
        int err_code;
        const char* err_msg;
    };

    constexpr auto error_definitions = std::array<error_definition, 181>{
#include FT_ERRORS_H
    };

    std::string ft_error_string(const FT_Error error_code)
    {
        using namespace std::string_literals;
        const auto it =
            ranges::find_if(error_definitions, [&](const error_definition& e) { return e.err_code == error_code; });

        if (it == error_definitions.end())
            return "Unknown freetype error code: "s + std::to_string(error_code);

        return "FreeType Error: "s + it->err_msg;
    }
}
