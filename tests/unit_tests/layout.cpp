#include "mfl/layout.hpp"

#include <doctest/doctest.h>
#include "framework/mock_font_face.hpp"

namespace mfl
{
    using namespace units_literals;

    TEST_SUITE("layout")
    {
        TEST_CASE("layout call with valid input")
        {
            const auto result = layout(R"(\frac{1}{x+y})", 10_pt, create_mock_font_face);
            CHECK(!result.error);
            CHECK(result.width > 24_pt);
            CHECK(result.height > 11_pt);
            CHECK(result.glyphs.size() == 4);
            CHECK(result.lines.size() == 1);
        }

        TEST_CASE("layout call with invalid input")
        {
            const auto result = layout(R"(\frac{1{x})", 10_pt, create_mock_font_face);
            CHECK(result.error);
            CHECK(result.width == 0_pt);
            CHECK(result.height == 0_pt);
            CHECK(result.glyphs.empty());
            CHECK(result.lines.empty());
        }
    }
}