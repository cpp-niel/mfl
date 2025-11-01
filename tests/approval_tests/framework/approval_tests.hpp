#pragma once

#include <doctest/doctest.h>
#include <framework/ApprovalTests.v.10.0.1.hpp>

namespace mfl
{
    inline void approve_svg(const std::string& str, const bool quiet = false)
    {
        if (quiet)
            ApprovalTests::Approvals::verify(
                str, ApprovalTests::Options(ApprovalTests::Scrubbers::createRegexScrubber(R"(<g id="surface\d+">)",
                                                                                          R"(<g id="surface0">)"))
                         .fileOptions()
                         .withFileExtension(".svg")
                         .withReporter(ApprovalTests::QuietReporter()));
        //.withReporter(ApprovalTests::AutoApproveReporter()));
        else
            ApprovalTests::Approvals::verify(
                str, ApprovalTests::Options(ApprovalTests::Scrubbers::createRegexScrubber(R"(<g id="surface\d+">)",
                                                                                          R"(<g id="surface0">)"))
                         .fileOptions()
                         .withFileExtension(".svg"));
    }
}