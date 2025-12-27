#include "noad/matrix.hpp"

#include "node/box.hpp"
#include "node/hlist.hpp"
#include "settings.hpp"

#include <algorithm>
#include <ranges>

namespace mfl
{
    settings matrix_settings(const settings s)
    {
        if (s.style == formula_style::display) return {.style = formula_style::text, .fonts = s.fonts};
        if (s.style == formula_style::text) return {.style = formula_style::script, .fonts = s.fonts};
        return {.style = formula_style::script_script, .fonts = s.fonts};
    }

    // todo cramp should probably influence the glue sizes?
    hlist matrix_to_hlist(const settings s, [[maybe_unused]] const cramping cramp, const matrix& m)
    {
        const auto cell_settings = matrix_settings(s);
        auto cell_box_rows = std::vector<std::vector<box>>{};
        for (const auto& row : m.rows)
        {
            auto cell_row = std::vector<box>{};
            for (const auto& cell : row)
                cell_row.push_back(clean_box(cell_settings, cramping::on, cell));

            cell_box_rows.push_back(cell_row);
        }

        const auto num_rows = cell_box_rows.size();
        if (num_rows == 0) return {};

        const auto num_cols = std::ranges::max(cell_box_rows | std::views::transform(&std::vector<box>::size));

        auto col_widths = std::vector(num_cols, dist_t{0});
        for (const auto& row : cell_box_rows)
        {
            for (const auto& [index, cell_box] : std::views::enumerate(row))
            {
                const auto i = static_cast<std::size_t>(index);
                col_widths[i] = std::max(col_widths[i], cell_box.dims.width);
            }
        }

        auto row_hlists = std::vector<hlist>(num_rows);
        for (auto&& [row_index, row] : std::views::enumerate(cell_box_rows))
        {
            const auto i = static_cast<std::size_t>(row_index);
            auto& row_hlist = row_hlists[i];
            for (auto&& [col_index, cell_box] : std::views::enumerate(row))
            {
                const auto j = static_cast<std::size_t>(col_index);
                row_hlist.nodes.emplace_back(rebox(col_widths[j], std::move(cell_box)));

                if (j + 1 < num_cols) row_hlist.nodes.push_back(glue_spec{.size = quad(s)});
            }
        }

        const auto width = hlist_width(row_hlists.front());
        auto stacked_rows = vlist{};
        for (auto&& row_hlist : row_hlists | std::views::drop(1))
        {
            stacked_rows.nodes.push_back(glue_spec{.size = atop_min_gap(s)});
            stacked_rows.nodes.push_back(make_hbox(std::move(row_hlist)));
        }

        return make_hlist(center_on_axis(
            s, make_down_vbox(width, make_hbox(std::move(row_hlists.front())), std::move(stacked_rows))));
    }
}