#define APPROVALS_DOCTEST

#include "framework/approval_tests.hpp"
#include "framework/svg_comparator.hpp"

static auto directory = ApprovalTests::Approvals::useApprovalsSubdirectory("approved_files");  // NOLINT
static auto disposer = ApprovalTests::FileApprover::registerComparatorForExtension(".svg", std::make_shared<mfl::svg_comparator>()); // NOLINT