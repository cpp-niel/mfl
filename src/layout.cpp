#include "mfl/layout.hpp"

#include "font_library.hpp"
#include "noad/noad.hpp"
#include "node/box.hpp"
#include "node/hlist.hpp"
#include "parser/parse.hpp"
#include "settings.hpp"

namespace mfl
{
    namespace
    {
        using namespace units_literals;

        points dist_to_points(const dist_t d)
        {
            return points(static_cast<double>(d) / static_cast<double>(unit_distance));
        }

        points shift(const node_variant& n)
        {
            if (std::holds_alternative<wrapped_box>(n))
            {
                const box& b = std::get<wrapped_box>(n);
                return dist_to_points(b.shift);
            }

            return 0_pt;
        }

        dist_t scaled_width(const node_variant& n, const double glue_scale)
        {
            if (std::holds_alternative<glue_spec>(n))
            {
                const auto& glue = std::get<glue_spec>(n);
                if (glue_scale == 0.0) return glue.size;

                const auto w = (glue_scale < 0.0) ? glue.shrink.value : glue.stretch.value;
                return static_cast<dist_t>(glue_scale * static_cast<double>(w));
            }

            return width(n);
        }

        void layout_box(const box& b, const points x, const points y, layout_elements& elements);

        void layout_glyph(const glyph& g, const points x, const points y, layout_elements& elements)
        {
            elements.glyphs.push_back({.family = g.family, .index = g.index, .size = g.size, .x = x, .y = y});
        }

        void layout_rule(const rule& r, const points x, const points y, layout_elements& elements)
        {
            const auto thickness = dist_to_points(r.depth + r.height);
            const auto min_y = y - dist_to_points(r.depth);
            const auto length = dist_to_points(r.width);
            elements.lines.push_back({.x = x, .y = min_y, .length = length, .thickness = thickness});
        }

        void layout_node(const node_variant& n, const points x, const points y, layout_elements& elements)
        {
            std::visit(overload{[&](const box& b) { layout_box(b, x, y, elements); }, [](const glue_spec&) {},
                                [&](const glyph& g) { layout_glyph(g, x, y, elements); }, [](const kern&) {},
                                [&](const rule& r) { layout_rule(r, x, y, elements); }},
                       n);
        }

        void layout_hbox(const box& b, const points x, const points y, layout_elements& elements)
        {
            using namespace units_literals;

            if (!b.nodes.empty())
            {
                auto cur_x = x;
                for (const auto& n : b.nodes)
                {
                    layout_node(n, cur_x, y - shift(n), elements);

                    cur_x += dist_to_points(scaled_width(n, b.glue.scale));
                }
            }
        }

        void layout_vbox(const box& b, const points x, const points y, layout_elements& elements)
        {
            if (!b.nodes.empty())
            {
                auto cur_y = y + dist_to_points(b.dims.height);
                for (const auto& n : b.nodes)
                {
                    layout_node(n, x + shift(n), cur_y - dist_to_points(vheight(n)), elements);

                    cur_y -= dist_to_points(vsize(n));
                }
            }
        }

        void layout_box(const box& b, const points x, const points y, layout_elements& elements)
        {
            if (b.kind == box_kind::hbox)
                layout_hbox(b, x, y, elements);
            else
                layout_vbox(b, x, y, elements);
        }
    }

    layout_elements layout(const std::string_view input, const points font_size,
                           const font_face_creator& create_font_face)
    {
        font_library fonts{font_size, create_font_face};

        const auto [noads, error] = parse(input);
        if (error) return {.error = error};

        const auto hbox =
            make_hbox(to_hlist({.style = formula_style::display, .fonts = &fonts}, cramping::off, false, noads));
        layout_elements result{.width = dist_to_points(hbox.dims.width), .height = dist_to_points(hbox.dims.height)};
        layout_box(hbox, 0_pt, 0_pt, result);
        return result;
    }
}