cmake_minimum_required (VERSION 3.5)

project (unit_tests)

add_executable (unit_tests
        dist.cpp
        framework/mock_font_face.cpp
        layout.cpp
        main.cpp
        noad/accent.cpp
        noad/big_op.cpp
        noad/fraction.cpp
        noad/left_right.cpp
        noad/math_char.cpp
        noad/noad.cpp
        noad/overline.cpp
        noad/radical.cpp
        noad/script.cpp
        noad/underline.cpp
        node/box.cpp
        node/glue.cpp
        node/hlist.cpp
        node/node.cpp
        node/vlist.cpp
        node/vstack.cpp
        parser/accent.cpp
        parser/big_op.cpp
        parser/font_group.cpp
        parser/fraction.cpp
        parser/function.cpp
        parser/left_right.cpp
        parser/lexer.cpp
        parser/line.cpp
        parser/math_char.cpp
        parser/math_space.cpp
        parser/parse.cpp
        parser/radical.cpp
        parser/script.cpp
        parser/unicode_index.cpp
        units.cpp
        )

set_property(TARGET unit_tests PROPERTY CXX_STANDARD 20)

target_include_directories(unit_tests PUBLIC ./ ./../../include ./../../src)

target_link_libraries(unit_tests PUBLIC project_options project_warnings mfl)
