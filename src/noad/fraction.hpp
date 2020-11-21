#pragma once

#include "noad/noad.hpp"

#include <optional>
#include <vector>

namespace mfl
{
    // if there is no thickness, then the default rule thickness is used. If the thickness is
    // zero then there is no rule and the fraction becomes like an 'atop' or 'binom'
    struct fraction
    {
        code_point left_delim_code = 0;
        std::vector<noad> numerator;
        std::vector<noad> denominator;
        std::optional<dist_t> thickness;
        code_point right_delim_code = 0;
    };

    struct settings;
    struct hlist;

    hlist fraction_to_hlist(const settings s, const cramping cramp, const fraction& f);
}