#pragma once

namespace mfl
{
    enum class item_kind
    {
        none,
        any,
        ord,
        op,
        bin,
        rel,
        open,
        close,
        punct,
        inner
    };
}