#include "noad/left_right.hpp"

#include "node/box.hpp"
#include "node/node.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

namespace mfl
{
    hlist left_right_to_hlist(const settings s, const bool is_cramped, const left_right& l)
    {
        auto content = clean_box(s, is_cramped, l.noads);
        const auto requested_height = content.dims.depth + content.dims.height;
        hlist result;
        if (l.left_delim_code != 0)
            result.nodes.emplace_back(make_auto_height_glyph(s, font_family::roman, l.left_delim_code, requested_height).first);

        result.nodes.emplace_back(std::move(content));

        if (l.right_delim_code != 0)
            result.nodes.emplace_back(make_auto_height_glyph(s, font_family::roman, l.right_delim_code, requested_height).first);

        return result;
    }
}
