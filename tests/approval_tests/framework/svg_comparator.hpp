#pragma once

#include "approval_tests.hpp"

namespace mfl
{
    struct svg_comparator : ApprovalTests::ApprovalComparator
    {
        [[nodiscard]] bool contentsAreEquivalent(const std::string received_path, const std::string approved_path) const override;
    };
}