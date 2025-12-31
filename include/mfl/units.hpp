#pragma once

#include "mfl/detail/quantity.hpp"

#include <array>
#include <format>

namespace mfl
{
    using dots_per_inch = detail::quantity<struct dots_per_inch_tag>;
    using inches = detail::quantity<struct inches_tag>;
    using pixels = detail::quantity<struct pixels_tag>;
    using points = detail::quantity<struct points_tag>;

    [[nodiscard]] pixels inches_to_pixels(const inches x, const dots_per_inch dpi);
    [[nodiscard]] points pixels_to_points(const pixels x, const dots_per_inch dpi);
    [[nodiscard]] pixels points_to_pixels(const points x, const dots_per_inch dpi);

    namespace units_literals
    {
        constexpr inches operator""_in(const long double x) { return inches{static_cast<double>(x)}; }
        constexpr inches operator""_in(const unsigned long long x) { return inches{static_cast<double>(x)}; }

        constexpr dots_per_inch operator""_dpi(const long double x) { return dots_per_inch{static_cast<double>(x)}; }
        constexpr dots_per_inch operator""_dpi(const unsigned long long x)
        {
            return dots_per_inch{static_cast<double>(x)};
        }

        constexpr pixels operator""_px(const long double x) { return pixels{static_cast<double>(x)}; }
        constexpr pixels operator""_px(const unsigned long long x) { return pixels{static_cast<double>(x)}; }

        constexpr points operator""_pt(const long double x) { return points{static_cast<double>(x)}; }
        constexpr points operator""_pt(const unsigned long long x) { return points{static_cast<double>(x)}; }
    }

    template <typename Unit, char c0 = 0, char c1 = 0, char c2 = 0>
    struct unit_formatter
    {
        constexpr auto parse(const std::format_parse_context& ctx) { return ctx.begin(); }

        template <typename FormatContext>
        constexpr auto format(const Unit x, FormatContext& ctx) const
        {
            constexpr auto suffix = std::array{c0, c1, c2, '\0'};
            return std::format_to(ctx.out(), "{}{}", x.value(), suffix.data());
        }
    };
}

namespace std
{
    template <>
    struct [[maybe_unused]] formatter<mfl::inches> : mfl::unit_formatter<mfl::inches, '\"'>
    {
    };

    template <>
    struct [[maybe_unused]] formatter<mfl::dots_per_inch> : mfl::unit_formatter<mfl::dots_per_inch, 'd', 'p', 'i'>
    {
    };

    template <>
    struct [[maybe_unused]] formatter<mfl::pixels> : mfl::unit_formatter<mfl::pixels, 'p', 'x'>
    {
    };

    template <>
    struct [[maybe_unused]] formatter<mfl::points> : mfl::unit_formatter<mfl::points, 'p', 't'>
    {
    };
}
