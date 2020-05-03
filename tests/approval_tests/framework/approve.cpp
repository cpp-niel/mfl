#include "framework/approve.hpp"

#include "framework/approval_tests.hpp"

namespace mfl
{
    void approve(const std::string_view text, bool auto_approve)
    {
        if (auto_approve)
            ApprovalTests::Approvals::verifyWithExtension(text, ".svg", ApprovalTests::AutoApproveReporter());
        else
            ApprovalTests::Approvals::verifyWithExtension(text, ".svg");
    }
}
