cmake_minimum_required (VERSION 3.5)

find_package(fmt REQUIRED)
find_package(range-v3 REQUIRED)
find_package(Threads)

add_library(mfl
        dist.cpp
        font_library.cpp
        layout.cpp
        noad/accent.cpp
        noad/big_op.cpp
        noad/fraction.cpp
        noad/gen_script.cpp
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
        parser/command.cpp
        parser/font_group.cpp
        parser/fraction.cpp
        parser/function.cpp
        parser/left_right.cpp
        parser/lexer.cpp
        parser/line.cpp
        parser/math_char.cpp
        parser/math_space.cpp
        parser/parse.cpp
        parser/parser_state.cpp
        parser/parser_utilities.cpp
        parser/radical.cpp
        parser/scanner.cpp
        parser/script.cpp
        parser/tokens.cpp
        parser/unicode_index.cpp
        parser/utf8.cpp
        settings.cpp
        units.cpp
        )

target_include_directories(mfl PUBLIC ./../include)
target_include_directories(mfl PRIVATE ./../src)

target_link_libraries(mfl
        PUBLIC
        project_options
        project_warnings
        range-v3
        fmt::fmt
        fmt::fmt-header-only
        ${CMAKE_THREAD_LIBS_INIT}
        )
