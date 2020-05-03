#include "noad/big_op.hpp"

#include "noad/gen_script.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

namespace mfl
{
    hlist big_op_to_hlist(const settings s, const bool is_cramped, const big_op& op)
    {
        const auto use_limit_pos = (op.limits == big_op_limits::yes)
                                   or ((s.style == formula_style::display) and (op.limits == big_op_limits::normal));
        return gen_script_to_hlist(s, is_cramped, use_limit_pos, op);
    }
}