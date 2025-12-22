#include "mfl/units.hpp"

#include "framework/doctest.hpp"

#include <format>
#include <sstream>

namespace mfl
{
    TEST_SUITE("units")
    {
        using namespace units_literals;

        TEST_CASE("inches to pixels at 100 dpi")
        {
            CHECK(inches_to_pixels(2_in, 100_dpi) == 200_px);
        }

        TEST_CASE("inches to pixels at 300 dpi")
        {
            CHECK(inches_to_pixels(2_in, 300_dpi) == 600_px);
        }

        TEST_CASE("fractional inches to pixels")
        {
            CHECK(inches_to_pixels(2_in, 150_dpi) == inches_to_pixels(1.5_in, 200_dpi));
        }

        TEST_CASE("72 points is as many pixels as dpi because there are 72 points per inch")
        {
            CHECK(points_to_pixels(72_pt, 100_dpi) == 100_px);
        }

        TEST_CASE("36 points is half dpi because there are 72 points per inch")
        {
            CHECK(points_to_pixels(36_pt, 100_dpi) == 50_px);
        }

        TEST_CASE("108 points is 1.5 * dpi because there are 72 points per inch")
        {
            CHECK(points_to_pixels(108_pt, 100_dpi) == 150_px);
        }

        TEST_CASE("inverse of the point to pixel conversions")
        {
            CHECK(pixels_to_points(150_px, 100_dpi) == 108_pt);
            CHECK(pixels_to_points(50_px, 100_dpi) == 36_pt);
            CHECK(pixels_to_points(100_px, 100_dpi) == 72_pt);
        }

        TEST_CASE_TEMPLATE("in place arithmetic", unit, inches, dots_per_inch, pixels, points)
        {
            auto x = unit(2);
            x += unit(3);
            CHECK(x == unit(5));
            x -= unit(1.5);
            CHECK(x == unit(3.5));
            x *= 4.0;
            CHECK(x == unit(14));
            x /= 2.0;
            CHECK(x == unit(7));
        }

        TEST_CASE_TEMPLATE("arithmetic", unit, inches, dots_per_inch, pixels, points)
        {
            CHECK(unit(2) + unit(3) == unit(5));
            CHECK(unit(2.5) - unit(1.2) == unit(1.3));
            CHECK(unit(2.5) - unit(3) == unit(-0.5));
            CHECK(unit(2.5) * 2.0 == unit(5));
            CHECK(unit(5) / 2.0 == unit(2.5));
            CHECK(unit(5) / unit(2.5) == 2.0);
        }

        TEST_CASE_TEMPLATE("comparisons", unit, inches, dots_per_inch, pixels, points)
        {
            CHECK(unit(2) < unit(3));
            CHECK(unit(3) > unit(2));
            CHECK(unit(2) <= unit(3));
            CHECK(unit(3) >= unit(2));
            CHECK(unit(3) != unit(2));
            CHECK(unit(3) == unit(3));
            CHECK(unit(3) <= unit(3));
            CHECK(unit(3) >= unit(3));
            CHECK(!(unit(3) < unit(2)));
            CHECK(!(unit(2) > unit(3)));
            CHECK(!(unit(3) < unit(3)));
            CHECK(!(unit(3) > unit(3)));
            CHECK(!(unit(2) >= unit(3)));
            CHECK(!(unit(3) <= unit(2)));
        }

        TEST_CASE("formatting inches")
        {
            CHECK(std::format("{}", 42_in) == "42\"");
            CHECK(std::format("{}", -42_in) == "-42\"");
            CHECK(std::format("{}", 42.42_in) == "42.42\"");
        }

        TEST_CASE("formatting dots per inch")
        {
            CHECK(std::format("{}", 42_dpi) == "42dpi");
            CHECK(std::format("{}", -42_dpi) == "-42dpi");
            CHECK(std::format("{}", 42.42_dpi) == "42.42dpi");
        }

        TEST_CASE("formatting pixels")
        {
            CHECK(std::format("{}", 42_px) == "42px");
            CHECK(std::format("{}", -42_px) == "-42px");
            CHECK(std::format("{}", 42.42_px) == "42.42px");
        }

        TEST_CASE("formatting points")
        {
            CHECK(std::format("{}", 42_pt) == "42pt");
            CHECK(std::format("{}", -42_pt) == "-42pt");
            CHECK(std::format("{}", 42.42_pt) == "42.42pt");
        }

        TEST_CASE("stream output inches")
        {
            std::ostringstream os;
            os << 42_in;
            CHECK(os.str() == "42\"");
        }

        TEST_CASE("stream output dots per inch")
        {
            std::ostringstream os;
            os << 42_dpi;
            CHECK(os.str() == "42dpi");
        }

        TEST_CASE("stream output pixels")
        {
            std::ostringstream os;
            os << 42_px;
            CHECK(os.str() == "42px");
        }

        TEST_CASE("stream output points")
        {
            std::ostringstream os;
            os << 42_pt;
            CHECK(os.str() == "42pt");
        }
    }
}
