#include "renderer/svg_renderer.hpp"

#include "fonts_for_tests/font_face.hpp"

#include <cairo-ft.h>
#include <cairo-svg.h>
#include <cairo.h>

#include <iostream>
#include <utility>

namespace mfl
{
    namespace
    {
        using namespace ::mfl::units_literals;

        cairo_surface_t* create_svg_surface(const pixels width, const pixels height, const dots_per_inch,
                                            std::ostream& os)
        {
            const auto callback = [](void* closure, const unsigned char* data, unsigned int size) {
                std::ostream& stream = *static_cast<std::ostream*>(closure);
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
                const auto* chars = reinterpret_cast<const std::ostream::char_type*>(data);
                stream.write(chars, size);
                return CAIRO_STATUS_SUCCESS;
            };

            auto* result = cairo_svg_surface_create_for_stream(callback, &os, width.value(), height.value());
            cairo_svg_surface_set_document_unit(result, CAIRO_SVG_UNIT_PX);
            return result;
        }
    }

    class svg_renderer::impl
    {
    public:
        impl(const pixels width, const pixels height, const dots_per_inch dpi, const fft::freetype& ft,
             std::ostream& os)
            : width_(width)
            , height_(height)
            , dpi_(dpi)
            , surface_(create_svg_surface(width, height, dpi, os))
            , cr_(cairo_create(surface_))
            , cr_mono_face_(cairo_ft_font_face_create_for_ft_face(ft.face(font_family::mono), 0))
            , ft_(ft)
        {
        }

        ~impl()
        {
            cairo_font_face_destroy(cr_mono_face_);
            cairo_surface_finish(surface_);
            cairo_destroy(cr_);
            cairo_surface_destroy(surface_);

            // If we don't reset the static data valgrind complains and also cairo eventually picks the wrong
            // font from its internal cache and renders glyphs in the wrong font.
            cairo_debug_reset_static_data();
        }

        impl(const impl&) = delete;
        impl& operator=(const impl&) = delete;
        impl(impl&&) = delete;
        impl& operator=(impl&&) = delete;

        void render_glyph(const pixels x, const pixels y, const shaped_glyph& g)
        {
            auto* face = ft_.face(g.family);
            fft::ft_set_size(face, g.size);
            std::unique_ptr<cairo_font_face_t, decltype(&cairo_font_face_destroy)> cr_face(
                cairo_ft_font_face_create_for_ft_face(face, 0), cairo_font_face_destroy);
            cairo_set_font_face(cr_, cr_face.get());
            cairo_set_font_size(cr_, points_to_pixels(g.size, dpi_).value());
            const auto [px, py] = to_cairo_pos(x + points_to_pixels(g.x, dpi_), y + points_to_pixels(g.y, dpi_));
            cairo_glyph_t glyph{.index = g.index, .x = px.value(), .y = py.value()};
            cairo_show_glyphs(cr_, &glyph, 1);
        }

        void render_line(const pixels x, const pixels y, const line& l)
        {
            const auto [minx, miny] = to_cairo_pos(x + points_to_pixels(l.x, dpi_), y + points_to_pixels(l.y, dpi_));
            const auto [maxx, maxy] =
                to_cairo_pos(x + points_to_pixels(l.x + l.length, dpi_), y + points_to_pixels(l.y + l.thickness, dpi_));

            cairo_move_to(cr_, minx.value(), miny.value());
            cairo_line_to(cr_, maxx.value(), miny.value());
            cairo_line_to(cr_, maxx.value(), maxy.value());
            cairo_line_to(cr_, minx.value(), maxy.value());
            cairo_line_to(cr_, minx.value(), miny.value());
            cairo_fill(cr_);
        }

        void render_tt_text(const pixels x, const pixels y, const std::string& text)
        {
            const auto [cx, cy] = to_cairo_pos(x, y);

            constexpr auto tt_font_size = 7_pt;
            constexpr auto tt_font_color_r = 0.0;
            constexpr auto tt_font_color_g = 0.05;
            constexpr auto tt_font_color_b = 0.35;
            cairo_set_source_rgb(cr_, tt_font_color_r, tt_font_color_g, tt_font_color_b);
            cairo_set_font_face(cr_, cr_mono_face_);
            cairo_set_font_size(cr_, points_to_pixels(tt_font_size, dpi_).value());
            cairo_move_to(cr_, cx.value(), cy.value());
            cairo_show_text(cr_, text.c_str());
            cairo_set_source_rgb(cr_, 0.0, 0.0, 0.0);
        }

    private:
        pixels width_ = 0_px;
        pixels height_ = 0_px;
        dots_per_inch dpi_{100.0};
        cairo_surface_t* surface_ = nullptr;
        cairo_t* cr_ = nullptr;
        cairo_font_face_t* cr_mono_face_ = nullptr;
        const fft::freetype& ft_;

        std::pair<pixels, pixels> to_cairo_pos(const pixels x, const pixels y) { return {x, height_ - y}; }
    };

    svg_renderer::svg_renderer(std::ostream& os, const pixels width, const pixels height, const dots_per_inch dpi,
                               const fft::freetype& ft)
        : impl_(std::make_unique<impl>(width, height, dpi, ft, os))
    {
    }

    svg_renderer::~svg_renderer() = default;

    void svg_renderer::render(const pixels x, const pixels y, const layout_elements& elements)
    {
        if (elements.error)
        {
            std::cout << *elements.error << std::endl;
            return;
        }

        for (const auto& g : elements.glyphs)
        {
            impl_->render_glyph(x, y, g);
        }

        for (const auto& l : elements.lines)
        {
            impl_->render_line(x, y, l);
        }
    }

    void svg_renderer::render_tt_text(const pixels x, const pixels y, const std::string& text)
    {
        impl_->render_tt_text(x, y, text);
    }
}