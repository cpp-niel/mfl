#include "mfl/units.hpp"

#include "framework/doctest.hpp"

namespace mfl
{
    TEST_SUITE("units")
    {
        using namespace units_literals;

        TEST_CASE("[units] inches to pixels at 100 dpi")
        {
            CHECK((2_in).to_pixels(100_dpi) == 200_px);
        }

        TEST_CASE("[units] inches to pixels at 300 dpi")
        {
            CHECK((2_in).to_pixels(300_dpi) == 600_px);
        }

        TEST_CASE("[units] fractional inches to pixels")
        {
            CHECK((2_in).to_pixels(150_dpi) == (1.5_in).to_pixels(200_dpi));
        }

        TEST_CASE("[units] 72 points is as many pixels as dpi because there are 72 points per inch")
        {
            CHECK((72_pt).to_pixels(100_dpi) == 100_px);
        }

        TEST_CASE("[units] 36 points is half dpi because there are 72 points per inch")
        {
            CHECK((36_pt).to_pixels(100_dpi) == 50_px);
        }

        TEST_CASE("[units] 108 points is 1.5 * dpi because there are 72 points per inch")
        {
            CHECK((108_pt).to_pixels(100_dpi) == 150_px);
        }

        TEST_CASE("[units] inverse of the point to pixel conversions")
        {
            CHECK((150_px).to_points(100_dpi) == 108_pt);
            CHECK((50_px).to_points(100_dpi) == 36_pt);
            CHECK((100_px).to_points(100_dpi) == 72_pt);
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

        TEST_CASE("[units] formatting inches")
        {
            CHECK(fmt::format("{}", 42_in) == "42.0\"");
            CHECK(fmt::format("{}", -42_in) == "-42.0\"");
            CHECK(fmt::format("{}", 42.42_in) == "42.42\"");
        }

        TEST_CASE("[units] formatting dots per inch")
        {
            CHECK(fmt::format("{}", 42_dpi) == "42.0dpi");
            CHECK(fmt::format("{}", -42_dpi) == "-42.0dpi");
            CHECK(fmt::format("{}", 42.42_dpi) == "42.42dpi");
        }

        TEST_CASE("[units] formatting pixels")
        {
            CHECK(fmt::format("{}", 42_px) == "42.0px");
            CHECK(fmt::format("{}", -42_px) == "-42.0px");
            CHECK(fmt::format("{}", 42.42_px) == "42.42px");
        }

        TEST_CASE("[units] formatting points")
        {
            CHECK(fmt::format("{}", 42_pt) == "42.0pt");
            CHECK(fmt::format("{}", -42_pt) == "-42.0pt");
            CHECK(fmt::format("{}", 42.42_pt) == "42.42pt");
        }

        TEST_CASE("[units] stream output inches")
        {
            std::ostringstream os;
            os << 42_in;
            CHECK(os.str() == "42.0\"");
        }

        TEST_CASE("[units] stream output dots per inch")
        {
            std::ostringstream os;
            os << 42_dpi;
            CHECK(os.str() == "42.0dpi");
        }

        TEST_CASE("[units] stream output pixels")
        {
            std::ostringstream os;
            os << 42_px;
            CHECK(os.str() == "42.0px");
        }

        TEST_CASE("[units] stream output points")
        {
            std::ostringstream os;
            os << 42_pt;
            CHECK(os.str() == "42.0pt");
        }
    }
}
