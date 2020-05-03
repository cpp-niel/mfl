#pragma once

namespace mfl::detail
{
    template <typename T>
    struct operators
    {
        friend constexpr T operator+(const T x0, const T x1)
        {
            auto result = x0;
            return result += x1;
        }

        friend constexpr T operator-(const T x0, const T x1)
        {
            auto result = x0;
            return result -= x1;
        }

        friend constexpr T operator-(const T x) { return T(-x.value); }

        friend constexpr T operator*(const T x, const double scalar)
        {
            auto result = x;
            return result *= scalar;
        }

        friend constexpr T operator*(const double scalar, const T x)
        {
            auto result = x;
            return result *= scalar;
        }

        friend constexpr T operator/(const T x, const double scalar)
        {
            auto result = x;
            return result /= scalar;
        }

        friend constexpr double operator/(const T x0, const T x1) { return x0.value / x1.value; }

        friend constexpr auto operator<=>(operators p0, operators p1) = default;
    };
}
