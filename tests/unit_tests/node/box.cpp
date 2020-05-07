#include "node/box.hpp"

#include "font_library.hpp"
#include "node/hlist.hpp"
#include "node/vlist.hpp"
#include "settings.hpp"

#include "framework/doctest.hpp"
#include "framework/mock_font_face.hpp"

namespace mfl
{
    TEST_CASE("box node")
    {
        const auto fonts = font_library(create_mock_font_face);
        const auto display_style = settings{.style = formula_style::display, .fonts = &fonts};

        SUBCASE("Create box from empty hlist")
        {
            const auto b = make_hbox(hlist{});
            CHECK(b.dims.width == 0);
            CHECK(b.dims.height == 0);
            CHECK(b.dims.depth == 0);
        }

        SUBCASE("Creating a box from hlist with single box returns that box")
        {
            auto empty_box = make_empty_box();
            empty_box.shift = 42;
            const auto b = make_hbox(make_hlist(std::move(empty_box)));
            CHECK(b.shift == 42);
        }

        SUBCASE("An hlist box adopts the dimensions from it contents")
        {
            const auto b =
                make_hbox(make_hlist(glyph{.width = 200, .height = 20, .depth = 2},
                                     make_hbox({.width = 420, .height = 42, .depth = 4}, {}), glue_spec{.size = 120}));
            CHECK(b.dims.width == 740);
            CHECK(b.dims.height == 42);
            CHECK(b.dims.depth == 4);
        }

        SUBCASE("center on axis")
        {
            const auto b =
                center_on_axis(display_style, make_hbox({.height = unit_distance * 3, .depth = unit_distance}, {}));
            CHECK(b.shift == -(axis_height(display_style) - unit_distance));
        }

        SUBCASE("rebox")
        {
            const auto b = rebox(100 * unit_distance, make_hbox(make_hlist(glyph{.width = 20 * unit_distance})));
            CHECK(b.dims.width == 100 * unit_distance);
            CHECK(b.nodes.size() == 3);  // one glue node either side of the glyph
            CHECK(std::get<glue_spec>(b.nodes[0]).size == 0);
            CHECK(std::get<glue_spec>(b.nodes[2]).size == 0);

            CHECK(b.glue.scale == 40);
            CHECK(b.glue.order == infinity_order::fil);
        }

        SUBCASE("make_vbox")
        {
            auto ref_node = rule{.width = 42, .height = 2, .depth = 4};
            auto up_list = make_vlist(glyph{.width = 10, .height = 10, .depth = 10},
                                      glyph{.width = 20, .height = 20, .depth = 20});
            auto down_list = make_vlist(glyph{.width = 30, .height = 30, .depth = 30},
                                        glyph{.width = 40, .height = 40, .depth = 40});
            const auto b = make_vbox(42, ref_node, std::move(up_list), std::move(down_list));

            CHECK(b.nodes.size() == 5);
            CHECK(b.dims.width == 42);
            CHECK(b.dims.height == 62);
            CHECK(b.dims.depth == 144);
            CHECK(std::get<glyph>(b.nodes[0]).width == 20);  // the up_list has been reversed
            CHECK(std::get<glyph>(b.nodes[1]).width == 10);  // the up_list has been reversed
            CHECK(std::get<rule>(b.nodes[2]).width == 42);
            CHECK(std::get<glyph>(b.nodes[3]).width == 30);
            CHECK(std::get<glyph>(b.nodes[4]).width == 40);
        }

        SUBCASE("make_down_vbox")
        {
            hlist l;
            l.nodes.emplace_back(rule{.width = 42, .height = 2, .depth = 4});
            auto ref_node = make_hbox(std::move(l));
            auto down_list = make_vlist(glyph{.width = 30, .height = 30, .depth = 30},
                                        glyph{.width = 40, .height = 40, .depth = 40});

            const auto b = make_down_vbox(42, std::move(ref_node), std::move(down_list));

            CHECK(b.nodes.size() == 3);
            CHECK(b.dims.width == 42);
            CHECK(b.dims.height == 2);
            CHECK(b.dims.depth == 144);
            CHECK(static_cast<const box&>(std::get<wrapped_box>(b.nodes[0])).dims.width == 42);
            CHECK(std::get<glyph>(b.nodes[1]).width == 30);
            CHECK(std::get<glyph>(b.nodes[2]).width == 40);
        }

        SUBCASE("make_up_vbox")
        {
            hlist l;
            l.nodes.emplace_back(rule{.width = 42, .height = 2, .depth = 4});
            auto ref_node = make_hbox(std::move(l));
            auto up_list = make_vlist(glyph{.width = 10, .height = 10, .depth = 10},
                                      glyph{.width = 20, .height = 20, .depth = 20});

            const auto b = make_up_vbox(42, std::move(ref_node), std::move(up_list));

            CHECK(b.nodes.size() == 3);
            CHECK(b.dims.width == 42);
            CHECK(b.dims.height == 62);
            CHECK(b.dims.depth == 4);
            CHECK(std::get<glyph>(b.nodes[0]).width == 20);
            CHECK(std::get<glyph>(b.nodes[1]).width == 10);
            CHECK(static_cast<const box&>(std::get<wrapped_box>(b.nodes[2])).dims.width == 42);
        }
    }
}