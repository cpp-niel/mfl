#include "noad/script.hpp"

#include "node/hlist.hpp"
#include "noad/gen_script.hpp"
#include "settings.hpp"

namespace mfl
{
    hlist script_to_hlist(const settings s, const bool is_cramped, const script& sc)
    {
        return gen_script_to_hlist(s, is_cramped, false, sc);
    }
}