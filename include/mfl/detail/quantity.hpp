#pragma once

#include <format>
#include <iostream>

namespace mfl::detail
{
    template <typename Tag>
    struct quantity
    {
        constexpr quantity() = default;
        constexpr explicit quantity(const double val) : value_(val) {}

        [[nodiscard]] constexpr double value() const { return value_; }

        constexpr quantity& operator+=(const quantity x)
        {
            value_ += x.value_;
            return *this;
        }

        constexpr quantity& operator-=(const quantity x)
        {
            value_ -= x.value_;
            return *this;
        }

        constexpr quantity& operator*=(const double x)
        {
            value_ *= x;
            return *this;
        }

        constexpr quantity& operator/=(const double x)
        {
            value_ /= x;
            return *this;
        }

        friend constexpr quantity operator+(const quantity x0, const quantity x1)
        {
            auto result = x0;
            result += x1;
            return result;
        }

        friend constexpr quantity operator-(const quantity x0, const quantity x1)
        {
            auto result = x0;
            result -= x1;
            return result;
        }

        friend constexpr quantity operator-(const quantity x) { return quantity(-x.value()); }

        friend constexpr quantity operator*(const quantity x, const double scalar)
        {
            auto result = x;
            result *= scalar;
            return result;
        }

        friend constexpr quantity operator*(const double scalar, const quantity x)
        {
            auto result = x;
            result *= scalar;
            return result;
        }

        friend constexpr quantity operator/(const quantity x, const double scalar)
        {
            auto result = x;
            result /= scalar;
            return result;
        }

        friend constexpr double operator/(const quantity x0, const quantity x1) { return x0.value_ / x1.value_; }

        // NOLINTNEXTLINE(modernize-use-nullptr,hicpp-use-nullptr)
        friend constexpr auto operator<=>(quantity x0, quantity x1) = default;

    private:
        double value_ = 0.0;
    };

    template <typename Tag>
    std::ostream& operator<<(std::ostream& os, const quantity<Tag>& p)
    {
        return os << std::format("{}", p);
    }
}