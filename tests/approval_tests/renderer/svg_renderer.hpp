#pragma once

#include "fonts_for_tests/freetype.hpp"
#include "mfl/layout.hpp"

#include <iosfwd>
#include <memory>
#include <string>

namespace mfl
{
    class svg_renderer
    {
    public:
        svg_renderer(std::ostream& os, const pixels width, const pixels height, const dots_per_inch dpi,
                     const fft::freetype& ft);

        ~svg_renderer();

        svg_renderer(const svg_renderer&) = delete;
        svg_renderer& operator=(const svg_renderer&) = delete;
        svg_renderer(svg_renderer&&) = delete;
        svg_renderer& operator=(svg_renderer&&) = delete;

        void render(const pixels x, const pixels y, const layout_elements& elements) const;
        void render_tt_text(const pixels x, const pixels y, const std::string& text) const;

    private:
        class impl;
        std::unique_ptr<impl> impl_;
    };
}