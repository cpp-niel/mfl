#include "parser/parse.hpp"

#include "parser/lexer.hpp"
#include "parser/parser_state.hpp"
#include "parser/parser_utilities.hpp"

#include <istream>
#include <streambuf>

namespace mfl
{
    namespace
    {
        struct string_view_buf : std::streambuf
        {
            explicit string_view_buf(const std::string_view s)
            {
                // const cast and pointer arithmetic look nasty, but they should be ok
                // (https://stackoverflow.com/questions/13059091/creating-an-input-stream-from-constant-memory)
                auto* first = const_cast<char*>(s.data());   // NOLINT(cppcoreguidelines-pro-type-const-cast)
                this->setg(first, first, first + s.size());  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            }
        };

        struct istring_view_stream : virtual string_view_buf, std::istream
        {
            explicit istring_view_stream(const std::string_view s)
                : string_view_buf(s), std::istream(static_cast<std::streambuf*>(this))
            {
            }
        };
    }

    std::pair<std::vector<noad>, std::optional<std::string>> parse(const std::string_view input)
    {
        istring_view_stream is(input);
        parser::lexer lx(is);
        parser::parser_state state(lx);
        const auto result = parser::parse_until_token(state, parser::tokens::eof);
        return {result.noads, state.error()};
    }
}
