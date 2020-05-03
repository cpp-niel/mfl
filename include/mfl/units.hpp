#pragma once

#include "mfl/detail/operators.hpp"

#include <fmt/format.h>

#include <compare>
#include <iosfwd>

namespace mfl
{
    struct pixels;
    struct dots_per_inch;
    struct points;

    struct inches : detail::operators<inches>
    {
        constexpr inches() = default;
        constexpr explicit inches(const double val) : value(val) {}

        friend constexpr auto operator<=>(inches p0, inches p1) = default;
        constexpr inches& operator+=(const inches x)
        {
            value += x.value;
            return *this;
        }
        constexpr inches& operator-=(const inches x)
        {
            value -= x.value;
            return *this;
        }
        constexpr inches& operator*=(const double x)
        {
            value *= x;
            return *this;
        }
        constexpr inches& operator/=(const double x)
        {
            value /= x;
            return *this;
        }

        [[nodiscard]] pixels to_pixels(const dots_per_inch dpi) const;

        double value = 0.0;
    };

    std::ostream& operator<<(std::ostream& os, const inches& p);

    struct dots_per_inch : detail::operators<dots_per_inch>
    {
        constexpr dots_per_inch() = default;
        constexpr explicit dots_per_inch(const double val) : value(val) {}

        friend constexpr auto operator<=>(dots_per_inch p0, dots_per_inch p1) = default;
        constexpr dots_per_inch& operator+=(const dots_per_inch x)
        {
            value += x.value;
            return *this;
        }
        constexpr dots_per_inch& operator-=(const dots_per_inch x)
        {
            value -= x.value;
            return *this;
        }
        constexpr dots_per_inch& operator*=(const double x)
        {
            value *= x;
            return *this;
        }
        constexpr dots_per_inch& operator/=(const double x)
        {
            value /= x;
            return *this;
        }

        double value = 0.0;
    };

    std::ostream& operator<<(std::ostream& os, const dots_per_inch& p);

    struct pixels : detail::operators<pixels>
    {
        constexpr pixels() = default;
        constexpr explicit pixels(const double val) : value(val) {}

        friend constexpr auto operator<=>(pixels p0, pixels p1) = default;
        constexpr pixels& operator+=(const pixels x)
        {
            value += x.value;
            return *this;
        }
        constexpr pixels& operator-=(const pixels x)
        {
            value -= x.value;
            return *this;
        }
        constexpr pixels& operator*=(const double x)
        {
            value *= x;
            return *this;
        }
        constexpr pixels& operator/=(const double x)
        {
            value /= x;
            return *this;
        }

        [[nodiscard]] inches to_inches(const dots_per_inch dpi) const;
        [[nodiscard]] points to_points(const dots_per_inch dpi) const;

        double value = 0.0;
    };

    std::ostream& operator<<(std::ostream& os, const pixels& p);

    struct points : detail::operators<points>
    {
        constexpr points() = default;
        constexpr explicit points(const double val) : value(val) {}

        friend constexpr auto operator<=>(points p0, points p1) = default;
        constexpr points& operator+=(const points x)
        {
            value += x.value;
            return *this;
        }
        constexpr points& operator-=(const points x)
        {
            value -= x.value;
            return *this;
        }
        constexpr points& operator*=(const double x)
        {
            value *= x;
            return *this;
        }
        constexpr points& operator/=(const double x)
        {
            value /= x;
            return *this;
        }

        [[nodiscard]] inches to_inches() const;
        [[nodiscard]] pixels to_pixels(const dots_per_inch dpi) const;

        double value = 0.0;
    };

    std::ostream& operator<<(std::ostream& os, const points& p);

    namespace units_literals
    {
        constexpr inches operator""_in(long double x) { return inches{static_cast<double>(x)}; }

        constexpr inches operator""_in(unsigned long long x) { return inches{static_cast<double>(x)}; }

        constexpr dots_per_inch operator""_dpi(long double x) { return dots_per_inch{static_cast<double>(x)}; }

        constexpr dots_per_inch operator""_dpi(unsigned long long x) { return dots_per_inch{static_cast<double>(x)}; }

        constexpr pixels operator""_px(long double x) { return pixels{static_cast<double>(x)}; }

        constexpr pixels operator""_px(unsigned long long x) { return pixels{static_cast<double>(x)}; }

        constexpr points operator""_pt(long double x) { return points{static_cast<double>(x)}; }

        constexpr points operator""_pt(unsigned long long x) { return points{static_cast<double>(x)}; }
    }

    template <typename Unit, char c0 = 0, char c1 = 0, char c2 = 0>
    struct unit_formatter
    {
        constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

        template <typename FormatContext>
        auto format(const Unit x, FormatContext& ctx)
        {
            constexpr char suffix[4] = {c0, c1, c2, 0};
            return fmt::format_to(ctx.out(), "{}{}", x.value, suffix);
        }
    };
}

namespace fmt
{
    template<>
    struct formatter<mfl::inches> : mfl::unit_formatter<mfl::inches, '\"'> {};

    template <>
    struct formatter<mfl::dots_per_inch> : mfl::unit_formatter<mfl::dots_per_inch, 'd', 'p', 'i'> {};

    template <>
    struct formatter<mfl::pixels> : mfl::unit_formatter<mfl::pixels, 'p', 'x'> {};

    template <>
    struct formatter<mfl::points> : mfl::unit_formatter<mfl::points, 'p', 't'> {};
}
