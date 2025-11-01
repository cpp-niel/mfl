#include "mfl/units.hpp"

namespace mfl
{
    namespace
    {
        constexpr auto points_per_inch = 72.0;

        inches pixels_to_inches(const pixels x, const dots_per_inch dpi) { return inches{x.value() / dpi.value()}; }
        inches points_to_inches(const points x) { return inches{x.value() / points_per_inch}; }
    }

    pixels inches_to_pixels(const inches x, const dots_per_inch dpi) { return pixels{x.value() * dpi.value()}; }

    points pixels_to_points(const pixels x, const dots_per_inch dpi)
    {
        return points{pixels_to_inches(x, dpi).value() * points_per_inch};
    }
    pixels points_to_pixels(const points x, const dots_per_inch dpi)
    {
        return inches_to_pixels(points_to_inches(x), dpi);
    }
}
