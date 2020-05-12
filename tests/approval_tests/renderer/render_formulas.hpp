#pragma once

#include "fonts_for_tests/freetype.hpp"
#include "renderer/svg_renderer.hpp"

#include "concepts.hpp"
#include "mfl/layout.hpp"
#include "mfl/units.hpp"

#include <range/v3/view/drop.hpp>
#include <range/v3/view/take.hpp>

#include <fonts_for_tests/font_face.hpp>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace mfl
{
    using namespace units_literals;

    struct column_config
    {
        pixels initial_offset = 30_px;
        pixels line_height = 50_px;
        pixels x = 10_px;
        size_t num_rows = 12;
    };

    struct approval_test_config
    {
        pixels width = 800_px;
        pixels height = 600_px;
        dots_per_inch dpi = 140_dpi;
        bool render_input = false;
        pixels input_offset = 50_px;
        points font_size = 10_pt;
        std::vector<column_config> columns;
    };

    std::string render_formulas(const approval_test_config& config, const range_of_convertible_to<std::string> auto& formulas)
    {
        namespace rv = ranges::views;
        std::ostringstream os;
        {
            const auto ft = fft::freetype();
            auto create_font_face = [&](const font_family family) { return std::make_unique<fft::font_face>(family, ft); };

            svg_renderer renderer(os, config.width, config.height, config.dpi, ft);
            auto num_formulas_processed = size_t(0);
            for (const auto& col : config.columns)
            {
                auto y = config.height - col.initial_offset;
                for (const auto& formula : formulas | rv::drop(num_formulas_processed) | rv::take(col.num_rows))
                {
                    renderer.render(col.x, y, layout(formula, config.font_size, create_font_face));
                    if (config.render_input) renderer.render_tt_text(col.x + config.input_offset, y, formula);

                    y -= col.line_height;
                }

                num_formulas_processed += col.num_rows;
            }
        }

        return os.str();
    }
}
