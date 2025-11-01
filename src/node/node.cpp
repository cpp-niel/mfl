#include "node/node.hpp"

#include "node/box.hpp"

#include <variant>

namespace mfl
{
    dist_t depth(const node_variant& n)
    {
        return std::visit(overload{[](const box& b) { return b.dims.depth + b.shift; },
                                   [](const glue_spec&) { return dist_t(0); }, [](const glyph& g) { return g.depth; },
                                   [](const kern&) { return dist_t(0); }, [](const rule& r) { return r.depth; }},
                          n);
    }

    dist_t height(const node_variant& n)
    {
        return std::visit(overload{[](const box& b) { return b.dims.height - b.shift; },
                                   [](const glue_spec&) { return dist_t(0); }, [](const glyph& g) { return g.height; },
                                   [](const kern&) { return dist_t(0); }, [](const rule& r) { return r.height; }},
                          n);
    }

    dist_t vsize(const node_variant& n)
    {
        return std::visit(overload{[](const box& b) { return b.dims.height + b.dims.depth; },
                                   [](const glyph& g) { return g.height + g.depth; },
                                   [](const glue_spec& g) { return g.size; }, [](const kern& k) { return k.size; },
                                   [](const rule& r) { return r.height + r.depth; }},
                          n);
    }

    dist_t width(const node_variant& n)
    {
        return std::visit(overload{[](const box& b) { return b.dims.width; }, [](const glue_spec& g) { return g.size; },
                                   [](const glyph& g) { return g.width; }, [](const kern& k) { return k.size; },
                                   [](const rule& r) { return r.width; }},
                          n);
    }

    dist_t vwidth(const node_variant& n)
    {
        return std::visit(overload{[](const box& b) { return b.shift + b.dims.width; },
                                   [](const glue_spec&) { return dist_t(0); }, [](const glyph& g) { return g.width; },
                                   [](const kern&) { return dist_t(0); }, [](const rule& r) { return r.width; }},
                          n);
    }

    dist_t vheight(const node_variant& n)
    {
        return std::visit(overload{[](const box& b) { return b.dims.height; },
                                   [](const glue_spec& g) { return g.size; }, [](const glyph& g) { return g.height; },
                                   [](const kern& k) { return k.size; }, [](const rule& r) { return r.height; }},
                          n);
    }
}