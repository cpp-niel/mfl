#include "mfl/units.hpp"

#include <iostream>

namespace mfl
{
    namespace
    {
        constexpr auto points_per_inch = 72.0;
    }

    std::ostream& operator<<(std::ostream& os, const inches& p) { return os << fmt::format("{}", p); }

    pixels inches::to_pixels(const dots_per_inch dpi) const { return pixels{value * dpi.value}; }

    std::ostream& operator<<(std::ostream& os, const dots_per_inch& p) { return os << fmt::format("{}", p); }

    std::ostream& operator<<(std::ostream& os, const pixels& p) { return os << fmt::format("{}", p); }

    std::ostream& operator<<(std::ostream& os, const points& p) { return os << fmt::format("{}", p); }

    inches pixels::to_inches(const dots_per_inch dpi) const { return inches{value / dpi.value}; }

    points pixels::to_points(const dots_per_inch dpi) const { return points{to_inches(dpi).value * points_per_inch}; }

    inches points::to_inches() const { return inches{value / points_per_inch}; }

    pixels points::to_pixels(const dots_per_inch dpi) const { return to_inches().to_pixels(dpi); }
}
