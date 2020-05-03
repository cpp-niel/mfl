#pragma once

#include "approval_tests.hpp"

namespace mfl
{
    struct svg_comparator : ApprovalTests::ApprovalComparator
    {
        bool contentsAreEquivalent(const std::string received_path, const std::string approved_path) const override;
    };
}