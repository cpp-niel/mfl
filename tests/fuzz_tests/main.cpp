#include "fonts_for_tests/font_face.hpp"
#include "fonts_for_tests/freetype.hpp"
#include "mfl/layout.hpp"

#include <string_view>

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, size_t size)
{
    using namespace mfl::units_literals;
    static const auto ft = mfl::fft::freetype();
    auto create_font_face = [&](const mfl::font_family family) { return std::make_unique<mfl::fft::font_face>(family, ft); };
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    mfl::layout(std::string_view(reinterpret_cast<const char*>(data), size), 10_pt, create_font_face);
    return 0;
}