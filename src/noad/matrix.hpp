#pragma once

#include "noad/noad.hpp"

#include <vector>

namespace mfl
{
    using matrix_row = std::vector<std::vector<noad>>;

    struct matrix
    {
        std::vector<matrix_row> rows;
    };

    struct settings;
    struct hlist;

    hlist matrix_to_hlist(const settings s, const cramping cramp, const matrix& m);
}
