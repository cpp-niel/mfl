cmake_minimum_required (VERSION 3.14)

find_path(CAIRO_INCLUDE_DIR cairo.h PATH_SUFFIXES include "include/cairo")
find_library(CAIRO_LIB cairo)
find_library(PIXMAN_LIB pixman-1)
find_package(doctest CONFIG REQUIRED)

project (approval_tests)

add_executable (approval_tests
        docs.cpp
        main.cpp
        mfl.cpp
        renderer/svg_renderer.cpp
        )

set_property(TARGET approval_tests PROPERTY CXX_STANDARD 23)

target_include_directories(approval_tests PUBLIC
        ./
        ./../fonts_for_tests/include
        ./../../include
        ./../../src
        ${CAIRO_INCLUDE_DIR}
        )

target_link_libraries(approval_tests PUBLIC
        mfl_options
        mfl_warnings
        fonts_for_tests
        mfl
        ${CAIRO_LIB}
        ${PIXMAN_LIB}
        doctest::doctest
        )
