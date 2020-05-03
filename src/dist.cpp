#include "dist.hpp"

namespace mfl
{
    namespace
    {
        template <typename T>
        constexpr T one = static_cast<T>(unit_distance);
    }

    dist_t ratio_to_dist(const int numerator, const int denominator) { return (numerator * one<int>) / denominator; }
}